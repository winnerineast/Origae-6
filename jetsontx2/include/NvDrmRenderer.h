/*
 * Copyright (c) 2016-2017, NVIDIA CORPORATION.  All rights reserved.
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */

/**
 * @file
 *
 * <b>NVIDIA Multimedia API: DRM Renderer API</b>
 *
 * @b Description: Helper class for rendering using LibDRM.
 */
#ifndef __NV_DRM_RENDERER_H__
#define __NV_DRM_RENDERER_H__

#include "NvElement.h"
#include <stdint.h>
#include <pthread.h>
#include <queue>
#include <unordered_map>

/**
 *
 * @defgroup l4t_mm_nvdrmrenderer_group DRM Renderer API
 *
 * @ingroup aa_framework_api_group
 * @{
 */

/** Holds a buffer object handle. */
typedef struct _NvDrmBO {
    uint32_t bo_handle;  /**< DRM buffer index. */
    int width;           /**< Width of the DRM buffer, in pixels. */
    int height;          /**< Heigth of the DRM buffer, in pixels. */
    int pitch;           /**< Memory layout of the DRM buffer. */
    uint8_t* data;       /**< Mapped CPU accessible address. */
} NvDrmBO;

/** Holds information about the frame. */
typedef struct _NvDrmFB {
    uint32_t fb_id;  /**< Frame ID. */
    int width;       /**< Width of the frame, in pixels. */
    int height;      /**< Height of the frame, in pixels. */
    int format;      /**< Frame format, such as @c DRM_FORMAT_RGB332.
                          This class supports a subset of the
                          formats defined in drm_fourcc.h, the standard DRM header. */
    NvDrmBO bo[4];   /**< DRM buffer handles. */
    int num_buffers; /**< Number of DRM buffers, which depends on the buffer format. */
} NvDrmFB;


/**
 * @brief Helper class for rendering using LibDRM.
 *
 * The renderer requires the fd of a buffer as an input. The caller must
 * set the rendering rate, in terms of frames per second (FPS).
 *
 * The caller specifies the width, height connector and crtc index.
 * Based on connector and crtc index, the renderer finds a suitable encoder and
 * configures the crtc mode.
 */
class NvDrmRenderer:public NvElement
{
public:
    /**
     * Creates a new DRM based renderer named @a name.
     *
     * @param[in] name Unique name to identity the element instance.
     * @param[in] width Width of the window in pixels.
     * @param[in] height Height of the window in pixels.
     * @param[in] w_x x offset of window location.
     * @param[in] w_y y offset of window location.
     * @param[in] connector index of connector to use.
     * @param[in] crtc index of crtc to use.
     * @returns Reference to the newly created renderer object else @a NULL in
     *          case of failure during initialization.
     */
    static NvDrmRenderer *createDrmRenderer(const char *name, uint32_t width,
                                          uint32_t height, uint32_t w_x, uint32_t w_y,
                                          uint32_t connector, uint32_t crtc);
     ~NvDrmRenderer();

    /**
     * Enqueues a buffer fd for rendering.
     *
     * This is a non-blocking call. This function waits until the rendering time
     * of the next buffer. The rendering time is calculated based on the
     * rendering time of the last buffer and the rendering rate @a fps.
     *
     * @param[in] fd FD of the exported buffer to render.
     * @returns 0 for success, -1 for failure
     */

    int enqueBuffer(int fd);

    /**
     *  Dequeues a previously rendered buffer.
     *
     *  This is blocking function that waits until a free buffer is available.
     *  The renderer retains one buffer, which must not be overwritten
     *  by any other component. This last buffer can be used when the renderer
     *  is closed or after sending an EOS to the component.
     *
     *  @returns fd of the previously rendered buffer.
     */

    int dequeBuffer();

    /**
     * Sets the rendering rate in terms of frames per second.
     *
     * \warning fps of zero is not allowed
     *
     * @param[in] fps Render rate in terms of frames per second
     * @returns 0 for success, -1 for failure
     */
    int setFPS(float fps);

    /**
     * Enables/disables DRM universal planes client caps,
     * such as @c DRM_CLIENT_CAP_UNIVERSAL_PLANES.
     *
     * @param[in] enable 1 or 0 to enable/disable the caps.
     * @returns true for success and false for failure.
     */
    bool enableUniversalPlanes(int enable);

    /**
     * Allocates a framebuffer of size (w, h).
     *
     * @post  If the call is successful, the application must remove (free) the
     * framebuffer by calling removeFB().
     *
     * @param[in]  width         Framebuffer width in pixels.
     * @param[in]  height        Framebuffer height in pixels.
     * @param[in]  drm_format    DRM format of the @a bo_handle(s).
     * @param[out] fb            A pointer to an \ref NvDrmFB structure that contains the fb_id
     *                           and the buffer mapping.
     *
     * @return 1 for success, 0 for failure.
     */
    uint32_t createDumbFB(uint32_t width, uint32_t height, uint32_t drm_format, NvDrmFB *fb);

