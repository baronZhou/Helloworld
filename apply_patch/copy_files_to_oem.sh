#! /bin/bash

###################
#### 将豆荚的修改一键打入客户代码

PLATFORM=mt6780
PROJECT=k80hd_bsp_fwv_513
ANDROID_SORCE_DIR=~/workspace/github/6580_androidO_patch_last_20171226_zhouhehe/android_test/alps
MODIFY_FILES_DIR=~/workspace/github/6580_androidO_patch_last_20171226_zhouhehe/temp_from_oem

CURRENT_DIR=`pwd`
FILELIST=$CURRENT_DIR/filelist.txt

TEMP_1txt=$MODIFY_FILES_DIR/1.txt
TEMP_2txt=$MODIFY_FILES_DIR/2.txt
TEMP_3txt=$MODIFY_FILES_DIR/3.txt

cd $MODIFY_FILES_DIR/alps_bean

find * > $TEMP_1txt


#####若此patch包含完整的Tz driver则放开这三句
cat $TEMP_1txt | sed  "/misc\/mediatek\/teei/"d >$FILELIST  
rm -rf $ANDROID_SORCE_DIR/kernel-3.18/drivers/misc/mediatek/teei
cp $MODIFY_FILES_DIR/alps_bean/kernel-3.18/drivers/misc/mediatek/teei $ANDROID_SORCE_DIR/kernel-3.18/drivers/misc/mediatek/
#------------------------------end------------------------------------------#

for iF in `cat $FILELIST`  #222222222222222222222222222222222222222222222222222
do
filename=`echo $iF`
if [ -d $filename ];then
	xx=1
else
	cp --parent $filename $ANDROID_SORCE_DIR 2>/dev/null
fi

#echo "x:"$filename
done  #222222222222222222222222222222222222222222222222222
