mkdir Build.control
mkdir Build.html
mkdir Build.meta
mkdir Build.nsp
mkdir Build.program
mkdir Control
mkdir ExeFs
del Build*\*.nca
del Control\control.nacp
del ExeFs\*
del Output\*
make clean
make
copy -Y Browser.nso ExeFs\main
npdmtool npdm.json ExeFs\main.npdm
copy -Y \Browser.nacp Control\control.nacp
Build-Tools\hacpack.exe -k prod.keys --type nca --ncatype manual --titleid 0100C4C320C0FFEE --romfsdir=OfflineHtml --outdir=Build.html
Build-Tools\hacpack.exe -k prod.keys --type nca --ncatype program --titleid 0100C4C320C0FFEE --exefsdir=ExeFs --romfsdir=RomFs --logodir=Logo --outdir=Build.program
Build-Tools\hacpack.exe -k prod.keys --type nca --ncatype control --titleid 0100C4C320C0FFEE --romfsdir=Control --outdir=Build.control
Build-Tools\hacpack.exe -k prod.keys --type nca --ncatype meta --titleid 0100C4C320C0FFEE --titletype application --programnca Build.program\*.nca --controlnca Build.control\*.nca --htmldocnca Build.html\*.nca --outdir=Build.meta
copy Build*\*.nca Build.nsp\
Build-Tools\hacpack.exe -k prod.keys --type nsp --titleid 0100C4C320C0FFEE --ncadir=Build.nsp --outdir=Output
ren Output\0100c4c320c0ffee.nsp Browser.nsp