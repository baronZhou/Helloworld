/*
 * Copyright (c) 2015-2016 MICROTRUST Incorporated
 * All rights reserved
 *
 * This file and software is confidential and proprietary to MICROTRUST Inc.
 * Unauthorized copying of this file and software is strictly prohibited.
 * You MUST NOT disclose this file and software unless you get a license
 * agreement from MICROTRUST Incorporated.
 */

#define IMSG_TAG "[mTEE]"

#include <cutils/properties.h>
#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <imsg_log.h>

#include <dirent.h>
#include <sys/types.h>

#include "md5_utils.h"
#include "utos_version.h"

#define LOAD_TZ_DRIVER 0

#if LOAD_TZ_DRIVER
#include "module.h"
#include <linux/kernel.h>
#endif
#include "TEEI.h"

#define BUFFER_SIZE (512 * 1024)
#define DEV_FILE "/dev/tz_vfs"

#define TEEI_IOC_MAGIC 'T'
#define TEEI_CONFIG_IOC_MAGIC TEEI_IOC_MAGIC

#define TEEI_CONFIG_IOCTL_INIT_TEEI _IOWR(TEEI_CONFIG_IOC_MAGIC, 3, int)
#define TEEI_CONFIG_IOCTL_UNLOCK _IOWR(TEEI_CONFIG_IOC_MAGIC, 4, int)
#define SOTER_TUI_ENTER _IOWR(TEEI_IOC_MAGIC, 0x70, int)
#define SOTER_TUI_LEAVE _IOWR(TEEI_IOC_MAGIC, 0x71, int)
#define GET_FP_VENDOR_CMD _IOWR(TEEI_IOC_MAGIC, 0x80, int)

enum {
        FP_VENDOR_INVALID = 0,
        FPC_VENDOR,
        GOODIX_VENDOR,
};

#if PLATFORM_VERSION_MAJOR >= 7
#define SYSTEM_THH_PATH	"/vendor/thh/"
#else
#define SYSTEM_THH_PATH	"/system/thh/"
#endif

extern int init_module(void*, unsigned long, const char*);
static int do_mkdir(int nargs, char** args);
static void do_ota_cert_update();
static int do_link(char* fn, char* ln);

