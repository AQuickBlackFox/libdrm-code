CC=gcc
CFLAGS=-I/usr/local/include/libdrm
CLINKS=-L/usr/local/lib

all: checkDrm openAllGPUs

checkDrm:
	$(CC) $(CFLAGS) checkDrm.c $(CLINKS) -ldrm -o checkDrm

openAllGPUs:
	$(CC) $(CFLAGS) openAllGPUs.c $(CLINKS) -ldrm -o openAllGPUs

clean:
	rm checkDrm openAllGPUs
