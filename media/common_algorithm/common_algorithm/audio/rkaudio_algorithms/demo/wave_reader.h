#ifndef WAVE_READER_H
#define WAVE_READER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WR_NO_ERROR = 0,
    WR_OPEN_ERROR,
    WR_IO_ERROR,
    WR_ALLOC_ERROR,
    WR_BAD_CONTENT,
} wave_reader_error;

typedef struct wave_reader wave_reader;

wave_reader *wave_reader_open(char *filename, wave_reader_error *error);
void wave_reader_close(wave_reader *wr);
int wave_reader_get_format(wave_reader *wr);
int wave_reader_get_num_channels(wave_reader *wr);
int wave_reader_get_sample_rate(wave_reader *wr);
int wave_reader_get_sample_bits(wave_reader *wr);
int wave_reader_get_num_samples(wave_reader *wr);
int wave_reader_get_samples(wave_reader *wr, int n, void *buf);

#ifdef __cplusplus
}
#endif

#endif//WAVE_READER_H

