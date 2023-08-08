#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xf86drm.h>
#include <xf86drmMode.h>
#include <drm_fourcc.h>

#include "modeset.h"
#include "bo.h"
#include "dev.h"

int initialize_screens(struct sp_dev* dev) {
  int ret, i, j;

  for (i = 0; i < dev->num_connectors; i++) {
    drmModeConnectorPtr c = dev->connectors[i];
    drmModeModeInfoPtr m = NULL;
    drmModeEncoderPtr e = NULL;
    struct sp_crtc* cr = NULL;

    if (c->connection != DRM_MODE_CONNECTED)
      continue;

    if (!c->count_modes) {
      printf("connector has no modes, skipping\n");
      continue;
    }

    /* Take the first unless there's a preferred mode */
    m = &c->modes[0];
    for (j = 0; j < c->count_modes; j++) {
      drmModeModeInfoPtr tmp_m = &c->modes[j];

      if (!(tmp_m->type & DRM_MODE_TYPE_PREFERRED))
        continue;

      m = tmp_m;
      break;
    }

    if (!c->encoder_id) {
      /*
       * default drm encoder not attached connector, just
       * select the first one.
       */
      if (dev->num_encoders) {
        e = dev->encoders[0];
        c->encoder_id = e->encoder_id;
      } else {
        printf("no encoder attached to the connector\n");
        continue;
      }
    }

    for (j = 0; j < dev->num_encoders; j++) {
      e = dev->encoders[j];
      if (e->encoder_id == c->encoder_id)
        break;
    }
    if (j == dev->num_encoders) {
      printf("could not find encoder for the connector\n");
      continue;
    }

    if (!e->crtc_id) {
      /*
       * default drm crtc not attached encoder, just
       * select the first one.
       */
      if (dev->num_crtcs) {
        cr = &dev->crtcs[j];
        e->crtc_id = cr->crtc->crtc_id;
      } else {
        printf("no crtc attached to the encoder\n");
        continue;
      }
    }

    for (j = 0; j < dev->num_crtcs; j++) {
      cr = &dev->crtcs[j];

      if (cr->crtc->crtc_id == e->crtc_id)
        break;
    }
    if (j == dev->num_crtcs) {
      printf("could not find crtc for the encoder\n");
      continue;
    }
    if (cr->scanout) {
      printf("crtc already in use\n");
      continue;
    }

    /* XXX: Hardcoding the format here... :| */
    cr->scanout = create_sp_bo(dev, m->hdisplay, m->vdisplay,
                               24, 32, DRM_FORMAT_XRGB8888, 0);
    if (!cr->scanout) {
      printf("failed to create new scanout bo\n");
      continue;
    }

    ret = add_fb_sp_bo(cr->scanout, DRM_FORMAT_XRGB8888);
    if (ret) {
      printf("failed to add fb ret=%d\n", ret);
      continue;
    }

    //fill_bo(cr->scanout, 0xFF, 0x0, 0x0, 0xFF);

    ret = drmModeSetCrtc(dev->fd, cr->crtc->crtc_id,
                         cr->scanout->fb_id, 0, 0, &c->connector_id,
                         1, m);
    if (ret) {
      printf("failed to set crtc mode ret=%d\n", ret);
      continue;
    }
    cr->crtc = drmModeGetCrtc(dev->fd, cr->crtc->crtc_id);
    /*
     * Todo:
     * I don't know why crtc mode is empty, just copy PREFERRED mode
     * for it.
     */
    memcpy(&cr->crtc->mode, m, sizeof(*m));
  }
  return 0;
}

struct sp_plane* get_sp_plane(struct sp_dev* dev, struct sp_crtc* crtc) {
  int i;

  for (i = 0; i < dev->num_planes; i++) {
    struct sp_plane* p = &dev->planes[i];

    if (p->in_use)
      continue;

    if (!(p->plane->possible_crtcs & (1 << crtc->pipe)))
      continue;

    p->in_use = 1;
    return p;
  }
  return NULL;
}

void put_sp_plane(struct sp_plane* plane) {
  drmModePlanePtr p;

  /* Get the latest plane information (most notably the crtc_id) */
  p = drmModeGetPlane(plane->dev->fd, plane->plane->plane_id);
  if (p)
    plane->plane = p;

  if (plane->plane->crtc_id)
    drmModeSetPlane(plane->dev->fd, plane->plane->plane_id,
                    plane->plane->crtc_id, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0);

  if (plane->bo) {
    free_sp_bo(plane->bo);
    plane->bo = NULL;
  }
  plane->in_use = 0;
}

int set_sp_plane(struct sp_dev* dev, struct sp_plane* plane,
                 struct sp_crtc* crtc, int x, int y) {
  int ret;
  uint32_t w, h;

  w = plane->bo->width;
  h = plane->bo->height;

  if ((w + x) > crtc->crtc->mode.hdisplay)
    w = crtc->crtc->mode.hdisplay - x;
  if ((h + y) > crtc->crtc->mode.vdisplay)
    h = crtc->crtc->mode.vdisplay - y;

  ret = drmModeSetPlane(dev->fd, plane->plane->plane_id,
                        crtc->crtc->crtc_id, plane->bo->fb_id, 0, x, y, w, h,
                        0, 0, w << 16, h << 16);
  if (ret) {
    printf("failed to set plane to crtc ret=%d\n", ret);
    return ret;
  }

  return ret;
}
#ifdef USE_ATOMIC_API
int set_sp_plane_pset(struct sp_dev* dev, struct sp_plane* plane,
                      drmModePropertySetPtr pset, struct sp_crtc* crtc, int x, int y) {
  int ret;
  uint32_t w, h;

  w = plane->bo->width;
  h = plane->bo->height;

  if ((w + x) > crtc->crtc->mode.hdisplay)
    w = crtc->crtc->mode.hdisplay - x;
  if ((h + y) > crtc->crtc->mode.vdisplay)
    h = crtc->crtc->mode.vdisplay - y;

  ret = drmModePropertySetAdd(pset, plane->plane->plane_id,
                              plane->crtc_pid, crtc->crtc->crtc_id)
        || drmModePropertySetAdd(pset, plane->plane->plane_id,
                                 plane->fb_pid, plane->bo->fb_id)
        || drmModePropertySetAdd(pset, plane->plane->plane_id,
                                 plane->crtc_x_pid, x)
        || drmModePropertySetAdd(pset, plane->plane->plane_id,
                                 plane->crtc_y_pid, y)
        || drmModePropertySetAdd(pset, plane->plane->plane_id,
                                 plane->crtc_w_pid, w)
        || drmModePropertySetAdd(pset, plane->plane->plane_id,
                                 plane->crtc_h_pid, h)
        || drmModePropertySetAdd(pset, plane->plane->plane_id,
                                 plane->src_x_pid, 0)
        || drmModePropertySetAdd(pset, plane->plane->plane_id,
                                 plane->src_y_pid, 0)
        || drmModePropertySetAdd(pset, plane->plane->plane_id,
                                 plane->src_w_pid, w << 16)
        || drmModePropertySetAdd(pset, plane->plane->plane_id,
                                 plane->src_h_pid, h << 16);
  if (ret) {
    printf("failed to add properties to the set\n");
    return -1;
  }

  return ret;
}
#endif
