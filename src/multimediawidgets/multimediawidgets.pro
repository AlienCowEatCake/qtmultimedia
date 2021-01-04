# distinct from Qt Multimedia
TARGET = QtMultimediaWidgets
QT = core gui multimedia widgets-private
QT_PRIVATE += multimedia-private
qtHaveModule(opengl) {
    QT += openglwidgets
    QT_PRIVATE += opengl
}

PRIVATE_HEADERS += \
    qvideowidget_p.h \
    qpaintervideosurface_p.h \

PUBLIC_HEADERS += \
    qtmultimediawidgetdefs.h \
    qvideowidgetcontrol.h \
    qvideowidget.h

SOURCES += \
    qpaintervideosurface.cpp \
    qvideowidgetcontrol.cpp \
    qvideowidget.cpp

qtConfig(graphicsview) {
    SOURCES        += qgraphicsvideoitem.cpp
    PUBLIC_HEADERS += qgraphicsvideoitem.h
}


HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

msvc:lessThan(QMAKE_MSC_VER, 1900): QMAKE_CXXFLAGS += -Zm200

load(qt_module)
