#include "sample_comm.h"

int uapi_usage_show(uapi_case_t* uapi_list)
{
  int case_i = 0;

  if (!uapi_list) {
    return -1;
  }

  CLEAR();
  printf("Usage <key>:\n");

  while (1) {
    if (!uapi_list[case_i].desc) {
      break;
    }

    if (uapi_list[case_i].desc) {
      printf("\t\t%c) %s.\n", '0' + case_i, uapi_list[case_i].desc);
    }

    case_i++;
  }

  return 0;
}

int uapi_list_count(uapi_case_t* uapi_list)
{
  int case_i = 0;
  while (1) {
    if (!uapi_list[case_i].desc) {
      break;
    }
    case_i++;
  }

  return case_i;
}

int uapi_process_loop(const rk_aiq_sys_ctx_t* ctx, uapi_case_t* uapi_list)
{
  int key = -1;
  int max_key = -1;

  max_key = uapi_list_count(uapi_list);

  uapi_usage_show(uapi_list);

  do {
    printf("\t please press the key: ");
    key = getchar();
    while (key == '\n' || key == '\r')
      key = getchar();
    printf("\n");

    if (key == 'h') {
      uapi_usage_show(uapi_list);
      continue;
    }

    if (key - '0' > max_key) {
      printf("\t invalid input key:[%c]\n", key);
      continue;
    }

    if (!uapi_list[key - '0'].func) {
      printf("\t invalid function for key:[%c]\n", key);
      continue;
    }

    uapi_list[key - '0'].func(ctx);

  } while (key != 'q' && key != 'Q');

  return XCAM_RETURN_NO_ERROR;
}
