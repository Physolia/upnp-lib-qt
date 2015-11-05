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

#include "upnpserverwebsocket.h"

#include <QtWebSockets/QWebSocket>

#include <QtCore/QScopedPointer>

class UpnpSsdpServerSocketPrivate
{
public:

    QScopedPointer<QWebSocket> mClientSocket;

};

UpnpSsdpServerSocket::UpnpSsdpServerSocket(QObject *parent)
    : QObject(parent), d(new UpnpSsdpServerSocketPrivate)
{
}

UpnpSsdpServerSocket::~UpnpSsdpServerSocket()
{
    delete d;
}

void UpnpSsdpServerSocket::init()
{
    d->mClientSocket.reset(new QWebSocket);
}


#include "moc_upnpserverwebsocket.cpp"
