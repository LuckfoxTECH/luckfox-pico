#include "wiced.h"
#include "dual_a2dp_hfp_audio.h"
#include "bluetooth_a2dp.h"
#include "wiced_dct_common.h"
#include "wiced_codec_if.h"

/***********************
 * codec interface
*/
#if 0
#define DUMP_STAMP()   rt_kprintf("%s\n", __FUNCTION__)

static wiced_codec_status_t soft_codec_init(wiced_codec_data_transfer_api_t *data_transfer_methods, void *data)
{
    DUMP_STAMP();
    return WICED_SUCCESS;
}

static wiced_codec_status_t soft_codec_close()
{
    DUMP_STAMP();
    return WICED_SUCCESS;
}

void soft_codec_get_capabilities(void *codec_capabilities, void *codec_preferred_params)
{
    DUMP_STAMP();
    return;
}

static wiced_codec_status_t soft_codec_set_configuration(void *config)
{
    DUMP_STAMP();
    return WICED_SUCCESS;
}

static wiced_codec_status_t soft_codec_encode()
{
    DUMP_STAMP();
    return WICED_SUCCESS;
}

static wiced_codec_status_t soft_codec_decode()
{
    DUMP_STAMP();
    return WICED_SUCCESS;
}

static wiced_codec_interface_t soft_codec =
{
    .version_major = 1,
    .version_minor = 2,
    .type = WICED_CODEC_SBC,
    .configured = 1,
    .init = soft_codec_init,
    .close = soft_codec_close,
    .get_capabilities = soft_codec_get_capabilities,
    .set_configuration = soft_codec_set_configuration,
    .encode = soft_codec_encode,
    .decode = soft_codec_decode
};
#endif
wiced_result_t brcm_dac_device_register();
wiced_result_t platform_init_audio(void)
{
    brcm_dac_device_register();
    return WICED_SUCCESS;
}

static uint8_t dct_mem[2048];
static bool is_dct_available(uint32_t offset, uint32_t size)
{
    return ((offset + size) < sizeof(dct_mem)) && (offset < sizeof(dct_mem)) ;
}
/*
 */
wiced_result_t wiced_dct_read_lock(void **info_ptr, wiced_bool_t ptr_is_writable, dct_section_t section, uint32_t offset, uint32_t size)
{
    RT_ASSERT(is_dct_available(offset, size));
    *info_ptr = dct_mem + offset;
    return WICED_SUCCESS;
}

wiced_result_t wiced_dct_read_unlock(void *info_ptr, wiced_bool_t ptr_is_writable)
{
    return WICED_SUCCESS;
}

wiced_result_t wiced_dct_write(const void *data, dct_section_t section, uint32_t offset, uint32_t data_length)
{
    RT_ASSERT(is_dct_available(offset, data_length));
    memcpy(dct_mem + offset, data, data_length);
    return WICED_SUCCESS;
}

/**
 * @fn wiced_get_codec_by_type
 * @brief Get the interface object for a given codec by specifying the codec type.
 *
 * @param type The codec type from the enum list in this file.
 * @return codec_interface_t* Pointer to the codec of type codec_type_t if supported
 *                            NULL if not supported.
 */
wiced_codec_interface_t *wiced_get_codec_by_type(wiced_codec_type_t type)
{
    extern wiced_codec_interface_t codec_sbc;
    return &codec_sbc;
}

wiced_result_t wiced_dct_clean(void)
{
    memset(dct_mem, 0, sizeof(dct_mem));
    return WICED_SUCCESS;
}

