#pragma once

#include "driver/ledc.h"

class PwmController
{
public:
    PwmController();

    virtual ~PwmController();

    static void setDefaultChannelParams(ledc_channel_config_t* params);

    static void setDefaultTimerParams(ledc_timer_config_t* params);

    static esp_err_t configChannel(const ledc_channel_config_t* params);

    static esp_err_t configTimer(const ledc_timer_config_t *timer_conf);

    static esp_err_t setFreq(ledc_mode_t speed_mode, ledc_timer_t timer_num, uint32_t freq_hz);

    static uint32_t getFreq(ledc_mode_t speedMode, ledc_timer_t timerNum);

    static esp_err_t setDuty(ledc_mode_t speedMode, ledc_channel_t channel, uint32_t duty);

    static int getDuty(ledc_mode_t speedMode, ledc_channel_t channel);

    static esp_err_t updateDuty(ledc_mode_t speedMode, ledc_channel_t channel);

    static esp_err_t installFadeFunc(int flags);

    static esp_err_t setFadeTime(ledc_mode_t speedMode, ledc_channel_t channel,
                                 int targetDuty, int maxFadeTimeMs);

    static esp_err_t startFade(ledc_mode_t speedMode, ledc_channel_t channel,
                               ledc_fade_mode_t waitDone);

private:
    ledc_channel_config_t _channelConf;
    ledc_timer_config_t _timerConf;
};
