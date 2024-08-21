/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 */
#include "wiced_rtos.h"
#include "platform_i2s.h"
#include "wwd_assert.h"
#include "platform_audio.h"
#include "platform_config.h"
#include "string.h"

/******************************************************
 *                      Macros
 ******************************************************/
#define I2SCFGR_CLEAR_MASK        ((uint16_t)0xF040)

/******************************************************
 *                    Constants
 ******************************************************/

#define I2S_STANDARD_PHILLIPS

#ifdef I2S_STANDARD_PHILLIPS
#define I2S_STANDARD                   I2S_Standard_Phillips
#elif defined(I2S_STANDARD_MSB)
#define I2S_STANDARD                   I2S_Standard_MSB
#elif defined(I2S_STANDARD_LSB)
#define I2S_STANDARD                   I2S_Standard_LSB
#else
#error "Error: No audio communication standard selected !"
#endif

#define I2S_SLAVE_WS_WAR( I2S )        while (!platform_gpio_input_get(i2s_interfaces[I2S].pin_ws)){}

#define HSE_RATIO                   ((double)(HSE_VALUE/PLL_M_CONSTANT)/256) /* for example for HSE=26Mhz and PLLM=26: ==> (26 000 000 / 26)/256 = 3906.25Hz */
#define I2S_PRESCALING_FACTOR_MIN   (4)
#define I2S_PRESCALING_FACTOR_MAX   (511)
#define PLLI2S_N_MIN                (192)
#define PLLI2S_N_MAX                (432)
#define PLLI2S_R_MIN                (2)
#define PLLI2S_R_MAX                (7)
#define CLOSEST_LOW_COEFFS          (0)
#define CLOSEST_HIGH_COEFFS         (1)


/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/
typedef struct
{
    uint8_t pllm;
    uint32_t sample_rate;
    uint32_t mclk_freq;
    uint16_t plli2sn;
    uint8_t plli2sr;
} mclk_pickup_entry_t ;


typedef struct
{
    uint16_t plli2sn;
    uint8_t  plli2sr;
    uint16_t i2sodd;
    uint16_t i2sdiv;
    uint32_t freq;
} clock_system_setup_for_freq_t;


/******************************************************
 *                    Structures
 ******************************************************/

struct i2s_control
{
    /* Configuration from initialize. */
    uint8_t                         sample_bits;

    /* RX/TX specific data. */
    wiced_i2s_transfer_t            op_type;
    const platform_dma_t           *dma;

    uint16_t                        period_size;
    uint32_t                        position;

    uint8_t                        *audio_buffer_ptr;
    uint16_t                        audio_buffer_size;

    uint16_t                        periods_count;
    wiced_audio_session_ref         sh;

    /* current configuration */
    I2S_InitTypeDef                 i2s_config;
    wiced_bool_t                    is_initialised;
    int                             selected_pll_configuration;

    /* current PLL and divider settings */
    clock_system_setup_for_freq_t   sample_rate_clocks_coeffs;

} i2s_control[WICED_I2S_MAX];

/******************************************************
 *               Static Function Declarations
 ******************************************************/

//void i2s1_dma_irq( void );
//void i2s1_irq( void );

static void           i2s_gpio_init(wiced_i2s_t i2s);
static void           i2s_gpio_deinit(wiced_i2s_t i2s);
static wiced_result_t i2s_peripheral_init(wiced_i2s_t i2s, const wiced_i2s_params_t *config, uint32_t *mclk);
static void           i2s_peripheral_deinit(wiced_i2s_t i2s);
static void           i2s_dma_init(wiced_i2s_t i2s, SPI_TypeDef *spi, const platform_dma_t *dma, uint32_t dir);
static void           i2s_dma_deinit(const platform_dma_t *dma);

extern wiced_result_t wiced_audio_buffer_platform_event(wiced_audio_session_ref sh, wiced_audio_platform_event_t event);
extern uint16_t       wiced_audio_buffer_platform_get_periods(wiced_audio_session_ref sh);
extern wiced_result_t wiced_audio_buffer_platform_event(wiced_audio_session_ref sh, wiced_audio_platform_event_t event);

/******************************************************
 *               Variable Definitions
 ******************************************************/

static uint32_t played_periods = 0;
static uint32_t audio_hiccups = 0;

