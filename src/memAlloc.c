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
#define BUFFER_SIZE (4*1024)
#define BUFFER_ALIGN (4*1024)

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
    amdgpu_va_handle va_handle;
    uint64_t va;

    printf("Available AMDGPUs\n");

    for(i=0;i<NUM_GPUS;i++){
      gpu = open(gpu_loc, O_RDWR | O_CLOEXEC);
      if(gpu >= 0){
        drmVersionPtr ver = drmGetVersion(gpu);
        if(ver->name[0] == 'a'){
          int r = amdgpu_device_initialize(gpu, &major, &minor, &handle);
          if(r){
            printf("Unable to initialize AMDGPU\n");
            return -1;
          }
          printf("GPU on %s is [%s]\n", gpu_loc, ver->name);

          struct amdgpu_bo_alloc_request req = {0};
          amdgpu_bo_handle buf_handle;
          req.alloc_size = BUFFER_SIZE;
          req.phys_alignment = BUFFER_ALIGN;
          req.preferred_heap = AMDGPU_GEM_DOMAIN_GTT;

          r = amdgpu_bo_alloc(handle, &req, &buf_handle);
          if(r){
            printf("Unable to create buffer\n");
            return -1;
          }
          r = amdgpu_va_range_alloc(handle,
                                    amdgpu_gpu_va_range_general,
                                    BUFFER_SIZE, BUFFER_ALIGN, 0,
                                    &va, &va_handle, 0);

          if(r){
            printf("Unable to range alloc va\n");
          }else{printf("Range Alloc Successful\n");
}
          r = amdgpu_bo_va_op(buf_handle, 0, BUFFER_SIZE, va, 0, AMDGPU_VA_OP_MAP);

          if(r){
            printf("Unable to map va\n");
          }else{printf("Mapped Successful \n");}

          r = amdgpu_va_range_free(va_handle);
          if(r){
            printf("Unable to free range\n");
          }else{printf("Freed range\n");}

          r = amdgpu_bo_free(buf_handle);
          if(r){
            printf("Unable to free buffer\n");
          }else{printf("Freed Buffer\n");}

          r = amdgpu_device_deinitialize(handle);
          if(r){
            printf("Unable to deinitialize device\n");
          }
        }

        close(gpu);
      }
      gpu_loc[13] += 0x1;
    }

    return 0;
}
