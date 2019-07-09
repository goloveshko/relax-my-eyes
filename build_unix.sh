#!/bin/sh -u

RM="/bin/rm -rf"
set -u
set -e

cd ..

mkdir relax-my-eyes-bin || true
cd relax-my-eyes-bin

cmake ../relax-my-eyes/src

make

cd ..
