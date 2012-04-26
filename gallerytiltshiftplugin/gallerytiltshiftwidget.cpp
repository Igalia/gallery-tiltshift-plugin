/*
 * gallerytiltshiftwidget.cpp
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

#include "gallerytiltshiftwidget.h"
#include "gallerytiltshiftwidget_p.h"
#include "gallerytiltshiftaboutwidget.h"

#include <MApplication>
#include <MButton>
#include <MButtonGroup>
#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MSeparator>
#include <MSlider>
#include <MWidgetController>
#include <MWidgetCreator>
#include <QGraphicsLinearLayout>

M_REGISTER_WIDGET_NO_CREATE(GalleryTiltShiftWidget)

GalleryTiltShiftWidgetPrivate::GalleryTiltShiftWidgetPrivate() :
    m_applyHorizontallyButton(0),
    m_applyVerticallyButton(0),
    m_buttonGroup(new MButtonGroup),
    m_slider(0),
    m_orientationLayout(new MLayout),
    m_landscapePolicy(new MLinearLayoutPolicy(m_orientationLayout, Qt::Horizontal)),
    m_portraitPolicy(new MLinearLayoutPolicy(m_orientationLayout, Qt::Vertical)),
    m_aboutSeparator(new MSeparator),
    m_aboutWidget(new GalleryTiltShiftAboutWidget)
{
    m_orientationLayout->setContentsMargins(0, 0, 0, 0);
    m_orientationLayout->setAnimation(0);

    m_landscapePolicy->setContentsMargins(128, 0, 128, 0);
    m_landscapePolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    m_landscapePolicy->setSpacing(0);

    m_portraitPolicy->setContentsMargins(64, 0, 64, 0);
    m_portraitPolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    m_portraitPolicy->setSpacing(0);

    m_orientationLayout->setLandscapePolicy(m_landscapePolicy);
    m_orientationLayout->setPortraitPolicy(m_portraitPolicy);

    m_aboutSeparator->setStyleName("CommonHeaderDividerInverted");
}

GalleryTiltShiftWidgetPrivate::~GalleryTiltShiftWidgetPrivate()
{
    delete m_applyHorizontallyButton;
    delete m_applyVerticallyButton;
    delete m_buttonGroup;
    delete m_slider;
    delete m_portraitPolicy;
    delete m_landscapePolicy;
    delete m_orientationLayout;
    delete m_aboutSeparator;
    delete m_aboutWidget;
}

MButton* GalleryTiltShiftWidgetPrivate::addButton(const QString& label,
                                                  const QString& objectName,
                                                  const QString& styleName)
{
    MButton* button = new MButton(label);
    button->setObjectName(objectName);
    button->setCheckable(true);
    button->setStyleName(styleName);

    m_buttonGroup->addButton(button);
    m_landscapePolicy->addItem(button);
    m_portraitPolicy->addItem(button, Qt::AlignCenter);

    return button;
}

GalleryTiltShiftWidget::GalleryTiltShiftWidget(QGraphicsItem* parent) :
    MWidgetController(parent),
    d_ptr(new GalleryTiltShiftWidgetPrivate)
{
    setObjectName("GalleryTiltShiftWidget");
    setStyleName("ImageEditorBcControl");
    setContentsMargins(0, 0, 0, 0);

    Q_D(GalleryTiltShiftWidget);
    d->m_slider = new MSlider();
    d->m_slider->setStyleName("CommonSliderInverted");
    d->m_slider->setObjectName("TiltShiftAreaSlider");
    d->m_slider->setRange(5, 30);
    d->m_slider->setValue(15);

    connect(d->m_slider, SIGNAL(valueChanged(int)),
            SLOT(handleValueChanged(int)));
    connect(d->m_slider, SIGNAL(sliderPressed()),
            SLOT(showSliderHandle()));
    connect(d->m_slider, SIGNAL(sliderReleased()),
            SLOT(hideSliderHandle()));

    d->m_applyHorizontallyButton = d->addButton("Apply Horizontally",
                                                "ApplyHorizontallyButton",
                                                "CommonTopSplitButtonInverted");

    d->m_applyVerticallyButton = d->addButton("Apply Vertically",
                                              "ApplyVerticallyButton",
                                              "CommonBottomSplitButtonInverted");

    // TODO: Is there another way to have a MButton in a MButtonGroup selected?
    d->m_applyHorizontallyButton->click();

    connect(d->m_applyHorizontallyButton, SIGNAL(clicked()),
            SLOT(onApplyHorizontallyButtonClicked()));
    connect(d->m_applyVerticallyButton, SIGNAL(clicked()),
            SLOT(onApplyVerticallyButtonClicked()));
    connect(d->m_aboutWidget, SIGNAL(linkActivated(QString)),
            SIGNAL(aboutLinkActivated(QString)));

    QGraphicsLinearLayout* mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addItem(d->m_slider);
    mainLayout->setAlignment(d->m_slider, Qt::AlignCenter);
    mainLayout->addItem(d->m_orientationLayout);
    mainLayout->setAlignment(d->m_orientationLayout, Qt::AlignCenter);
    mainLayout->addItem(d->m_aboutSeparator);
    mainLayout->addItem(d->m_aboutWidget);

    setLayout(mainLayout);

    // Make sure initial policy is correct
    if (MApplication::activeWindow()) {
        if (MApplication::activeWindow()->orientation() == M::Portrait) {
            d->m_portraitPolicy->activate();
        } else {
            d->m_landscapePolicy->activate();
        }
    } else {
        d->m_landscapePolicy->activate();
    }
}

GalleryTiltShiftWidget::~GalleryTiltShiftWidget()
{
    delete d_ptr;
    d_ptr = 0;
}

int GalleryTiltShiftWidget::sliderValue() const
{
    Q_D(const GalleryTiltShiftWidget);
    return d->m_slider->value();
}

bool GalleryTiltShiftWidget::isSliderHandleLabelVisible() const
{
    Q_D(const GalleryTiltShiftWidget);
    return d->m_slider->isHandleLabelVisible();
}

Qt::Orientation GalleryTiltShiftWidget::applicationOrientation() const
{
    Q_D(const GalleryTiltShiftWidget);
    if (d->m_buttonGroup->checkedButton() == d->m_applyHorizontallyButton) {
        return Qt::Horizontal;
    } else {
        return Qt::Vertical;
    }
}

void GalleryTiltShiftWidget::onApplyHorizontallyButtonClicked()
{
    emit applicationOrientationChanged();
}

void GalleryTiltShiftWidget::onApplyVerticallyButtonClicked()
{
    emit applicationOrientationChanged();
}

void GalleryTiltShiftWidget::showSliderHandle()
{
    Q_D(GalleryTiltShiftWidget);
    d->m_slider->setHandleLabelVisible(true);
    emit sliderHandleVisibilityChanged();
}

void GalleryTiltShiftWidget::hideSliderHandle()
{
    Q_D(GalleryTiltShiftWidget);
    d->m_slider->setHandleLabelVisible(false);
    emit sliderHandleVisibilityChanged();
    emit sliderValueChanged();
}

void GalleryTiltShiftWidget::handleValueChanged(int newValue)
{
    Q_UNUSED(newValue)
}

void GalleryTiltShiftWidget::setSliderHandleText(const QString& text)
{
    Q_D(GalleryTiltShiftWidget);
    d->m_slider->setHandleLabel(text);
}
