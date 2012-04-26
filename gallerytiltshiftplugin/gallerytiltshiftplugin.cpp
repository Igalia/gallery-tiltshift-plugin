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
#include <MMessageBox>
#include <MBanner>
#include <MLabel>
#include <MLibrary>
#include <MSceneWindow>
#include <QuillImageFilter>
#include <QDesktopServices>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QUrl>
#include <QTextOption>

static const int PORTRAIT_HEIGHT        = 224;
static const int LANDSCAPE_HEIGHT       = 170;
static const int TAP_DISTANCE           = 20;
static const int INFO_BANNER_TIMEOUT    = 2000;

M_LIBRARY

GalleryTiltShiftPluginPrivate::GalleryTiltShiftPluginPrivate() :
    m_focusPosition(),
    m_validImage(true)
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
    // XXX: Gallery plugins load their icons by ID, so they must be placed in a
    // location watched by the MThemeDaemon. However, it seems that the MThemeDaemon
    // only re-scans its content when booting. As a consequence, the red icon
    // corresponding to a missing resource will appear until the device is rebooted.
    return QString("icon-m-image-edit-tilt-shift");
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
    connect(pluginWidget, SIGNAL(aboutLinkActivated(QString)),
            SLOT(onAboutLinkActivated(QString)));
    return pluginWidget;
}

bool GalleryTiltShiftPlugin::receiveMouseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event &&
            event->type() == QEvent::GraphicsSceneMouseRelease &&
            event->button() == Qt::LeftButton &&
            (event->scenePos() - event->buttonDownScenePos(Qt::LeftButton)).manhattanLength() < TAP_DISTANCE) {
        Q_D(GalleryTiltShiftPlugin);
        if (d->m_validImage) {
            d->m_focusPosition.setX(event->pos().toPoint().x());
            d->m_focusPosition.setY(event->pos().toPoint().y());
            performEditOperation();
            return true;
        } else {
            showInfoBanner("Plugin disabled for this image size");
        }
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
            editUiProvider()->runEditFilter("com.igalia.tiltshift", optionHash, true);
            emit editOperationPerformed();
        }
    }
}

void GalleryTiltShiftPlugin::activate()
{
    if (editUiProvider()) {
        Q_D(GalleryTiltShiftPlugin);
        d->m_validImage = editUiProvider()->fullImageSize().height() <= 512 &&
                editUiProvider()->fullImageSize().width() <= 512;
        if (d->m_validImage) {
            showInfoBanner("Tap on an area to keep it focused");
        } else {
            showMessageBox("Tilt Shift plugin limitations",
                           "Gallery Tilt Shift plugin is currently limited to "
                           "small images (512x512)<br />"
                           "For a given image:"
                           "<ol>"
                           "<li>Scale it or crop it</li>"
                           "<li>Save it with a different name</li>"
                           "<li>Apply the filter to the new one</li>"
                           "</ol>");
            GalleryTiltShiftWidget* widget = static_cast<GalleryTiltShiftWidget*>(toolBarWidget());
            widget->setEnabled(d->m_validImage);
        }
    }
}

void GalleryTiltShiftPlugin::onAboutLinkActivated(const QString &link)
{
    Q_UNUSED(link)
    if (link.toLower().startsWith("http") || link.toLower().startsWith("mailto")) {
        QDesktopServices::openUrl(QUrl(link));
    } else {
        showMessageBox("Tilt Shift plugin - 0.1.0",
                       "Copyright (c) 2012 Igalia S.L."
                       "<br /><br />"
                       "<a href=\"mailto:spena@igalia.com\">spena@igalia.com</a> | "
                       "<a href=\"http://www.igalia.com\">www.igalia.com</a>"
                       "<br /><br />"
                       "This library is free software; you can redistribute it and/or "
                       "modify it under the terms of the GNU Lesser General Public License "
                       "as published by the Free Software Foundation; version 2.1 of "
                       "the License, or (at your option) any later version.");
    }
}

MMessageBox* GalleryTiltShiftPlugin::showMessageBox(const QString& title, const QString& text) const
{
    MMessageBox* messageBox = new MMessageBox(title, "");
    MLabel* innerLabel = new MLabel(messageBox);
    innerLabel->setWordWrap(true);
    innerLabel->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    innerLabel->setStyleName("CommonQueryText");
    innerLabel->setText(text);
    innerLabel->setAlignment(Qt::AlignHCenter);
    messageBox->setCentralWidget(innerLabel);

    connect(innerLabel, SIGNAL(linkActivated(QString)),
            this, SLOT(onAboutLinkActivated(QString)));
    connect(this, SIGNAL(deactivated()),
            messageBox, SLOT(disappear()));

    messageBox->appear(MSceneWindow::DestroyWhenDone);

    return messageBox;
}

MBanner* GalleryTiltShiftPlugin::showInfoBanner(const QString& title) const
{
    MBanner *infoBanner = new MBanner;
    infoBanner->setTitle(title);
    infoBanner->setStyleName(MBannerType::InformationBanner);
    infoBanner->model()->setDisappearTimeout(2000);
    connect(this, SIGNAL(deactivated()),
            infoBanner, SLOT(disappear()));

    infoBanner->appear(MApplication::activeWindow(), MSceneWindow::DestroyWhenDone);

    return infoBanner;
}

Q_EXPORT_PLUGIN2(gallerytiltshiftplugin, GalleryTiltShiftPlugin)

