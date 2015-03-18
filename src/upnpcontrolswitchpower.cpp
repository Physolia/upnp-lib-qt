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

#include "upnpcontrolswitchpower.h"

#include <KDSoapClient/KDSoapPendingCall.h>
#include <KDSoapClient/KDSoapPendingCallWatcher.h>

UpnpControlSwitchPower::UpnpControlSwitchPower(QObject *parent) : UpnpServiceDescription(parent)
{

}

UpnpControlSwitchPower::~UpnpControlSwitchPower()
{

}

void UpnpControlSwitchPower::setTarget(bool newTargetValue)
{
    const KDSoapPendingCall &pendingAnswer(callAction(QStringLiteral("SetTarget"), {newTargetValue}));

    KDSoapPendingCallWatcher *replyHandler = new KDSoapPendingCallWatcher(pendingAnswer, this);

    connect(replyHandler, &KDSoapPendingCallWatcher::finished, this, &UpnpControlSwitchPower::finishedSetTargetCall);
}

void UpnpControlSwitchPower::getTarget()
{
    const KDSoapPendingCall &pendingAnswer(callAction(QStringLiteral("GetTarget"), {}));

    KDSoapPendingCallWatcher *replyHandler = new KDSoapPendingCallWatcher(pendingAnswer, this);

    connect(replyHandler, &KDSoapPendingCallWatcher::finished, this, &UpnpControlSwitchPower::finishedGetTargetCall);
}

void UpnpControlSwitchPower::getStatus()
{
    const KDSoapPendingCall &pendingAnswer(callAction(QStringLiteral("GetStatus"), {}));

    KDSoapPendingCallWatcher *replyHandler = new KDSoapPendingCallWatcher(pendingAnswer, this);

    connect(replyHandler, &KDSoapPendingCallWatcher::finished, this, &UpnpControlSwitchPower::finishedGetStatusCall);
}

void UpnpControlSwitchPower::finishedSetTargetCall(KDSoapPendingCallWatcher *self)
{
    self->deleteLater();

    Q_EMIT setTargetFinished(!self->returnMessage().isFault());
}

void UpnpControlSwitchPower::finishedGetTargetCall(KDSoapPendingCallWatcher *self)
{
    self->deleteLater();

    Q_EMIT getTargetFinished(!self->returnMessage().isFault(), self->returnValue().toBool());
}

void UpnpControlSwitchPower::finishedGetStatusCall(KDSoapPendingCallWatcher *self)
{
    self->deleteLater();

    Q_EMIT getStatusFinished(!self->returnMessage().isFault(), self->returnValue().toBool());
}

#include "moc_upnpcontrolswitchpower.cpp"