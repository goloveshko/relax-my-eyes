#!/bin/sh -u

RM="/bin/rm -rf"
set -u
set -e

cd ..

mkdir relax-my-eyes-bin || true
cd relax-my-eyes-bin

export QTDIR=/Users/sergey/Qt5.9.1/5.9.1/clang_64

cmake -G "Xcode" -DCMAKE_PREFIX_PATH="/Users/sergey/Qt5.9.1/5.9.1/clang_64" -DCMAKE_OSX_SYSROOT=macosx10.12 -DCMAKE_OSX_DEPLOYMENT_TARGET=10.12 ../relax-my-eyes/src

cd ..