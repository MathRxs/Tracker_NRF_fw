#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_


#include <zephyr/drivers/sensor.h>
#include "general_include.h"
#define ACCEL_NODE DT_ALIAS(accel0)
extern struct k_sem accelerometer_data_ready;

bool init_accelerometer(void);
void get_accelerometer_data(double *x_accel, double *y_accel, double *z_accel);
void accelerometer_init_thread(void);
void enable_and_set_interrupt(void);
void accelerometer_trigger_handler(const struct device *dev, const struct sensor_trigger *trig);
#endif // ACCELEROMETER_H_
