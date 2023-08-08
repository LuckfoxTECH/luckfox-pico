#ifndef TUYA_G711_UTILS_H
#define TUYA_G711_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#define TUYA_G711_A_LAW (0)
#define TUYA_G711_MU_LAW (1)
#if 1
int tuya_g711_encode(unsigned char type, unsigned short *src, unsigned int srcLen,
                     unsigned char *drc, unsigned int *pOut);
int tuya_g711_decode(unsigned char type, unsigned short *src, unsigned int srcLen,
                     unsigned char *drc, unsigned int *pOut);
int tuya_g711_encode_16K(unsigned char type, unsigned short *src, unsigned int srcLen,
                         unsigned char *drc, unsigned int *pOut);
void tuya8KResample16K(short *pInAudioData, int nInAudioLen, short *pOutAudioData,
                       int *nOutAudioLen);

#else

void tuya_g711_encode(unsigned int type, unsigned int sample_num, unsigned char *samples,
                      unsigned char *bitstream);

void tuya_g711_encode_16k(unsigned int type, unsigned int sample_num, unsigned char *samples,
                          unsigned char *bitstream);

void tuya_g711_decode(unsigned int type, unsigned int sample_num, unsigned char *samples,
                      unsigned char *bitstream);

#endif
#ifdef __cplusplus
}
#endif

#endif // TUYA_G711_UTILS_H
