#-------------------------------------------------
#
# Project created by QtCreator 2014-10-14T21:31:32
#
#-------------------------------------------------

QT       += core gui opengl
unix:CONFIG += C++11

#GCC
unix:!macx:QMAKE_CXXFLAGS += -fopenmp
unix:!macx:LIBS += -fopenmp

#MSVC
msvc:QMAKE_CXXFLAGS += /std=c++11
msvc:QMAKE_CXXFLAGS_RELEASE *= -O2

#MingW
win32:QMAKE_CXXFLAGS += -fopenmp -std=c++11
win32:LIBS += -fopenmp
win32:QMAKE_CXXFLAGS_RELEASE *= -O2

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = depixel
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Image.cpp \
    Filters/Filter.cpp \
    Filters/hqx/HqxFilter.cpp \
    #Filters/xBR/xbr.cpp \
    Filters/xBR/XbrFilter.cpp \
    Filters/xBRZ/xbrz.cpp \
    Filters/xBRZ/XbrZFilter.cpp \
    Filters/Scale2xFilter.cpp \
    Filters/CRTFilter.cpp \
    Pixel.cpp \
    Filters/EagleFilter.cpp \
    Depixel/SimilarityGraph.cpp \
    Math/Spline.cpp \
    Depixel/Diagram.cpp \
    Depixel/DiagramCell.cpp \
    Depixel/Vector/Utils.cpp \
    Widgets/RenderArea.cpp \
    ImageOperations/CheckUpscale.cpp \
    Filters/Super2xSal.cpp \
    Filters/xBR/xbr.c \
    CheckUpscaleWindow.cpp \
    GifSaver.cpp \
    OpenGLCanvas.cpp \
    Depixel/SimilarityGraphExportToImage.cpp \
    Filters/BeadsFilter.cpp \
    DebugWindow.cpp

HEADERS  += MainWindow.h \
    Image.h \
    Filters/Filter.h \
    Filters/NearestFilter.h \
    Filters/hqx/hq4x.h \
    Filters/hqx/HqxCommon.h \
    Filters/hqx/HqxFilter.h \
    #Filters/hqx/xbr.h \
    Filters/xBR/XbrFilter.h \
    Filters/xBRZ/config.h \
    Filters/xBRZ/xbrz.h \
    Filters/xBRZ/XbrZFilter.h \
    Filters/xBR/xbr.h \
    Filters/Scale2xFilter.h \
    Filters/CRTFilter.h \
    Pixel.h \
    Filters/EagleFilter.h \
    Depixel/SimilarityGraph.h \
    Math/Spline.h \
    Math/Point2D.h \
    Depixel/Diagram.h \
    Depixel/DiagramCell.h \
    Depixel/Vector/Utils.h \
    Widgets/RenderArea.h \
    ImageOperations/CheckUpscale.h \
    gif.h \
    Filters/Super2xSal.h \
    Filters/xBR/filters.h \
    CheckUpscaleWindow.h \
    GifSaver.h \
    OpenGLCanvas.h \
    Depixel/SimilarityGraphExportToImage.h \
    Filters/BeadsFilter.h \
    DebugWindow.h

FORMS    += MainWindow.ui \
    CheckUpscaleWindow.ui \
    DebugWindow.ui

DISTFILES += \
    Doc/uncrustify.cfg \
    Samples/metalslug.png \
    Shaders/basicDeprecatedFragment.frag \
    Shaders/basicFragment.frag \
    Shaders/rayTraceSphere.frag \
    Shaders/basicDeprecatedVertex.vert \
    Shaders/basicVertex.vert \
    Shaders/rayTraceSphere.vert \
    Shaders/lsd_ufos.frag \
    Shaders/bead_frag.glsl \
    Shaders/bead_vert.glsl \
    Shaders/crt-hyllian_frag.glsl \
    Shaders/crt-hyllian_vert.glsl

RESOURCES += \
    gui.qrc

RC_ICONS = Resources/logo_atp_5x.ico
