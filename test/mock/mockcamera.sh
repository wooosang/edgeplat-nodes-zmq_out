#!/bin/bash

python3.9 mocksendimage.py /home/ubuntu/work/images/mock/20221027/0 5030,5020 192.168.40.20 0 &
python3.9 mocksendimage.py /home/ubuntu/work/images/mock/20221027/1 5031,5021 192.168.20.20 1 &
python3.9 mocksendimage.py /home/ubuntu/work/images/mock/20221027/2 5032,5022 192.168.50.20 2 &
python3.9 mocksendimage.py /home/ubuntu/work/images/mock/20221027/3 5033,5023 192.168.30.20 3 &
python3.9 mocksendimage.py /home/ubuntu/work/images/mock/20221027/4 5034,5024 192.168.10.20 4 &
python3.9 mocksendimage.py /home/ubuntu/work/images/mock/20221027/5 5035,5025 192.168.30.20 5 &

