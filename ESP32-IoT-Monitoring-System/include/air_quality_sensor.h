#ifndef AIR_QUALITY_SENSOR_H
#define AIR_QUALITY_SENSOR_H

#include "global.h"
#include <ModbusMaster.h>

#define NO2_SLAVE_ID            35
#define REG_ADDR_NO2_CONTENT    0x0006

#define AQI_SLAVE_ID            36
#define REG_ADDR_PM25_CONTENT   0x0004
#define REG_ADDR_PM10_CONTENT   0x0009
void air_quality_task(void * pvParameter);

#endif