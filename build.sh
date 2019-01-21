#!/bin/sh
mkdir Build.control
mkdir Build.html
mkdir Build.meta
mkdir Build.nsp
mkdir Build.program
mkdir Control
mkdir ExeFs
mkdir RomFs
rm ./Build*/*.nca
rm ./Control/control.nacp
rm ./ExeFs/*
rm ./Output/*
make clean
make
cp ./Browser.nso ./ExeFs/main
npdmtool ./npdm.json ./ExeFs/main.npdm
cp ./Browser.nacp ./Control/control.nacp
./hacpack -k ./prod.keys --type nca --ncatype manual --titleid 0100C4C320C0FFEE --romfsdir=OfflineHtml --outdir=Build.html
./hacpack -k ./prod.keys --type nca --ncatype program --titleid 0100C4C320C0FFEE --exefsdir=ExeFs --romfsdir=RomFs --logodir=Logo --outdir=Build.program
./hacpack -k ./prod.keys --type nca --ncatype control --titleid 0100C4C320C0FFEE --romfsdir=Control --outdir=Build.control
./hacpack -k ./prod.keys --type nca --ncatype meta --titleid 0100C4C320C0FFEE --titletype application --programnca Build.program/*.nca --controlnca Build.control/*.nca --htmldocnca Build.html/*.nca --outdir=Build.meta
cp ./Build*/*.nca ./Build.nsp/
./hacpack -k ./prod.keys --type nsp --titleid 0100C4C320C0FFEE --ncadir=Build.nsp --outdir=Output
mv Output/0100c4c320c0ffee.nsp Browser.nsp