extern const platform_i2s_t i2s_interfaces[];

/******************************************************
 *               Function Definitions
 ******************************************************/

wiced_result_t wiced_i2s_init(wiced_audio_session_ref sh, wiced_i2s_t i2s, wiced_i2s_params_t *config, uint32_t *mclk)
{
    wiced_result_t status;

    i2s_gpio_init(i2s);

    status = i2s_peripheral_init(i2s, config, mclk);
    if (WICED_SUCCESS != status)
    {
        i2s_gpio_deinit(i2s);
        return status;
    }

    i2s_control[ i2s ].sample_bits = config->bits_per_sample;
    i2s_control[ i2s ].period_size = config->period_size;
    i2s_control[ i2s ].sh = sh;

    return WICED_SUCCESS;
}

wiced_result_t wiced_i2s_deinit(wiced_i2s_t i2s)
{
#if 0
    i2s_gpio_deinit(i2s);
    i2s_peripheral_deinit(i2s);
    i2s_dma_deinit(&i2s_interfaces[i2s].tx_dma);
#endif
    return WICED_SUCCESS;
}

wiced_result_t wiced_i2s_start(wiced_i2s_t i2s)
{
#if 0
    i2s_dma_init(i2s, i2s_interfaces[i2s].spi, &i2s_interfaces[i2s].tx_dma, DMA_DIR_MemoryToPeripheral);

    /* Enable the DMA Tx request. */
    SPI_I2S_DMACmd(i2s_interfaces[i2s].spi, SPI_I2S_DMAReq_Tx, ENABLE);

    i2s_control[i2s].dma = &i2s_interfaces[i2s].tx_dma;

    /* Enable the I2S DMA stream. */
    DMA_Cmd(i2s_interfaces[i2s].tx_dma.stream, ENABLE);

    /* I2S slave mode errata. */
    if (i2s_interfaces[ i2s ].is_master == WICED_FALSE)
    {
        /* Wait for WS to go high or low (depending on I2S standard) before enabling peripheral. */
        /* STM32F4 device limitations, section 2.4.1. */
        I2S_SLAVE_WS_WAR(i2s);
    }
    /* Enable the I2S peripheral. */
    I2S_Cmd(i2s_interfaces[i2s].spi, ENABLE);
#endif
    return WICED_SUCCESS;
}
wiced_result_t wiced_i2s_stop(wiced_i2s_t i2s)
{
#if 0
    /* DISABLE the I2S DMA stream. */
    DMA_Cmd(i2s_interfaces[i2s].tx_dma.stream, DISABLE);

    /* Disable the I2S peripheral. */
    I2S_Cmd(i2s_interfaces[i2s].spi, DISABLE);
#endif
    return WICED_SUCCESS;
}

wiced_result_t wiced_i2s_get_clocking_details(const wiced_i2s_params_t *config, uint32_t *mclk)
{
    UNUSED_PARAMETER(config);
    UNUSED_PARAMETER(mclk);
    return WICED_UNSUPPORTED;
}

/******************************************************
 *          Static Function Definitions
 ******************************************************/

static void i2s_gpio_init(wiced_i2s_t i2s)
{
#if 0
    platform_gpio_set_alternate_function(i2s_interfaces[i2s].pin_ck->port, i2s_interfaces[i2s].pin_ck->pin_number, GPIO_OType_PP, GPIO_PuPd_NOPULL, i2s_interfaces[i2s].gpio_af);
    platform_gpio_set_alternate_function(i2s_interfaces[i2s].pin_sd->port, i2s_interfaces[i2s].pin_sd->pin_number, GPIO_OType_PP, GPIO_PuPd_NOPULL, i2s_interfaces[i2s].gpio_af);
    platform_gpio_set_alternate_function(i2s_interfaces[i2s].pin_ws->port, i2s_interfaces[i2s].pin_ws->pin_number, GPIO_OType_PP, GPIO_PuPd_NOPULL, i2s_interfaces[i2s].gpio_af);

    /* MCLK */
    if (i2s_interfaces[i2s].enable_mclk != 0)
    {
        platform_gpio_set_alternate_function(i2s_interfaces[i2s].pin_mclk->port, i2s_interfaces[i2s].pin_mclk->pin_number, GPIO_OType_PP, GPIO_PuPd_NOPULL, i2s_interfaces[i2s].gpio_af);
    }
    else
    {
        platform_gpio_init(i2s_interfaces[i2s].pin_mclk, INPUT_HIGH_IMPEDANCE);
    }
#endif
}

