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

#ifndef UPNPBINARYLIGHT_H
#define UPNPBINARYLIGHT_H

#include "upnpabstractdevice.h"

#include <QtCore/QUrl>

class BinaryLightPrivate;
class UpnpWebSocketPublisher;

class BinaryLight : public QObject
{
    Q_OBJECT

    Q_PROPERTY(UpnpDeviceDescription* description
               READ description
               NOTIFY descriptionChanged)

    Q_PROPERTY(UpnpWebSocketPublisher* webSocketPublisher
               READ webSocketPublisher
               WRITE setWebSocketPublisher
               NOTIFY webSocketPublisherChanged)

public:
    explicit BinaryLight(int cacheDuration = 1800, QObject *parent = 0);

    virtual ~BinaryLight();

    UpnpDeviceDescription* description();

    const UpnpDeviceDescription* description() const;

    void setWebSocketPublisher(UpnpWebSocketPublisher *value);

    UpnpWebSocketPublisher* webSocketPublisher() const;

Q_SIGNALS:

    void descriptionChanged();

    void webSocketPublisherChanged();

public Q_SLOTS:

private:

    BinaryLightPrivate *d;

};

#endif
