/*
 * gallerytiltshiftplugin.h
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

#ifndef GALLERYTILTSHIFTPLUGIN_H
#define GALLERYTILTSHIFTPLUGIN_H

#include <galleryeditplugin.h>

class MMessageBox;
class GalleryTiltShiftPluginPrivate;

class GalleryTiltShiftPlugin: public GalleryEditPlugin
{
    Q_OBJECT
    Q_INTERFACES(GalleryEditPlugin)

public:
    //! GalleryTiltShiftPlugin constructor
    //! \param parent Optional parent
    GalleryTiltShiftPlugin(QObject* parent = 0);

    //! GalleryTiltShiftPlugin destructor
    virtual ~GalleryTiltShiftPlugin();

    //! \reimp
    QString name() const;

    //! \reimp
    int menuIndex() const;

    //! \reimp
    QString iconID() const;

    //! \reimp
    bool containsUi() const;

    //! \reimp
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

    //! \reimp
    bool receiveMouseEvent(QGraphicsSceneMouseEvent* event);

    //! \reimp
    const QSize toolBarWidgetSize(const M::Orientation& orientation) const;

protected:
    //! \reimp
    QGraphicsWidget* createToolBarWidget(QGraphicsItem* parent = 0);

public Q_SLOTS:
    //! \reimp
    void performEditOperation();

    //! \reimp
    void activate();

    //! Invoked when a link in the about notice has been activated
    //! \param link The activated link
    void onAboutLinkActivated(const QString& link);

    //! Shows a message box with a given title and text
    //! \param title Text to be shown as a title in the message box
    //! \param text Text to be shown as the body in the message box
    //! \return The message box
    MMessageBox* showMessageBox(const QString& title, const QString& text) const;

private:
    Q_DISABLE_COPY(GalleryTiltShiftPlugin)
    Q_DECLARE_PRIVATE(GalleryTiltShiftPlugin)
    GalleryTiltShiftPluginPrivate* d_ptr;
};

#endif // GALLERYTILTSHIFTPLUGIN_H

