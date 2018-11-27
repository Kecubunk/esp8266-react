#ifndef ADCFun_h
#define ADCFun_h

#include <functional>
#include <Arduino.h>
#include <FastLed.h>
#include <driver/spi_master.h>

#define DAC_DMA_CHANNEL 1
#define SAMPLING_FREQUENCY 15000

#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18

#define PIN_NUM_DAC_CS   5
#define PIN_NUM_ADC_CS 17

class ADCFun {

public:
  ADCFun();
  ~ADCFun();

  void begin();
  void onSampleTriggered();
  void adcReadComplete(spi_transaction_t *t);

private:

  esp_err_t _ret;
  spi_device_handle_t _adcDevice, _dacDevice;
  spi_bus_config_t _buscfg;
  spi_device_interface_config_t _adcCfg, _dacCfg;
  spi_transaction_t _adcTrans, _dacTrans;
  uint16_t _sampleValue;
};

#endif
