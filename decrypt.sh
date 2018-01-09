#! /bin/bash

#####################
#### SOURCE_DIR改成要解密的文件路径
###  FILE_OUT_DIR 输出文件路径
SOURCE_DIR=/home/zhouhehe/gerrit/workshop/src/reallytek_F311/android_N/kernel-3.18/drivers/misc/mediatek/teei
FILE_OUT_DIR=/home/zhouhehe/gerrit/workshop/src/reallytek_F311/teei


TEMP_D=$FILE_OUT_DIR/1.txt
TEMP_F=$FILE_OUT_DIR/2.txt
TEMP_TARGET=$FILE_OUT_DIR/3.txt


cd $SOURCE_DIR
find ./ -type d > $TEMP_D

for iD in `cat $TEMP_D`
do

pathname=`echo $iD`
#echo "######################"$pathname
cd $SOURCE_DIR/$iD
ls > $TEMP_F
for iF in `cat $TEMP_F`
do
filename=`echo $iF`
if [ -d $filename ];then
	echo $filename"is a DIR,skip!"
else
	cat $filename>$filename"bzhouhehe.txt"
fi

#echo "x:"$filename
done

#filename=`echo $i | awk -F/ '{print $NF}'`
#pach=`echo $i | xargs find -type d`

done
rm $TEMP_D
rm $TEMP_F

cd $SOURCE_DIR
#echo $TEMP_TARGET
find ./ -name "*zhouhehe.txt" > $TEMP_TARGET

for ii in `cat $TEMP_TARGET`
do
echo $SOURCE_DIR/$ii
echo $FILE_OUT_DIR
cp --parents $ii $FILE_OUT_DIR
done
cd $SOURCE_DIR
find ./ -name "*zhouhehe.txt" | xargs rm 
rm $TEMP_TARGET

################################
FILE_OUT_DIRRR=$FILE_OUT_DIR

TEMP1=$TEMP_D

cd $FILE_OUT_DIRRR
find ./ -name "*zhouhehe.txt" >$TEMP1

for i in `cat $TEMP1`
do
echo $i
ii=`echo $i | awk -F'bzhouhehe' '{print $1}'`
echo $ii
mv $i $ii
done
rm $TEMP_D