    /**
     * Destroys a framebuffer.
     *
     * Destroys (frees) a framebuffer previously allocated by createDumbFB().
     *
     * @param fb_id  The ID of the framebuffer to destroy.
     * @return 0 if destruction is successful, or @c -ENOENT if the framebuffer is
     *          not found.
     */
    int removeFB(uint32_t fb_id);

    /**
     * Changes a plane's framebuffer and position.
     *
     * @note  The @a crtc_... and @a src_... parameters accept the special input
     * value -1, which indicates that the hardware offset value is not to be
     * changed. (Kernel-based DRM drivers return the error code @c -ERANGE when
     * given this value.)
     *
     * @note  All @c %setPlane operations are synced to vblank and are
     * blocking.
     *
     * @param pl_index Plane Index of the plane to be changed.
     * @param fb_id    Framebuffer ID of the framebuffer to display on the plane,
     *                  or -1 to leave the framebuffer unchanged.
     * @param crtc_x   Offset from left of active display region to show plane.
     * @param crtc_y   Offset from top of active display region to show plane.
     * @param crtc_w   Width of output rectangle on display.
     * @param crtc_h   Height of output rectangle on display.
     * @param src_x    Clip offset from left of source framebuffer (Q16.16 fixed point).
     * @param src_y    Clip offset from top of source framebuffer (Q16.16 fixed point).
     * @param src_w    Width of source rectangle (Q16.16 fixed point).
     * @param src_h    Height of source rectangle (Q16.16 fixed point).
     * @retval 0 if successful.
     * @retval -EINVAL if @a pl_index is invalid.
     * @retval -errno otherwise.
     */
    int setPlane(uint32_t pl_index,
                 uint32_t fb_id,
                 uint32_t crtc_x,
                 uint32_t crtc_y,
                 uint32_t crtc_w,
                 uint32_t crtc_h,
                 uint32_t src_x,
                 uint32_t src_y,
                 uint32_t src_w,
                 uint32_t src_h);

    /**
     * Gets total number of planes available.
     *
     * By default, the count returned includes only "Overlay" type (regular)
     * planes -- not "Primary" and "Cursor" planes. If @c
     * DRM_CLIENT_CAP_UNIVERSAL_PLANES has been enabled with enableUniversalPlanes(),
     * the count returned includes "Primary" and "Cursor" planes as well.
     *
     * @return  Count of total planes available.
     */
    int getPlaneCount();

    /**
     * Gets count of available crtcs.
     *
     * @return  Count of available CRTCs.
     */
    int getCrtcCount();

    /**
     * Gets count of available encoders.
     *
     * @return  Count of available encoders.
     */
    int getEncoderCount();

private:

    struct timespec last_render_time;   /**< Rendering time of the last buffer. */

    int drm_fd;           /**< FD of opened DRM device. */
    int conn, crtc;
    uint32_t width, height;
    uint32_t drm_conn_id;    /** DRM connector ID. */
    uint32_t drm_enc_id;     /** DRM encoder ID. */
    uint32_t drm_crtc_id;    /** DRM CRTC ID. */
    uint32_t last_fb;
    int activeFd;
    int flippedFd;
    bool flipPending;
    bool renderingStarted;

    std::queue<int> freeBuffers;
    std::queue<int> pendingBuffers;
    std::unordered_map <int, int> map_list;

    bool stop_thread;   /**< Boolean variable used to signal rendering thread
                             to stop. */
    pthread_t render_thread;        /**< pthread id of the rendering thread. */

    pthread_mutex_t render_lock;     /**< Used for synchronization. */
    pthread_cond_t render_cond;      /**< Used for synchronization. */
    pthread_mutex_t enqueue_lock;    /**< Used for synchronization. */
    pthread_cond_t enqueue_cond;     /**< Used for synchronization. */
    pthread_mutex_t dequeue_lock;    /**< Used for synchronization. */
    pthread_cond_t dequeue_cond;     /**< Used for synchronization. */

    float fps;                      /**< Render rate in frames per second. */
    uint64_t render_time_sec;       /**< Seconds part of the time for which a
                                         frame should be displayed. */
    uint64_t render_time_nsec;      /**< Nanoseconds part. */

    /**
     * Contructor called by the wrapper createDrmRenderer
     */
    NvDrmRenderer(const char *name, uint32_t width, uint32_t height,
                  uint32_t w_x, uint32_t w_y,
                  uint32_t connector, uint32_t crtc);

    /**
     * Function executed by the renderThread.
     *
     * This function keeps on executing infinitely until signalled to stop
     * using the stop_thread variable. This function contains a while loop
     * which calls renderInternal().
     */
    static void * renderThread(void *arg);

    /**
     * Callback function for DRM flip event.
     */

    static void page_flip_handler(int fd, unsigned int frame,
                                  unsigned int sec, unsigned int usec, void *data);

    /**
     * This function contains the actual logic of rendering a buffer
     * and waiting until the buffer render time.
     */
    int renderInternal(int fd);

    /*
     *  Returns a DRM buffer_object handle allocated of size (w,h)
     */
    int createDumbBO(int w, int h, int bpp, NvDrmBO *bo);
};

/** @} */
#endif
