#!/bin/bash

password_count=apdlyswqum   ## please modify it

SOURCE_GITHUB="https://github.com/signfpserver/signfpserver.git"
CURRENT_DIR=`pwd`

if [ -d signfpserver ]
then
cd $CURRENT_DIR
patchname=`cat setting.file | awk -F":" '{print $1}'`
echo "patchname="$patchname

echo "git pull ....."
cd $CURRENT_DIR/signfpserver
git checkout  $patchname
git checkout .
git pull origin $patchname

else
cd $CURRENT_DIR
./x   #setting.file
patchname=`cat setting.file | awk -F":" '{print $1}'`
echo "patchname="$patchname

echo  -e "\033[31mIt will spend some time because you use this shell in the first time. \033[0m"
echo -e "\033[31mPlease wait it patiently. \033[0m"
echo "git clone ....."
git clone $SOURCE_GITHUB
cd $CURRENT_DIR/signfpserver
git reset 69da49d --hard
git checkout -b $patchname

mkdir $patchname
fi


cd $CURRENT_DIR/signfpserver
echo $password_count":" > $patchname/passwork_count.txt
cp ../fp_server $patchname/ ####################copy##########################
git add $patchname
git commit $patchname -m "upload fp_server:$patchname"

/usr/bin/expect <<-EOF
spawn git push origin $patchname
expect "com\':"
send "signfpserver\r"
sleep 1
send "signfpserver0000\r"
expect eof
EOF
echo "we have upload fp_server and we are waiting for signing it completely...."
echo "Don't worry. The time which you wait for must be less than 150s. Please wait it patiently...."


echo -e "\033[31m------------------------------------------------\033[0m"
echo -e "\033[31m It has uploaded the fp_server to Server.\n Please run fetch_fpserver.sh for fetching signfp_server in 10min. THX\033[0m"
echo -e "\033[31m------------------------------------------------\033[0m"
 