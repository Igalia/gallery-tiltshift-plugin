/*
 * gallerytiltshiftwidget_p.h
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

#ifndef GALLERYTILTSHIFTWIDGET_P_H
#define GALLERYTILTSHIFTWIDGET_P_H

class MButton;
class MButtonGroup;
class MLabel;
class MLayout;
class MLinearLayoutPolicy;
class MSeparator;
class MSlider;
class GalleryTiltShiftAboutWidget;

class GalleryTiltShiftWidgetPrivate
{
public:
    //! GalleryTiltShiftWidgetPrivate constructor
    GalleryTiltShiftWidgetPrivate();

    //! GalleryTiltShiftWidgetPrivate destructor
    virtual ~GalleryTiltShiftWidgetPrivate();

    //! Creates a button with the given label, object name and style name,
    //! and adds it to the MButtonGroup
    //! \param label Label to display on the button
    //! \param objectName Object name for the button
    //! \param styleName Style name for the button
    //! \return Newly created button
    MButton* addButton(const QString& label, const QString& objectName,
                       const QString& styleName);

public:
    //! Button to keep the focused area horizontal
    MButton* m_applyHorizontallyButton;

    //! Button to keep the focused area vertical
    MButton* m_applyVerticallyButton;

    //! Button group for both focus area's orientations
    MButtonGroup* m_buttonGroup;

    //! Slider to choose the focus area radius
    MSlider* m_slider;

    //! Focus area layout
    MLayout* m_orientationLayout;

    //! Landscape policy for the focus area layout
    MLinearLayoutPolicy* m_landscapePolicy;

    //! Portrait policy for the focus area layout
    MLinearLayoutPolicy* m_portraitPolicy;

    //! Separator dividing the widget from the about claim
    MSeparator* m_aboutSeparator;

    //! Widget containing the about disclaimer
    GalleryTiltShiftAboutWidget* m_aboutWidget;
};

#endif // GALLERYTILTSHIFTWIDGET_P_H
