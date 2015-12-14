QT += core gui opengl

TARGET = particleScene123
TEMPLATE = app

CONFIG += c++11

win32 {
    DEFINES += GLEW_STATIC
}

INCLUDEPATH += src glm cs123_lib glew-1.10.0/include
DEPENDPATH += src glm cs123_lib glew-1.10.0/include

SOURCES += src/settings.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/glwidget.cpp \
    src/databinding.cpp \
    cs123_lib/resourceloader.cpp \
    src/openglshape.cpp \
    glew-1.10.0/src/glew.c \
    src/FramebufferObject.cpp \
    src/ParticleSystem.cpp \
    src/FBO.cpp \
    src/cauldron.cpp


HEADERS += \
    src/settings.h \
    src/mainwindow.h \
    src/glwidget.h \
    src/databinding.h \
    cs123_lib/resourceloader.h \
    src/openglshape.h \
    glew-1.10.0/include/GL/glew.h \
    src/FramebufferObject.h \
    src/ParticleSystem.h \
    src/FBO.h \
    src/cauldron.h

FORMS += src/mainwindow.ui

OTHER_FILES += \
    shaders/solid.frag \
    shaders/solid.vert \
    shaders/gradient.frag \
    shaders/gradient.vert \
    shaders/texture.frag \
    shaders/texture.vert \
    shaders/update.frag\
    shaders/draw.frag \
    shaders/draw.vert \
    shaders/phong.frag \
    shaders/phong.vert \
    shaders/quad.vert \
    shaders/ambient.frag \
    shaders/ambient.vert \
    shaders/depth.vert \
    shaders/depth.frag \
    shaders/position.frag \
    shaders/position.vert \
    shaders/normal.frag \
    shaders/normal.vert \
    shaders/myPhong.frag \
    shaders/color.vert \
    shaders/color.frag

RESOURCES += \
    shaders/shaders.qrc \
    images/images.qrc
