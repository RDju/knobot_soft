PORT=/dev/ttyACM1
avrdude -c stk500v2 -p atmega2560 -P $PORT -b 115200 -D -U flash:w:knub_servo.cpp.hex 2> avrdude.log