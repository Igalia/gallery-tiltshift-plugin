/****************************************************************************
**
** Copyright (C) 2009-11 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Pekka Marjola <pekka.marjola@nokia.com>
**
** This file is part of the Quill Image Filters package.
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/
#include <cmath>
#include "quillimagefilter.h"
#include "gaussian.h"


Gaussian::Gaussian() : radius(0), kernel(0),
                       kernelRadius(0), kernelSize(0), kernelSum(0)
{
}

Gaussian::~Gaussian()
{
    delete[] kernel;
}

bool Gaussian::setOption(const QString &option, const QVariant &value)
{
    bool bOK = true;

    if (option == QuillImageFilter::Radius) {
        float oldRadius = radius;
        radius = value.toDouble();
        if (radius != oldRadius)
            createKernel(radius);
    }
    else
        bOK = false;

    return bOK;
}

QVariant Gaussian::option(const QString &option) const
{
    if (option == QuillImageFilter::Radius)
        return QVariant(radius);
    else
        return QVariant();
}

const QStringList Gaussian::supportedOptions() const
{
    return QStringList() << QuillImageFilter::Radius;
}

int Gaussian::gaussianFunction(int x, int y, float radius)
{
    return exp(-(x*x+y*y)/2.0/radius/radius)*256;
}

void Gaussian::createKernel(float radius)
{
    delete[] kernel;
    kernelRadius = ceil(radius*3);
    kernelSize = kernelRadius*2+1;
    kernelSum = 0;

    if (kernelSize > 0) {
        kernel = new int[kernelSize];
        for (int i=-kernelRadius; i<=kernelRadius; i++) {
            int g = gaussianFunction(i, 0, radius);
            kernel[i+kernelRadius] = g;
            kernelSum += g;
        }
    }
}

int Gaussian::kernelValue(int x) const
{
    return kernel[x+kernelRadius];
}

void Gaussian::cap(int *value)
{
    if (*value < 0)
        *value = 0;
    else if (*value > 255)
        *value = 255;
}

QuillImage Gaussian::apply(const QuillImage &image) const
{
    // No kernel/image, no operation
    if ((kernelRadius <= 0) || image.isNull())
        return image;

    QImage passImage(image.size(), QImage::Format_RGB32);
    QImage targetImage(image.size(), QImage::Format_RGB32);

    // First pass in X direction

    for (int y=0; y<image.height(); y++)
        for (int x=0; x<image.width(); x++) {
            int red = 0, green = 0, blue = 0;

            for (int i=-kernelRadius; i<=kernelRadius; i++) {
                int sourceX = x + i;
                if (sourceX < 0)
                    sourceX = 0;
                else if (sourceX >= image.width())
                    sourceX = image.width() - 1;

                QRgb pixel = image.pixel(sourceX, y);

                int g = kernelValue(i);

                red += g*qRed(pixel);
                green += g*qGreen(pixel);
                blue += g*qBlue(pixel);
            }

            red /= kernelSum;
            green /= kernelSum;
            blue /= kernelSum;

            cap(&red);
            cap(&green);
            cap(&blue);

            passImage.setPixel(QPoint(x, y), qRgb(red, green, blue));
        }

    // Second pass in Y direction

    for (int y=0; y<passImage.height(); y++)
        for (int x=0; x<passImage.width(); x++) {
            int red = 0, green = 0, blue = 0;

            for (int j=-kernelRadius; j<=kernelRadius; j++) {
                int sourceY = y + j;
                if (sourceY < 0)
                    sourceY = 0;
                else if (sourceY >= image.height())
                    sourceY = image.height() - 1;

                QRgb pixel = passImage.pixel(x, sourceY);

                int g = kernelValue(j);

                red += g*qRed(pixel);
                green += g*qGreen(pixel);
                blue += g*qBlue(pixel);
            }

            red /= kernelSum;
            green /= kernelSum;
            blue /= kernelSum;

            cap(&red);
            cap(&green);
            cap(&blue);

            targetImage.setPixel(QPoint(x, y), qRgb(red, green, blue));
        }

    return QuillImage(image, targetImage);
}
