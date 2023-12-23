#include "accelerometer.h"
const struct device *accel;
LOG_MODULE_REGISTER(accelerometer, LOG_LEVEL_INF);
K_THREAD_DEFINE(accelerometer_init_thread_id, 2048, accelerometer_init_thread, NULL, NULL, NULL, 0, 0, 0);
K_SEM_DEFINE(motion_detect, 0, 1);

uint16_t counter = 0;
#define ACCEL_NODE DT_ALIAS(accel0)
bool init_accelerometer(void)
{
	accel = DEVICE_DT_GET(ACCEL_NODE);
	if (!accel) {
		printk("Error getting accelerometer device binding\r\n");

		return false;
	}


	return true;
}

void get_accelerometer_data(double *x_accel, double *y_accel, double *z_accel)
{
	sensor_sample_fetch(accel);

	struct sensor_value value_x;
	sensor_channel_get(accel, SENSOR_CHAN_ACCEL_X, &value_x);
	*x_accel = sensor_value_to_double(&value_x);

	struct sensor_value value_y;
	sensor_channel_get(accel, SENSOR_CHAN_ACCEL_Y, &value_y);
	*y_accel = sensor_value_to_double(&value_y);

	struct sensor_value value_z;
	sensor_channel_get(accel, SENSOR_CHAN_ACCEL_Z, &value_z);
	*z_accel = sensor_value_to_double(&value_z);
}
void accelerometer_trigger_handler(const struct device *dev, const struct sensor_trigger *trig)
{
	printk("\nTriggered %d\n", counter++);
	printk("%d\n", trig->type);

	if(waked_up){
		timer_restart(TIMER_TIMEOUT_SEC);
	}
	else {
		send_vibration_detect();
		waked_up = 1;
		k_sem_give(&motion_detect);
		timer_start(TIMER_TIMEOUT_SEC);
	}
	

}
void enable_and_set_interrupt(void)
{
	int err = 0;
	struct sensor_trigger trig = { .chan = SENSOR_CHAN_ACCEL_XYZ};
	trig.type = SENSOR_TRIG_DELTA;
	struct sensor_value val = {
		.val1 = 7,
		.val2 = 500000,
	};
	struct sensor_value val1 = {
		.val1 = 1,
	};
	err = sensor_attr_set(accel, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_SLOPE_TH, &val);

	if(err){
		printk("Error setting SENSOR_ATTR_SLOPE_TH, error code: %d\n", err);
	}
	err = sensor_attr_set(accel, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_SLOPE_DUR, &val1);
	if(err){
		printk("Error setting SENSOR_ATTR_SLOPE_DUR, error code: %d\n", err);
	}
	err = sensor_trigger_set(accel, &trig, accelerometer_trigger_handler);
	if(err){
		printk("Error setting trigger, error code: %d\n", err);
	}
}
void accelerometer_init_thread(void)
{
	printk("Accelerometer init thread\n");
	if (!init_accelerometer()) {
		printk("Error initializing accelerometer\n");
		return;
	}
	
	enable_and_set_interrupt();

	k_sleep(K_FOREVER);
}

