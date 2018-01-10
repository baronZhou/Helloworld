#! /bin/bash

COMMITOLD="e9f42a37"
COMMITNEW="a0ae7f7b"
SORCE_DR=/home/michael/code/6580O/alps-release-o1.mp2-default_2
PATCH=/home/michael/code/6580O/bkp_doujia_zhouhehe/111111111111/6580_androidO_patch

NEW=$PATCH/new
OLD=$PATCH/old
mkdir $NEW $OLD

for i in `cat g_differ_all_house.file`
do
#echo $i
cangku=`echo $i | awk -F":" '{print $3}'`
commitold=`echo $i | awk -F":" '{print $1}'`
commitnew=`echo $i | awk -F":" '{print $2}'`
echo "cangku: "$cangku
echo "commitold: "$commitold
echo "commitnew: "$commitnew

cd $SORCE_DR/$cangku
touch 1temp.txt
cp --parent 1temp.txt $NEW
cp --parent 1temp.txt $OLD
rm 1temp.txt
rm $NEW/1temp.txt
rm $OLD/1temp.txt
#### $NEW/$cangku   $OLD/$cangku


##########################################################
COMMITOLD=commitold
COMMITNEW=commitnew
echo "commit old="$COMMITOLD" , commit new="$COMMITNEW

cd $SORCE_DR/$cangku

git reset $COMMITNEW --hard

`git diff $COMMITOLD $COMMITNEW >> $PATCH/android_patch.diff`
`git diff $COMMITOLD $COMMITNEW --name-only > $PATCH/temp.txt`
for i in `cat $PATCH/temp.txt`
do
#echo "xx:"$i
cp --parents $i $NEW -r
done

#git revert $COMMITNEW
git reset $COMMITOLD --hard

for i in `cat $PATCH/temp.txt`
do
#echo "xx:"$i
cp --parents $i $OLD -r
done

rm $PATCH/temp.txt
##########################################################

done ##for i in `cat g_differ_all_house.file`



