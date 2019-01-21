make clean
make
move Browser.nso ExeFs\main
npdmtool npdm.json ExeFs\main.npdm
hacpack.exe -k prod.keys --type nca --ncatype manual --titleid 0100C4C320C0FFEE --romfsdir=OfflineHtml --outdir=Build.html
mkdir RomFs
mkdir ExeFs
hacpack.exe -k prod.keys --type nca --ncatype program --titleid 0100C4C320C0FFEE --exefsdir=ExeFs --romfsdir=RomFs --logodir=Logo --outdir=Build.program
rmdir RomFs
rmdir ExeFs
mkdir Control
move Browser.nacp Control\control.nacp
hacpack.exe -k prod.keys --type nca --ncatype control --titleid 0100C4C320C0FFEE --romfsdir=Control --outdir=Build.control
rmdir Control /s /q
hacpack.exe -k prod.keys --type nca --ncatype meta --titleid 0100C4C320C0FFEE --titletype application --programnca Build.program\*.nca --controlnca Build.control\*.nca --htmldocnca Build.html\*.nca --outdir=Build.meta
cd Build.control
mv *.nca ..\
cd ..\
rmdir Build.control
cd Build.html
mv *.nca ..\
cd ..\
rmdir Build.html
cd Build.meta
mv *.nca ..\
cd ..\
rmdir Build.meta
cd Build.program
mv *.nca ..\
cd ..\
rmdir Build.program
mkdir Build.nsp
mv *.nca Build.nsp\
hacpack.exe -k prod.keys --type nsp --titleid 0100C4C320C0FFEE --ncadir=Build.nsp --outdir=Output
rmdir Build.nsp /s /q
rm Browser.*
rmdir build /s /q
rmdir hacpack_backup /s /q
cd Output
ren 0100c4c320c0ffee.nsp Browser.nsp
