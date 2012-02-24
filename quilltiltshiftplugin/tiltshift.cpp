#include <cmath>
#include "tiltshift.h"
#include "quillimagefilter.h"
#include <QPoint>
#include <QColor>

#define FILTER_NAME_TILTSHIFT "com.igalia.spena.tiltshift"

static const int GAUSSIAN_RADIUS = 52;

TiltShift::TiltShift() :
    m_radius(0),
    m_horizontalEffect(true),
    m_focusPoint(),
    m_gaussianFilter(new Gaussian)
{
    m_gaussianFilter->setOption(QuillImageFilter::Radius, QVariant(5));
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
    QuillImage blurredImage = m_gaussianFilter->apply(image);

    float** mask = maskImage(image);
    QImage resultImage(image.size(), QImage::Format_RGB32);

    for (int x = 0; x < image.size().width(); x ++) {
        for (int y = 0; y < image.size().height(); y ++) {
            QRgb originalPixel = image.pixel(x, y);
            QRgb blurredPixel = blurredImage.pixel(x, y);

            int red = qRed(originalPixel) * mask[x][y] + qRed(blurredPixel) * (1 - mask[x][y]);
            int blue = qBlue(originalPixel) * mask[x][y] + qBlue(blurredPixel) * (1 - mask[x][y]);
            int green = qGreen(originalPixel) * mask[x][y] + qGreen(blurredPixel) * (1 - mask[x][y]);

            QColor result(red, green, blue);
            result.setHsvF(result.hueF(), qMin(1.8 * result.saturationF(), 1.0), result.valueF());

            resultImage.setPixel(QPoint(x, y), result.rgb());
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
    return exp(-(x - x0) * (x - x0) / (x0 * x0 / GAUSSIAN_RADIUS));
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
                    maskImage[x][y] = gaussF(y, updatedFocusPoint.y() - scaledRadius / 2);
                } else if ((y + image.area().top()) >= updatedFocusPoint.y() + scaledRadius / 2) {
                    maskImage[x][y] = gaussF(y, updatedFocusPoint.y() + scaledRadius / 2);
                }
            } else {
                if ((x + image.area().left()) < updatedFocusPoint.x() - scaledRadius / 2) {
                    maskImage[x][y] = gaussF(x, updatedFocusPoint.x() - scaledRadius / 2);
                } else if ((x + image.area().left()) >= updatedFocusPoint.x() + scaledRadius / 2) {
                    maskImage[x][y] = gaussF(x, updatedFocusPoint.x() + scaledRadius / 2);
                }
            }
        }
    }

    return maskImage;
}
