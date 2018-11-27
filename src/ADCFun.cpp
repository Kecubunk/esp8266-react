#include <ADCFun.h>

static ADCFun * instance = NULL;

void adcCallback(spi_transaction_t *t) {
    if (instance != NULL){
      instance->adcReadComplete(t);
    }
}

ADCFun::ADCFun() {
  instance = this;

  // configure SPI bus
  memset(&_buscfg, 0, sizeof(_buscfg));
  _buscfg.quadwp_io_num=-1;
  _buscfg.quadhd_io_num=-1;
  _buscfg.miso_io_num=PIN_NUM_MISO;
  _buscfg.mosi_io_num=PIN_NUM_MOSI;
  _buscfg.sclk_io_num=PIN_NUM_CLK;

  // configure adc device
  memset(&_adcCfg, 0, sizeof(_adcCfg));
  _adcCfg.clock_speed_hz=1*1000*1000;
  _adcCfg.mode=1;
  _adcCfg.spics_io_num=PIN_NUM_ADC_CS;  
  _adcCfg.queue_size=1;
  _adcCfg.pre_cb=adcCallback;

  // configure ADC transaction
  memset(&_adcTrans, 0, sizeof(_adcTrans));
  _adcTrans.length=16;
  _adcTrans.flags=SPI_TRANS_USE_RXDATA;

  // configure dac device
  memset(&_dacCfg, 0, sizeof(_dacCfg));
  _dacCfg.clock_speed_hz=20*1000*1000;
  _dacCfg.mode=0;
  _dacCfg.spics_io_num=PIN_NUM_DAC_CS;  
  _dacCfg.queue_size=1;

  // configure DAC transaction
  memset(&_dacTrans, 0, sizeof(_dacTrans));
  _dacTrans.length=16;
  _dacTrans.flags=SPI_TRANS_USE_TXDATA;
}

ADCFun::~ADCFun() {}

void ADCFun::begin() {
  //Initialize the SPI bus
  _ret= spi_bus_initialize(VSPI_HOST, &_buscfg, 2);
  ESP_ERROR_CHECK(_ret);

  //Attach the adc device to the SPI bus
  _ret = spi_bus_add_device(VSPI_HOST, &_adcCfg, &_adcDevice);
  ESP_ERROR_CHECK(_ret);

  //Attach the dac device to the SPI bus
  _ret = spi_bus_add_device(VSPI_HOST, &_dacCfg, &_dacDevice);
  ESP_ERROR_CHECK(_ret);
}

void ADCFun::adcReadComplete(spi_transaction_t *t) {
    _sampleValue = ((uint16_t) t->rx_data[0] & 0x0f) << 8 | t->rx_data[1];

    // configure the transasction
    _dacTrans.tx_data[0] = 0b00110000 | (_sampleValue & 0xF00) >> 8;
    _dacTrans.tx_data[1] = _sampleValue & 0xFF;

    // queue the transaction - fire and forget fashion (portMAX_DELAY)
    _ret= spi_device_queue_trans(_dacDevice, &_dacTrans, portMAX_DELAY);
    ESP_ERROR_CHECK(_ret);
}

void ADCFun::onSampleTriggered() {
  _ret = spi_device_queue_trans(_adcDevice, &_adcTrans, portMAX_DELAY);
  ESP_ERROR_CHECK(_ret);
}
