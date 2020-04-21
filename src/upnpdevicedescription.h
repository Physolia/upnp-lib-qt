/*
   SPDX-FileCopyrightText: 2015 (c) Matthieu Gallien <matthieu_gallien@yahoo.fr>

   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef UPNPDEVICEDESCRIPTION_H
#define UPNPDEVICEDESCRIPTION_H

#include "upnplibqt_export.h"

#include <QPointer>
#include <QVector>

#include <memory>

class UpnpServiceDescription;
class UpnpDeviceDescriptionPrivate;

class UPNPLIBQT_EXPORT UpnpDeviceDescription
{

public:
    UpnpDeviceDescription();

    UpnpDeviceDescription(const UpnpDeviceDescription &other);

    UpnpDeviceDescription(UpnpDeviceDescription &&other);

    ~UpnpDeviceDescription();

    UpnpDeviceDescription &operator=(const UpnpDeviceDescription &other);

    UpnpDeviceDescription &operator=(UpnpDeviceDescription &&other);

    UpnpServiceDescription serviceById(const QString &serviceId) const;

    const UpnpServiceDescription &serviceByIndex(int serviceIndex) const;

    UpnpServiceDescription &serviceByIndex(int serviceIndex);

    const QList<UpnpServiceDescription> &services() const;

    QList<UpnpServiceDescription> &services();

    QList<QString> servicesName() const;

    /**
     * @brief setUDN will set the UDN (i.e. Unique Device Name) of this device
     *
     * setUDN will set the UDN (i.e. Unique Device Name) of this device. It is required.
     * Universally-unique identifier for the device, whether
     * root or embedded. Must be the same over time for a specific device instance (i.e.,
     * must survive reboots). Must match the value of the NT header in device discovery
     * messages. Must match the prefix of the USN header in all discovery messages. Must
     * begin with uuid: followed by a UUID suffix specified by a UPnP vendor. Single URI.
     *
     * @param value will be the new UDN of this device
     */
    void setUDN(const QString &value);

    /**
     * @brief UDN will return the UDN (i.e. Unique Device Name) of this device
     *
     * UDN will return the UDN (i.e. Unique Device Name) of this device It is required.
     * Universally-unique identifier for the device, whether
     * root or embedded. Must be the same over time for a specific device instance (i.e.,
     * must survive reboots). Must match the value of the NT header in device discovery
     * messages. Must match the prefix of the USN header in all discovery messages. Must
     * begin with uuid: followed by a UUID suffix specified by a UPnP vendor. Single URI.
     */
    const QString &UDN() const;

    /**
     * @brief setUPC: Set Universal Product Code
     * Setting UPC is optional. 12-digit, all-numeric code that identifies the consumer package.
     * Managed by the Uniform Code Council. Specified by UPnP vendor. Single UPC.
     *
     * @param value will be the new UPC of this device
     */
    void setUPC(const QString &value);

    /**
     * @brief UPC: Get Universal Product Code
     * UPC is optional. 12-digit, all-numeric code that identifies the consumer package.
     * Managed by the Uniform Code Council. Specified by UPnP vendor. Single UPC.
     */
    const QString &UPC() const;

    void setDeviceType(const QString &value);

    const QString &deviceType() const;

    void setFriendlyName(const QString &value);

    const QString &friendlyName() const;

    void setManufacturer(const QString &value);

    const QString &manufacturer() const;

    void setManufacturerURL(const QUrl &value);

    const QUrl &manufacturerURL() const;

    void setModelDescription(const QString &value);

    const QString &modelDescription() const;

    void setModelName(const QString &value);

    const QString &modelName() const;

    void setModelNumber(const QString &value);

    const QString &modelNumber() const;

    void setModelURL(const QUrl &value);

    const QUrl &modelURL() const;

    void setSerialNumber(const QString &value);

    const QString &serialNumber() const;

    void setURLBase(const QString &value);

    const QString &URLBase() const;

    void setCacheControl(int value);

    int cacheControl() const;

    void setLocationUrl(const QUrl &value);

    const QUrl &locationUrl() const;

    int addService(const UpnpServiceDescription &newService);

private:
    std::unique_ptr<UpnpDeviceDescriptionPrivate> d;
};

#endif