# libdrm-code

DRM AMDGPU/Radeon Samples

## Pre-reqs

For building drm on ubuntu
`sudo apt-get install autoconf xutils-dev libtool libpciaccess-dev libpthread-stubs0-dev`

## Installation
`./autogen.sh; make`

`make install`

The command `make install` installs headers in `/usr/local/include/libdrm` and libraries in `/usr/local/lib`. We need `libdrm_amdgpu.so` and `libdrm.so` for the sample codes to work.

## Building Sample
`make`

## Running Sample
`sudo ./checkDrm; sudo ./openAllGPUs`
