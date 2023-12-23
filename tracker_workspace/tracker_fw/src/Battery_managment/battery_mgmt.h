#ifndef BAT_MGMT_H_
#define BAT_MGMT_H_
#include "stdint.h"
#include "stdbool.h"
int get_battery_voltage(uint16_t *battery_voltage);
bool init_adc();
#endif /* BAT_MGMT_H_ */