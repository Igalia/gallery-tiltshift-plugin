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

  Domain based name: com.igalia.spena.tiltshift

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
