CC=gcc
CFLAGS=-I/usr/local/include/libdrm
CLINKS=-L/usr/local/lib

all: checkDrm openAllGPUs openAMDGPU memAlloc deviceInfo memAllocMap

checkDrm:
	$(CC) $(CFLAGS) checkDrm.c $(CLINKS) -ldrm -o checkDrm

openAllGPUs:
	$(CC) $(CFLAGS) openAllGPUs.c $(CLINKS) -ldrm -o openAllGPUs

openAMDGPU:
	$(CC) $(CFLAGS) openAMDGPU.c $(CLINKS) -ldrm -ldrm_amdgpu -o openAMDGPU

memAlloc:
	$(CC) $(CFLAGS) memAlloc.c $(CLINKS) -ldrm -ldrm_amdgpu -o memAlloc

memAllocMap:
	$(CC) $(CFLAGS) memAllocMap.c $(CLINKS) -ldrm -ldrm_amdgpu -o memAllocMap

deviceInfo:
	$(CC) $(CFLAGS) deviceInfo.c $(CLINKS) -ldrm -ldrm_amdgpu -o deviceInfo

clean:
	rm checkDrm openAllGPUs openAMDGPU memAlloc deviceInfo memAllocMap
