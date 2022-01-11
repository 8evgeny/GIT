mcufmloader - linux executable file: Alt Linux 9, Astra Linux.

Usage samples:
Write HEX file within "/dev/ttyS0" ("-port 0", "-fmt ihex"):
$ ./mcufmloader -port 0 -mcu 1986BE9x -fmt ihex -file firmwarefile.hex -write
$ ./mcufmloader -port 0 -mcu 1901BC1x -fmt ihex -file firmwarefile.hex -write
$ ./mcufmloader -port 0 -mcu 1986BE1x -fmt ihex -file firmwarefile.hex -write

Write BINARY file within "/dev/ttyS0" ("-port 0", "-fmt binary"):
$ ./mcufmloader -port 0 -mcu 1986BE9x -fmt binary -file firmwarefile.bin -write
$ ./mcufmloader -port 0 -mcu 1901BC1x -fmt binary -file firmwarefile.bin -write
$ ./mcufmloader -port 0 -mcu 1986BE1x -fmt binary -file firmwarefile.bin -write

Write HEX file within "/dev/ttyUSB0" ("-port u0")
$ ./mcufmloader -port u0 -mcu 1986BE9x -fmt ihex -file firmwarefile.hex -write

Erase EEPROM
$ ./mcufmloader -mcu 1986BE9x -erase
$ ./mcufmloader -mcu 1901BC1x -erase
$ ./mcufmloader -mcu 1986BE1x -erase

Print help and exit
$ ./mcufmloader --help

Print list of supported MCUs and exit
$ ./mcufmloader --list


Source code in file "mcufmloader-2020-03-10.src.tar.gz"
