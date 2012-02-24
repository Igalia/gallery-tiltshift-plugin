TEMPLATE        = lib
QMAKE_LFLAGS   += -Wl,--as-needed
QMAKE_CXXFLAGS += -Werror -Wall -W
CONFIG         += plugin \
                  quillimagefilter

HEADERS += quilltiltshiftplugin.h \
           tiltshift.h \
           gaussian.h

SOURCES += quilltiltshiftplugin.cpp \
           tiltshift.cpp \
           gaussian.cpp

target.path = $$[QT_INSTALL_PLUGINS]/quillimagefilters
INSTALLS += target

