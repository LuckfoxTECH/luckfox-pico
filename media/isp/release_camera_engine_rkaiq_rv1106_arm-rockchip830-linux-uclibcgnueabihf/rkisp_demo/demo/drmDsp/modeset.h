#ifndef __MODESET_H_INCLUDED__
#define __MODESET_H_INCLUDED__

struct sp_dev;
struct sp_crtc;

int initialize_screens(struct sp_dev* dev);


struct sp_plane* get_sp_plane(struct sp_dev* dev, struct sp_crtc* crtc);
void put_sp_plane(struct sp_plane* plane);

int set_sp_plane(struct sp_dev* dev, struct sp_plane* plane,
                 struct sp_crtc* crtc, int x, int y);

#ifdef USE_ATOMIC_API
int set_sp_plane_pset(struct sp_dev* dev, struct sp_plane* plane,
                      drmModePropertySetPtr pset, struct sp_crtc* crtc, int x, int y);
#endif

#endif /* __MODESET_H_INCLUDED__ */
