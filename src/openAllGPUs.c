#include<stdio.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include"drm.h"
#include"xf86drm.h"

#define NUM_GPUS 10

char gpu_loc[] = "/dev/dri/card0";
int gpu;

/*
This sample checks each drm devices and report
backend they can be accessed.
*/
int main()
{
    int avail = drmAvailable();
    if(avail == 0){
        return -1;
    }

    int i;

    for(i=0;i<NUM_GPUS;i++){
      gpu = open(gpu_loc, O_RDWR | O_CLOEXEC);
      if(gpu >= 0){
        drmVersionPtr ver = drmGetVersion(gpu);
        printf("GPU on %s is [%s]\n",gpu_loc, ver->name);
        close(gpu);
      }
      gpu_loc[13] += 0x1;
    }

    return 0;
}
