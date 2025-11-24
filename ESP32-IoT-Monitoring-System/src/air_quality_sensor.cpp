#include "air_quality_sensor.h"

void air_quality_task(void * pvParameter){
    float no2 = 0.0f;
    float pm10 = 0.0f;
    float pm25 = 0.0f;
    char json[64];

    for(;;) {
        vTaskDelay(200 / portTICK_PERIOD_MS);

        // Wait for trigger to read sensor
        EventBits_t uxBits = xEventGroupWaitBits(
        xSensorEventGroup,
        SENSOR_TRIGGER_READ_ALL_BIT,
        pdTRUE,
        pdFALSE,
        portMAX_DELAY
        );
        
        no2         = (rand() % 100) / 1.0;         // random 0-99
        pm10        = (rand() % 200) / 1.0;         // random 0-199
        pm25        = (rand() % 150) / 1.0;         // random 0-149

        snprintf(json, sizeof(json), "{\"no2\":%.2f,\"pm10\":%.2f,\"pm25\":%.2f}", no2, pm10, pm25);
        if (xQueueSend(xSensorDataQueue, json, 0) != pdPASS) {
            Serial.println("Error: [AIR] Failed to send telemetry data to queue");
        }
        // Serial.printf("Info: [AIR] Done (%dms)\n", millis() - start);
    }
}