#define COPY_FILE_FULLPATH(name, from, to)                                                         \
    {                                                                                              \
        char* name[] = {"", from, to};                                                             \
        IMSG_INFO("copy %s\n", from);                                                                   \
        int name##_ret = do_copy(3, name);                                                         \
        if (name##_ret < 0)                                                                        \
            IMSG_ERROR("%s copy failed[%d] to %s\n", from, name##_ret, to);                              \
    }

static int do_copy(int nargs, char** args);
#define OTA_REBOOT 1
#define OTA_NO 0

/* Notice :
 * According to the form, please correct the code below.
 * -----------------------------------------------------------------
 * |   odm  |      ota file positon          |  ota flag (success) |
 * -----------------------------------------------------------------
 * | lenovo |  /cache/ota/updateResult       |         0           |
 * -----------------------------------------------------------------
 * |  qiku  |  /cache/recovery/last_install  |         1           |
 * -----------------------------------------------------------------
 */
static int get_ota_status() {

    int ret = OTA_NO;
    char* path = "/cache/ota/updateResult";
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        IMSG_INFO("file no exists : %s", path);
        return OTA_NO;
    }

    char ch = -1;
    read(fd, &ch, 1);
    close(fd);

    IMSG_INFO("status : %d", ch);
    if ('0' == ch) {
        IMSG_INFO("OTA has been updated OK");
        ret = OTA_REBOOT;
    } else {
        IMSG_INFO("OTA has been updated FAILED, %d", errno);
        ret = OTA_NO;
    }
    close(fd);
    return ret;
}

static void do_ota_cert_update() {
    // /system/thh/cert_update to /data/local/thh_update
    char* upt_dir[] = {"mkdir", "/data/local/thh_update", "0775"};
    char* upt_dir1[] = {"mkdir", "/data/local/thh_update/tee1", "0775"};
    char* upt_dir2[] = {"mkdir", "/data/local/thh_update/tee2", "0775"};
    char* upt_dir3[] = {"mkdir", "/data/local/thh_update/tee3", "0775"};
    char* upt_dir4[] = {"mkdir", "/data/local/thh_update/tee4", "0775"};
    char* upt_dir5[] = {"mkdir", "/data/local/thh_update/tee", "0775"};
    char* upt_dir6[] = {"mkdir", "/data/local/thh_update/tee7", "0775"};
    char* upt_dir7[] = {"mkdir", "/data/local/thh_update/tee8", "0775"};
    char* upt_dir8[] = {"mkdir", "/data/local/thh_update/tee9", "0775"};
    char* upt_dir9[] = {"mkdir", "/data/local/thh_update/teeA", "0775"};
    char* upt_dirA[] = {"mkdir", "/data/local/thh_update/teeB", "0775"};
    char* upt_dirB[] = {"mkdir", "/data/local/thh_update/teeC", "0775"};
    char* upt_dirC[] = {"mkdir", "/data/local/thh_update/teeD", "0775"};
    char* upt_dirD[] = {"mkdir", "/data/local/thh_update/teeE", "0775"};
    char* upt_dirE[] = {"mkdir", "/data/local/thh_update/teeF", "0775"};
    do_mkdir(3, upt_dir);
    do_mkdir(3, upt_dir1);
    do_mkdir(3, upt_dir2);
    do_mkdir(3, upt_dir3);
    do_mkdir(3, upt_dir4);
    do_mkdir(3, upt_dir5);
    do_mkdir(3, upt_dir6);
    do_mkdir(3, upt_dir7);
    do_mkdir(3, upt_dir8);
    do_mkdir(3, upt_dir9);
    do_mkdir(3, upt_dirA);
    do_mkdir(3, upt_dirB);
    do_mkdir(3, upt_dirC);
    do_mkdir(3, upt_dirD);
    do_mkdir(3, upt_dirE);

    char* thhCert[] = {"thh.cert", SYSTEM_THH_PATH "cert_update/thh.cert",
                       "/data/local/thh_update/thh.cert"};
    do_copy(3, thhCert);

    char* tee1RootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/tee1/root.cert",
                            "/data/local/thh_update/tee1/root.cert"};
    char* tee2RootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/tee2/root.cert",
                            "/data/local/thh_update/tee2/root.cert"};
    char* tee3RootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/tee3/root.cert",
                            "/data/local/thh_update/tee3/root.cert"};
    char* tee4RootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/tee4/root.cert",
                            "/data/local/thh_update/tee4/root.cert"};
    char* tee7RootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/tee7/root.cert",
                            "/data/local/thh_update/tee7/root.cert"};
    char* teeRootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/tee/root.cert",
                           "/data/local/thh_update/tee/root.cert"};
    char* tee8RootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/tee8/root.cert",
                           "/data/local/thh_update/tee8/root.cert"};
    char* tee9RootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/tee9/root.cert",
                           "/data/local/thh_update/tee9/root.cert"};
    char* teeARootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/teeA/root.cert",
                           "/data/local/thh_update/teeA/root.cert"};
    char* teeBRootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/teeB/root.cert",
                           "/data/local/thh_update/teeB/root.cert"};
    char* teeCRootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/teeC/root.cert",
                           "/data/local/thh_update/teeC/root.cert"};
    char* teeDRootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/teeD/root.cert",
                           "/data/local/thh_update/teeD/root.cert"};
    char* teeERootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/teeE/root.cert",
                           "/data/local/thh_update/teeE/root.cert"};
    char* teeFRootCert[] = {"root.cert", SYSTEM_THH_PATH "cert_update/teeF/root.cert",
                           "/data/local/thh_update/teeF/root.cert"};
    do_copy(3, tee1RootCert);
    do_copy(3, tee2RootCert);
    do_copy(3, tee3RootCert);
    do_copy(3, tee4RootCert);
    do_copy(3, tee7RootCert);
    do_copy(3, teeRootCert);
    do_copy(3, tee8RootCert);
    do_copy(3, tee9RootCert);
    do_copy(3, teeARootCert);
    do_copy(3, teeBRootCert);
    do_copy(3, teeCRootCert);
    do_copy(3, teeDRootCert);
    do_copy(3, teeERootCert);
    do_copy(3, teeFRootCert);

    char* tee1SignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/tee1/sign.cert",
                            "/data/local/thh_update/tee1/sign.cert"};
    char* tee2SignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/tee2/sign.cert",
                            "/data/local/thh_update/tee2/sign.cert"};
    char* tee3SignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/tee3/sign.cert",
                            "/data/local/thh_update/tee3/sign.cert"};
    char* tee4SignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/tee4/sign.cert",
                            "/data/local/thh_update/tee4/sign.cert"};
    char* tee7SignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/tee7/sign.cert",
                            "/data/local/thh_update/tee7/sign.cert"};
    char* teeSignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/tee/sign.cert",
                           "/data/local/thh_update/tee/sign.cert"};
    char* tee8SignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/tee8/sign.cert",
                            "/data/local/thh_update/tee8/sign.cert"};
    char* tee9SignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/tee9/sign.cert",
                            "/data/local/thh_update/tee9/sign.cert"};
    char* teeASignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/teeA/sign.cert",
                            "/data/local/thh_update/teeA/sign.cert"};
    char* teeBSignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/teeB/sign.cert",
                            "/data/local/thh_update/teeB/sign.cert"};
    char* teeCSignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/teeC/sign.cert",
                            "/data/local/thh_update/teeC/sign.cert"};
    char* teeDSignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/teeD/sign.cert",
                            "/data/local/thh_update/teeD/sign.cert"};
    char* teeESignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/teeE/sign.cert",
                            "/data/local/thh_update/teeE/sign.cert"};
    char* teeFSignCert[] = {"sign.cert", SYSTEM_THH_PATH "cert_update/teeF/sign.cert",
                            "/data/local/thh_update/teeF/sign.cert"};
    do_copy(3, tee1SignCert);
    do_copy(3, tee2SignCert);
    do_copy(3, tee3SignCert);
    do_copy(3, tee4SignCert);
    do_copy(3, tee7SignCert);
    do_copy(3, teeSignCert);
    do_copy(3, tee7SignCert);
    do_copy(3, tee8SignCert);
    do_copy(3, tee9SignCert);
    do_copy(3, teeASignCert);
    do_copy(3, teeBSignCert);
    do_copy(3, teeCSignCert);
    do_copy(3, teeDSignCert);
    do_copy(3, teeESignCert);
    do_copy(3, teeFSignCert);

}

static int get_image_md5(char* filepath, unsigned char* ret) {
    unsigned char dest[16] = {0};
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        IMSG_INFO("file can not be open %s ", filepath);
        return 0;
    }

    struct stat status;
    fstat(fd, &status);
    char* data = mmap(0, status.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (NULL == data) {
        IMSG_INFO("file can not be mmap %s ", filepath);
        close(fd);
        return 0;
    }

    MD5_CTX context;
    MD5Init(&context);
    MD5Update(&context, (md5byte*)data, status.st_size);
    MD5Final(dest, &context);
    munmap(data, status.st_size);
    close(fd);

    if (dest[0] == 0 && dest[1] == 0 && dest[2] == 0 && dest[3] == 0 && dest[4] == 0 &&
        dest[5] == 0 && dest[6] == 0 && dest[7] == 0 && dest[8] == 0 && dest[9] == 0 &&
        dest[10] == 0 && dest[11] == 0 && dest[12] == 0 && dest[13] == 0 && dest[14] == 0 &&
        dest[15] == 0) {
        IMSG_ERROR("file %s md5 == 0", filepath);
        return 0;
    }

    sprintf((void*)ret, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", dest[0],
            dest[1], dest[2], dest[3], dest[4], dest[5], dest[6], dest[7], dest[8], dest[9],
            dest[10], dest[11], dest[12], dest[13], dest[14], dest[15]);

    return 1;
}

