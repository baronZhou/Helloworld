#! /bin/bash

#####################
#### 使用说明: 将此脚本放在old/new的同级目录下，执行这个脚本，即输出了filelist.txt文件

#####################
#### 在使用脚本之前，以下三个信息需填写
BEANPOD_PLATFORM=mt6580
BEANPOD_PROJECT=k80hd_bsp_fwv_512m
BEANPOD_PATCH_DIR=~/workspace/github/6580_androidO_patch_last_20171226_zhouhehe


CURRENT_DIR=`pwd` ##########
TEMP_1txt=$BEANPOD_PATCH_DIR/filelist.txt  #dir
TEMP_2txt=$BEANPOD_PATCH_DIR/filelist2.txt  #dir

cd $BEANPOD_PATCH_DIR

####---------------自动修改copy_files_to_beanpod.sh脚本-------------------##
NAME=`cat copy_files_to_beanpod.sh | grep "BEANPOD_PLATFORM=" | awk -F"=" '{print $2}'`
cat copy_files_to_beanpod.sh | sed "s/BEANPOD_PLATFORM=$NAME/BEANPOD_PLATFORM=$BEANPOD_PLATFORM/" >$TEMP_1txt
NAME=`cat $TEMP_1txt | grep "BEANPOD_PROJECT=" | awk -F"=" '{print $2}'`
cat $TEMP_1txt | sed "s/BEANPOD_PROJECT=$NAME/BEANPOD_PROJECT=$BEANPOD_PROJECT/" >$TEMP_2txt
cat $TEMP_2txt > copy_files_to_beanpod.sh  # 自动修改mt6580/k80hd_bsp_fwv_512m到copy_files_to_beanpod.sh
rm  $TEMP_1txt $TEMP_2txt
#------------------------------end----------------------------------------##



##-----------将old/new拷贝到temp_dir，并生成filelist.txt-----------########
mkdir temp_dir
cp new/alps/  temp_dir/ -r
cp old/alps/  temp_dir/ -r
cd $BEANPOD_PATCH_DIR/temp_dir/alps
find * > $TEMP_1txt
cat $TEMP_1txt
#------------------------------end----------------------------------------##


cd $BEANPOD_PATCH_DIR
rm -rf temp_dir
cd $CURRENT_DIR
cp $TEMP_1txt .

echo "\n\n\n"
echo "####################################################"
echo "####################################################"
echo -e "\033[31m# Succeful!!! \033[0m"
echo "# Please send filelist.txt to OEM/ODM, and merge the patch completely!"
echo "####################################################"
echo "####################################################"
echo $NAME $BEANPOD_PLATFORM
