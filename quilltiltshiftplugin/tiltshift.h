/*
 * tiltshift.h
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

#ifndef TILTSHIFT_H
#define TILTSHIFT_H

/*!
  \class TiltShift

  \brief Applies a fake tilt shift effect to an image

  The algorithm will keep an area focused around a given point, taking
  all the available space in one dimension (vertical or horizontal), and
  a given amount around the point in the other. A gaussian blur filter is
  applied outside the focused area, and then some color enhancement process
  is applied.

  Domain based name: com.igalia.tiltshift

  Options supported: Radius (amount of space to keep focused in the secondary
  orientation), Horizontal (boolean specifying whether to apply the filter
  horizontally or vertically), Center (coordinates of the center of the focused
  area).
*/

#include "gaussian.h"
#include <QuillImageFilterImplementation>

class TiltShift : public QuillImageFilterImplementation
{
public:
    //! TiltShift constructor
    TiltShift();

    //! TiltShift destructor
    virtual ~TiltShift();

    //! \reimp
    QuillImage apply(const QuillImage& image) const;

    //! \reimp
    bool setOption(const QString& filterOption, const QVariant& value);

    //! \reimp
    QVariant option(const QString& filterOption) const;

    //! \reimp
    const QStringList supportedOptions() const;

    //! \reimp
    virtual const QString name() const;

private:
    float gaussF(float x, float x0) const;
    float** maskImage(const QuillImage &image) const;

private:
    //! Radius of application around the focused point (in the secondary direction)
    int m_radius;

    //! Whether the effect will be applied horizontally (true) or vertically (false)
    bool m_horizontalEffect;

    //! Coordinates of the point which will act as the center of the focused area
    QPoint m_focusPoint;

    Gaussian* m_gaussianFilter;
};

#endif // TILTSHIFT_H
