#!/bin/bash

#SOURCE_GITHUB="https://github.com/baronZhou/Helloworld.git"
SOURCE_GITHUB="https://github.com/signfpserver/signfpserver.git"
CURRENT_DIR=`pwd`

allfp_serverfile=$CURRENT_DIR/allfp_serverfile.txt

echo "###################################################"
echo "Start to sign fpserver....."
cd $CURRENT_DIR
FLAG="true"


if [ -d signfpserver ]
then
echo "git pull ....."
cd $CURRENT_DIR/signfpserver
git checkout .
git pull
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

touch $CURRENT_DIR/signfpserver/$every_name/md5_fpserver.txt
md5fpserverold=`cat $CURRENT_DIR/signfpserver/$every_name/md5_fpserver.txt`
md5fpservernew=`md5sum fp_server | awk '{print $1}'`

echo "zhouhehe test:00::"
cat $CURRENT_DIR/signfpserver/$every_name/md5_fpserver.txt
echo $md5fpserverold $md5fpservernew
if [ "$md5fpserverold" = "$md5fpservernew" ]        #999999999999999999999999999999999999999999999999999999
then
echo "fp_server has not changed"

############################################# end ############################

else  #999999999999999999999999999999999999999999999999999999
FLAG="false"
echo "fp_server has changed"
echo $md5fpservernew > $CURRENT_DIR/signfpserver/$every_name/md5_fpserver.txt
echo "zhouhehe test:::"
cat $CURRENT_DIR/signfpserver/$every_name/md5_fpserver.txt

if [ -f fp_server ]
then
echo "fp_server is exsist..."
#./auto_sign.sh
else
echo "fp_server is not exsist..."
fi

#------------------------------
if [ -f signfp_server ]
then
echo "sign signfp_server succesfull"

cp signfp_server $CURRENT_DIR/signfpserver/$every_name/
cd $CURRENT_DIR/signfpserver
fpserverinfomd5=`md5sum ../fp_server | awk '{print $1}'`
signfpserverinfomd5=`md5sum ../signfp_server | awk '{print $1}'`
echo "fpserverinfomd5:"$fpserverinfomd5"  ,  signfpserverinfomd5:"$signfpserverinfomd5
echo "fp_server:"$fpserverinfomd5 >$every_name/md5info.txt
echo "signfp_server:"$signfpserverinfomd5 >>$every_name/md5info.txt

else
echo "sign signfp_server NOT succesfull"
fi
#------------------------------

fi  #999999999999999999999999999999999999999999999999999999
done



if [ "$FLAG" = "false" ]
then
cd $CURRENT_DIR/signfpserver
git status .
git add .
git commit . -m "upload signfpserver test_hehehe"


/usr/bin/expect <<-EOF
spawn git push
expect "com\':"
send "signfpserver\r"
sleep 1
send "signfpserver0000\r"
expect eof
EOF

fi

################################
#rm fp_server signfp_server

echo "sign fpserver succesfull"







