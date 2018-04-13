#!/bin/bash

#SOURCE_GITHUB="https://github.com/baronZhou/Helloworld.git"
SOURCE_GITHUB="https://github.com/signfpserver/signfpserver.git"
CURRENT_DIR=`pwd`
TEMP1=$CURRENT_DIR/temp1.txt
TRUE=0


echo "###################################################"
echo "Start to sign fpserver....."
cd $CURRENT_DIR


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
git clone --depth=1 $SOURCE_GITHUB
fi

cd $CURRENT_DIR/signfpserver


cd $CURRENT_DIR/signfpserver
every_name=`echo $every_fp`

cd $CURRENT_DIR
mkdir temp_fpserver_dir


# 1 #############  for循环： 把需要签名的，都拎出来############################
cd $CURRENT_DIR/signfpserver
find * >$TEMP1
for fpserver_name in `cat $TEMP1 | grep -v "sign" `
do
FLAG5=`find ./ -name $fpserver_name | awk -F"_" '{print $5}' `   #是否为新提交的fp_server
FLAG4=`find ./ -name $fpserver_name | awk -F"_" '{print $4}' `   #用户随机生成的id码
FLAG3=`find ./ -name $fpserver_name | awk -F"_" '{print $3}' `   #密码次数
cd $CURRENT_DIR

password_right=1;
if [ "$FLAG5" = "$password_right" ]   #如果flag5=1，是新的fp_server
then

./y $FLAG3   #密码减一
password_result=`cat $CURRENT_DIR/result.file | awk -F":" '{print  $1}'`
password_right=1;
if [ "$password_result" = "$password_right" ]  #如果flag4，密码大于0
then
############### 需要签名 ###################
mv signfpserver/$fpserver_name temp_fpserver_dir
fi #flag4
fi  #flag5
done
##############  for循环：end############################





# 2 #############  for循环： 签名 ############################
cd $CURRENT_DIR/signfpserver
cd ../temp_fpserver_dir
find * >$TEMP1
for fpserver_name in `cat $TEMP1`
do
FLAG5=`find ./ -name $fpserver_name | awk -F"_" '{print $5}' `   #是否为新提交的fp_server
FLAG4=`find ./ -name $fpserver_name | awk -F"_" '{print $4}' `   #用户随机生成的id码
FLAG3=`find ./ -name $fpserver_name | awk -F"_" '{print $3}' `   #密码次数
fpserver_result_name="fp_server_"$FLAG3"_"$FLAG4"_0_0"  #返回成功的结果
signfpserver_result_name="signfp_server_"$FLAG3"_"$FLAG4"_0_0"  #返回成功的结果

cd $CURRENT_DIR
cp temp_fpserver_dir/$fpserver_name fp_server
./auto_sign.sh
mv signfp_server temp_fpserver_dir/$signfpserver_result_name
#cp temp_fpserver_dir/$fpserver_name temp_fpserver_dir/$fpserver_result_name
rm temp_fpserver_dir/$fpserver_name
rm signfpserver/$fpserver_name

echo -e "\033[31mxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\033[0m"
TRUE=1
done
##############  for循环：end############################



# 3 #############  fp_server和signfp_server全部拷贝回去 ############################
cd $CURRENT_DIR/signfpserver
cd ../temp_fpserver_dir
cp * $CURRENT_DIR/signfpserver
##############  end############################


password_right=1;
if [ "$TRUE" = "$password_right" ]  #TRUE=1 需要提交代码
then


cd $CURRENT_DIR/signfpserver
git status .
git add -A
git commit . -m "upload signfpserver test_hehehe"

/usr/bin/expect <<-EOF
spawn git push origin $every_name
expect "com\':"
send "signfpserver\r"
sleep 1
send "signfpserver0000\r"
expect eof
EOF

fi


cd $CURRENT_DIR
rm -rf  temp_fpserver_dir
#rm $TEMP1

echo "sign fpserver succesfull"