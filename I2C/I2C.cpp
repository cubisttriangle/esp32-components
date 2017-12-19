#include "I2C.h"
#include "LogFacility.h"

namespace esp
{

esp_err_t I2C::config(i2c_port_t i2cNum, const i2c_config_t *i2cConf)
{
    #ifdef DEBUG_I2C
    return log("espI2C::config()", "Configuring I2C params.",
               {
                 { ESP_OK, "Successfully configured I2C params." },
                 { ESP_ERR_INVALID_ARG, "Parameter error." }
               },
               i2c_param_config, i2cNum, i2cConf);
    #else
    return i2c_param_config(i2cNum, i2cConf);
    #endif
}

esp_err_t I2C::installDriver(i2c_port_t i2c_num, i2c_mode_t mode,
                             size_t slv_rx_buf_len, size_t slv_tx_buf_len,
                             int intr_alloc_flags)
{
    #ifdef DEBUG_I2C
    return log("espI2C::installDriver()", "Installing I2C driver.",
               {
                 { ESP_OK, "Successfully installed I2C driver." },
                 { ESP_ERR_INVALID_ARG, "Parameter error." },
                 { ESP_FAIL, "Driver installation error." }
               },
               i2c_driver_install, i2c_num, mode, 
               slv_rx_buf_len, slv_tx_buf_len, 
               intr_alloc_flags);
    #else
    return i2c_driver_install(i2c_num, mode,
                              slv_rx_buf_len, slv_tx_buf_len,
                              intr_alloc_flags);
    #endif
}

int I2C::slaveRead(i2c_port_t i2c_num, uint8_t *data,
                   size_t max_size, portBASE_TYPE ticks_to_wait)
{
    #ifdef DEBUG_I2C
    const char* tag = "espI2C::slaveRead()";
    ESP_LOGI(tag, "Attempting to read from buffer.");
    #endif

    int numBytesRead = i2c_slave_read_buffer(i2c_num, data, max_size, ticks_to_wait);

    #ifdef DEBUG_I2C
    if (ESP_FAIL == numBytesRead)
    {
        ESP_LOGE(tag, "Parameter error.");
    }
    else
    {
        ESP_LOGI(tag, "Read %d bytes.", numBytesRead);
    }
    #endif

    return numBytesRead;
}

int I2C::slaveWrite(i2c_port_t i2cNum, uint8_t *data, int size, portBASE_TYPE ticksToWait)
{
    #ifdef DEBUG_I2C
    const char* tag = "espI2C::slaveWrite()";
    ESP_LOGI(tag, "Attempting to write to buffer.");
    #endif

    int numBytesWritten = i2c_slave_write_buffer(i2cNum, data, size, ticksToWait);

    #ifdef DEBUG_I2C
    if (ESP_FAIL == numBytesWritten)
    {
        ESP_LOGE(tag, "Parameter error.");
    }
    else
    {
        ESP_LOGI(tag, "Wrote %d bytes.", numBytesWritten);
    }
    #endif

    return numBytesWritten;
}

}; // End namespace esp
