#include<stdio.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include"drm.h"
#include"xf86drm.h"
#include"amdgpu.h"
#include"amdgpu_drm.h"

#define NUM_GPUS 10

char gpu_loc[] = "/dev/dri/card0";
int gpu;

/*
This sample checks each drm devices and open AMDGPU device.
*/
int main()
{
    int avail = drmAvailable();
    if(avail == 0){
        return -1;
    }

    int i;
    uint32_t major, minor;
    amdgpu_device_handle handle;

    printf("Available AMDGPUs\n");

    for(i=0;i<NUM_GPUS;i++){
      gpu = open(gpu_loc, O_RDWR | O_CLOEXEC);
      if(gpu >= 0){
        drmVersionPtr ver = drmGetVersion(gpu);

        if(ver->name[0] == 'a'){
          int r = amdgpu_device_initialize(gpu, &major, &minor, &handle);
          if(r){}else{
            printf("GPU on %s is [%s]\n", gpu_loc, ver->name);
            struct amdgpu_gpu_info info = {0};
            r = amdgpu_query_gpu_info(handle, &info);
            printf("Num of Shader Engines: %u \nNum of graphics context: %u\n", info.num_shader_engines, info.num_hw_gfx_contexts);
            struct amdgpu_gds_resource_info gds_info = {0};
            r = amdgpu_query_gds_info(handle, &gds_info);
            printf("Total GDS size: %u\n",gds_info.gds_total_size);
            r = amdgpu_device_deinitialize(handle);
          }
        }

        close(gpu);
      }
      gpu_loc[13] += 0x1;
    }

    return 0;
}
