#ifndef GALLERYTILTSHIFTWIDGET_H
#define GALLERYTILTSHIFTWIDGET_H

#include <MLabel>

class GalleryTiltShiftWidgetPrivate;

class GalleryTiltShiftWidget : public MLabel
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
