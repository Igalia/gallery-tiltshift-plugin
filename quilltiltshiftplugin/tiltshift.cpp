#include "tiltshift.h"

#define FILTER_NAME_TILTSHIFT "com.igalia.spena.tiltshift"

TiltShift::TiltShift() :
    m_radius(0),
    m_horizontalEffect(true),
    m_focusPoint()
{
}

TiltShift::~TiltShift()
{
}

QuillImage TiltShift::apply(const QuillImage& image) const
{
    QuillImage result(image);

    QRgb* endp(reinterpret_cast<QRgb*>(result.bits() + result.numBytes()));
    for (QRgb *p = reinterpret_cast<QRgb*>(result.bits()); p < endp; p++) {
        int value = (qRed(*p) + qGreen(*p) + qBlue(*p)) / 3;
        *p = qRgba(value, value, value, qAlpha(*p));
    }

    return result;
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
