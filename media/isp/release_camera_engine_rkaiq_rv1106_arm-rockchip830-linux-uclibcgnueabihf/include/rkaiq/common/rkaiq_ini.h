#ifndef __RKAIQ_INI_H__
#define __RKAIQ_INI_H__

#ifdef __cplusplus
extern "C" {
#endif

#define INI_VERSION "0.1.1"
#define OFFLINE_INI_FILE "/tmp/aiq_offline.ini"

typedef struct ini_t ini_t;

extern ini_t*      rkaiq_ini_load(const char *filename);
extern void        rkaiq_ini_free(ini_t *ini);
extern const char* rkaiq_ini_get(ini_t *ini, const char *section, const char *key);
extern int         rkaiq_ini_sget(ini_t *ini, const char *section, const char *key, const char *scanfmt, void *dst);

#ifdef __cplusplus
}
#endif

#endif  /*__RKAIQ_INI_H__*/
