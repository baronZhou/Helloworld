#!/bin/bash

SOURCE_GITHUB="https://github.com/signfpserver/signfpserver.git"
CURRENT_DIR=`pwd`

echo "Start to sign fpserver....."


if [ -d signfpserver ]
then
echo "git pull ....."
cd $CURRENT_DIR/signfpserver
git checkout .
git pull
cd $CURRENT_DIR
patchname=`cat setting.file | awk -F":" '{print $1}'`
echo "patchname="$patchname
else
echo "git clone ....."
cd $CURRENT_DIR
git clone $SOURCE_GITHUB

./x   #setting.file
patchname=`cat setting.file | awk -F":" '{print $1}'`
echo "patchname="$patchname
cd $CURRENT_DIR/signfpserver
mkdir $patchname
fi


cd $CURRENT_DIR/signfpserver
cp ../fp_server $patchname/ ####################copy##########################
git add $patchname
git commit $patchname -m "upload fpserver testhhhhh"
#git push
#./secret.sh

/usr/bin/expect <<-EOF
spawn git push
expect "com\':"
send "signfpserver\r"
sleep 1
send "signfpserver0000\r"
expect eof
EOF
echo "we have upload fp_server and we are waiting for signing it completely...."
echo "Don't worry. The time which you wait for must be less than 150s. Please wait it patiently...."



#echo -e "\033[31m#1 sign Succeful!!! \033[0m"
