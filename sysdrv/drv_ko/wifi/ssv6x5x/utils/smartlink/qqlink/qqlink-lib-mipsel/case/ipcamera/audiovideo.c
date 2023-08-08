/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "TXSDKCommonDef.h"
#include "TXAudioVideo.h"
static FILE *fstream = NULL;
static char* s_cData = NULL;
static bool s_bstart = false;
static pthread_t thread_enc_id = 0;
static unsigned long s_dwTotalFrameIndex = 0;
unsigned long _GetTickCount() {
    struct timeval current = {0};
 gettimeofday(&current, NULL);
 return (current.tv_sec*1000 + current.tv_usec/1000);
}
static void* EncoderThread(void* pThreadData) {
 while(s_bstart) {
  if(s_bstart) {
   if(fstream &&!feof(fstream)) {
    int iLen = 0;
    int iType = -1;
    fread(&iLen, 1, 4, fstream);
    fread(&iType, 1, 4, fstream);
    if (iLen > 0) {
     fread(s_cData, 1, iLen, fstream);
     static int s_nFrameIndex = 0;
     static int s_gopIndex = -1;
     unsigned char * pcEncData = (unsigned char *)s_cData;
     int nEncDataLen = iLen;
     int nFrameType = 1;
     if(iType == 0) {
      s_nFrameIndex = 0;
      s_gopIndex++;
      nFrameType = 0;
     }
     else {
      s_nFrameIndex++;
      nFrameType = 1;
     }
     if(s_gopIndex == -1) {
      printf("No I Frame s_gopIndex == -1\n");
     }
     else {
      if(nEncDataLen != 0) {
       tx_set_video_data(pcEncData, nEncDataLen, nFrameType, (int)_GetTickCount(), s_gopIndex, s_nFrameIndex, (int)s_dwTotalFrameIndex++, 40);
      }
     }
    }
   }
   else if(fstream && feof(fstream)) {
    fclose(fstream);
    fstream = fopen("test.264", "rb");
   }
  }
  usleep(90000);
 }
    return 0;
}
bool test_start_camera() {
 printf("###### test_start_camera ###################################### \n");
 if (!fstream) {
  fstream = fopen("test.264", "rb");
 }
 if (!s_cData) {
  s_cData = (char*)malloc(1280*720);
 }
 if (!fstream || !s_cData) {
  return false;
 }
 s_bstart = true;
 int ret = pthread_create(&thread_enc_id, NULL, EncoderThread, NULL);
 if (ret || !thread_enc_id) {
  s_bstart = false;
  return false;
 }
    return true;
}
bool test_stop_camera() {
 printf("###### test_stop_camera ###################################### \n");
 s_bstart = false;
 if(fstream) {
  fclose(fstream);
  fstream = NULL;
 }
 if(s_cData) {
  free(s_cData);
  s_cData = NULL;
 }
 if(thread_enc_id !=0) {
        pthread_join(thread_enc_id,NULL);
        thread_enc_id = 0;
    }
    return true;
}
bool test_set_bitrate(int bit_rate) {
    printf("###### test_set_bitrate  ##################################### %d \n", bit_rate);
 return true;
}
bool test_restart_gop() {
 printf("###### test_restart_gop ###################################### \n");
    return true;
}
bool test_start_mic() {
 printf("###### test_start_mic ###################################### \n");
 return true;
}
bool test_stop_mic() {
 printf("###### test_stop_mic ######################################\n");
 return true;
}
void test_recv_audiodata(tx_audio_encode_param *param, unsigned char *pcEncData, int nEncDataLen)
{
    printf("##### test_recv_audiodata ######################################\n");
}