static int is_file_exist(char* path) {
    if (access(path, 0) == 0) {
        return 1;
    } else {
        return 0;
    }
}

static int mkdir_err_exist = 0;
int make_dir(const char* path, mode_t mode) {
    int rc;
    rc = mkdir(path, mode);
    IMSG_DEBUG("creat:%s %d", path, rc);
    return rc;
}

static int _chmod(const char* path, mode_t mode) {
    int ret;

    struct stat p_statbuf;

    ret = lstat(path, &p_statbuf);
    if (ret < 0) {
        return -1;
    }

    if (S_ISLNK(p_statbuf.st_mode) == 1) {
        errno = EINVAL;
        return -1;
    }

    ret = chmod(path, mode);

    return ret;
}
static int do_mkdir(int nargs, char** args) {
    if (is_file_exist(args[1])) {
        IMSG_INFO("file exist : %s", args[1]);
        return 0;
    }

    mode_t mode = 0755;
    int ret;

    /* mkdir <path> [mode] [owner] [group] */

    if (nargs >= 3) {
        mode = strtoul(args[2], 0, 8);
    }

    IMSG_DEBUG("creat:%s", args[1]);
    ret = make_dir(args[1], mode);
    /* chmod in case the directory already exists */
    if (ret == -1 && errno == EEXIST) {
        ret = _chmod(args[1], mode);
        IMSG_WARN("creat:%s failed ", args[1]);
        mkdir_err_exist = 1;
    } else {
        mkdir_err_exist = 0;
    }
    if (ret == -1) {
        return -errno;
    }
    return 0;
}

static int do_link(char* fn, char* to) {
    int file_descriptor;
    int ret = 0;
    if ((file_descriptor = open(fn, O_RDONLY)) < 0)
        IMSG_WARN("file no exists : %s, %d", fn, file_descriptor);
    else {
        close(file_descriptor);
        if ((ret = symlink(fn, to)) != 0) {
            IMSG_ERROR("link error %s, %d", fn, errno);
            // unlink(fn);
            ret = -1;
        }
    }
    return ret;
}

static int do_copy(int nargs, char** args) {
    char* buffer = NULL;
    int rc = 0;
    int fd1 = -1, fd2 = -1;
    struct stat info;
    int brtw, brtr;
    char* p;

    if (nargs != 3) {
        IMSG_DEBUG("do_copy: nargs = %d, need to be 3!\n", nargs);
        return -1;
    }

    if (stat(args[1], &info) < 0) {

        IMSG_DEBUG("do_copy: stat(args[1], &info) <0!\n");
        return -1;
    }

    if ((fd1 = open(args[1], O_RDONLY)) < 0) {

        IMSG_DEBUG("do_copy: open %s failed!\n", args[1]);
        goto out_err;
    }
    if ((fd2 = open(args[2], O_WRONLY | O_CREAT | O_TRUNC, 0660)) < 0) {
        IMSG_DEBUG("do_copy: open %s failed! fd=%d errno=%d from=%s\n", args[2], fd2, errno, args[1]);
        goto out_err;
    }
    if (!(buffer = malloc(info.st_size))) {

        IMSG_DEBUG("do_copy: malloc failed\n");
        goto out_err;
    }
    p = buffer;
    brtr = info.st_size;
    while (brtr) {
        rc = read(fd1, p, brtr);
        if (rc < 0)
            goto out_err;
        if (rc == 0)
            break;
        p += rc;
        brtr -= rc;
    }

    p = buffer;
    brtw = info.st_size;
    while (brtw) {
        rc = write(fd2, p, brtw);
        if (rc < 0)
            goto out_err;
        if (rc == 0)
            break;
        p += rc;
        brtw -= rc;
    }

    rc = 0;
    goto out;
out_err:
    rc = -1;
out:
    if (buffer)
        free(buffer);
    if (fd1 >= 0)
        close(fd1);
    if (fd2 >= 0)
        close(fd2);
    return rc;
}

char* dir5[] = {"mkdir", "/data/thh", "0770"};
char* dirs[][3] = {
    {"mkdir", "/data/thh/system",  "0770"},
    {"mkdir", "/data/thh/tee_01",  "0770"},
    {"mkdir", "/data/thh/tee_02",  "0770"},
    {"mkdir", "/data/thh/tee_03",  "0770"},
    {"mkdir", "/data/thh/tee_04",  "0770"},
    {"mkdir", "/data/thh/tee_05",  "0770"},
    {"mkdir", "/data/thh/tee_00",  "0770"},
    {"mkdir", "/data/thh/tee_06",  "0770"},
    {"mkdir", "/data/thh/tee",     "0770"},
    {"mkdir", "/data/thh/tee/tas", "0770"},
    {"mkdir", "/data/thh/tee_07",  "0770"},
    {"mkdir", "/data/thh/tee_08",  "0770"},
    {"mkdir", "/data/thh/tee_09",  "0770"},
    {"mkdir", "/data/thh/tee_0A",  "0770"},
    {"mkdir", "/data/thh/tee_0B",  "0770"},
    {"mkdir", "/data/thh/tee_0C",  "0770"},
    {"mkdir", "/data/thh/tee_0D",  "0770"},
    {"mkdir", "/data/thh/tee_0E",  "0770"},
    {"mkdir", "/data/thh/tee_0F",  "0770"},
};

#define SOTER_WORK_PATH "/data/thh/system"
#define IMAGE_ORI_PATH  "/vendor/thh"
#define IMG_ORI 0
#define IMG_DST_DIR 1
#define IMG_DST_NAME 2
#define IMG_MD5_ORI 3
#define IMG_MD5_CUR 4

