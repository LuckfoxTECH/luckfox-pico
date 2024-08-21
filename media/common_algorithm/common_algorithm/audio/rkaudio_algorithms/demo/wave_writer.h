#ifndef WAVE_WRITER_H
#define WAVE_WRITER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WW_NO_ERROR = 0,
    WW_OPEN_ERROR,
    WW_IO_ERROR,
    WW_ALLOC_ERROR,
    WW_BAD_FORMAT,
} wave_writer_error;

typedef struct {
    int num_channels;
    int sample_rate;
    int sample_bits;
} wave_writer_format;

typedef struct wave_writer wave_writer;

struct wave_writer * wave_writer_open(char *filename, wave_writer_format *format, wave_writer_error *error);
int wave_writer_close(struct wave_writer *ww, wave_writer_error *error);
int wave_writer_get_format(wave_writer *ww);
int wave_writer_get_num_channels(wave_writer *ww);
int wave_writer_get_sample_rate(wave_writer *ww);
int wave_writer_get_sample_bits(wave_writer *ww);
int wave_writer_get_num_samples(wave_writer *ww);
int wave_writer_put_samples(wave_writer *ww, int n, void *buf);

#ifdef __cplusplus
}
#endif

#endif//WAVE_WRITER_H
