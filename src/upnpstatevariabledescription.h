/*
   SPDX-FileCopyrightText: 2015 (c) Matthieu Gallien <matthieu_gallien@yahoo.fr>

   SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef UPNPSTATEVARIABLEDESCRIPTION_H
#define UPNPSTATEVARIABLEDESCRIPTION_H

#include "upnplibqt_export.h"

#include <QByteArray>
#include <QString>
#include <QVariant>
#include <QVector>

class QObject;

class UPNPLIBQT_EXPORT UpnpStateVariableDescription
{
public:
    UpnpStateVariableDescription();

    bool mIsValid;

    QString mUpnpName;

    QObject *mObject;

    QByteArray mPropertyName;

    int mPropertyIndex;

    bool mEvented;

    QString mDataType;

    QVariant mDefaultValue;

    QVariant mMinimumValue;

    QVariant mMaximumValue;

    QVariant mStep;

    QVector<QString> mValueList;
};

#endif // UPNPSTATEVARIABLEDESCRIPTION_H