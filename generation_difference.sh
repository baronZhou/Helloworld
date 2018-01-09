#! /bin/bash

COMMITOLD="e9f42a37"
COMMITNEW="a0ae7f7b"
SORCE_DR=/home/michael/code/6580O/alps-release-o1.mp2-default_2
PATCH=/home/michael/code/6580O/bkp_doujia_zhouhehe/111111111111/6580_androidO_patch

NEW=$PATCH/new
OLD=$PATCH/old

mkdir $NEW $OLD

echo $COMMITOLD
echo $COMMITNEW

cd $SORCE_DR

git reset $COMMITNEW --hard


`git diff $COMMITOLD $COMMITNEW > $PATCH/android_patch.diff`
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