char* table[16][6] = {
    {SYSTEM_THH_PATH "soter.raw", "/data/thh/system", "/data/thh/system/soter.raw",
     "/data/thh/system/soter.ori.md5", "/data/thh/system/soter.cur.md5"},
    {SYSTEM_THH_PATH "alipayapp", "/data/thh/tee_01", "/data/thh/tee_01/tee",
     "/data/thh/tee_01/tee.ori.md5", "/data/thh/tee_01/tee.cur.md5"},
    {SYSTEM_THH_PATH "fp_server", "/data/thh/tee_05", "/data/thh/tee_05/tee",
     "/data/thh/tee_05/tee.ori.md5", "/data/thh/tee_05/tee.cur.md5"},
    {SYSTEM_THH_PATH "uTAgent", "/data/thh/tee_00", "/data/thh/tee_00/tee",
     "/data/thh/tee_00/tee.ori.md5", "/data/thh/tee_00/tee.cur.md5"},
    {SYSTEM_THH_PATH "tee", "/data/thh/tee", "/data/thh/tee/tee",
     "/data/thh/tee/tee.ori.md5", "/data/thh/tee/tee.cur.md5"},
    {SYSTEM_THH_PATH "fido", "/data/thh/tee_08", "/data/thh/tee_08/tee",
     "/data/thh/tee_08/tee.ori.md5", "/data/thh/tee_08/tee.cur.md5"},
    {SYSTEM_THH_PATH "general_pay", "/data/thh/tee_09", "/data/thh/tee_09/tee",
     "/data/thh/tee_09/tee.ori.md5", "/data/thh/tee_09/tee.cur.md5"},
    {SYSTEM_THH_PATH "general_storage", "/data/thh/tee_0A", "/data/thh/tee_0A/tee",
     "/data/thh/tee_0A/tee.ori.md5", "/data/thh/tee_0A/tee.cur.md5"},
    {SYSTEM_THH_PATH "general_antitheft", "/data/thh/tee_0B", "/data/thh/tee_0B/tee",
     "/data/thh/tee_0B/tee.ori.md5", "/data/thh/tee_0B/tee.cur.md5"},
    {SYSTEM_THH_PATH "general_antiroot", "/data/thh/tee_0C", "/data/thh/tee_0C/tee",
     "/data/thh/tee_0C/tee.ori.md5", "/data/thh/tee_0C/tee.cur.md5"},
    {SYSTEM_THH_PATH "general_antivirus", "/data/thh/tee_0D", "/data/thh/tee_0D/tee",
     "/data/thh/tee_0D/tee.ori.md5", "/data/thh/tee_0D/tee.cur.md5"},
    {SYSTEM_THH_PATH "general_reserved1", "/data/thh/tee_0E", "/data/thh/tee_0E/tee",
     "/data/thh/tee_0E/tee.ori.md5", "/data/thh/tee_0E/tee.cur.md5"},
    {SYSTEM_THH_PATH "general_reserved2", "/data/thh/tee_0F", "/data/thh/tee_0F/tee",
     "/data/thh/tee_0F/tee.ori.md5", "/data/thh/tee_0F/tee.cur.md5"},
    {SYSTEM_THH_PATH "inner_1", "/data/thh/tee_03", "/data/thh/tee_03/tee",
     "/data/thh/tee_03/tee.ori.md5", "/data/thh/tee_03/tee.cur.md5"},
    {SYSTEM_THH_PATH "inner_2", "/data/thh/tee_04", "/data/thh/tee_04/tee",
     "/data/thh/tee_04/tee.ori.md5", "/data/thh/tee_04/tee.cur.md5"},
    {SYSTEM_THH_PATH "tui", "/data/thh/tee_07", "/data/thh/tee_07/tee",
     "/data/thh/tee_07/tee.ori.md5", "/data/thh/tee_07/tee.cur.md5"}
};

static void showMD5(unsigned char* data, char* des) {
    // int i = 0;
	(void)&des;
    char md5[33] = {0};
    memcpy(md5, data, 32);
    IMSG_INFO("[MD5](%s:%s)", des, md5);
}
static int get_image_md5_from_file(char* path, unsigned char* data) {
    int fd = open(path, O_RDONLY, 0775);
    int cnt = 0;
    if (0 > fd) {
        IMSG_ERROR("can't read md5 file :%s", path);
        return 0;
    }
    cnt = read(fd, data, 32);

    if (cnt == 0 || (data[0] == 0 && data[1] == 0 && data[2] == 0 && data[3] == 0 && data[4] == 0 &&
                     data[5] == 0 && data[6] == 0 && data[7] == 0 && data[8] == 0 && data[9] == 0 &&
                     data[10] == 0 && data[11] == 0 && data[12] == 0 && data[13] == 0 &&
                     data[14] == 0 && data[15] == 0)) {
        IMSG_ERROR("file %s md5 == 0", path);
        return 0;
    }

    // IMSG_INFO("cnt %d",cnt);
    showMD5(data, path);
    close(fd);
    return 1;
}