static void i2s_gpio_deinit(wiced_i2s_t i2s)
{
#if 0
    platform_gpio_deinit(i2s_interfaces[ i2s ].pin_ck);
    platform_gpio_deinit(i2s_interfaces[ i2s ].pin_sd);
    platform_gpio_deinit(i2s_interfaces[ i2s ].pin_ws);

    if (i2s_interfaces[i2s].enable_mclk != 0)
    {
        platform_gpio_deinit(i2s_interfaces[ i2s ].pin_mclk);
    }
#endif
}

wiced_result_t wiced_i2s_set_audio_buffer_details(wiced_i2s_t i2s, uint8_t *buffer_ptr, uint16_t size)
{
#if 0
    if ((buffer_ptr != NULL) && (size != 0))
    {
        i2s_control[i2s].audio_buffer_ptr = buffer_ptr;
        i2s_control[i2s].position = 0;
        i2s_control[i2s].audio_buffer_size = size;
        return WICED_SUCCESS;
    }
    else
    {
        return WICED_ERROR;
    }
#else
    return WICED_SUCCESS;
#endif
}

void setup_i2s_pll_and_freq_dividers(wiced_i2s_t i2s, uint16_t i2sn, uint16_t i2sr, uint16_t i2sdiv, uint8_t i2sodd)
{
#if 0
    I2S_InitTypeDef *i2s_init_ptr = &i2s_control[i2s].i2s_config;

    RCC->CR &= ~(1 << 26);

    RCC->CFGR &= ~RCC_CFGR_I2SSRC;
    RCC->PLLI2SCFGR = (i2sn << 6) | (i2sr << 28);
    RCC->CR |= (1 << 26);

    /* Wait till PLLI2S is ready */
    while ((RCC->CR & (1 << 27)) == 0) {}

    i2s_interfaces[i2s].spi->I2SPR = 0;
    i2s_interfaces[i2s].spi->I2SPR = (uint16_t)((uint16_t)i2sdiv | (uint16_t)((uint16_t)(i2sodd << 8) | (uint16_t)i2s_init_ptr->I2S_MCLKOutput));
#endif
}

