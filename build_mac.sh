#!/bin/sh -u

RM="/bin/rm -rf"
set -u
set -e

cd ..

mkdir relax-my-eyes-bin || true
cd relax-my-eyes-bin

export QTDIR=/Users/sergey/Qt/5.12.0/clang_64
export PATH=/Users/sergey/Qt/5.12.0/clang_64/bin:$PATH

SDK='10.14'

cmake -G "Xcode" -DCMAKE_OSX_SYSROOT=macosx$SDK -DCMAKE_OSX_DEPLOYMENT_TARGET=$SDK ../relax-my-eyes/src

xcodebuild -sdk macosx$SDK -project RelaxMyEyes.xcodeproj -target ALL_BUILD -configuration Release

macdeployqt ./Release/RelaxMyEyes.app

cd ..