static void write_md5_file(char* path, unsigned char* data) {
    int fd = open(path, O_CREAT | O_WRONLY, 0775);
    if (0 > fd)
        IMSG_ERROR("can't create md5 file :%s", path);
    write(fd, data, 32);
    close(fd);
}
static int is_equal_md5(unsigned char* src, unsigned char* dst) {
    if (NULL == src || dst == NULL) {
        IMSG_ERROR("md5 must not empty !");
        return 0;
    }
    int i = 0;
    for (; i < 32; i++) {
        if (src[i] != dst[i])
            return 0;
    }
    return 1;
}
static int is_ota_update() {
    /*Notice : Some odms will create /cache/xxxxx after ota, and write value in it, so we need check
     * it to decide whether to ota.*/
    // return (get_ota_status()==OTA_REBOOT);
    /*Notice : Other odms don't have this mechanism, so we do ota default.*/
    IMSG_INFO("==== Force check whether the origin image is updated , if need restore it ====");
    return 1;
}
static void create_tee_storage(char* table[]) {
#if 0
    unsigned char md5_ori_image[33] = {0};
    unsigned char ori_file[33] = {0};
    unsigned char md5_work[33] = {0};
    unsigned char md5_cur_file[33] = {0};

    IMSG_INFO("-----------------------------");
    if (is_file_exist(table[IMG_DST_DIR])) {
        /* caclulate md5 from file */
        IMSG_INFO("%s is exists", table[IMG_DST_DIR]);
        IMSG_INFO("get md5 from image");
        if (!get_image_md5(table[IMG_DST_NAME], md5_work)) {
            goto restore_image;
        }
        IMSG_INFO("get md5 from cur file");
        if (!get_image_md5_from_file(table[IMG_MD5_CUR], md5_cur_file)) {
            goto restore_image;
        }
        if (is_equal_md5(md5_work, md5_cur_file)) {
            IMSG_INFO("md5 == table.cur.md5");
            if (is_ota_update()) {
                IMSG_INFO("OTA == Y");
                if (!get_image_md5_from_file(table[IMG_MD5_ORI], ori_file)) {
                    goto restore_image;
                }
                showMD5(ori_file, table[IMG_MD5_ORI]);
                if (!get_image_md5(table[IMG_ORI], md5_ori_image)) {
                    goto restore_image;
                }
                showMD5(ori_file, table[IMG_MD5_ORI]);
                if (is_equal_md5(ori_file, md5_ori_image)) {
                    IMSG_INFO("*.ori.md5 == *.ori.md5");
                    goto creat_done;
                } else {
                    IMSG_INFO("*.ori.md5 != *.ori.md5");
                    goto restore_image;
                }

            } else {
                IMSG_INFO("OTA == N");
                goto creat_done;
            }
        } else {
            IMSG_INFO("md5 != table.cur.md5");
            goto restore_image;
        }

    } else {
        IMSG_INFO("%s is  not exists", table[IMG_DST_DIR]);
        goto restore_image;
    }
restore_image:
    IMSG_INFO("need restore_images");
    do_mkdir(3, dir5);
    do_mkdir(3, dirs[index]);
    unsigned char md5_src[32] = {0};
    get_image_md5(table[IMG_ORI], md5_src);
    write_md5_file(table[IMG_MD5_ORI], md5_src);
    write_md5_file(table[IMG_MD5_CUR], md5_src);
    COPY_FILE_FULLPATH(image_raw, table[IMG_ORI], table[IMG_DST_NAME]);
creat_done:
    IMSG_INFO("-----------------------------");
#else
    COPY_FILE_FULLPATH(image_raw, table[IMG_ORI], table[IMG_DST_NAME]);
#endif
    return;
}

#if LOAD_TZ_DRIVER
static void load_tz_driver() {
    char opts[1024] = {'\0'};
    int a = argc;
    char b = argv[0][0];
    int ret = init_module(vfsFun_ko, vfsFun_ko_len, opts);
    IMSG_DEBUG("start teei loading ...");

    if (ret != 0) {
        IMSG_DEBUG("failed (%s)", strerror(errno));
    }
    IMSG_DEBUG("loading result:%d", ret);
    ret = init_module(teei_ko, teei_ko_len, opts);
    if (ret != 0) {
        IMSG_DEBUG("failed (%s)", strerror(errno));
    }
    IMSG_DEBUG("loading result:%d", ret);

    return ret;
}
#endif

