"C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avrdude" -CC:\Users\airtn\AppData\Local\Arduino15\packages\MiniCore\hardware\avr\2.0.3/avrdude.conf -v -patmega328pb -carduino -PCOM4 -b19200 -e -Ulock:w:0x3f:m -Uefuse:w:0b11110101:m -Uhfuse:w:0xd6:m -Ulfuse:w:0b11111111:m

"C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avrdude" -CC:\Users\airtn\AppData\Local\Arduino15\packages\MiniCore\hardware\avr\2.0.3/avrdude.conf -v -patmega328pb -carduino -PCOM4 -b19200 -Uflash:w:D:\PEDRO\Projects_Arduino\feeder/feeder.ino_atmega328pb_16000000L.hex:i -Ulock:w:0x0f:m

pause