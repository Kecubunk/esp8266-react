#ifndef LoopStation_h
#define LoopStation_h

#include <ESPAsyncWebServer.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"

#include <FastLED.h>

// define for chunk size
#define CHUNK_SIZE_IN_SAMPLES 5
#define CHUNK_SIZE_IN_BYTES CHUNK_SIZE_IN_SAMPLES * sizeof(int16_t)

// we have two chunks in the buffer
#define TOTAL_SIZE_IN_BYTES CHUNK_SIZE_IN_BYTES * 2

#define TIMER_DELAY 1000000

class LoopStation {

  public:

    LoopStation(AsyncWebServer *server);

    void begin();
    void loop();

    void onSampleTriggered();
    void onSampleComplete();

  private:

    AsyncWebServer* _server;

    // main buffer - allocated in constructor
    int16_t* buffer;

    // back and front buffers for swapping - initialized in constructor
    int16_t* frontBuffer;
    int16_t* backBuffer;

    bool initialised = false;

    // previous and next samples
    uint8_t sampleOffset;

    int16_t currentSample;
    int16_t nextSample;

    void resetBuffers();
};

#endif // end LoopStation_h
