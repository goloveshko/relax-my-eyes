#!/bin/sh -u

RM="/bin/rm -rf"
set -u
set -e

cd ..

mkdir relax-my-eyes-bin || true
cd relax-my-eyes-bin

cmake -G "Xcode" -DCMAKE_OSX_SYSROOT=macosx10.10 -DCMAKE_OSX_DEPLOYMENT_TARGET=10.10 ../relax-my-eyes/src

cd ..