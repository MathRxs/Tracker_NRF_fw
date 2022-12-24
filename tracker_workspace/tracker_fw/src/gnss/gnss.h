#ifndef GNSS_H_
#define GNSS_H_

// Includes
#include "general_include.h"
#include "modem.h"
// Variables
extern struct nrf_modem_gnss_pvt_data_frame last_pvt;

// Constants
#define GNSS_THREAD_STACK_SIZE 2048
#define PI 3.14159265358979323846
#define EARTH_RADIUS_METERS (6371.0 * 1000.0)

K_MSGQ_DEFINE(nmea_queue, sizeof(struct nrf_modem_gnss_nmea_data_frame *), 10, 4);
static K_SEM_DEFINE(pvt_data_sem, 0, 1);
static struct k_poll_event events[2] = {
	K_POLL_EVENT_STATIC_INITIALIZER(K_POLL_TYPE_SEM_AVAILABLE,
					K_POLL_MODE_NOTIFY_ONLY,
					&pvt_data_sem, 0),
	K_POLL_EVENT_STATIC_INITIALIZER(K_POLL_TYPE_MSGQ_DATA_AVAILABLE,
					K_POLL_MODE_NOTIFY_ONLY,
					&nmea_queue, 0),
};
// Prototypes
int gnss_init_and_start(void);
int gnss_init_and_start(void);
static void gnss_event_handler(int event);
static void gnss_thread_fn(void *arg1, void *arg2, void *arg3);
static void lte_lc_event_handler(const struct lte_lc_evt *const evt);
void lte_connect(void);
void lte_disconnect(void);
static void agps_data_get_work_fn(struct k_work *item);
static void ttff_test_got_fix_work_fn(struct k_work *item);
static int ttff_test_force_cold_start(void);
static void ttff_test_prepare_work_fn(struct k_work *item);
static void ttff_test_start_work_fn(struct k_work *item);
static int gnss_module_init(void);
static bool output_paused(void);
static double distance_calculate(double lat1, double lon1,double lat2, double lon2);
static void print_distance_from_reference(struct nrf_modem_gnss_pvt_data_frame *pvt_data);
static void print_satellite_stats(struct nrf_modem_gnss_pvt_data_frame *pvt_data);
static void print_fix_data(struct nrf_modem_gnss_pvt_data_frame *pvt_data);


#endif /* GNSS_H_ */