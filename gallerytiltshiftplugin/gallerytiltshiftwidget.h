/*
 * gallerytiltshiftwidget.h
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

#ifndef GALLERYTILTSHIFTWIDGET_H
#define GALLERYTILTSHIFTWIDGET_H

#include <MWidgetController>

class GalleryTiltShiftWidgetPrivate;

class GalleryTiltShiftWidget : public MWidgetController
{
    Q_OBJECT
public:
    //! GalleryTiltShiftWidget constructor
    //! \param parent Optional parent
    GalleryTiltShiftWidget(QGraphicsItem* parent = 0);

    //! GalleryTiltShiftWidget destructor
    virtual ~GalleryTiltShiftWidget();

    //! Get function for slider value
    //! \return Slider value
    int sliderValue() const;

    //! Get function for application orientation
    //! \return Application orientation
    Qt::Orientation applicationOrientation() const;

    //! Get function for slider handle visibility
    //! \return True if visible, otherwise false
    bool isSliderHandleLabelVisible() const;

public Q_SLOTS:
    //! Called when the apply vertically button is clicked
    void onApplyVerticallyButtonClicked();

    //! Called when the apply horizontally button is clicked
    void onApplyHorizontallyButtonClicked();

    //! Called to show slider handle
    void showSliderHandle();

    //! Called to hide slider handle
    void hideSliderHandle();

    //! Called when slider value changes
    //! \param newValue New value of the slider
    void handleValueChanged(int newValue);

    //! Called to set the slider handle text
    //! \param text Text to be shown in the slider handle
    void setSliderHandleText(const QString& text);

Q_SIGNALS:
    //! Emitted when the application orientation changes
    void applicationOrientationChanged();

    //! Emitted when the slider value changes
    void sliderValueChanged();

    //! Emitted when the slider handle visibility changes
    void sliderHandleVisibilityChanged();

private:
    Q_DISABLE_COPY(GalleryTiltShiftWidget)
    Q_DECLARE_PRIVATE(GalleryTiltShiftWidget)
    GalleryTiltShiftWidgetPrivate* d_ptr;
};

#endif // GALLERYTILTSHIFTWIDGET_H
