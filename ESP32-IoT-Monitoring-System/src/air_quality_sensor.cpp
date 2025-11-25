#include "air_quality_sensor.h"

ModbusMaster no2Node;
ModbusMaster aqiNode;

void air_quality_task(void * pvParameter){
    no2Node.begin(NO2_SLAVE_ID, SERIAL_RS485);
    aqiNode.begin(AQI_SLAVE_ID, SERIAL_RS485);
    
    uint8_t result;
    uint16_t no2 = 0;
    uint16_t pm25 = 0;
    uint16_t pm10 = 0;
    char json[64];
    for(;;){
        EventBits_t uxBits = xEventGroupWaitBits(
            xSensorEventGroup,
            SENSOR_TRIGGER_READ_ALL_BIT,
            pdTRUE,
            pdFALSE,
            portMAX_DELAY
        );
        // auto start = millis();
        result = no2Node.readHoldingRegisters(REG_ADDR_NO2_CONTENT, 1);
        if( result == no2Node.ku8MBSuccess) {
            no2 = no2Node.getResponseBuffer(0);
            // Serial.printf("Error: [AIR] NO2 Content: %u ppm\n", no2);
        } else {
            Serial.printf("Error: [AIR] Read NO2 error: %u\n", result);
            no2 = 0;
        }

        vTaskDelay(200 / portTICK_PERIOD_MS); // Short delay between requests

        result = aqiNode.readHoldingRegisters(REG_ADDR_PM25_CONTENT, 6);
        if (result == aqiNode.ku8MBSuccess) {
            pm25 = aqiNode.getResponseBuffer(0);
            pm10 = aqiNode.getResponseBuffer(5);
            // Serial.printf("[AQI] PM2.5 Content: %u μg/m³\n", pm25);
            // Serial.printf("[AQI] PM10 Content: %u μg/m³\n", pm10);
        } else {
            Serial.printf("Error: [AIR] Read PM error: %u\n", result);
            pm25 = 0;
            pm10 = 0;
        }
        snprintf(json, sizeof(json), "{\"no2\":%u,\"pm25\":%u,\"pm10\":%u}", no2, pm25, pm10);
        if (xQueueSend(xSensorDataQueue, json, 0) != pdPASS) {
            Serial.println("Error: [AIR] Failed to send telemetry data to queue");
        }
        // Serial.printf("[AQI] Done (%dms)\n", millis() - start);
    }
}
