#include<stdio.h>
#include<assert.h>

#include"drm.h"

int main()
{
    int avail = drmAvailable();
    if(avail != 0){
        printf("-- DRM AVAILABLE --\n");
    }
    return 0;
}
