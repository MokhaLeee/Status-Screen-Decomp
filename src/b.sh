FileName='StatScreen'

rm $FileName.o $FileName.asm $FileName.lyn.event
cd include
rm *.o
cd ..
make $FileName.asm
