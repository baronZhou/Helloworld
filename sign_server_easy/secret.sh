#!/usr/bin/expect -f

spawn scp -P 36046 fp_server zhouhehe@59.46.77.87:~/gerrit/workshop/burn/sign_img
expect "zhouhehe:"
send "zhh1234@\r"

sleep 1
spawn ssh zhouhehe@59.46.77.87 -p 36046
expect "zhouhehe:"
send "zhh1234@\r"

sleep 1
send "fp_rel_make_sign \r"

spawn scp -P 36046 zhouhehe@59.46.77.87:~/gerrit/workshop/burn/sign_img/signfp_server .
expect "zhouhehe:"
send "zhh1234@\r"

send "\n\r"

interact
