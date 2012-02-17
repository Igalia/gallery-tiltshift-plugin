#ifndef GALLERYTILTSHIFTWIDGET_P_H
#define GALLERYTILTSHIFTWIDGET_P_H

class MButton;
class MButtonGroup;
class MLayout;
class MLinearLayoutPolicy;
class MSlider;

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
};

#endif // GALLERYTILTSHIFTWIDGET_P_H
