#!/bin/bash

g++ -g -o halcon_fp_fn halcon_fp_fn.cc ./libevl.so /usr/lib/x86_64-linux-gnu/libjsoncpp.so  `pkg-config --cflags --libs opencv` -I/usr/local/include/opencv2/ -I/usr/local/include/opencv/ -Wall -std=c++17 
