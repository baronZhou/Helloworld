#! /bin/bash

workshop=~/workspace/gerrit/workshop
CURRENT_DIR=`pwd`


null="NULL"
LOG=$CURRENT_DIR/log.txt
TEMP1=$CURRENT_DIR/temp1.txt

cd $CURRENT_DIR
cat $CURRENT_DIR/project_file.txt | grep "##" >$TEMP1

for LINE in `cat $TEMP1`
do
platform=`echo $LINE | awk -F"##" '{print $1}'`
module=`echo $LINE | awk -F"##" '{print $2}'`
reposist=`echo $LINE | awk -F"##" '{print $3}'`
branch=`echo $LINE | awk -F"##" '{print $4}'`
echo "11111111111111111111start11111111">>$LOG
echo "The "$platform":"$module "start..........." >>$LOG
echo "platform="$platform" , module="$module" , reposist="$reposist" , branch="$branch >>$LOG

path_name=$workshop/src/$platform
#path_name=~/workspace/github/Helloworld
echo "path name="$path_name>>$LOG
if [ -d "$path_name" ]
then

cd $path_name

if [ "$branch" = "$null" ]
then
proxychains git clone $reposist
#echo "branch="$branch"    , "$module" branch==null"
else
proxychains git clone $reposist -b $branch
fi

echo "The "$platform":"$module "download completely." >>$LOG
#echo -e "\033[31mThe "$platform":"$module "download completely.\033[0m"

fi
done
#rm $TEMP1
echo "THE shell end">>$LOG
