#include "quilltiltshiftplugin.h"
#include "tiltshift.h"

#define FILTER_NAME_TILTSHIFT "com.igalia.spena.tiltshift"

QuillTiltShiftPlugin::QuillTiltShiftPlugin():
    QObject()
{
}

QuillImageFilterImplementation*
QuillTiltShiftPlugin::create(const QString& name)
{
    if (name == FILTER_NAME_TILTSHIFT) {
        return new TiltShift;
    } else {
        return 0;
    }
}

const QStringList QuillTiltShiftPlugin::name() const
{
    return QStringList() << FILTER_NAME_TILTSHIFT;
}

Q_EXPORT_PLUGIN2(quilltiltshiftplugin, QuillTiltShiftPlugin)
