CC=gcc
CFLAGS=-I/usr/local/include/libdrm
CLINKS=-L/usr/local/lib

all: checkDrm openAllGPUs openAMDGPU

checkDrm:
	$(CC) $(CFLAGS) checkDrm.c $(CLINKS) -ldrm -o checkDrm

openAllGPUs:
	$(CC) $(CFLAGS) openAllGPUs.c $(CLINKS) -ldrm -o openAllGPUs

openAMDGPU:
	$(CC) $(CFLAGS) openAMDGPU.c $(CLINKS) -ldrm -ldrm_amdgpu -o openAMDGPU

clean:
	rm checkDrm openAllGPUs openAMDGPU
