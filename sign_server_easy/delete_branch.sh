#!/bin/bash

CURRENT_DIR=`pwd`

cd $CURRENT_DIR/signfpserver
#git branch -a | grep "origin" | grep -v "master"  | awk -F"/" '{print $3}'
for every_fp in `git branch -a | grep "origin" | grep -v "master"  | awk -F"/" '{print $3}'`
do
every_name=`echo $every_fp`
bbranch="origin/"$every_name
echo $bbranch

git branch -r -d $bbranch

/usr/bin/expect <<-EOF
spawn git push origin :$every_name
expect "com\':"
send "signfpserver\r"
sleep 1
send "signfpserver0000\r"
expect eof
EOF

done



echo "sign fpserver succesfull"







