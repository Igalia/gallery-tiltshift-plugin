#include "gallerytiltshiftplugin.h"
#include "gallerytiltshiftplugin_p.h"
#include "gallerytiltshiftwidget.h"

#include <galleryedituiprovider.h>
#include <MApplication>
#include <MBanner>
#include <MSceneWindow>
#include <QuillImageFilter>
#include <QGraphicsSceneMouseEvent>

static const int PORTRAIT_HEIGHT        = 216;
static const int LANDSCAPE_HEIGHT       = 156;
static const int TAP_DISTANCE           = 20;
static const int INFO_BANNER_TIMEOUT    = 2000;

GalleryTiltShiftPluginPrivate::GalleryTiltShiftPluginPrivate() :
    m_focusPosition()
{
}

GalleryTiltShiftPluginPrivate::~GalleryTiltShiftPluginPrivate()
{
}

GalleryTiltShiftPlugin::GalleryTiltShiftPlugin(QObject* parent):
    GalleryEditPlugin(parent),
    d_ptr(new GalleryTiltShiftPluginPrivate())
{
}


GalleryTiltShiftPlugin::~GalleryTiltShiftPlugin()
{
    delete d_ptr;
}


QString GalleryTiltShiftPlugin::name() const
{
    return QString("Tilt Shift");
}

int GalleryTiltShiftPlugin::menuIndex() const
{
    return 10;
}

QString GalleryTiltShiftPlugin::iconID() const
{
    // TODO: use the icon id for an actual icon
    return QString("icon-m-missing-icon");
}

bool GalleryTiltShiftPlugin::containsUi() const
{
    return true;
}

QGraphicsWidget* GalleryTiltShiftPlugin::createToolBarWidget(QGraphicsItem* parent)
{
    GalleryTiltShiftWidget* pluginWidget = new GalleryTiltShiftWidget(parent);
    connect(pluginWidget, SIGNAL(applicationOrientationChanged()),
            SLOT(performEditOperation()));
    connect(pluginWidget, SIGNAL(sliderValueChanged()),
            SLOT(performEditOperation()));
    return pluginWidget;
}

bool GalleryTiltShiftPlugin::receiveMouseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event &&
            event->type() == QEvent::GraphicsSceneMouseRelease &&
            event->button() == Qt::LeftButton &&
            (event->scenePos() - event->buttonDownScenePos(Qt::LeftButton)).manhattanLength() < TAP_DISTANCE) {
        Q_D(GalleryTiltShiftPlugin);
        d->m_focusPosition.setX(event->pos().toPoint().x());
        d->m_focusPosition.setY(event->pos().toPoint().y());
        performEditOperation();
        return true;
    }
    return false;
}

const QSize GalleryTiltShiftPlugin::toolBarWidgetSize(const M::Orientation& orientation) const
{
    QSize size = GalleryEditPlugin::toolBarWidgetSize(orientation);

    if (M::Portrait == orientation) {
        size.setHeight(PORTRAIT_HEIGHT);
    } else {
        size.setHeight(LANDSCAPE_HEIGHT);
    }

    return size;
}

void GalleryTiltShiftPlugin::performEditOperation()
{
    if (editUiProvider()) {
        Q_D(GalleryTiltShiftPlugin);
        GalleryTiltShiftWidget* widget = static_cast<GalleryTiltShiftWidget*>(toolBarWidget());
        const QPoint imagePoint = editUiProvider()->convertScreenCoordToImageCoord(d->m_focusPosition);
        if (imagePoint != QPoint(-1,-1)) {
            QHash<QuillImageFilter::QuillFilterOption, QVariant> optionHash;
            optionHash.insert(QuillImageFilter::Center,
                              imagePoint);
            optionHash.insert(QuillImageFilter::Radius,
                              QVariant(widget->sliderValue()));
            optionHash.insert(QuillImageFilter::Horizontal,
                              QVariant(widget->applicationOrientation() == Qt::Horizontal));
            // Use QuillImageFilter::Name_Gaussian to try the Gaussian Blur filter
            editUiProvider()->runEditFilter("com.igalia.spena.tiltshift", optionHash);
            emit editOperationPerformed();
        }
    }
}

void GalleryTiltShiftPlugin::activate()
{
    MBanner *infoBanner = new MBanner();
    infoBanner->setTitle("Tap on an area to keep it focused");
    infoBanner->setStyleName("InformationBanner");
    infoBanner->appear(MApplication::activeWindow(), MSceneWindow::DestroyWhenDone);
    connect(this, SIGNAL(deactivated()),
            infoBanner, SLOT(disappear()));
    // This banner should last no more than 2 seconds
    infoBanner->model()->setDisappearTimeout(2000);
}

Q_EXPORT_PLUGIN2(gallerytiltshiftplugin, GalleryTiltShiftPlugin)

