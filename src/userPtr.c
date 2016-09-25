#include<stdio.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

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
Map user pointer to buffer object
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
    amdgpu_bo_handle bo_handle;
    amdgpu_va_handle va_handle;
    uint64_t vmc_addr;
    void *cpu;

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

          int i, j;
          uint32_t *pm4 = NULL;
          uint64_t bo_mc;
          void *ptr = NULL;
          int pm4_dw = 256;
          int sdma_write_length = 4;
          amdgpu_context_handle ctx_handle;
          struct amdgpu_cs_ib_info *ib_info;
          struct amdgpu_cs_request *ibs_request;

          pm4 = calloc(pm4_dw, sizeof(*pm4));
          assert(pm4 != NULL);

          ib_info = calloc(1, sizeof(*ib_info));
          assert(ib_info != NULL);

          ibs_request = calloc(1, sizeof(*ibs_request));
          assert(ibs_request != NULL);

          r = amdgpu_cs_ctx_create(handle, &ctx_handle);
          assert(r == 0);

          posix_memalign(&ptr, sysconf(_SC_PAGE_SIZE), BUFFER_SIZE);
          assert(ptr != NULL);
          memset(ptr, 0, BUFFER_SIZE);

          r = amdgpu_create_bo_from_user_mem(handle, ptr,
                                  BUFFER_SIZE, &bo_handle);

          assert(r == 0);

          r = amdgpu_va_range_alloc(handle, amdgpu_gpu_va_range_general,
                                    BUFFER_SIZE, 1, 0, &bo_mc, &va_handle, 0);
          assert(r == 0);

          r = amdgpu_bo_va_op(bo_handle, 0, BUFFER_SIZE,
                                bo_mc, 0, AMDGPU_VA_OP_MAP);
          assert(r == 0);

          r = amdgpu_bo_va_op(bo_handle, 0, BUFFER_SIZE,
                                bo_mc, 0, AMDGPU_VA_OP_UNMAP);
          assert(r == 0);

          r = amdgpu_va_range_free(va_handle);
          assert(r == 0);

          r = amdgpu_bo_free(bo_handle);
          assert(r == 0);
          free(ptr);

          free(ibs_request);
          free(ib_info);
          free(pm4);


          r = amdgpu_cs_ctx_free(ctx_handle);
          assert(r == 0);

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
