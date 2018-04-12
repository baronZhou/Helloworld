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
git checkout master
git checkout .
git pull
else
echo "git clone ....."
cd $CURRENT_DIR
git clone $SOURCE_GITHUB
fi

cd $CURRENT_DIR/signfpserver
find ./ -name "fp_server" >$allfp_serverfile

#git branch -a | grep "origin" | grep -v "master"  | awk -F"/" '{print $3}'
for every_fp in `git branch -a | grep "origin" | grep -v "master"  | awk -F"/" '{print $3}'`  ##for for for for
do
cd $CURRENT_DIR/signfpserver
every_name=`echo $every_fp`
echo -e "\033[31m[every_name]\033[0m:"$every_name
git checkout $every_name
git pull origin $every_name

cd $CURRENT_DIR
cp $CURRENT_DIR/signfpserver/$every_name/fp_server . #################copy#########

touch $CURRENT_DIR/signfpserver/$every_name/md5_fpserver.txt
md5fpserverold=`cat $CURRENT_DIR/signfpserver/$every_name/md5_fpserver.txt`
md5fpservernew=`md5sum fp_server | awk '{print $1}'`

cat $CURRENT_DIR/signfpserver/$every_name/md5_fpserver.txt
#echo $md5fpserverold $md5fpservernew
if [ "$md5fpserverold" = "$md5fpservernew" ]        #999999999999999999999999999999999999999999999999999999
then
echo "fp_server has not changed"

############################################# end ############################

else  #999999999999999999999999999999999999999999999999999999
FLAG="false"
echo -e "\033[31mfp_server has changed\033[0m"


cd $CURRENT_DIR
password_counter=`cat $CURRENT_DIR/signfpserver/$every_name/passwork_count.txt | awk -F":" '{print  $1}'`
echo "password_counter="$password_counter
./y $password_counter
password_result=`cat $CURRENT_DIR/result.file | awk -F":" '{print  $1}'` ##%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
password_right=1;
if [ "$password_result" = "$password_right" ]
then
echo -e "\033[31m 11111111111111111111111111111 \033[0m"



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

cd $CURRENT_DIR/signfpserver
git status .
git add .
git commit . -m "upload signfpserver test_hehehe"

/usr/bin/expect <<-EOF
spawn git push origin $every_name
expect "com\':"
send "signfpserver\r"
sleep 1
send "signfpserver0000\r"
expect eof
EOF

else
echo "sign signfp_server NOT succesfull"
fi
#------------------------------

fi  #999999999999999999999999999999999999999999999999999999


fi
done


################################
#rm fp_server signfp_server

echo "sign fpserver succesfull"







