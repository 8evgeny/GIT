rem chcp 1251
rem mdrfwucli.exe --no-gui --mcu=ve9x --port=COM4 --speed=115200 --fw=fw.axf --dump=dump.hex -v 1>out.txt 2>err.txt
mdrfwucli.exe --no-gui --mcu=ve9x --port=COM4 --speed=115200 --fw=fw.axf --dump=dump.hex -d
