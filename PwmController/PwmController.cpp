#include <map>

#include "PwmController.h"
#include "LogFacility.h"
#include "driver/ledc.h"

#define NAME "PwmController"

#ifdef DEBUG_PWM_CONTROLLER
    #include <sstream>
    std::stringstream ss;

    std::string channelStr(ledc_channel_config_t* params)
    {
        ss << "\n\nledc_channel_config_t {\n"
           << "   gpio_num = " << params->gpio_num << ";\n"
           << "   speed_mode = " << params->speed_mode << ";\n"
           << "   channel = " << params->channel << ";\n"
           << "   intr_type = " << params->intr_type << ";\n"
           << "   timer_sel = " << params->timer_sel << ";\n"
           << "   duty = " << params->duty << ";\n}\n";
        std::string s = ss.str();
        ss.clear();
        return s;
    };

    std::string timerStr(ledc_timer_config_t* params)
    {
        ss << "\nledc_timer_config_t {\n"   
           << "   speed_mode = " << params->speed_mode << ";\n"
           << "   bit_num = " << params->bit_num << ";\n"
           << "   timer_num = " << params->timer_num << ";\n"
           << "   freq_hz = " << params->freq_hz << ";\n}\n";
        std::string s = ss.str();
        ss.clear();
        return s;
    };

#endif

#define DEFAULT_PWM_GPIO 16
#define DEFAULT_PWM_SPEED_MODE LEDC_HIGH_SPEED_MODE
#define DEFAULT_PWM_CHANNEL LEDC_CHANNEL_0
#define DEFAULT_PWM_INTR_MODE LEDC_INTR_FADE_END
#define DEFAULT_PWM_CHANNEL_TIMER LEDC_TIMER_0
#define DEFAULT_PWM_DUTY_CYCLE 10
#define DEFAULT_PWM_TIMER_BIT_DEPTH LEDC_TIMER_10_BIT
#define DEFAULT_PWM_FREQ 100
#define DEFAULT_FADE_FN_INTR_FLAGS ESP_INTR_FLAG_LEVEL1

PwmController::PwmController()
{
}

PwmController::~PwmController()
{
}

void PwmController::setDefaultChannelParams(ledc_channel_config_t* params)
{
    #ifdef DEBUG_PWM_CONTROLLER
    const char* tag = NAME"::setDefaultChannelParams()";
    ESP_LOGI(tag, "Setting PWM channel defaults.");
    #endif

    if (0 != params)
    {
        params->gpio_num = DEFAULT_PWM_GPIO;
        params->speed_mode = DEFAULT_PWM_SPEED_MODE;
        params->channel = DEFAULT_PWM_CHANNEL;
        params->intr_type = DEFAULT_PWM_INTR_MODE;
        params->timer_sel = DEFAULT_PWM_CHANNEL_TIMER;
        params->duty = DEFAULT_PWM_DUTY_CYCLE;

        #ifdef DEBUG_PWM_CONTROLLER
        ESP_LOGI(tag, "%s", channelStr(params).c_str());
        #endif
    }
    #ifdef DEBUG_PWM_CONTROLLER
    else
    {
        ESP_LOGE(tag, "Cannot initialize NULL params.");
    }
    #endif
}

void PwmController::setDefaultTimerParams(ledc_timer_config_t* params)
{
    #ifdef DEBUG_PWM_CONTROLLER
    const char* tag = NAME"::setDefaultTimerParams()";
    ESP_LOGI(tag, "Setting PWM timer defaults.");
    #endif

    if (0 != params)
    {
        params->speed_mode = DEFAULT_PWM_SPEED_MODE;
        params->bit_num = DEFAULT_PWM_TIMER_BIT_DEPTH;
        params->timer_num = DEFAULT_PWM_CHANNEL_TIMER;
        params->freq_hz = DEFAULT_PWM_FREQ;

        #ifdef DEBUG_PWM_CONTROLLER
        ESP_LOGI(tag, "%s", timerStr(params).c_str());
        ss.clear();
        #endif

    }
    #ifdef DEBUG_PWM_CONTROLLER
    else
    {
        ESP_LOGE(tag, "Cannot initialize NULL params.");
    }
    #endif
}

esp_err_t PwmController::configChannel(const ledc_channel_config_t* params)
{
    #ifdef DEBUG_PWM_CONTROLLER
    return log(NAME"::configChannel()", "Configuring PWM channel.",
               {
                 {ESP_OK, "PWM Controller channel succesfully configured." },
                 {ESP_ERR_INVALID_ARG, "Invalid parameter." }
               },
               ledc_channel_config, params);
    #else
    return ledc_channel_config(params);
    #endif
}

esp_err_t PwmController::configTimer(const ledc_timer_config_t *timerConf)
{
    #ifdef DEBUG_PWM_CONTROLLER
    return log(NAME"::configTimer()", "Configuring PWM timer.",
               {
                 { ESP_OK, "Successfully configured PWM timer." },
                 { ESP_ERR_INVALID_ARG, "Invalid parameter. Cannot configure timer." },
                 { ESP_FAIL, "Cannot find a proper pre-divider number base on the "
                             " given frequency and the current bit_num." }
               },
               ledc_timer_config, timerConf);
    #else
    return ledc_timer_config(timerConf);
    #endif
}

