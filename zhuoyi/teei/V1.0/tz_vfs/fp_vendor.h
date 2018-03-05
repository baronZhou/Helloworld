#ifndef __FP_VENDOR_H__
#define __FP_VENDOR_H__
/*@Droi.zhangyonghua for tee 2018/1/16 start */
#if CFG_MICROTRUST_TEE_SUPPORT
#include <linux/types.h>

#define MAX_TA_NAME 64

// if add a new id , sould sort by alphabetic
enum {
    FP_VENDOR_MICROARRAY,//迈瑞微

    FP_VENDOR_SUNWAVE,//信炜
};

void set_fp_vendor(uint8_t fp_vendor_id);
uint8_t get_fp_vendor(void);
void get_fp_ta_load_path(char* fp_ta_load_path);
/*
 * return 1 if fp driver can call spi in ree, else return 0
*/
int get_fp_spi_enable(void);

extern int fp_spi_enable;
#endif
/*@Droi.zhangyonghua for tee 2018/1/16 end*/
#endif  /*__FP_VENDOR_H__*/
