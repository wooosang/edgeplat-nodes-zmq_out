#!/bin/bash

g++ -g -o testreload testreload.cc ./libevl.so  `pkg-config --cflags --libs opencv` -I/usr/local/include/opencv2/ -I/usr/local/include/opencv/ -Wall -std=c++11
