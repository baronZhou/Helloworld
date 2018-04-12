#!/bin/bash

SOURCE_GITHUB="https://github.com/signfpserver/signfpserver.git"
CURRENT_DIR=`pwd`


cd $CURRENT_DIR
patchname=`cat setting.file | awk -F":" '{print $1}'`
echo "patchname="$patchname

echo "git pull ....."
cd $CURRENT_DIR/signfpserver
git checkout  $patchname
git checkout .
git pull origin $patchname

cd $CURRENT_DIR
md5server=`cat $CURRENT_DIR/signfpserver/$patchname/md5_fpserver.txt`
md5bendi=`md5sum fp_server | awk '{print $1}'`


echo "md5server="$md5server" ,md5bendi="$md5bendi

if [ "$md5server" = "$md5bendi" ]
then
cp $CURRENT_DIR/signfpserver/$patchname/signfp_server .
echo -e "\033[31m------------------------------------------------\033[0m"
echo -e "\033[31m Succesfullly\033[0m"
echo -e "\033[31m------------------------------------------------\033[0m"
else
echo -e "\033[31m------------------------------------------------\033[0m"
echo -e "\033[31m Faild\033[0m"
echo -e "\033[31m------------------------------------------------\033[0m"
fi
