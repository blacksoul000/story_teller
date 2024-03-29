######################################################################
# Automatically generated by qmake (3.1) Sat Nov 6 15:16:46 2021
######################################################################

QT += quick sql svg network multimedia

CONFIG += c++11

#TEMPLATE = app
TARGET = story_teller
INCLUDEPATH += include

android {
    include(/home/user/workspace/android/android_openssl/openssl.pri)

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
    DISTFILES += \
        android/AndroidManifest.xml
}

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += include/app/main_window.hpp \
           include/domain/deti_online_handler.hpp \
           include/domain/i_page_handler.hpp \
           include/domain/meta_info.hpp \
           include/domain/ozornik_handler.hpp \
           include/domain/page_handler_factory.hpp \
           include/domain/page_loader.hpp \
           include/domain/story_database.hpp \
           include/domain/story_group.hpp \
           include/domain/story_list.hpp \
           include/domain/story_object.hpp \
           include/domain/web_skazki_handler.hpp
SOURCES += src/main.cpp \
           src/app/main_window.cpp \
           src/domain/deti_online_handler.cpp \
           src/domain/ozornik_handler.cpp \
           src/domain/page_handler_factory.cpp \
           src/domain/page_loader.cpp \
           src/domain/story_database.cpp \
           src/domain/story_group.cpp \
           src/domain/story_list.cpp \
           src/domain/story_object.cpp \
           src/domain/web_skazki_handler.cpp
RESOURCES += src/resources.qrc src/presentation/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
