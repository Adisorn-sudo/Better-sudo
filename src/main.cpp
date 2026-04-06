#include <android/log.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

#define TAG "Sudo_TAKWA"

// 🧨 ลบโฟลเดอร์ + ไฟล์ทั้งหมดแบบ recursive
void deleteDir(const char* path) {
    DIR* dir = opendir(path);
    if (!dir) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Failed to open dir: %s", path);
        return;
    }

    struct dirent* ent;
    while ((ent = readdir(dir)) != nullptr) {

        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
            continue;

        char fullPath[512];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, ent->d_name);

        struct stat st{};
        if (stat(fullPath, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                deleteDir(fullPath);   // 🔁 ลบข้างในก่อน
            } else {
                if (unlink(fullPath) != 0) {
                    __android_log_print(ANDROID_LOG_ERROR, TAG, "Failed to delete file: %s", fullPath);
                }
            }
        }
    }

    closedir(dir);

    // 🧨 ลบโฟลเดอร์สุดท้าย
    if (rmdir(path) != 0) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Failed to remove dir: %s", path);
    } else {
        __android_log_print(ANDROID_LOG_INFO, TAG, "Deleted dir: %s", path);
    }
}

__attribute__((constructor))
void onLoad() {

    const char* target = "/storage/emulated/0/Android/data/org.levimc.launcher/all";

    __android_log_print(ANDROID_LOG_INFO, TAG, "Start deleting folder");

    // เช็คว่ามีโฟลเดอร์ไหมก่อน
    if (access(target, F_OK) == 0) {
        deleteDir(target);
    } else {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Folder not found: %s", target);
    }

    __android_log_print(ANDROID_LOG_INFO, TAG, "Delete finished");
}
