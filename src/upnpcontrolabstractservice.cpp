/*
 * Copyright 2015 Matthieu Gallien <matthieu_gallien@yahoo.fr>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "upnpcontrolabstractservice.h"
#include "upnphttpserver.h"
#include "upnpservereventobject.h"
#include "upnpbasictypes.h"

#include "upnpactiondescription.h"
#include "upnpservicedescription.h"

#include <KDSoapClient/KDSoapClientInterface.h>
#include <KDSoapClient/KDSoapMessage.h>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QDnsLookup>
#include <QtNetwork/QNetworkInterface>

#include <QtXml/QDomDocument>

#include <QtCore/QTimer>
#include <QtCore/QPointer>
#include <QtCore/QBuffer>
#include <QtCore/QTextStream>

class UpnpAbstractServiceDescriptionPrivate
{
public:

    UpnpAbstractServiceDescriptionPrivate()
        : mNetworkAccess(), mInterface(nullptr), mEventServer(),
          mPublicAddress(), mRealEventSubscriptionTimeout(0), mEventSubscriptionTimer(nullptr)
    {
    }

    QNetworkAccessManager mNetworkAccess;

    KDSoapClientInterface *mInterface;

    UpnpHttpServer mEventServer;

    QHostAddress mPublicAddress;

    int mRealEventSubscriptionTimeout;

    QPointer<QTimer> mEventSubscriptionTimer;
};

UpnpControlAbstractService::UpnpControlAbstractService(QObject *parent)
    : UpnpAbstractService(parent), d(new UpnpAbstractServiceDescriptionPrivate)
{
    connect(&d->mNetworkAccess, &QNetworkAccessManager::finished, this, &UpnpControlAbstractService::finishedDownload);

    d->mEventServer.setService(this);

    d->mEventServer.listen(QHostAddress::Any);

    const QList<QHostAddress> &list = QNetworkInterface::allAddresses();
    for (const auto &address : list) {
        if (!address.isLoopback()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol) {
                d->mPublicAddress = address;
                break;
            }
        }
    }
}

UpnpControlAbstractService::~UpnpControlAbstractService()
{
    delete d->mInterface;
}

UpnpControlAbstractServiceReply* UpnpControlAbstractService::callAction(const QString &actionName, const QVector<QVariant> &arguments)
{
    KDSoapMessage message;

    const UpnpActionDescription &actionDescription(action(actionName));

    auto itArgumentName = actionDescription.mArguments.begin();
    auto itArgumentValue = arguments.begin();
    for (; itArgumentName != actionDescription.mArguments.end() && itArgumentValue != arguments.end(); ++itArgumentName) {
        message.addArgument(itArgumentName->mName, itArgumentValue->toString());

        ++itArgumentValue;
        if (itArgumentValue == arguments.end()) {
            break;
        }
    }

    if (!d->mInterface) {
        d->mInterface = new KDSoapClientInterface(description().controlURL().toString(), description().serviceType());
        d->mInterface->setSoapVersion(KDSoapClientInterface::SOAP1_1);
        d->mInterface->setStyle(KDSoapClientInterface::RPCStyle);
    }

    return new UpnpControlAbstractServiceReply(d->mInterface->asyncCall(actionName, message, description().serviceType() + QStringLiteral("#") + actionName), this);
}

void UpnpControlAbstractService::subscribeEvents(int duration)
{
    QString webServerAddess(QStringLiteral("<http://"));

    if (!d->mPublicAddress.isNull()) {
        webServerAddess += d->mPublicAddress.toString();
    } else {
        webServerAddess += QStringLiteral("127.0.0.1");
    }

    webServerAddess += QStringLiteral(":") + QString::number(d->mEventServer.serverPort()) + QStringLiteral(">");

    QNetworkRequest myRequest(description().eventURL());
    myRequest.setRawHeader("CALLBACK", webServerAddess.toUtf8());
    myRequest.setRawHeader("NT", "upnp:event");
    QString timeoutDefinition(QStringLiteral("Second-"));
    timeoutDefinition += QString::number(duration);
    myRequest.setRawHeader("TIMEOUT", timeoutDefinition.toLatin1());

    d->mNetworkAccess.sendCustomRequest(myRequest, "SUBSCRIBE");
}

void UpnpControlAbstractService::handleEventNotification(const QByteArray &requestData, const QMap<QByteArray, QByteArray> &headers)
{
    Q_UNUSED(headers)

    const QString &requestAnswer(QString::fromLatin1(requestData));

    QDomDocument requestDocument;
    requestDocument.setContent(requestAnswer);

    const QDomElement &eventRoot = requestDocument.documentElement();

    const QDomElement &propertysetRoot = eventRoot.firstChildElement();
    if (!propertysetRoot.isNull()) {
        const QDomElement &propertyNode = propertysetRoot.firstChildElement();
        if (!propertyNode.isNull()) {
            QDomNode currentChild = propertyNode.firstChild();
            if (!currentChild.isNull()) {
                parseEventNotification(propertyNode.tagName(), currentChild.toCharacterData().data());
            }
        }
    }
}

void UpnpControlAbstractService::downloadServiceDescription(const QUrl &serviceUrl)
{
    d->mNetworkAccess.get(QNetworkRequest(serviceUrl));
}

void UpnpControlAbstractService::finishedDownload(QNetworkReply *reply)
{
    if (reply->isFinished() && reply->error() == QNetworkReply::NoError) {
        if (reply->url() == description().eventURL()) {
            if (reply->hasRawHeader("TIMEOUT")) {
                if (reply->rawHeader("TIMEOUT").startsWith("Second-")) {
                    d->mRealEventSubscriptionTimeout = reply->rawHeader("TIMEOUT").mid(7).toInt();

                    if (!d->mEventSubscriptionTimer) {
                        d->mEventSubscriptionTimer = new QTimer;
                        connect(d->mEventSubscriptionTimer.data(), &QTimer::timeout, this, &UpnpControlAbstractService::eventSubscriptionTimeout);
                        d->mEventSubscriptionTimer->setInterval(1000 * (d->mRealEventSubscriptionTimeout > 60 ? d->mRealEventSubscriptionTimeout - 60 : d->mRealEventSubscriptionTimeout));
                        d->mEventSubscriptionTimer->start();
                    }
                }
            }
        } else {
            parseServiceDescription(reply);
        }
    } else if (reply->isFinished()) {
        qDebug() << "UpnpAbstractServiceDescription::finishedDownload" << "error";
    }
}

void UpnpControlAbstractService::eventSubscriptionTimeout()
{
    subscribeEvents(d->mRealEventSubscriptionTimeout);
}

void UpnpControlAbstractService::parseServiceDescription(QIODevice *serviceDescriptionContent)
{
    QDomDocument serviceDescriptionDocument;
    serviceDescriptionDocument.setContent(serviceDescriptionContent);

    const QDomElement &scpdRoot = serviceDescriptionDocument.documentElement();

    const QDomElement &actionListRoot = scpdRoot.firstChildElement(QStringLiteral("actionList"));
    QDomNode currentChild = actionListRoot.firstChild();
    while (!currentChild.isNull()) {
        const QDomNode &nameNode = currentChild.firstChildElement(QStringLiteral("name"));

        QString actionName;
        if (!nameNode.isNull()) {
            actionName = nameNode.toElement().text();
        }

        UpnpActionDescription newAction;

        newAction.mName = actionName;

        const QDomNode &argumentListNode = currentChild.firstChildElement(QStringLiteral("argumentList"));
        QDomNode argumentNode = argumentListNode.firstChild();
        while (!argumentNode.isNull()) {
            const QDomNode &argumentNameNode = argumentNode.firstChildElement(QStringLiteral("name"));
            const QDomNode &argumentDirectionNode = argumentNode.firstChildElement(QStringLiteral("direction"));
            const QDomNode &argumentRetvalNode = argumentNode.firstChildElement(QStringLiteral("retval"));
            const QDomNode &argumentRelatedStateVariableNode = argumentNode.firstChildElement(QStringLiteral("relatedStateVariable"));

            UpnpActionArgumentDescription newArgument;
            newArgument.mName = argumentNameNode.toElement().text();
            newArgument.mDirection = (argumentDirectionNode.toElement().text() == QStringLiteral("in") ? UpnpArgumentDirection::In : UpnpArgumentDirection::Out);
            newArgument.mIsReturnValue = !argumentRetvalNode.isNull();
            newArgument.mRelatedStateVariable = argumentRelatedStateVariableNode.toElement().text();

            newAction.mArguments.push_back(newArgument);

            addAction(newAction);

            argumentNode = argumentNode.nextSibling();
        }

        currentChild = currentChild.nextSibling();
    }

#if 0
    const QDomElement &serviceStateTableRoot = scpdRoot.firstChildElement(QStringLiteral("serviceStateTable"));
    currentChild = serviceStateTableRoot.firstChild();
    while (!currentChild.isNull()) {
        const QDomNode &nameNode = currentChild.firstChildElement(QStringLiteral("name"));
        if (!nameNode.isNull()) {
            qDebug() << "state variable name" << nameNode.toElement().text();
        }

        currentChild = currentChild.nextSibling();
    }
#endif
}

void UpnpControlAbstractService::parseEventNotification(const QString &eventName, const QString &eventValue)
{
    Q_UNUSED(eventName);
    Q_UNUSED(eventValue);
}

#include "moc_upnpcontrolabstractservice.cpp"
