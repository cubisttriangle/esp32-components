#pragma once

#include <map>
#include "esp_err.h"
#include "esp_log.h"

static void decipherEspFnRetCode(const char* fnName, esp_err_t retCode,
                                 const std::map<esp_err_t, const char*>& errMap)
{
    std::map<esp_err_t, const char*>::const_iterator it = errMap.find(retCode);

    if (it != errMap.end())
    {
        if (ESP_OK == retCode)
        {
            ESP_LOGI(fnName, "%s", it->second);
        }
        else
        {
            ESP_LOGE(fnName, "%s", it->second);
        }
    }
    else
    {
        ESP_LOGE(fnName, "WARNING: Could not identify error code.");
    }
};

template <typename Function, typename... FnArgs>
static esp_err_t log(const char* tag, const char* initMsg,
                     const std::map<esp_err_t, const char*>&& errMap,
                     Function fn, FnArgs&&... args)
{
    ESP_LOGI(tag, "%s", initMsg);
    esp_err_t fnRetCode = fn(std::forward<FnArgs>(args)...);
    decipherEspFnRetCode(tag, fnRetCode, errMap);

    return fnRetCode;
};

