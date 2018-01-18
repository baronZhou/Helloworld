#! /bin/bash

#####################
####在使用脚本之前，以下三个信息需填写
# PLATFORM平台，PROJECT项目名字
# ANDROID_SORCE_DIR： android代码的绝对路径
# OUT_FILES_DIR：抽取的文件所放的位置

PLATFORM=mt6580
PROJECT=k80hd_bsp_fwv_512m
ANDROID_SORCE_DIR=~/workspace/github/6580_androidO_patch_last_20171226_zhouhehe/new/alps
OUT_FILES_DIR=~/workspace/github/6580_androidO_patch_last_20171226_zhouhehe/temp_from_oem

CURRENT_DIR=`pwd`
#------------------------------------------------------
BEANPOD_PLATFORM=mt6580
BEANPOD_PROJECT=k80hd_bsp_fwv_512m
FILELIST=$CURRENT_DIR/filelist.txt
FILELIST1=$CURRENT_DIR/filelist1.txt
FILELIST2=$CURRENT_DIR/filelist2.txt

cd $CURRENT_DIR

####----------改名：将filelist.txt文件中的mt6580/k80hd_bsp_fwv_512m改成客户办理对应的名字-----------####
#cat filelist.txt | grep  -v -E "mt6580|k80hd_bsp_fwv_512m" >$FILELIST1
echo $BEANPOD_PLATFORM   $PLATFORM
echo $BEANPOD_PROJECT  $PROJECT
cat filelist.txt | sed  "s/$BEANPOD_PLATFORM/$PLATFORM/" >$FILELIST1
cat $FILELIST1 | sed  "s/$BEANPOD_PLATFORM/$PLATFORM/" >$FILELIST2

cat $FILELIST2 | sed  "s/$BEANPOD_PROJECT/$PROJECT/" >$FILELIST1
cat $FILELIST1 | sed  "s/$BEANPOD_PROJECT/$PROJECT/" >$FILELIST2
cat $FILELIST2>$FILELIST

#cat $FILELIST2 | sed  "/misc\/mediatek\/teei/"d >$FILELIST  #####若此patch包含完整的Tz driver则放开这一句
rm $FILELIST1 $FILELIST2
#------------------------------------------------------





FILELIST=$CURRENT_DIR/filelist.txt
cd $OUT_FILES_DIR
rm -rf alps_bean
mkdir alps_bean

TEMP1TXT=$FILE_OUT_DIR/1.txt
TEMP2TXT=$FILE_OUT_DIR/2.txt
TEMP3TXT=$FILE_OUT_DIR/3.txt

cd $ANDROID_SORCE_DIR
for iF in `cat $FILELIST`
do
filename=`echo $iF`
if [ -d $filename ];then
	xx=1
else
	cp --parent $filename $OUT_FILES_DIR/alps_bean 2>/dev/null
fi

#echo "x:"$filename
done
