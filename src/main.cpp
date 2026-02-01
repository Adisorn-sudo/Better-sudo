#include <jni.h>
#include <android/log.h>
#include <dirent.h>

#define TAG "LEVI_CHECK"

extern "C"
JNIEXPORT void JNICALL
Java_org_levimc_launcher_Native_checkFiles(JNIEnv* env, jobject thiz) {

    DIR* dir = opendir("/data/user/0/org.levimc.launcher");
    if (!dir) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Cannot open dir");
        return;
    }

    struct dirent* ent;
    while ((ent = readdir(dir)) != nullptr) {
        __android_log_print(
            ANDROID_LOG_INFO,
            TAG,
            "Found: %s",
            ent->d_name
        );
    }
    closedir(dir);
}
