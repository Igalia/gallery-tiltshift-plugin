#ifndef GALLERYTILTSHIFTPLUGIN_P_H
#define GALLERYTILTSHIFTPLUGIN_P_H

#include <QPoint>

class GalleryTiltShiftPluginPrivate
{
public:
    //! GalleryTiltShiftPluginPrivate constructor
    GalleryTiltShiftPluginPrivate();

    //! GalleryTiltShiftPluginPrivate destructor
    virtual ~GalleryTiltShiftPluginPrivate();

    //! Focus position
    QPoint m_focusPosition;
};

#endif // GALLERYTILTSHIFTPLUGIN_P_H