esp_err_t PwmController::setFreq(ledc_mode_t speedMode, ledc_timer_t timerNum, uint32_t freqHz)
{
    #ifdef DEBUG_PWM_CONTROLLER
    ss << "speedMode = " << speedMode << ", timerNum = " << timerNum
       << ", freqHz = " << freqHz << "\n";
    std::string s = ss.str();
    ss.clear();
    return log(NAME"::setFreq()", s.c_str(),
               {
                 { ESP_OK, "Successfully set PWM frequency." },
                 { ESP_ERR_INVALID_ARG, "Parameter error." },
                 { ESP_FAIL, "Can not find a proper pre-divider number base "
                             "on the given frequency and the current bit_num." }
               },
               ledc_set_freq, speedMode, timerNum, freqHz);
    #else
    return ledc_set_freq(speedMode, timerNum, freqHz);
    #endif
}

uint32_t PwmController::getFreq(ledc_mode_t speedMode, ledc_timer_t timerNum)
{
    uint32_t freq = ledc_get_freq(speedMode, timerNum);
    #ifdef DEBUG_PWM_CONTROLLER
    if (freq == 0)
    {
        ESP_LOGE(NAME"::getFreq()", "ERROR retrieving frequency.");
    }
    #endif
    return freq;
}

esp_err_t PwmController::setDuty(ledc_mode_t speedMode, ledc_channel_t channel, uint32_t duty)
{
    #ifdef DEBUG_PWM_CONTROLLER
    ss << "speedMode = " << speedMode << ", channel = " << channel
       << ", duty = " << duty << "\n";
    std::string s = ss.str();
    ss.clear();
    return log(NAME"::setDuty()", s.c_str(),
               {
                 { ESP_OK, "Successfully set PWM duty cycle." },
                 { ESP_ERR_INVALID_ARG, "Parameter error." }
               },
               ledc_set_duty, speedMode, channel, duty);
    #else
    return ledc_set_duty(speedMode, channel, duty);
    #endif
}

int PwmController::getDuty(ledc_mode_t speedMode, ledc_channel_t channel)
{
    int duty = ledc_get_duty(speedMode, channel);
    #ifdef DEBUG_PWM_CONTROLLER
    if (duty == -1)
    {
        ESP_LOGE(NAME"::getDuty()", "ERROR retrieving duty cycle.");
    }
    #endif
    return duty;
}

esp_err_t PwmController::updateDuty(ledc_mode_t speedMode, ledc_channel_t channel)
{
    #ifdef DEBUG_PWM_CONTROLLER
    ss << "Updating PWM duty cycle: speedMode = " << speedMode
       << ", channel = " << channel << "\n";
    std::string s = ss.str();
    ss.clear();
    return log(NAME"::updateDuty()", s.c_str(),
               {
                 { ESP_OK, "Successfully updated PWM duty cycle." },
                 { ESP_ERR_INVALID_ARG, "Parameter error." }
               },
               ledc_update_duty, speedMode, channel);
    #else
    return ledc_update_duty(speedMode, channel);
    #endif
}

esp_err_t PwmController::installFadeFunc(int flags)
{
    #ifdef DEBUG_PWM_CONTROLLER
    return log(NAME"::installFadeFunc()", "Installing fade function.",
               {
                 { ESP_OK, "Successully installed fade function." },
                 { ESP_ERR_INVALID_STATE, "Fade function already installed." }
               },
               ledc_fade_func_install, flags);
    #else
    return ledc_fade_func_install(flags);
    #endif
}

esp_err_t PwmController::setFadeTime(ledc_mode_t speedMode, ledc_channel_t channel,
                                            int targetDuty, int maxFadeTimeMs)
{
    #ifdef DEBUG_PWM_CONTROLLER
    return log(NAME"::setFadeTime()", "Setting PWM fade time.",
               {
                   { ESP_OK, "Successfully set PWM fade time." },
                   { ESP_ERR_INVALID_ARG, "Parameter error." },
                   { ESP_ERR_INVALID_STATE, "Fade function not installed." },
                   { ESP_FAIL, "Fade function init error." }
               },
               ledc_set_fade_with_time, speedMode, channel, targetDuty, maxFadeTimeMs);
    #else
    return ledc_set_fade_with_time(speedMode, channel, targetDuty, maxFadeTimeMs);
    #endif
}

esp_err_t PwmController::startFade(ledc_mode_t speedMode, ledc_channel_t channel,
                                          ledc_fade_mode_t waitDone)
{
    #ifdef DEBUG_PWM_CONTROLLER
    return log(NAME"::startFade()", "Starting PWM fade.",
               {
                 { ESP_OK, "Successfully faded." },
                 { ESP_ERR_INVALID_STATE, "Fade function not installed." },
                 { ESP_ERR_INVALID_ARG, "Parameter error." }
               },
               ledc_fade_start, speedMode, channel, waitDone);
    #else
    return ledc_fade_start(speedMode, channel, waitDone);
    #endif
}
