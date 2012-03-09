#ifndef GALLERYTILTSHIFTPLUGIN_H
#define GALLERYTILTSHIFTPLUGIN_H

#include <galleryeditplugin.h>

class GalleryTiltShiftPluginPrivate;

class GalleryTiltShiftPlugin: public GalleryEditPlugin
{
    Q_OBJECT
    Q_INTERFACES(GalleryEditPlugin)

public:
    //! GalleryTiltShiftPlugin constructor
    //! \param parent Optional parent
    GalleryTiltShiftPlugin(QObject* parent = 0);

    //! GalleryTiltShiftPlugin destructor
    virtual ~GalleryTiltShiftPlugin();

    //! \reimp
    QString name() const;

    //! \reimp
    int menuIndex() const;

    //! \reimp
    QString iconID() const;

    //! \reimp
    bool containsUi() const;

    //! \reimp
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

    //! \reimp
    bool receiveMouseEvent(QGraphicsSceneMouseEvent* event);

    //! \reimp
    const QSize toolBarWidgetSize(const M::Orientation& orientation) const;

protected:
    //! \reimp
    QGraphicsWidget* createToolBarWidget(QGraphicsItem* parent = 0);

public Q_SLOTS:
    //! \reimp
    void performEditOperation();

    //! \reimp
    void activate();

private:
    Q_DISABLE_COPY(GalleryTiltShiftPlugin)
    Q_DECLARE_PRIVATE(GalleryTiltShiftPlugin)
    GalleryTiltShiftPluginPrivate* d_ptr;
};

#endif // GALLERYTILTSHIFTPLUGIN_H

