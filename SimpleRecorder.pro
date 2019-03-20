QT += quick quickcontrols2 multimedia
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEPENDPATH += src
INCLUDEPATH += src
INCLUDEPATH += SmtpClient-for-Qt/src
SOURCES += \
        src/main.cpp \
    src/simplerecorder.cpp \
    SmtpClient-for-Qt/src/emailaddress.cpp \
    SmtpClient-for-Qt/src/mimeattachment.cpp \
    SmtpClient-for-Qt/src/mimecontentformatter.cpp \
    SmtpClient-for-Qt/src/mimefile.cpp \
    SmtpClient-for-Qt/src/mimehtml.cpp \
    SmtpClient-for-Qt/src/mimeinlinefile.cpp \
    SmtpClient-for-Qt/src/mimemessage.cpp \
    SmtpClient-for-Qt/src/mimemultipart.cpp \
    SmtpClient-for-Qt/src/mimepart.cpp \
    SmtpClient-for-Qt/src/mimetext.cpp \
    SmtpClient-for-Qt/src/quotedprintable.cpp \
    SmtpClient-for-Qt/src/smtpclient.cpp \
    src/mailer.cpp

HEADERS += \
    src/simplerecorder.h \
    SmtpClient-for-Qt/src/emailaddress.h \
    SmtpClient-for-Qt/src/mimeattachment.h \
    SmtpClient-for-Qt/src/mimecontentformatter.h \
    SmtpClient-for-Qt/src/mimefile.h \
    SmtpClient-for-Qt/src/mimehtml.h \
    SmtpClient-for-Qt/src/mimeinlinefile.h \
    SmtpClient-for-Qt/src/mimemessage.h \
    SmtpClient-for-Qt/src/mimemultipart.h \
    SmtpClient-for-Qt/src/mimepart.h \
    SmtpClient-for-Qt/src/mimetext.h \
    SmtpClient-for-Qt/src/quotedprintable.h \
    SmtpClient-for-Qt/src/smtpclient.h \
    SmtpClient-for-Qt/src/smtpexports.h \
    SmtpClient-for-Qt/src/SmtpMime \
    src/mailer.h

RESOURCES += res.qrc 

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/src/my/utl/WakeLock.java

include(openssllibsbin.pri)

android {
#    LIBS +=-L/home/rusich/Devel/SSL/openssl-0.9.8zh/libcrypto.a
#    LIBS +=-l/home/rusich/Devel/SSL/openssl-0.9.8zh/libssl.a
#    LIBS +=-L/home/rusich/Devel/SSL/openssl-android-build/libs/android/clang/armeabi-v7a
#    LIBS +=-L/home/rusich/Devel/SSL/openssl-android-build/libs/android/clang/x86
#    LIBS +=-lssl -lcrypto
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
    QT+=androidextras
}
