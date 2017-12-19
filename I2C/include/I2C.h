#pragma once

#include "driver/i2c.h"

namespace esp
{
class I2C
{
public:
    static esp_err_t config(i2c_port_t i2cNum, const i2c_config_t *i2cConf);

    static esp_err_t installDriver(i2c_port_t i2cNum, i2c_mode_t mode,
                                   size_t slvRxBufLen, size_t slvTxBufLen,
                                   int intrAllocFlags);

    static int slaveRead(i2c_port_t i2cNum, uint8_t *data,
                               size_t maxSize, portBASE_TYPE ticksToWait);

    static int slaveWrite(i2c_port_t i2cNum, uint8_t *data, int size, portBASE_TYPE ticksToWait);
}; // End class I2C
}; // End namespace esp