wiced_result_t wiced_i2s_configure_clocks_for_sample_rate(wiced_i2s_t i2s, uint32_t *sample_rate)   /* in hertz */
{
#if 0
    uint16_t                        pll_i2sn, pll_i2sr;
    uint32_t                        closest_low_value = 0;
    uint32_t                        closest_high_value = 0;
    wiced_bool_t                    match_found = WICED_FALSE;
    clock_system_setup_for_freq_t   closest_setups[2];
    int                             prescaling_factor;
    long int required_sample_rate = (long int)(*sample_rate);
    memset(&closest_setups[0], 0x00, sizeof(closest_setups));

    /* formula ->      HSE_RATIO *( PLLI2SN/PLLI2SR ) / prescaling_factor   = required_sample_rate; */
    for (pll_i2sn = PLLI2S_N_MIN ; pll_i2sn <= PLLI2S_N_MAX; pll_i2sn++)
    {
        for (pll_i2sr = PLLI2S_R_MIN; pll_i2sr <= PLLI2S_R_MAX; pll_i2sr++)
        {
            for (prescaling_factor = I2S_PRESCALING_FACTOR_MIN; prescaling_factor <= I2S_PRESCALING_FACTOR_MAX; prescaling_factor++)
            {
                double      pll_i2sn_float, pll_i2sr_float, prescaling_factor_float;
                double      freq;
                long int    freq_int;

                pll_i2sn_float = (double)pll_i2sn;
                pll_i2sr_float = (double)pll_i2sr;
                prescaling_factor_float = (double)prescaling_factor;

                freq = HSE_RATIO * (pll_i2sn_float / pll_i2sr_float) / prescaling_factor_float;
                freq_int = (long int)freq;
                if (freq_int == required_sample_rate)
                {
                    match_found = WICED_TRUE;
                    i2s_control[i2s].sample_rate_clocks_coeffs.freq =       *sample_rate;
                    i2s_control[i2s].sample_rate_clocks_coeffs.plli2sn =    pll_i2sn;
                    i2s_control[i2s].sample_rate_clocks_coeffs.plli2sr =    pll_i2sr;
                    i2s_control[i2s].sample_rate_clocks_coeffs.i2sdiv =     prescaling_factor / 2;;
                    i2s_control[i2s].sample_rate_clocks_coeffs.i2sodd =     prescaling_factor % (2 * i2s_control[i2s].sample_rate_clocks_coeffs.i2sdiv);

                    break;
                }
                else
                {
                    wiced_bool_t update_closest_low = WICED_FALSE;
                    wiced_bool_t update_closest_high = WICED_FALSE;
                    if ((freq_int < required_sample_rate) && (closest_low_value < freq_int))
                    {
                        closest_low_value = freq_int;
                        update_closest_low = WICED_TRUE;
                    }
                    else if ((closest_low_value == 0) && (freq_int < required_sample_rate))
                    {
                        closest_low_value = freq_int;
                        update_closest_low = WICED_TRUE;
                    }

                    if ((freq_int > required_sample_rate) && (closest_high_value > freq_int))
                    {
                        closest_high_value = freq_int;
                        update_closest_high = WICED_TRUE;
                    }
                    else if ((closest_high_value == 0) && (freq_int > required_sample_rate))
                    {
                        closest_high_value = freq_int;
                        update_closest_high = WICED_TRUE;
                    }

                    if (update_closest_low == WICED_TRUE)
                    {
                        closest_setups[CLOSEST_LOW_COEFFS].freq = closest_low_value;
                        closest_setups[CLOSEST_LOW_COEFFS].plli2sn = pll_i2sn;
                        closest_setups[CLOSEST_LOW_COEFFS].plli2sr = pll_i2sr;
                        closest_setups[CLOSEST_LOW_COEFFS].i2sdiv = prescaling_factor / 2;
                        closest_setups[CLOSEST_LOW_COEFFS].i2sodd = prescaling_factor % (2 * closest_setups[CLOSEST_LOW_COEFFS].i2sdiv);

                    }
                    else if (update_closest_high == WICED_TRUE)
                    {
                        closest_setups[CLOSEST_HIGH_COEFFS].freq = closest_high_value;
                        closest_setups[CLOSEST_HIGH_COEFFS].plli2sn = pll_i2sn;
                        closest_setups[CLOSEST_HIGH_COEFFS].plli2sr = pll_i2sr;
                        closest_setups[CLOSEST_HIGH_COEFFS].i2sdiv = prescaling_factor / 2;
                        closest_setups[CLOSEST_HIGH_COEFFS].i2sodd = prescaling_factor % (2 * closest_setups[CLOSEST_HIGH_COEFFS].i2sdiv);
                    }
                    if (freq_int < required_sample_rate)
                    {
                        break;
                    }
                }
            }

            if (match_found == WICED_TRUE)
            {
                break;
            }
        }
        if (match_found == WICED_TRUE)
        {
            break;
        }
    }

    if (match_found == WICED_FALSE)
    {
        /* find the one which is more closer to the required sample rate */
        if ((closest_high_value - *sample_rate) < (*sample_rate - closest_low_value))
        {
            *sample_rate = closest_high_value;
            i2s_control[i2s].sample_rate_clocks_coeffs.freq =       *sample_rate;
            i2s_control[i2s].sample_rate_clocks_coeffs.plli2sn =    closest_setups[CLOSEST_HIGH_COEFFS].plli2sn;
            i2s_control[i2s].sample_rate_clocks_coeffs.plli2sr =    closest_setups[CLOSEST_HIGH_COEFFS].plli2sr;
            i2s_control[i2s].sample_rate_clocks_coeffs.i2sdiv =     closest_setups[CLOSEST_HIGH_COEFFS].i2sdiv;
            i2s_control[i2s].sample_rate_clocks_coeffs.i2sodd =     closest_setups[CLOSEST_HIGH_COEFFS].i2sodd;
        }
        else
        {
            *sample_rate = closest_low_value;
            i2s_control[i2s].sample_rate_clocks_coeffs.freq =       *sample_rate;
            i2s_control[i2s].sample_rate_clocks_coeffs.plli2sn =    closest_setups[CLOSEST_LOW_COEFFS].plli2sn;
            i2s_control[i2s].sample_rate_clocks_coeffs.plli2sr =    closest_setups[CLOSEST_LOW_COEFFS].plli2sr;
            i2s_control[i2s].sample_rate_clocks_coeffs.i2sdiv =     closest_setups[CLOSEST_LOW_COEFFS].i2sdiv;
            i2s_control[i2s].sample_rate_clocks_coeffs.i2sodd =     closest_setups[CLOSEST_LOW_COEFFS].i2sodd;
        }
    }

    setup_i2s_pll_and_freq_dividers(i2s, i2s_control[i2s].sample_rate_clocks_coeffs.plli2sn,
                                    i2s_control[i2s].sample_rate_clocks_coeffs.plli2sr,
                                    i2s_control[i2s].sample_rate_clocks_coeffs.i2sdiv,
                                    i2s_control[i2s].sample_rate_clocks_coeffs.i2sodd);
#endif
    return WICED_SUCCESS;
}

