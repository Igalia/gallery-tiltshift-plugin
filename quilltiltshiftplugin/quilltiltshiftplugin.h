#ifndef QUILLTILTSHIFTPLUGIN_H
#define QUILLTILTSHIFTPLUGIN_H

#include <QuillImageFilter>
#include <QuillImageFilterInterface>
#include <QuillImageFilterImplementation>

class QuillTiltShiftPlugin:
    public QObject,
    public QuillImageFilterInterface
{
    Q_OBJECT
    Q_INTERFACES(QuillImageFilterInterface)

public:
    //! QuillTiltShiftPlugin constructor
    QuillTiltShiftPlugin();

    //! \reimp
    QuillImageFilterImplementation* create(const QString& name);

    //! \reimp
    const QStringList name() const;

private:
    Q_DISABLE_COPY(QuillTiltShiftPlugin)
};

#endif // QUILLTILTSHIFTPLUGIN_H

