
#include <jni.h>
#include "proto.h"

JNIEXPORT jstring JNICALL Java_com_munchiecollision_protodatadriven_MainActivity_getMessage
          (JNIEnv *env, jobject thisObj) {
   return (*env)->NewStringUTF(env, "Hello from native code!");
}