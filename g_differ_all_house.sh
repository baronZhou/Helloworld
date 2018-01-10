#! /bin/bash

SORCE_DR=~/gerrit/workshop/src/leagoo_F518
PATCH=~/gerrit/workshop/src/leagoo_F518/patch
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

echo "11111111111"
echo "xx:"$SORCE_DR
echo "xx:"$cangku
echo "22222222222222222"
cd $SORCE_DR
echo "11111111111aaaaaaaaa"
cd $cangku
echo "333333333333333"
touch 1temp.txt
cd $SORCE_DR
ls $cangku/1temp.txt
ls $NEW
cp --parent $cangku/1temp.txt $NEW
cp --parent $cangku/1temp.txt $OLD
rm $cangku/1temp.txt
rm $NEW/$cangku/1temp.txt
rm $OLD/$cangku/1temp.txt
#### $NEW/$cangku   $OLD/$cangku


##########################################################
COMMITOLD=$commitold
COMMITNEW=$commitnew
echo "$$$$$$$$$$$$$$$$$$$$$$$$#"$cangku"$$$$$$$$$$$$$$$$$$$$$$$$#"
echo "commit old="$COMMITOLD" , commit new="$COMMITNEW

cd $SORCE_DR/$cangku

git reset $COMMITNEW --hard

`git diff $COMMITOLD $COMMITNEW >> $PATCH/android_patch.diff`
`git diff $COMMITOLD $COMMITNEW --name-only > $PATCH/temp.txt`
for i in `cat $PATCH/temp.txt`
do
#echo "xx:"$i
cp --parents $i $NEW/$cangku -r
done

#git revert $COMMITNEW
git reset $COMMITOLD --hard

for i in `cat $PATCH/temp.txt`
do
#echo "xx:"$i
cp --parents $i $OLD/$cangku -r
done

rm $PATCH/temp.txt
##########################################################

done ##for i in `cat g_differ_all_house.file`



