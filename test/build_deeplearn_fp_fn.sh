#!/bin/bash

g++ -g -o deeplearn_fp_fn deeplearn_fp_fn.cc ./libevl.so /usr/lib/x86_64-linux-gnu/libjsoncpp.so  `pkg-config --cflags --libs opencv` -I/usr/local/include/opencv2/ -I/usr/local/include/opencv/ -Wall -std=c++17 
