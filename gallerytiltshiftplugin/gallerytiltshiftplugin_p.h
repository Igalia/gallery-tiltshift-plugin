/*
 * gallerytiltshiftplugin_p.h
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

#ifndef GALLERYTILTSHIFTPLUGIN_P_H
#define GALLERYTILTSHIFTPLUGIN_P_H

#include <QPoint>

class GalleryTiltShiftPluginPrivate
{
public:
    //! GalleryTiltShiftPluginPrivate constructor
    GalleryTiltShiftPluginPrivate();

    //! GalleryTiltShiftPluginPrivate destructor
    virtual ~GalleryTiltShiftPluginPrivate();

    //! Focus position
    QPoint m_focusPosition;

    //! Tells whether the Tilt Shift can be applied to the image or not
    bool m_validImage;
};

#endif // GALLERYTILTSHIFTPLUGIN_P_H
