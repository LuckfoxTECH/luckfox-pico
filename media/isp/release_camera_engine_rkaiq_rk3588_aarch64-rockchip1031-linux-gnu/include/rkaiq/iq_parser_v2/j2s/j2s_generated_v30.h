#include <stdint.h>
#include "j2s.h"

#include "j2s_code2bin.h"

#define J2S_MAGIC 29572404
#define J2S_NUM_OBJ 3736
#define J2S_NUM_STRUCT 626
#define J2S_NUM_ENUM 69
#define J2S_NUM_ENUM_VALUE 284

static void _j2s_init(j2s_ctx *ctx) {
	size_t objs_len = 0;
	size_t structs_len = 0;
	size_t enums_len = 0;

	ctx->magic = J2S_MAGIC;
	ctx->priv = NULL;
	ctx->objs = (j2s_obj *)(&j2s_code2bin_bin[0]);
	objs_len = sizeof(j2s_obj) * J2S_NUM_OBJ;
	ctx->structs = (j2s_struct *)(&j2s_code2bin_bin[0] + objs_len);
	structs_len = sizeof(j2s_struct) * J2S_NUM_STRUCT;
	ctx->enums = (j2s_enum *)(&j2s_code2bin_bin[0] + objs_len + structs_len);
	enums_len = sizeof(j2s_enum) * J2S_NUM_ENUM;
	ctx->enum_values = (j2s_enum_value *)(&j2s_code2bin_bin[0] + objs_len + structs_len + enums_len);

	ctx->num_obj = J2S_NUM_OBJ;
	ctx->num_struct = J2S_NUM_STRUCT;
	ctx->num_enum = J2S_NUM_ENUM;
	ctx->num_enum_value = J2S_NUM_ENUM_VALUE;

	ctx->num_desc = 0;
	ctx->root_index = 624;
}

