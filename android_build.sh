export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
#export ANDROID_NDK=/home/user/workspace/android/android-ndk-r18b
#export ANDROID_NDK=/home/user/workspace/android/android-ndk-r19c
export ANDROID_NDK=/home/user/workspace/android/android-ndk-r21b
export ANDROID_SDK=/home/user/workspace/android/android-sdk-linux
export ANDROID_HOME=/home/user/workspace/android/android-sdk-linux
#export CMAKE_PREFIX_PATH=/opt/Qt5.12/5.12.0/android_armv7/lib/cmake
export CMAKE_PREFIX_PATH=/opt/Qt5.13.2/5.13.2/android_armv7/lib/cmake
#export CMAKE_PREFIX_PATH=/opt/Qt5.9.5/5.9.5/android_armv7/lib/cmake
#export CMAKE_PREFIX_PATH=/opt/Qt/5.11.1/android_armv7/lib/cmake
# export CMAKE_PREFIX_PATH=/opt/Qt5.14.2/5.14.2/android/lib/cmake
# export OPENSSL_ROOT_DIR=/home/user/workspace/android/openssl
# export OPENSSL_ROOT_DIR=/home/user/workspace/android/openssl/libs/armeabi-v7a
# export OPENSSL_ROOT_DIR=/home/user/workspace/android/openssl/openssl-1.1.1g
export OPENSSL_ROOT_DIR=/home/user/workspace/android/toolchain/usr/local/ssl

mkdir -p `pwd`/android-build
cd `pwd`/android-build
cmake .. -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
        -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ON \
        -DANDROID_ABI=armeabi-v7a \
        -DANDROID_ARM_NEON=TRUE \
        -DANDROID_PLATFORM=android-23 \
        -DANDROID_STL=c++_shared

make -j4 #&> 1.log

adb install -r ./story_teller-armeabi-v7a/build/outputs/apk/debug/story_teller-armeabi-v7a-debug.apk


# Honor9 libGLES_mali.so
# Sony X eglSubDriverAndroid.so libEGL_adreno.so libGLESv1_CM_adreno.so libGLESv2_adreno.so libQTapGLES.so libq3dtools_adreno.so libq3dtools_esx.so