int analysis_command(unsigned char* p_buffer) {

    long retVal = 0;
    int responseLen = 0;
    struct TEEI_vfs_command* tz_command = NULL;
    union TEEI_vfs_response tz_response;
    char full_path_name[PATH_MAX];
    char *tz_cmd_path;
    char* pathname = NULL;
    char* param_address = NULL;
    int namelength = 0;

    DIR* dir = NULL;
    struct dirent* p_dirent = NULL;
    unsigned long read_count = 0;
    unsigned long count = 0;
    /* unsigned char *ipAddr = NULL; */
    /* struct hostent *hostEntry = NULL; */

    tz_command = (struct TEEI_vfs_command*)p_buffer;
    responseLen = sizeof(tz_response);

    switch (tz_command->func) {
    case TZ_VFS_OPEN:
        IMSG_DEBUG(" come into the TZ_VFS_OPEN function\n");

        tz_cmd_path = (char*)((unsigned long)(tz_command) + PAGE_SIZE_4K);
        if (*tz_cmd_path  != '/') {
            /* Relative path */
            full_path_name[0] = 0;
            strcat(full_path_name, SYSTEM_THH_PATH);
            strcat(full_path_name, tz_cmd_path);
            pathname = &full_path_name[0];
            IMSG_DEBUG("[Relative path] system thh path: %s tz_cmd_path=%s\n",
            SYSTEM_THH_PATH, tz_cmd_path);
            IMSG_DEBUG("full_path_name: %s\n", full_path_name);
        } else {
            /* Absolute path */
            pathname = tz_cmd_path;
        }

        IMSG_DEBUG("pathname = %s\n", pathname);
        IMSG_DEBUG("flags = %d\n", tz_command->args.func_open_args.flags);
        IMSG_DEBUG("mode = %d\n", tz_command->args.func_open_args.mode);

        retVal = open(pathname, tz_command->args.func_open_args.flags,
                      tz_command->args.func_open_args.mode);

        if (retVal == -1) {
            IMSG_DEBUG(" errno = %d\n", errno);
            retVal = -(errno);
        }
        IMSG_DEBUG("retVal = %ld\n", retVal);

        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = retVal;
        return responseLen;

    case TZ_VFS_READ:

        IMSG_DEBUG(" come into the TZ_VFS_READ function\n");

        param_address = (char*)((unsigned long)(tz_command) + PAGE_SIZE_4K);

        retVal = read(tz_command->args.func_read_args.fd, (char*)param_address,
                      tz_command->args.func_read_args.count);

        if (retVal == -1) {
            retVal = -(errno);
        }

        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = retVal;

        if (retVal == -1) {
            return responseLen;
        } else {
            return PAGE_SIZE_4K + retVal;
        }

    case TZ_VFS_WRITE:

        IMSG_DEBUG(" come into the TZ_VFS_WRITE function\n");
        IMSG_DEBUG("fd = %d\n", tz_command->args.func_write_args.fd);
        IMSG_DEBUG("count = %d\n", tz_command->args.func_write_args.count);

        param_address = (char*)((unsigned long)(tz_command) + PAGE_SIZE_4K);

        retVal = write(tz_command->args.func_write_args.fd, (const char*)param_address,
                       tz_command->args.func_write_args.count);

        if (retVal == -1) {
            retVal = -(errno);
        }

        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = retVal;

        IMSG_DEBUG(" come into the TZ_VFS_WRITE function retVal = %ld\n", retVal);

        if (retVal > 0) {
            return PAGE_SIZE_4K + retVal;
        } else {
            return responseLen;
        }

    case TZ_VFS_IOCTL:

        IMSG_DEBUG(" come into the TZ_VFS_IOCTL function\n");
        IMSG_DEBUG(" fd = %d\n", tz_command->args.func_ioctl_args.fd);
        IMSG_DEBUG(" cmd = %d\n", tz_command->args.func_ioctl_args.cmd);

        param_address = (char*)((unsigned long)(tz_command) + PAGE_SIZE_4K);

        if (tz_command->args.func_ioctl_args.cmd == 0x70) {
            retVal =
                ioctl(tz_command->args.func_ioctl_args.fd, SOTER_TUI_ENTER, (void*)param_address);
        } else if (tz_command->args.func_ioctl_args.cmd == 0x71) {
            retVal =
                ioctl(tz_command->args.func_ioctl_args.fd, SOTER_TUI_LEAVE, (void*)param_address);
        } else {
            retVal = ioctl(tz_command->args.func_ioctl_args.fd,
                           tz_command->args.func_ioctl_args.cmd, (void*)param_address);
        }

        if (retVal == -1) {
            retVal = -(errno);
        }
        IMSG_DEBUG(" retVal = %x\n", *((int*)param_address));
        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = retVal;
        return PAGE_SIZE_4K + 0x9000;

    case TZ_VFS_CLOSE:

        IMSG_DEBUG(" come into the TZ_VFS_CLOSE function\n");
        IMSG_DEBUG(" fd = %d\n", tz_command->args.func_close_args.fd);

        retVal = close(tz_command->args.func_close_args.fd);

        if (retVal == -1) {
            retVal = -(errno);
        }

        IMSG_DEBUG(" retVal = %ld\n", retVal);
        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = retVal;
        return responseLen;

    case TZ_VFS_TRUNC:

        IMSG_DEBUG(" come into the TZ_VFS_TRUNC function\n");

        retVal =
            ftruncate(tz_command->args.func_trunc_args.fd, tz_command->args.func_trunc_args.length);

        if (retVal == -1) {
            retVal = -(errno);
        }

        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = retVal;
        return responseLen;

    case TZ_VFS_UNLINK:

        IMSG_DEBUG(" come into the TZ_VFS_UNLINK function\n");

        param_address = (char*)((unsigned long)(tz_command) + PAGE_SIZE_4K);

        retVal = unlink((char*)param_address);

        if (retVal == -1) {
            retVal = -(errno);
        }

        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = retVal;
        return responseLen;

    case TZ_VFS_LSEEK:

        IMSG_DEBUG(" come into the TZ_VFS_LSEEK function\n");

        retVal = lseek(tz_command->args.func_lseek_args.fd, tz_command->args.func_lseek_args.offset,
                       tz_command->args.func_lseek_args.origin);

        if (retVal == -1) {
            retVal = -(errno);
        }

        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = retVal;
        return responseLen;

    case TZ_VFS_RENAME:

        IMSG_DEBUG(" come into the TZ_VFS_RENAME function\n");

        param_address = (char*)((unsigned long)(tz_command) + PAGE_SIZE_4K);

        namelength = strlen((char*)param_address);

        IMSG_DEBUG(" namelength = %d\n", namelength);
        IMSG_DEBUG(" 1st name = %s\n", (char*)param_address);
        IMSG_DEBUG(" 2nd name = %s\n", (char*)((char*)param_address + namelength + 1));

        retVal = rename((char*)param_address, (char*)(param_address + namelength + 1));

        if (retVal == -1) {
            retVal = -(errno);
        }
        IMSG_DEBUG(" retVal = %ld\n", retVal);

        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = retVal;
        return responseLen;

    case TZ_VFS_MKDIR:

        IMSG_DEBUG(" come into the TZ_VFS_MKDIR function\n");

        param_address = (char*)((unsigned long)(tz_command) + PAGE_SIZE_4K);

        IMSG_DEBUG(" 1st name = %s\n", (char*)param_address);
        IMSG_DEBUG(" mode = %d\n", tz_command->args.func_mkdir_args.mode);

        retVal = mkdir((char*)param_address, tz_command->args.func_mkdir_args.mode);

        if (retVal == -1) {
            retVal = -(errno);
        }
        IMSG_DEBUG(" retVal = %ld\n", retVal);

        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = retVal;
        return responseLen;

    case TZ_VFS_RMDIR:

        IMSG_DEBUG(" come into the TZ_VFS_RMDIR function\n");

        param_address = (char*)((unsigned long)(tz_command) + PAGE_SIZE_4K);
        IMSG_DEBUG(" 1st name = %s\n", (char*)param_address);

        retVal = rmdir((char*)param_address);

        if (retVal == -1) {
            retVal = -(errno);
        }
        IMSG_DEBUG(" retVal = %ld\n", retVal);

        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = retVal;
        return responseLen;

    case TZ_VFS_OPENDIR:

        IMSG_DEBUG(" come into the TZ_VFS_OPENDIR function\n");

        pathname = (char*)((unsigned long)(tz_command) + PAGE_SIZE_4K);

        dir = opendir(pathname);

        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->p_dir = (unsigned long)dir;

        return responseLen;

    case TZ_VFS_READDIR:

        IMSG_DEBUG(" come into the TZ_VFS_READDIR function\n");

        dir = (DIR*)(tz_command->args.func_readdir_args.p_dir);
        read_count = tz_command->args.func_readdir_args.read_count;
        count = 0;

        while ((p_dirent = readdir(dir)) != NULL) {
            memcpy(
                (char*)((unsigned long)(tz_command) + PAGE_SIZE_4K + count * sizeof(struct dirent)),
                (char*)p_dirent, sizeof(struct dirent));
            count = count + 1;

            if (count == read_count)
                break;
        }

        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = count;

        return PAGE_SIZE_4K + count * sizeof(struct dirent);

    case TZ_VFS_CLOSEDIR:

        IMSG_DEBUG(" come into the TZ_VFS_CLOSEDIR function\n");

        dir = (DIR*)(tz_command->args.func_closedir_args.p_dir);

        retVal = closedir(dir);

        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = retVal;

        return responseLen;
//*/@Droi.zhangyonghua for tee 2018/1/16 start */
    case TZ_VFS_PRONODE:
        IMSG_DEBUG(" come into the TZ_VFS_PRONODE function\n");
        param_address = (char*)((unsigned long)(tz_command) + PAGE_SIZE_4K);
        IMSG_DEBUG("begin write data to %s ", PRO_NODE);
        retVal = strlen(PRO_NODE);
        memcpy((char *)param_address, PRO_NODE, retVal);
        memset((void*)p_buffer, '\0', responseLen);
        ((union TEEI_vfs_response*)p_buffer)->value = retVal;
        return PAGE_SIZE_4K + retVal;
//*/@Droi.zhangyonghua for tee 2018/1/16 end */
    default:
        IMSG_DEBUG(" Come into the default branch!!!\n");

        memset(&tz_response, '\0', responseLen);
        tz_response.value = -EOPNOTSUPP;
        memset((void*)p_buffer, '\0', responseLen);
        memcpy((void*)p_buffer, &tz_response, responseLen);
        return responseLen;
    }

    return 0;
}

