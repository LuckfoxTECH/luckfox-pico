#ifndef __DEV_H_INCLUDED__
#define __DEV_H_INCLUDED__

#include <stdint.h>
#include <xf86drmMode.h>

struct sp_bo;
struct sp_dev;

struct sp_plane {
  struct sp_dev* dev;
  drmModePlanePtr plane;
  struct sp_bo* bo;
  int in_use;
  uint32_t format;

  /* Property ID's */
  uint32_t crtc_pid;
  uint32_t fb_pid;
  uint32_t zpos_pid;
  uint32_t crtc_x_pid;
  uint32_t crtc_y_pid;
  uint32_t crtc_w_pid;
  uint32_t crtc_h_pid;
  uint32_t src_x_pid;
  uint32_t src_y_pid;
  uint32_t src_w_pid;
  uint32_t src_h_pid;
};

struct sp_crtc {
  drmModeCrtcPtr crtc;
  int pipe;
  int num_planes;
  struct sp_bo* scanout;
};

struct sp_dev {
  int fd;

  int num_connectors;
  drmModeConnectorPtr* connectors;

  int num_encoders;
  drmModeEncoderPtr* encoders;

  int num_crtcs;
  struct sp_crtc* crtcs;

  int num_planes;
  struct sp_plane* planes;
};

int is_supported_format(struct sp_plane* plane, uint32_t format);
struct sp_dev* create_sp_dev(void);
void destroy_sp_dev(struct sp_dev* dev);

#endif /* __DEV_H_INCLUDED__ */
