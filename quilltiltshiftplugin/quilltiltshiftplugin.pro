TEMPLATE        = lib
QMAKE_LFLAGS   += -Wl,--as-needed
QMAKE_CXXFLAGS += -Werror -Wall -W
CONFIG         += plugin \
                  quillimagefilter

HEADERS += quilltiltshiftplugin.h \
           tiltshift.h

SOURCES += quilltiltshiftplugin.cpp \
           tiltshift.cpp

target.path = $$[QT_INSTALL_PLUGINS]/quillimagefilters
INSTALLS += target