static int init_TEEI_OS() {
    int fd = open("/dev/teei_config", O_RDWR);
    int flag = 0;
    int ret = ioctl(fd, TEEI_CONFIG_IOCTL_INIT_TEEI, &flag);
    if (0 != ret) {
        IMSG_ERROR("Can not init Soter OS ,please fix it ret = %d !", ret);

    } else {
        IMSG_INFO("begin to init TEEI OS OK flag = %d", flag);
    }
    close(fd);
    return ret;
}


static int init_OS_fn(void) {
    int ret = 0;
    IMSG_INFO("begin to init TEEI OS");
    ret = init_TEEI_OS();
    if (!ret) {
        property_set("soter.teei.init", "INIT_OK");
    }

    return 0;
}

/****************************** Dual TA Functions *******************/

int get_fp_vendor(int *p_vendor)
{
        int fd = -1;
        int ret = 0;

        if(!p_vendor)
        {
                return -1;
        }

        fd = open(DEV_FILE, O_RDWR);

        if(fd < 0)
        {
                IMSG_ERROR("open /dev/tz_vfs failed");
                return fd;
        }

        ret = ioctl(fd, GET_FP_VENDOR_CMD, p_vendor);

        if(ret < 0)
        {
                IMSG_ERROR("ioctl /dev/tz_vfs failed");
                goto out;
        }

out:
        if(fd > 0)
        {
                close(fd);
        }

        return ret;
}

void create_tee_storage_compatible_fp_server(void)
{
        int fp_vendor = 0;
        int ret;
        char img_ori[64];
        char tmp[64];
        unsigned char md5[32];

        ret = get_fp_vendor(&fp_vendor);

        if(ret < 0)
        {
                IMSG_ERROR("cannot get fp vendor id. return value is %d", ret);
                return;
        }

        switch(fp_vendor)
        {
                case FPC_VENDOR:
                        memset(img_ori, 0, sizeof(img_ori));
                        memset(tmp, 0, sizeof(tmp));
                        sprintf(img_ori, "/system/vendor/thh/fp_server_fpc");
                        sprintf(tmp, "/system/vendor/thh/fp_server_fpc");
                        IMSG_INFO("it's FPC sensor");
                        break;

                case GOODIX_VENDOR:
                        memset(img_ori, 0, sizeof(img_ori));
                        memset(tmp, 0, sizeof(tmp));
                        sprintf(img_ori, "/system/vendor/thh/fp_server_goodix");
                        sprintf(tmp, "/system/vendor/thh/fp_server_goodix");
                        IMSG_INFO("it's GOODIX sensor");
                        break;

                default:
                        IMSG_INFO("invalid fp vendor id %d\n", fp_vendor);
                        return;
                        break;
        }

        do_mkdir(3,dir5);
        do_mkdir(3,dirs[5]);

        memset(md5, 0, sizeof(md5));
        get_image_md5(img_ori, md5);
        write_md5_file("/data/thh/tee_05/tee.ori.md5", md5);
        write_md5_file("/data/thh/tee_05/tee.cur.md5", md5);

        COPY_FILE_FULLPATH(image_raw, tmp, "/data/thh/tee_05/tee");
}

/****************************KEYMASTER START***************************/

#define CMD_NOTIFY_UTD _IO(TEEI_IOC_MAGIC, 0x3)

/* for unlocking keymaster,and then can load tee in (kernel/tz_driver) */

static void keymaster_unlock(void) {
    int fd = open("/dev/teei_config", O_RDWR);
    int flag = 0;
    int ret = ioctl(fd, TEEI_CONFIG_IOCTL_UNLOCK, &flag);
    if (0 != ret) {
        IMSG_ERROR("keymaster unlock boot_decrypt_lock failed !");
    } else {
        IMSG_INFO("unlock boot_decrypt_lock success");
    }
    close(fd);
}

/* for the system first time boot, then the keymaster can store the key in rpmb block 38, this rpmb
 * place is just for all disk encrypt rsa key */
#define CMD_FIRST_TIME_BOOT _IO(TEEI_IOC_MAGIC, 0x4)
#define CMD_ID_FIREST_TIME_BOOT 101
typedef struct {
    unsigned int command_id;
    unsigned int command_data;  // if command_data=0xaf015838, this indicates that the phone is the
                                // first time boot.
} keymaster_command_t;

