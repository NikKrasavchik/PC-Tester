set PATH=D:\MinGW\bin\;%PATH%
mingw32-gcc -Os -c SeedKey_ABS.c
mingw32-gcc -shared -o ABSSeedKey.dll SeedKey_ABS.o
