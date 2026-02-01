#include <android/log.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

#define TAG "Sudo_TAKWA"

const char* SRC = "/data/user/0/org.levimc.launcher/cache";
const char* DST = "/storage/emulated/0/Android/data/org.levimc.launcher/cache";

void copyFile(const char* src, const char* dst) {
    int in = open(src, O_RDONLY);
    if (in < 0) return;

    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) {
        close(in);
        return;
    }

    char buf[4096];
    ssize_t n;
    while ((n = read(in, buf, sizeof(buf))) > 0) {
        write(out, buf, n);
    }

    close(in);
    close(out);
}

__attribute__((constructor))
void onLoad() {

    __android_log_print(ANDROID_LOG_INFO, TAG, "Start copy cache");

    mkdir("/storage/emulated/0/Android/data", 0755);
    mkdir("/storage/emulated/0/Android/data/org.levimc.launcher", 0755);
    mkdir(DST, 0755);

    DIR* dir = opendir(SRC);
    if (!dir) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Cannot open source cache");
        return;
    }

    struct dirent* ent;
    while ((ent = readdir(dir)) != nullptr) {

        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
            continue;

        char srcPath[512];
        char dstPath[512];

        snprintf(srcPath, sizeof(srcPath), "%s/%s", SRC, ent->d_name);
        snprintf(dstPath, sizeof(dstPath), "%s/%s", DST, ent->d_name);

        copyFile(srcPath, dstPath);

        __android_log_print(
            ANDROID_LOG_INFO,
            TAG,
            "Copied: %s",
            ent->d_name
        );
    }

    closedir(dir);
    __android_log_print(ANDROID_LOG_INFO, TAG, "Copy cache finished");
}
