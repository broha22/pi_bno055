PI_ADDRESS = 192.168.4.1

all: main.o bno055.o bno055_support.o
	gcc -Wall  bno055_support.o bno055.o main.o -o sensor_read

remote_run: remote
	ssh pi@$(PI_ADDRESS) 'bno055_data/sensor_read'

remote:
	rsync -av -e ssh --exclude='.git/' ./ pi@$(PI_ADDRESS):~/bno055_data
	ssh pi@$(PI_ADDRESS) 'cd bno055_data; make all;'
 
main.o: main.c lib/BNO055_driver/bno055.h lib/BNO055_driver/bno055_support.h
	gcc -c main.c

bno055.o: lib/BNO055_driver/bno055.c lib/BNO055_driver/bno055.h
	gcc -c lib/BNO055_driver/bno055.c

bno055_support.o: lib/BNO055_driver/bno055_support.c lib/BNO055_driver/bno055.h lib/BNO055_driver/bno055_support.h
	gcc -c lib/BNO055_driver/bno055_support.c

clean:
	rm -rf *.o
	rm rpi_bno055_socket