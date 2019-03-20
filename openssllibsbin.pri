android {
#INCLUDEPATH += $$absolute_path($$PWD/3rdparty/include)

equals(ANDROID_TARGET_ARCH, arm64-v8a) {
    LIBPATH = $$absolute_path($$PWD/3rdparty/libs/openssl/arm64-v8a)
}

equals(ANDROID_TARGET_ARCH, armeabi-v7a) {
    LIBPATH = $$absolute_path($$PWD/3rdparty/libs/openssl/armeabi-v7a)
}

equals(ANDROID_TARGET_ARCH, x86) {
    LIBPATH = $$absolute_path($$PWD/3rdparty/libs/openssl/x86)
}

LIBS += \
    -L$$LIBPATH \
    -lssl -lcrypto

ANDROID_EXTRA_LIBS += \
    $$LIBPATH/libssl.so \
    $$LIBPATH/libcrypto.so
}
