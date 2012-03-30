#ifndef GALLERYTILTSHIFTABOUTWIDGET_H
#define GALLERYTILTSHIFTABOUTWIDGET_H

#include <MWidgetController>

class MLabel;

class GalleryTiltShiftAboutWidget : public MWidgetController
{
    Q_OBJECT
public:
    GalleryTiltShiftAboutWidget();

    virtual ~GalleryTiltShiftAboutWidget();

protected:
    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

Q_SIGNALS:
    void mousePressed();
    void linkActivated(const QString& link);

private:
    MLabel* m_aboutLabel;
    MLabel* m_disclaimerLabel;
};


#endif // GALLERYTILTSHIFTABOUTWIDGET_H