static void keymaster_firstboot(void) {
    unsigned char* temp_buffer;
    temp_buffer = malloc(sizeof(keymaster_command_t));
    keymaster_command_t keymaster_command;
    keymaster_command.command_id = CMD_ID_FIREST_TIME_BOOT;
    keymaster_command.command_data = 0x0;

    char value[PROPERTY_VALUE_MAX];
    property_get("ro.crypto.state", value, "");
    if (strcmp("", value) == 0) {
        keymaster_command.command_data = 0xaf015838;
        memcpy(temp_buffer, &keymaster_command, sizeof(keymaster_command_t));
        int fd = open("/dev/ut_keymaster", O_RDWR);
        int flag = 0;
        int ret = ioctl(fd, CMD_FIRST_TIME_BOOT, temp_buffer);
        if (0 != ret) {
            IMSG_ERROR("Can not load tees ,please fix it !");
        } else {
            IMSG_INFO("begin to load tees");
        }
        memcpy(&keymaster_command, temp_buffer, sizeof(keymaster_command_t));
        close(fd);
    }

    free(temp_buffer);
}

static int keymaster_first_time_fn(void) {

    keymaster_firstboot();
    return 0;
}
static int loadtee_fn(void) {
    IMSG_INFO("running in loadtee_fn.");

    char value[PROPERTY_VALUE_MAX];
    char value_type[PROPERTY_VALUE_MAX];
    char value_state[PROPERTY_VALUE_MAX];
    property_get("ro.crypto.state", value, "");
    property_get("ro.crypto.type", value_type, "");

    if (strcmp("unencrypted", value) != 0 && strcmp("unsupported", value) != 0) {
        /*data encrypted, wait for decrption.*/
        if(strcmp("file", value_type) != 0) {
            property_get("vold.decrypt", value, "");
            while (strcmp("trigger_restart_framework", value) != 0) {
            /*still decrypting... wait one second.*/
                usleep(10000);
                property_get("vold.decrypt", value, "");
            }
        }
    }

    property_get("soter.encrypt.state", value_state, "");

    while(strcmp("OK", value_state) != 0) {
        /*still decrypting... wait one second.*/
        usleep(10000);
        property_get("soter.encrypt.state", value_state, "");
    }
//*/@Droi.zhangyonghua for tee 2018/1/16 start */
#if 0
    IMSG_INFO("create tee storage ...");
    /*create_tee_storage(table[0],0);*/
    create_tee_storage(table[1]);
    create_tee_storage(table[2]);
    create_tee_storage(table[3]);
    create_tee_storage(table[4]);
	create_tee_storage(table[5]);
	create_tee_storage(table[6]);
	create_tee_storage(table[7]);
	create_tee_storage(table[8]);
	create_tee_storage(table[9]);
	create_tee_storage(table[10]);
	create_tee_storage(table[11]);
	create_tee_storage(table[12]);
	create_tee_storage(table[13]);
	create_tee_storage(table[14]);
	create_tee_storage(table[15]);
#endif
//*/@Droi.zhangyonghua for tee 2018/1/16 end */
#if 0
    do_mkdir(3, dirs[2]);
    do_mkdir(3, dirs[3]);
    do_mkdir(3, dirs[4]);
    do_mkdir(3, dirs[7]);
    do_mkdir(3, dirs[9]);
    do_mkdir(3, dirs[10]);
#endif


    keymaster_unlock();

    return 0;
}
/****************************KEYMASTER END***************************/

int main(int argc, char** argv) {
    int vfs_fd = 0;
    int len = 0;
    int retVal = 0;
    int writeLen = 0;
    unsigned char* rw_buffer = NULL;

    pthread_t ntid = 0;
    pthread_t loadtee_id = 0;
    pthread_t first_time_boot_id = 0;

    int debug = 0;
    (void)argc;
    (void)argv;


    IMSG_ERROR("TEEI Daemon VERSION [%s]", UTOS_VERSION);
    IMSG_INFO("TEEI Daemon VERSION [%s]", UTOS_VERSION);
    IMSG_INFO("TEEI Daemon start ...");
    property_set("soter.teei.init", "INIT_START");


    rw_buffer = (unsigned char*)malloc(BUFFER_SIZE);
    if (rw_buffer == NULL) {
        IMSG_ERROR("%s : %d Can not malloc enough memory.\n", __func__, __LINE__);
        return -1;
    }

    while (1) {
        vfs_fd = open(DEV_FILE, O_RDWR);
        if (vfs_fd < 0) {
            IMSG_ERROR("%s : %d Can not open the device node.\n", __func__, __LINE__);
            continue;
        }
        break;
    }

    pthread_create(&ntid, NULL, (void*)init_OS_fn, NULL);
    //	pthread_create(&first_time_boot_id, NULL, (void*)keymaster_first_time_fn, NULL);
    /*create a thread for start data area working*/
    pthread_create(&loadtee_id, NULL, (void*)loadtee_fn, NULL);

    while (1) {
        len = 31232;
        len = read(vfs_fd, rw_buffer, BUFFER_SIZE);
        IMSG_DEBUG("%s : %d read result = %d.\n", __func__, __LINE__, len);
        if (len < 0) {
            IMSG_ERROR("%s : %d Can not read the VFS device node, len = [%d] ret = [%d]. Wait for 1s\n",
                 __func__, __LINE__, len, errno);
            usleep(10000);
            continue;
        }
        retVal = analysis_command(rw_buffer);
        if (retVal < 0) {
            IMSG_ERROR("%s : %d Invail command read from VFS device node.\n", __func__, __LINE__);
            continue;
        }
        writeLen = retVal;
        retVal = write(vfs_fd, rw_buffer, writeLen);
        if (retVal < 0) {
            IMSG_ERROR("%s : %d Can not write to VFS device node.\n", __func__, __LINE__);
            continue;
        }
    }

    free(rw_buffer);
    close(vfs_fd);


    IMSG_INFO("TEEI Daemon start OK ...");
    return 0;
}