static wiced_result_t i2s_peripheral_init(wiced_i2s_t i2s, const wiced_i2s_params_t *config, uint32_t *mclk)
{
#if 0
    uint16_t                        tmpreg = 0;
    I2S_InitTypeDef                *i2s_init_ptr = &i2s_control[i2s].i2s_config;

    /* Enable I2S peripheral clock. */
    i2s_interfaces[i2s].peripheral_clock_func(i2s_interfaces[i2s].peripheral_clock, ENABLE);

    SPI_I2S_DeInit(i2s_interfaces[i2s].spi);

    switch (config->bits_per_sample)
    {
    case 16:
        i2s_init_ptr->I2S_DataFormat = I2S_DataFormat_16b;
        break;
    case 24:
        i2s_init_ptr->I2S_DataFormat = I2S_DataFormat_24b;
        break;
    case 32:
        i2s_init_ptr->I2S_DataFormat = I2S_DataFormat_32b;
        break;
    /* XXX: no 16bextended! */
    default:
        return WICED_BADARG;
    }

    if (IS_I2S_AUDIO_FREQ(config->sample_rate) == 0)
    {
        return WICED_BADARG;
    }



    i2s_init_ptr->I2S_AudioFreq = config->sample_rate; /* XXX: What does default (2) do? */


    i2s_init_ptr->I2S_Standard = I2S_STANDARD;
    i2s_init_ptr->I2S_CPOL = I2S_CPOL_Low;

    i2s_init_ptr->I2S_Mode = i2s_interfaces[i2s].is_master ? I2S_Mode_MasterTx : I2S_Mode_SlaveTx;

    /* XXX: Accord to the refman, mclk is only available for master mode. */
    i2s_init_ptr->I2S_MCLKOutput = i2s_interfaces[i2s].enable_mclk ? I2S_MCLKOutput_Enable : I2S_MCLKOutput_Disable;

    /* Configure I2S clocking system for a desired sample rate, return the real sample_rate which was set */
    if (i2s_interfaces[i2s].is_master == WICED_TRUE)
    {
        *mclk = 256 * i2s_init_ptr->I2S_AudioFreq;
        wiced_i2s_configure_clocks_for_sample_rate(i2s, &i2s_init_ptr->I2S_AudioFreq);
    }
    else
    {
        /* In slave mode, clocks left unconfigured */
        /* mclk is set to 0, to show to the audio device, that i2s is not providing mclk */
        *mclk = 0;
    }

    i2s_interfaces[i2s].spi->I2SCFGR &= I2SCFGR_CLEAR_MASK;

    /* Get the I2SCFGR register value */
    tmpreg = i2s_interfaces[i2s].spi->I2SCFGR;

    /* Configure the I2S with the SPI_InitStruct values */
    tmpreg |= (uint16_t)((uint16_t)SPI_I2SCFGR_I2SMOD | (uint16_t)(i2s_init_ptr->I2S_Mode | \
                         (uint16_t)(i2s_init_ptr->I2S_Standard | (uint16_t)(i2s_init_ptr->I2S_DataFormat | \
                                    (uint16_t)i2s_init_ptr->I2S_CPOL))));

    /* Write to SPIx I2SCFGR */
    i2s_interfaces[i2s].spi->I2SCFGR = tmpreg;

    i2s_control[i2s].is_initialised = WICED_TRUE;
    /* The I2S peripheral will be enabled when either an rx/tx function is called. */
#endif
    return WICED_SUCCESS;
}


