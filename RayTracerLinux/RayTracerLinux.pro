QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../XML/tinyxml2.cpp \
    main.cpp \
    #mainwindow.cpp

HEADERS += \
    ../XML/tinyxml2.h \
    ../camera.hpp \
    ../image.hpp \
    ../moteur_rendu.hpp \
    ../objet.hpp \
    ../ray.hpp \
    ../stb-master/stb_image_write.h \
    ../vecteur.hpp \
    ../xmlread.hpp \
    #mainwindow.h

FORMS += \
    mainwindow.ui

QMAKE_CXXFLAGS += -Wsign-compare -Wunused-parameter -O3 -fopenmp #-I/usr/lib/gcc/x86_64-linux-gnu/9/include
QMAKE_LFLAGS += -fopenmp
LIBS += -lgomp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
