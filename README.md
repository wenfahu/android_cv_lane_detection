# android_cv_lane_detection
Android lane detection app built with openCV ndk

Two kinds of image process method are implemented as two jni calls in `MainActiviy.java`. 

One of them(`com.example.wenfahu.lane_droid.MainActivity#process`) is a slightly improved implementation of [jdorweiler/lane-detection](https://github.com/jdorweiler/lane-detection). Details please visit [Revisiting Lane Detection using OpenCV](https://github.com/jdorweiler/lane-detection).

The other (`com.example.wenfahu.lane_droid.MainActivity#processOpt`) is a simplied version of the method above. And it turns out to outperform that one.

## Build

### Prerequisite
+ Android Studio 2.1.2
+ OpenCV Android SDK 3.1.0
+ Android SDK 24
+ Android NDK 12.1

### Highlight
Check out your sdk paths and replace them in the Android Studio's `File/Project Structure/SDKLocation` and gradle scripts and `jni/Android.mk` .

The project is built with ARM NEON support (check it out in `jni/Android.mk` : "`LOCAL_ARM_NEON := true`") and tested on an android device with ARM Cortex-A53 chips.
