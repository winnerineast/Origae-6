/*
 * Copyright (c) 2016-2017, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

/**
 * @file
 * <b>NVIDIA Multimedia API: Buffering API</b>
 *
 */

/**
 * @defgroup ee_nvbuffering_group NV HW Buffer Utility API
 *
 * NVIDIA buffering utility library for use by applications.
 * @{
 */

#ifndef _NVBUF_UTILS_H_
#define _NVBUF_UTILS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <errno.h>

#define MAX_NUM_PLANES 3

typedef enum
{
  NvBufferLayout_Pitch,
  NvBufferLayout_BlockLinear,
} NvBufferLayout;

typedef enum
{
  NvBufferMem_Read,
  NvBufferMem_Write,
  NvBufferMem_Read_Write,
} NvBufferMemFlags;

typedef enum
{
  NvBufferColorFormat_YUV420,
  NvBufferColorFormat_YVU420,
  NvBufferColorFormat_NV12,
  NvBufferColorFormat_NV21,
  NvBufferColorFormat_UYVY,
  NvBufferColorFormat_VYUY,
  NvBufferColorFormat_YUYV,
  NvBufferColorFormat_YVYU,
  NvBufferColorFormat_ABGR32,
  NvBufferColorFormat_XRGB32,
  NvBufferColorFormat_ARGB32,
  NvBufferColorFormat_Invalid,
} NvBufferColorFormat;

typedef struct _NvBufferParams
{
  uint32_t dmabuf_fd;
  void *nv_buffer;
  uint32_t nv_buffer_size;
  uint32_t pixel_format;
  uint32_t num_planes;
  uint32_t width[MAX_NUM_PLANES];
  uint32_t height[MAX_NUM_PLANES];
  uint32_t pitch[MAX_NUM_PLANES];
  uint32_t offset[MAX_NUM_PLANES];
  uint32_t psize[MAX_NUM_PLANES];
  uint32_t layout[MAX_NUM_PLANES];
}NvBufferParams;

/**
* This method must be used for getting `EGLImage` from `dmabuf-fd`.
*
* @param[in] display `EGLDisplay` object used during the creation of `EGLImage`.
* @param[in] dmabuf_fd `DMABUF FD` of buffer from which `EGLImage` to be created.
*
* @returns `EGLImageKHR` for success, `NULL` for failure
*/
EGLImageKHR NvEGLImageFromFd (EGLDisplay display, int dmabuf_fd);

/**
* This method must be used for destroying `EGLImage` object.

* @param[in] display `EGLDisplay` object used for destroying `EGLImage`.
* @param[in] eglImage `EGLImageKHR` object to be destroyed.
*
* @returns 0 for success, -1 for failure
*/
int NvDestroyEGLImage (EGLDisplay display, EGLImageKHR eglImage);

/**
 * Use this method to allocate HW buffer.
 * @param[out] dmabuf_fd Returns `dmabuf_fd` of hardware buffer.
 * @param[in] width Hardware buffer width, in bytes.
 * @param[in] height Hardware buffer height, in bytes.
 * @param[in] layout Layout of buffer.
 * @param[in] colorFormat The `colorFormat` of buffer.
 *
 * @returns 0 for success, -1 for failure
 */
int NvBufferCreate (int *dmabuf_fd, int width, int height,
    NvBufferLayout layout, NvBufferColorFormat colorFormat);

/**
 * Use this method to get buffer parameters.
 * @param[in] dmabuf_fd `DMABUF FD` of buffer.
 * @param[out] params A pointer to the structure to fill with parameters.
 *
 * @returns 0 for success, -1 for failure.
 */
int NvBufferGetParams (int dmabuf_fd, NvBufferParams *params);

/**
* This method must be used for destroying `hw_buffer`.
* @param[in] dmabuf_fd Specifies the `dmabuf_fd` `hw_buffer` to destroy.
*
* @returns 0 for success, -1 for failure.
*/
int NvBufferDestroy (int dmabuf_fd);

/**
* This method must be used for hw memory cache sync for the CPU.
* @param[in] dmabuf_fd DMABUF FD of buffer.
* @param[in] plane video frame plane.
* @param[in] pVirtAddr Virtual Addres pointer of the mem mapped plane.
*
* @returns 0 for success, -1 for failure.
*/
int NvBufferMemSyncForCpu (int dmabuf_fd, unsigned int plane, void **pVirtAddr);

/**
* This method must be used for hw memory cache sync for device.
* @param[in] dmabuf_fd DMABUF FD of buffer.
* @param[in] plane video frame plane.
* @param[in] pVirtAddr Virtual Addres pointer of the mem mapped plane.
*
* @returns 0 for success, -1 for failure.
*/
int NvBufferMemSyncForDevice (int dmabuf_fd, unsigned int plane, void **pVirtAddr);

/**
* This method must be used for getting mem mapped virtual Address of the plane.
* @param[in] dmabuf_fd DMABUF FD of buffer.
* @param[in] plane video frame plane.
* @param[in] memflag NvBuffer memory flag.
* @param[in] pVirtAddr Virtual Addres pointer of the mem mapped plane.
*
* @returns 0 for success, -1 for failure.
*/
int NvBufferMemMap (int dmabuf_fd, unsigned int plane, NvBufferMemFlags memflag, void **pVirtAddr);

/**
* This method must be used to Unmap the mapped virtual Address of the plane.
* @param[in] dmabuf_fd DMABUF FD of buffer.
* @param[in] plane video frame plane.
* @param[in] pVirtAddr mem mapped Virtual Addres pointer of the plane.
*
* @returns 0 for success, -1 for failure.
*/
int NvBufferMemUnMap (int dmabuf_fd, unsigned int plane, void **pVirtAddr);
/** @} */
#ifdef __cplusplus
}
#endif

#endif
