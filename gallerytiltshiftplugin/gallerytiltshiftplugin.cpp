/*
 * gallerytiltshiftplugin.cpp
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

#include "gallerytiltshiftplugin.h"
#include "gallerytiltshiftplugin_p.h"
#include "gallerytiltshiftwidget.h"

#include <galleryedituiprovider.h>
#include <MApplication>
#include <MBanner>
#include <MSceneWindow>
#include <QuillImageFilter>
#include <QGraphicsSceneMouseEvent>

static const int PORTRAIT_HEIGHT        = 216;
static const int LANDSCAPE_HEIGHT       = 156;
static const int TAP_DISTANCE           = 20;
static const int INFO_BANNER_TIMEOUT    = 2000;

GalleryTiltShiftPluginPrivate::GalleryTiltShiftPluginPrivate() :
    m_focusPosition()
{
}

GalleryTiltShiftPluginPrivate::~GalleryTiltShiftPluginPrivate()
{
}

GalleryTiltShiftPlugin::GalleryTiltShiftPlugin(QObject* parent):
    GalleryEditPlugin(parent),
    d_ptr(new GalleryTiltShiftPluginPrivate())
{
}


GalleryTiltShiftPlugin::~GalleryTiltShiftPlugin()
{
    delete d_ptr;
}


QString GalleryTiltShiftPlugin::name() const
{
    return QString("Tilt Shift");
}

int GalleryTiltShiftPlugin::menuIndex() const
{
    return 10;
}

QString GalleryTiltShiftPlugin::iconID() const
{
    // TODO: use the icon id for an actual icon
    return QString("icon-m-missing-icon");
}

bool GalleryTiltShiftPlugin::containsUi() const
{
    return true;
}

void GalleryTiltShiftPlugin::paint(QPainter* painter,
                                   const QStyleOptionGraphicsItem* option,
                                   QWidget* widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (editUiProvider() && toolBarWidget()) {
        GalleryTiltShiftWidget* widget = static_cast<GalleryTiltShiftWidget*>(toolBarWidget());
        if (widget->isSliderHandleLabelVisible()) {
            const QString text = QString("%L1").arg(widget->sliderValue());
            widget->setSliderHandleText(text);
        }
    }
}

QGraphicsWidget* GalleryTiltShiftPlugin::createToolBarWidget(QGraphicsItem* parent)
{
    GalleryTiltShiftWidget* pluginWidget = new GalleryTiltShiftWidget(parent);
    connect(pluginWidget, SIGNAL(applicationOrientationChanged()),
            SLOT(performEditOperation()));
    connect(pluginWidget, SIGNAL(sliderValueChanged()),
            SLOT(performEditOperation()));
    return pluginWidget;
}

bool GalleryTiltShiftPlugin::receiveMouseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event &&
            event->type() == QEvent::GraphicsSceneMouseRelease &&
            event->button() == Qt::LeftButton &&
            (event->scenePos() - event->buttonDownScenePos(Qt::LeftButton)).manhattanLength() < TAP_DISTANCE) {
        Q_D(GalleryTiltShiftPlugin);
        d->m_focusPosition.setX(event->pos().toPoint().x());
        d->m_focusPosition.setY(event->pos().toPoint().y());
        performEditOperation();
        return true;
    }
    return false;
}

const QSize GalleryTiltShiftPlugin::toolBarWidgetSize(const M::Orientation& orientation) const
{
    QSize size = GalleryEditPlugin::toolBarWidgetSize(orientation);

    if (M::Portrait == orientation) {
        size.setHeight(PORTRAIT_HEIGHT);
    } else {
        size.setHeight(LANDSCAPE_HEIGHT);
    }

    return size;
}

void GalleryTiltShiftPlugin::performEditOperation()
{
    if (editUiProvider()) {
        Q_D(GalleryTiltShiftPlugin);
        GalleryTiltShiftWidget* widget = static_cast<GalleryTiltShiftWidget*>(toolBarWidget());
        const QPoint imagePoint = editUiProvider()->convertScreenCoordToImageCoord(d->m_focusPosition);
        if (imagePoint != QPoint(-1,-1)) {
            QHash<QuillImageFilter::QuillFilterOption, QVariant> optionHash;
            optionHash.insert(QuillImageFilter::Center,
                              imagePoint);
            optionHash.insert(QuillImageFilter::Radius,
                              QVariant(widget->sliderValue()));
            optionHash.insert(QuillImageFilter::Horizontal,
                              QVariant(widget->applicationOrientation() == Qt::Horizontal));
            // Use QuillImageFilter::Name_Gaussian to try the Gaussian Blur filter
            editUiProvider()->runEditFilter("com.igalia.spena.tiltshift", optionHash, true);
            emit editOperationPerformed();
        }
    }
}

void GalleryTiltShiftPlugin::activate()
{
    MBanner *infoBanner = new MBanner();
    infoBanner->setTitle("Tap on an area to keep it focused");
    infoBanner->setStyleName("InformationBanner");
    infoBanner->appear(MApplication::activeWindow(), MSceneWindow::DestroyWhenDone);
    connect(this, SIGNAL(deactivated()),
            infoBanner, SLOT(disappear()));
    // This banner should last no more than 2 seconds
    infoBanner->model()->setDisappearTimeout(2000);
}

Q_EXPORT_PLUGIN2(gallerytiltshiftplugin, GalleryTiltShiftPlugin)

