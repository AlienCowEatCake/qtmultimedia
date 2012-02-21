load(qt_module)

TARGET = qtmedia_pulse
QT += multimedia-private
PLUGIN_TYPE = audio

load(qt_plugin)
DESTDIR = $$QT.multimedia.plugins/$${PLUGIN_TYPE}

CONFIG += link_pkgconfig
PKGCONFIG += libpulse

HEADERS += qpulseaudioplugin.h \
           qaudiodeviceinfo_pulse.h \
           qaudiooutput_pulse.h \
           qaudioinput_pulse.h \
           qpulseaudioengine.h \
           qpulsehelpers.h

SOURCES += qpulseaudioplugin.cpp \
           qaudiodeviceinfo_pulse.cpp \
           qaudiooutput_pulse.cpp \
           qaudioinput_pulse.cpp \
           qpulseaudioengine.cpp \
           qpulsehelpers.cpp

target.path += $$[QT_INSTALL_PLUGINS]/$${PLUGIN_TYPE}
INSTALLS += target
