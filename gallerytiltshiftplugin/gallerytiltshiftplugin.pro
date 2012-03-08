TEMPLATE        = lib
QMAKE_LFLAGS   += -Wl,--as-needed
QMAKE_CXXFLAGS += -Werror -Wall -W
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -fvisibility=hidden
QMAKE_CXXFLAGS_DEBUG += -O0 -ggdb
CONFIG         += plugin \
                  meegotouch \
                  gallerycore \
                  quillimagefilter

contains( debug, yes ) {
    message( "Configuring for debug build ..." )
    CONFIG += debug warn_on
} else {
    message( "Configuring for release build ..." )
    CONFIG += release warn_on
    DEFINES += QT_NO_DEBUG_OUTPUT
}

HEADERS += gallerytiltshiftplugin.h \
           gallerytiltshiftwidget.h \
           gallerytiltshiftwidget_p.h \
           gallerytiltshiftplugin_p.h

SOURCES += gallerytiltshiftplugin.cpp \
           gallerytiltshiftwidget.cpp

OTHER_FILES += \
           conf/libgallerytiltshiftplugin.css

target.path = /usr/lib/gallery

include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_defines.prf)
meegotouch.files = conf/libgallerytiltshiftplugin.css
meegotouch.path = $${M_THEME_DIR}/base/meegotouch/libgallerytiltshiftplugin/style

INSTALLS += target meegotouch
