#include "SOUNDGEN.h"
#include <driver/dac.h>
#include "hal/dac_types.h"
#include "soc/sens_reg.h"

int startSound(int freq)
{
    dac_cw_config_t config;
    config.en_ch = DAC_CHANNEL_2;
    config.freq = freq;
    config.offset = 2;
    config.phase = DAC_CW_PHASE_0;
    config.scale = DAC_CW_SCALE_1;

    dac_cw_generator_config(&config);
    dac_output_enable(DAC_CHANNEL_2);
    dac_cw_generator_enable();
    int frequency = (freq/130)*130;
    return frequency;
}

void stopSound()
{
    dac_output_disable(DAC_CHANNEL_2);
}
