/*
 * quilltiltshiftplugin.cpp
 *
 * Copyright (C) 2012 Igalia, S.L.
 * Author: Simon Pena <spena@igalia.com>
 *
 * This file is part of the Gallery Tilt Shift Plugin.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see http://www.gnu.org/licenses/ *
 */

#include "quilltiltshiftplugin.h"
#include "tiltshift.h"

#define FILTER_NAME_TILTSHIFT "com.igalia.tiltshift"

QuillTiltShiftPlugin::QuillTiltShiftPlugin():
    QObject()
{
}

QuillImageFilterImplementation*
QuillTiltShiftPlugin::create(const QString& name)
{
    if (name == FILTER_NAME_TILTSHIFT) {
        return new TiltShift;
    } else {
        return 0;
    }
}

const QStringList QuillTiltShiftPlugin::name() const
{
    return QStringList() << FILTER_NAME_TILTSHIFT;
}

Q_EXPORT_PLUGIN2(quilltiltshiftplugin, QuillTiltShiftPlugin)
