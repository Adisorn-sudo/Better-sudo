#include <android/log.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

#define TAG "Sudo_TAKWA"

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

void copyDir(const char* src, const char* dst) {
    mkdir(dst, 0755);

    DIR* dir = opendir(src);
    if (!dir) return;

    struct dirent* ent;
    while ((ent = readdir(dir)) != nullptr) {

        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
            continue;

        char srcPath[512];
        char dstPath[512];

        snprintf(srcPath, sizeof(srcPath), "%s/%s", src, ent->d_name);
        snprintf(dstPath, sizeof(dstPath), "%s/%s", dst, ent->d_name);

        struct stat st{};
        if (stat(srcPath, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                copyDir(srcPath, dstPath);   // 🔁 recursive
            } else {
                copyFile(srcPath, dstPath);
            }
        }
    }

    closedir(dir);
}

__attribute__((constructor))
void onLoad() {

    __android_log_print(ANDROID_LOG_INFO, TAG, "Start recursive cache copy");

    copyDir(
        "/data/user/0/org.levimc.launcher",
        "/storage/emulated/0/Android/data/org.levimc.launcher/all"
    );

    __android_log_print(ANDROID_LOG_INFO, TAG, "Cache copy finished");
}
