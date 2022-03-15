#!/bin/sh
cd irx/source/mechaproxy
make
cd -
cp irx/source/mechaproxy/irx/mechaproxy.irx irx/compiled/mechaproxy.irx

cd irx/source/masswatcher
make
cd -
cp irx/source/masswatcher/irx/masswatcher.irx irx/compiled/masswatcher.irx

make
