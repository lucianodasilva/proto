
#include <jni.h>

#include "proto.h"
#include "../../../../../../proto/harness.h"
#include "../../../../../../proto/imp_reference.h"
#include "../../../../../../proto/imp_datadriven.h"

JNIEXPORT jobjectArray JNICALL Java_com_munchiecollision_protodatadriven_MainActivity_ExecuteNativeTests
          (JNIEnv *env, jobject thisObj) {

    jobjectArray ret = env->NewObjectArray (2,env->FindClass("java/lang/String"),env->NewStringUTF(""));

    proto::abstract::world_desc source = proto::harness::generator::run (10000);

    proto::harness::tester tester = {
        5.0
    };

    auto r1 = tester.exec < proto::imp_reference::factory > ("reference", source);
    auto r2 = tester.exec < proto::imp_datadriven::factory > ("data driven", source);

    env->SetObjectArrayElement(ret, 0, env->NewStringUTF(r1.to_string ().c_str ()));
    env->SetObjectArrayElement(ret, 1, env->NewStringUTF(r2.to_string ().c_str ()));

    return ret;
}