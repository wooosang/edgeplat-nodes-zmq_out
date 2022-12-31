#/bin/bash

echo $LD_LIBRARY_PATH
#cat /home/ubuntu/work/test/models/10000/1.json
source ~/.bashrc
gdb -ex=r --args /app -p 8080
