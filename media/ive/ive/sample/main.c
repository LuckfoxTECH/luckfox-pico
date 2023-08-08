#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "sample_comm_rve.h"

static int g_argc = 0;
static char **g_argv = NULL;

void handle_signal(int s32Signo) {
    signal(SIGINT,SIG_IGN);
    signal(SIGTERM,SIG_IGN);

    if (SIGINT == s32Signo || SIGTERM == s32Signo)
    {
        if (g_argc < 2) {
            SAMPLE_IVE_TestHist_HandleSig();
            return;
        }

        switch (*g_argv[1])
        {
            case '0':
                {
                    printf("SAMPLE_IVE_TestHist_HandleSig");
                    SAMPLE_IVE_TestHist_HandleSig();
                }
                break;
            case '1':
                {
                    printf("SAMPLE_IVE_Object_Tracking_HandleSig");
                    SAMPLE_IVE_Object_Tracking_HandleSig();
                }
                break;
            default :
                {
                }
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    g_argc = argc;
    g_argv = argv;

    if (argc < 2)
    {
        SAMPLE_IVE_TestHist();
        return 0;
    }

    switch (*argv[1])
    {
        case '0':
            {
                printf("show histogram sample\n");
                SAMPLE_IVE_TestHist();
            }
            break;
        case '1':
            {
                printf("show object tracking sample\n");
                SAMPLE_IVE_Object_Tracking();
            }
            break;
        default :
            {
                SAMPLE_IVE_TestHist();
            }
            break;
    }

    return 0;
}