wiced_result_t  wiced_i2s_get_current_hw_pointer(wiced_i2s_t i2s, uint32_t *hw_pointer)
{
#if 0
    *hw_pointer = (i2s_control[i2s].period_size - i2s_interfaces[i2s].tx_dma.stream->NDTR * 2) / 4;
#endif
    return WICED_SUCCESS;
}


wiced_result_t wiced_i2s_pll_set_fractional_divider(wiced_i2s_t i2s, float value)
{
    UNUSED_PARAMETER(i2s);
    UNUSED_PARAMETER(value);
    return WICED_UNSUPPORTED;
}


static void i2s_peripheral_deinit(wiced_i2s_t i2s)
{
#if 0
    /* Disable I2S peripheral if not already done by rx/tx completion. */
    I2S_Cmd(i2s_interfaces[i2s].spi, DISABLE);

    /* Deinitialize the I2S peripheral. */
    SPI_I2S_DeInit(i2s_interfaces[i2s].spi);

    i2s_control[i2s].selected_pll_configuration = -1;
    memset((void *)&i2s_control[i2s].i2s_config, 0x00, sizeof(i2s_control[i2s].i2s_config));


    /* Disable I2S peripheral clock. */
    i2s_interfaces[i2s].peripheral_clock_func(i2s_interfaces[i2s].peripheral_clock, DISABLE);
#endif
}

static void i2s_dma_init(wiced_i2s_t i2s, SPI_TypeDef *spi, const platform_dma_t *dma, uint32_t dir)
{
#if 0
    DMA_InitTypeDef DMA_InitStructure;

    /* Enable DMA clock. */
    (*dma->peripheral_clock_func)(dma->peripheral_clock, ENABLE);

    /* Initialize DMA default values. */
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &spi->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&i2s_control[i2s].audio_buffer_ptr[0];
    DMA_InitStructure.DMA_BufferSize         = i2s_control[i2s].period_size;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_DIR                = dir;
    DMA_InitStructure.DMA_Channel            = dma->channel;

    /* Configure DMA. */
    DMA_Cmd(dma->stream, DISABLE);
    DMA_DeInit(dma->stream);
    DMA_Init(dma->stream, &DMA_InitStructure);

    /* Enable DMA interrupts. */
    DMA_ITConfig(dma->stream, DMA_IT_TC | DMA_IT_TE, ENABLE);

    /* I2S DMA IRQ channel configuration. */
    NVIC_EnableIRQ(dma->irq);
#endif
}

static void i2s_dma_deinit(const platform_dma_t *dma)
{
#if 0
    NVIC_DisableIRQ(dma->irq);

    /* Disable the DMA stream before the deinit. */
    DMA_Cmd(dma->stream, DISABLE);

    /* Deinitialize the DMA stream. */
    DMA_DeInit(dma->stream);

    /* The DMA clock is not disabled as it can be used by other streams. */
#endif
}

void platform_i2s_irq(uint32_t i2s)
{
}

#ifdef BT_AVK_SOFT_IRQ

void platform_i2s_tx_dma_irq(uint32_t i2s)
{
    ++played_periods;


    /* update our position in the audio buffer, so the framework knows how much real estate in the buffer is available for a user */
    wiced_audio_buffer_platform_event(i2s_control[i2s].sh, WICED_AUDIO_PERIOD_ELAPSED);

    /* We have played all requested periods, see if we have got more to play */
    i2s_control[i2s].periods_count = wiced_audio_buffer_platform_get_periods(i2s_control[i2s].sh);


    /* We have got more to play, schedule a new dma transfer */
    if (i2s_control[i2s].periods_count != 0)
    {
        if ((played_periods % 1000) == 999)
        {
            // more data to send, setup DMA gain.
            //rt_kprintf("setup DMA again. %d\n", played_periods);
        }
    }
    else
    {
        // underrun occurs.
        audio_hiccups++;
        wiced_audio_buffer_platform_event(i2s_control[i2s].sh, WICED_AUDIO_UNDERRUN);
        rt_kprintf("under run");
    }
}
#endif