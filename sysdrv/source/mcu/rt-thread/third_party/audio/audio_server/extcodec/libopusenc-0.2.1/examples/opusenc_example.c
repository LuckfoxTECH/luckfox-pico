#include <stdio.h>
#include "drv_heap.h"

#include "audio_server.h"
#include "rtthread.h"
#include "opusenc.h"

#ifdef OPUS_EXAMPLE
rt_thread_t opusenc_handle;
int opusenc_handle_run = 0;

#define EX_RTATE       (16000)
#define EX_CHANNEL     (1)

#define OPUSENC_DEBUG   (0)

struct opusenc_arg
{
    int argc;
    char *argv[];
};

typedef struct opusenc_arg *opusenc_handle_t;
#define READ_SIZE (EX_RTATE/50)

void opusenc(void *indata) {
  opusenc_handle_t opusenc_arg = (opusenc_handle_t)indata;
  FILE *fin;
  OggOpusEnc *enc;
  OggOpusComments *comments;
  int error;
  #if OPUSENC_DEBUG
  static int t00 = 0,t11 = 0;
  #endif
  if (opusenc_arg->argc != 3) {
    fprintf(stderr, "usage: %s <raw pcm input> <Ogg Opus output>\n", opusenc_arg->argv[0]);
    return;
  }
  fin = fopen(opusenc_arg->argv[1], "rb");
  if (!fin) {
    fprintf(stderr, "cannot open input file: %s\n", opusenc_arg->argv[1]);
    return;
  }
  comments = ope_comments_create();
  ope_comments_add(comments, "ARTIST", "Someone");
  ope_comments_add(comments, "TITLE", "Some track");
  enc = ope_encoder_create_file(opusenc_arg->argv[2], comments, EX_RTATE, EX_CHANNEL, 0, &error);
  if (!enc) {
    fprintf(stderr, "error encoding to file %s: %s\n", opusenc_arg->argv[2], ope_strerror(error));
    ope_comments_destroy(comments);
    fclose(fin);
    return;
  }
  while (opusenc_handle_run) {
    short buf[2*READ_SIZE*EX_CHANNEL];
    int ret = fread(buf, EX_CHANNEL*sizeof(short), READ_SIZE, fin);
    // printf("%s %d\n", __func__, __LINE__);
    if (ret > 0) {
      #if OPUSENC_DEBUG
      t00 = HAL_GetTick();
      #endif

      ope_encoder_write(enc, buf, ret);

      #if OPUSENC_DEBUG
      t11 = HAL_GetTick();
      printf("%s,time = %d\n",__func__,t11-t00);
      #endif

    } else break;
  }
  ope_encoder_drain(enc);
  ope_encoder_destroy(enc);
  ope_comments_destroy(comments);
  fclose(fin);
}

void opusenc_test(int argc, char *argv[])
{
    opusenc_handle_t arg = (opusenc_handle_t)rt_calloc(argc, sizeof(opusenc_handle_t));
    arg->argc = argc;
    printf("argc = %d\n", argc);
    int i = 0;
    for (i = 0 ; i < argc; i ++)
    {
        arg->argv[i] = argv[i];
        fprintf(stderr, "argv[%d] = %s\n", i, argv[i]);
    }

    opusenc_handle = rt_thread_create("opusenc", opusenc, arg, 1024 * 300, 5, 10);
    if (opusenc_handle)
    {
        rt_thread_startup(opusenc_handle);
        opusenc_handle_run = 1;
    }
}

int opusenc_stop(void)
{
    opusenc_handle_run = 0;

    return 0;
}

#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(opusenc_test, opusenc test);
MSH_CMD_EXPORT(opusenc_stop, opusenc stop);
#endif
#endif
