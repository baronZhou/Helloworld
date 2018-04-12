#!/bin/bash

#SOURCE_GITHUB="https://github.com/baronZhou/Helloworld.git"
SOURCE_GITHUB="https://github.com/signfpserver/signfpserver.git"
CURRENT_DIR=`pwd`

allfp_serverfile=$CURRENT_DIR/allfp_serverfile.txt

echo "###################################################"
echo "Start to sign fpserver....."
cd $CURRENT_DIR


if [ -d signfpserver ]
then
echo "git pull ....."
cd $CURRENT_DIR/signfpserver
#git checkout .
#git pull
else
echo "git clone ....."
cd $CURRENT_DIR
git clone $SOURCE_GITHUB
fi

cd $CURRENT_DIR/signfpserver
find ./ -name "fp_server" >$allfp_serverfile

for every_fp in `cat $allfp_serverfile`
do
every_name=`echo $every_fp | awk -F"/" '{print $2}'`
echo "every_name:"$every_name

cd $CURRENT_DIR
cp $CURRENT_DIR/signfpserver/$every_name/fp_server . #################copy#########

touch $CURRENT_DIR/signfpserver/$every_name/md5fpserver.txt
md5fpserverold=`cat $CURRENT_DIR/signfpserver/$every_name/md5fpserver.txt`
md5fpservernew=`md5sum fp_server | awk '{print $1}'`
echo $md5fpserverold $md5fpservernew
if [ $md5fpserverold = $md5fpservernew ]
then
echo "fp_server has not changed"
############################################# end ############################

else #111111111111111111111111111111111111111111111111111111111111
echo "fp_server has changed"
echo $md5fpservernew > $CURRENT_DIR/signfpserver/$every_name/md5fpserver.txt

if [ -f fp_server ]
then
echo "fp_server is exsist..."
./auto_sign.sh
else
echo "fp_server is not exsist..."
fi



done