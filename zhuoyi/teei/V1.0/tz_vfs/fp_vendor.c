#include "fp_vendor.h"
/*@Droi.zhangyonghua for tee 2018/1/16 start */
//#if CFG_MICROTRUST_TEE_SUPPORT
#include<linux/kernel.h>
#include <linux/platform_device.h>
#include<linux/module.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/errno.h>
#include<linux/mm.h>
#include<linux/sched.h>
#include<linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/semaphore.h>
#include <linux/slab.h>

#include <linux/mutex.h>
#include <linux/string.h>
#include <imsg_log.h>

static char* __default_ta_load_path = "fp_server";  //zhouhehe
int fp_spi_enable = 1;
// if add a new id , sould sort by alphabetic
int FP_VENDOR_MAX=3;
static char __fp_ta_load_path[3][MAX_TA_NAME] = {
    "fp_server_microarray",//FP_VENDOR_MICROARRAY
    "fp_server_sunwave",//FP_VENDOR_SUNWAVE
};

static DEFINE_MUTEX(fp_vendor_lock);
static uint8_t __fp_vendor_id = 255;

void set_fp_vendor(uint8_t fp_vendor_id)
{
    // check param
    if (fp_vendor_id>FP_VENDOR_MAX) {
        IMSG_ERROR("%s:%d param error , fp_vendor_id=%d,FP_VENDOR_MAX=%d, fp_vendor_id>FP_VENDOR_MAX", __func__, __LINE__,  fp_vendor_id, FP_VENDOR_MAX);
        return;
    }

    // set only onece
    if (__fp_vendor_id==255) {
        mutex_lock(&fp_vendor_lock);
        __fp_vendor_id = fp_vendor_id;
        mutex_unlock(&fp_vendor_lock);
        IMSG_INFO("%s:%d set __fp_vendor_id=%d", __func__, __LINE__, __fp_vendor_id);
    } else {
        IMSG_WARN("%s:%d __fp_vendor_id already seted __fp_vendor_id = %d, return Directly", __func__, __LINE__, __fp_vendor_id);
    }
}

uint8_t get_fp_vendor()
{
    if (__fp_vendor_id>FP_VENDOR_MAX) {
        IMSG_ERROR("%s:%d param error , __fp_vendor_id=%d, __fp_vendor_id<0 || __fp_vendor_id>FP_VENDOR_MAX , return -1  ", __func__, __LINE__,  __fp_vendor_id);
        return -1;
    }

    IMSG_INFO("%s:%d get __fp_vendor_id=%d", __func__, __LINE__, __fp_vendor_id);

    return __fp_vendor_id;
}

// make sure fp_ta_load_path's len <= MAX_TA_NAME
void get_fp_ta_load_path(char* fp_ta_load_path)
{
    if (__fp_vendor_id>FP_VENDOR_MAX) {
        IMSG_ERROR("%s:%d param error , __fp_vendor_id=%d, __fp_vendor_id<0 || __fp_vendor_id>FP_VENDOR_MAX , return __default_ta_load_path=%s", __func__, __LINE__,  __fp_vendor_id, __default_ta_load_path);
        strcpy(fp_ta_load_path, __default_ta_load_path);
        return;
    }

    memcpy(fp_ta_load_path,__fp_ta_load_path[__fp_vendor_id],MAX_TA_NAME);
    IMSG_INFO("%s:%d fp_ta_load_path = %s", __func__, __LINE__, fp_ta_load_path);
}

int get_fp_spi_enable()
{
    if (fp_spi_enable==0) {
        IMSG_ERROR("%s:%d  ERROR fp_spi_enable==0", __func__, __LINE__);
    }

    IMSG_INFO("%s:%d fp_spi_enable==%d", __func__, __LINE__, fp_spi_enable);

    return fp_spi_enable;
//#endif
/*@Droi.zhangyonghua for tee 2018/1/16 end*/
}
