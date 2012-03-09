/*
 * tiltshift.cpp
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

#include "tiltshift.h"
#include <cmath>
#include <QuillImageFilter>
#include <QPoint>
#include <QColor>

#define FILTER_NAME_TILTSHIFT "com.igalia.tiltshift"

static const int GRADIENT_GAUSSIAN_RADIUS = 52;
static const int GAUSSIAN_BLUR_RADIUS_THUMBNAIL = 2;
static const int GAUSSIAN_BLUR_RADIUS_PREVIEW = 5;
static const int GAUSSIAN_BLUR_RADIUS_TILE = 21;
static const double SATURATION_FACTOR = 1.5;

TiltShift::TiltShift() :
    m_radius(0),
    m_horizontalEffect(true),
    m_focusPoint(),
    m_gaussianFilter(new Gaussian)
{
}

TiltShift::~TiltShift()
{
    delete m_gaussianFilter;
}

QuillImage TiltShift::apply(const QuillImage& image) const
{
    if (image.isNull()) {
        return image;
    }
    if (image.isFragment()) {
        m_gaussianFilter->setOption(QuillImageFilter::Radius, QVariant(GAUSSIAN_BLUR_RADIUS_TILE));
    } else if (image.size() == QSize(170, 170)) {
        m_gaussianFilter->setOption(QuillImageFilter::Radius, QVariant(GAUSSIAN_BLUR_RADIUS_THUMBNAIL));
    } else {
        m_gaussianFilter->setOption(QuillImageFilter::Radius, QVariant(GAUSSIAN_BLUR_RADIUS_PREVIEW));
    }
    QuillImage blurredImage = m_gaussianFilter->apply(image);

    float** mask = maskImage(image);
    QImage resultImage(image.size(), QImage::Format_RGB32);

    for (int y=0; y<image.height(); y++) {
        const QRgb* originalPixelRow = (const QRgb*)image.constScanLine(y);
        const QRgb* blurredPixelRow = (const QRgb*)blurredImage.constScanLine(y);
        QRgb* resultPixelRow = (QRgb*)resultImage.scanLine(y);

        for (int x=0; x<image.width(); x++) {
            int red = qRed(originalPixelRow[x]) * mask[x][y] + qRed(blurredPixelRow[x]) * (1 - mask[x][y]);
            int blue = qBlue(originalPixelRow[x]) * mask[x][y] + qBlue(blurredPixelRow[x]) * (1 - mask[x][y]);
            int green = qGreen(originalPixelRow[x]) * mask[x][y] + qGreen(blurredPixelRow[x]) * (1 - mask[x][y]);
            QColor result(red, green, blue);
            result.setHsvF(result.hueF(), qMin(SATURATION_FACTOR * result.saturationF(), 1.0), result.valueF());

            resultPixelRow[x] = result.rgb();
        }
    }

    for (int i = 0; i < image.size().width(); i ++) {
        delete[] mask[i];
    }

    delete[] mask;

    return QuillImage(image, resultImage);
}

bool TiltShift::setOption(const QString& filterOption, const QVariant& value)
{
    bool result = true;
    if (filterOption == QuillImageFilter::Radius) {
        m_radius = value.toInt();
    } else if (filterOption == QuillImageFilter::Horizontal) {
        m_horizontalEffect = value.toBool();
    } else if (filterOption == QuillImageFilter::Center) {
        m_focusPoint = value.toPoint();
    } else {
        result = false;
    }

    return result;
}

QVariant TiltShift::option(const QString& filterOption) const
{
    if (filterOption == QuillImageFilter::Radius) {
        return QVariant(m_radius);
    } else if (filterOption == QuillImageFilter::Horizontal) {
        return QVariant(m_horizontalEffect);
    } else if (filterOption == QuillImageFilter::Center) {
        return QVariant(m_focusPoint);
    }
    return QVariant();
}

const QStringList TiltShift::supportedOptions() const
{
    QStringList supportedOptions;
    supportedOptions << QuillImageFilter::Radius
                     << QuillImageFilter::Center
                     << QuillImageFilter::Horizontal;
    return supportedOptions;
}

const QString TiltShift::name() const
{
    return FILTER_NAME_TILTSHIFT;
}

float TiltShift::gaussF(float x, float x0) const
{
    return exp(-(x - x0) * (x - x0) / (x0 * x0 / GRADIENT_GAUSSIAN_RADIUS));
}

float** TiltShift::maskImage(const QuillImage& image) const
{
    float** maskImage = new float*[image.size().width()];

    for (int i = 0; i < image.size().width(); i ++) {
        maskImage[i] = new float[image.size().height()];
    }

    QPointF updatedFocusPoint(m_focusPoint);
    int scaledRadius = m_radius * 0.01 * (m_horizontalEffect ?
                                              image.fullImageSize().height() :
                                              image.fullImageSize().width());

    if (!image.isFragment()) {
        // Scale point and radius
        updatedFocusPoint.setX(m_focusPoint.x() *
                               image.size().width() /
                               image.fullImageSize().width());
        updatedFocusPoint.setY(m_focusPoint.y() *
                               image.size().height() /
                               image.fullImageSize().height());
        scaledRadius = m_radius * 0.01 * (m_horizontalEffect ?
                                              image.size().height() :
                                              image.size().width());
    }


    for (int x = 0; x < image.size().width(); x ++) {
        for (int y = 0; y < image.size().height(); y ++) {
            maskImage[x][y] = 1.0;
            if (m_horizontalEffect) {
                if ((y + image.area().top()) < updatedFocusPoint.y() - scaledRadius / 2) {
                    maskImage[x][y] = gaussF(y + image.area().top(),
                                             updatedFocusPoint.y() - scaledRadius / 2);
                } else if ((y + image.area().top()) >= updatedFocusPoint.y() + scaledRadius / 2) {
                    maskImage[x][y] = gaussF(y + image.area().top(),
                                             updatedFocusPoint.y() + scaledRadius / 2);
                }
            } else {
                if ((x + image.area().left()) < updatedFocusPoint.x() - scaledRadius / 2) {
                    maskImage[x][y] = gaussF(x + image.area().left(),
                                             updatedFocusPoint.x() - scaledRadius / 2);
                } else if ((x + image.area().left()) >= updatedFocusPoint.x() + scaledRadius / 2) {
                    maskImage[x][y] = gaussF(x + image.area().left(),
                                             updatedFocusPoint.x() + scaledRadius / 2);
                }
            }
        }
    }

    return maskImage;
}
