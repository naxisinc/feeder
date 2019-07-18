"C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avrdude" -C"C:\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf" -v -patmega328pb -carduino -PCOM4 -b19200 -D -Uflash:w:D:\PEDRO\Projects_Arduino\feeder/feeder.ino_atmega328p_16000000L.hex:i -Ulock:w:0x0f:m

pause