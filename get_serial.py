#!/usr/bin/python3
import serial 
import sys

argLen=len(sys.argv)
if argLen!=3:
	port='/dev/ttyACM0'  # default port
	baudrate='9600'
else:
	port=sys.argv[1]
	baudrate=sys.argv[2]

ser= serial.Serial(port,baudrate)
i=0    
while(ser.isOpen()):
	# print(ser.isOpen())
	# print(ser.read(82).decode('utf-8'),end="", flush=True)
	line=ser.readline()
	line=line.decode('utf-8')
	if i>=3:
		print(line,end="", flush=True)   # start to wrtie data from the third line
	i=i+1	


 
