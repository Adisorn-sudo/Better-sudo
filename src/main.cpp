#include <android/log.h>
#include <dirent.h>

#define TAG "Sudo_TAKWA"

__attribute__((constructor))
void onLoad() {

    __android_log_print(
        ANDROID_LOG_INFO,
        TAG,
        "SO constructor called"
    );

    DIR* dir = opendir("/data/user/0/org.levimc.launcher");
    if (!dir) {
        __android_log_print(
            ANDROID_LOG_ERROR,
            TAG,
            "Cannot open /data/user/0/org.levimc.launcher"
        );
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
