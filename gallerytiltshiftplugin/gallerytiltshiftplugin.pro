TEMPLATE        = lib
QMAKE_LFLAGS   += -Wl,--as-needed
QMAKE_CXXFLAGS += -Werror -Wall -W
CONFIG         += plugin \
                  meegotouch \
                  gallerycore \
                  quillimagefilter

HEADERS += gallerytiltshiftplugin.h \
           gallerytiltshiftwidget.h \
           gallerytiltshiftwidget_p.h \
           gallerytiltshiftplugin_p.h

SOURCES += gallerytiltshiftplugin.cpp \
           gallerytiltshiftwidget.cpp

target.path = /usr/lib/gallery
INSTALLS += target
