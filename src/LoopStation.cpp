#include <LoopStation.h>

LoopStation::LoopStation(AsyncWebServer *server) : _server(server) { 
  // initialize buffer
  buffer = (int16_t *) malloc(TOTAL_SIZE_IN_BYTES);

  // reset the buffers
  resetBuffers();
}

void LoopStation::resetBuffers() {
  // clear buffer memory
  memset(buffer, 0, TOTAL_SIZE_IN_BYTES);

  // reset front and back buffers
  frontBuffer = buffer;
  backBuffer = buffer + CHUNK_SIZE_IN_SAMPLES;

  // initialize current sample to zero
  sampleOffset = 0;
}

void LoopStation::begin() {
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
    // Please check its source code and implement error recovery when developing
    // production applications.
    sdmmc_card_t* card;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                "If you want the card to be formatted, set format_if_mount_failed = true.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);
    
    initialised = true;
}

void LoopStation::onSampleTriggered() {  
  // trigger the reading of a sample via SPI
  onSampleComplete();
}

void LoopStation::onSampleComplete() {    
    // output current sample
    currentSample = frontBuffer[sampleOffset];

    // make this configurable
    // raizing the sample to the power of .8 for now
    currentSample = currentSample / 2;

    // output current sample here....
    // spiTransaction(currentSample)

    // grab next sample from SPI result
    nextSample = analogRead(36) - 2047;

    // detect clipping by looking for overflow, light an LED or something!
    if ((currentSample > 0) && (nextSample > 2047 - currentSample)) {
        // would overflow - clip!
        currentSample = 2047;
    } else if ((currentSample < 0) && (nextSample < -2047 - currentSample)) {
        // would underflow - clip!
        currentSample = -2047;
    } else {
        currentSample += nextSample;
    }   

    frontBuffer[sampleOffset] = currentSample;
    
    // increment sample offset
    if (++sampleOffset >= CHUNK_SIZE_IN_SAMPLES) {
        Serial.println("SWAPPING BUFFERS...");
        
        // reset offset
        sampleOffset = 0;

        // swap buffers
        if (frontBuffer == buffer){
          frontBuffer = backBuffer;
          backBuffer = buffer;
        } else{
          backBuffer = frontBuffer;
          frontBuffer = buffer;
        }

        // trigger write to SD...
    }
}

void LoopStation::loop() {

}
