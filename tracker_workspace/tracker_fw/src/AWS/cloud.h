#ifndef CLOUD_H
#define CLOUD_H

#include <net/aws_iot.h>
#include <zephyr/sys/reboot.h>
#include <date_time.h>
#include <zephyr/dfu/mcuboot.h>
#include <modem/lte_lc.h>
#include <modem/nrf_modem_lib.h>
#include <modem/modem_info.h>
#include <date_time.h>
#include "general_include.h"
#include "gnss.h"
#include "json_lib.h"
#include "modem.h"

#define APP_TOPICS_COUNT CONFIG_AWS_IOT_APP_SUBSCRIPTION_LIST_COUNT

static int shadow_update(bool version_number_include);
static void connect_work_fn(struct k_work *work);
static void shadow_update_work_fn(struct k_work *work);
static void shadow_update_version_work_fn(struct k_work *work);
static void print_received_data(const char *buf, const char *topic, size_t topic_len);
void aws_iot_event_handler(const struct aws_iot_evt *const evt);
static void work_init(void);
static int app_topics_subscribe(void);
static void get_imei(char* imei);
void publish(char *topic, char *payload);
static void aws_cloud_thread_fn(void *arg1, void *arg2, void *arg3);

#endif // CLOUD_H