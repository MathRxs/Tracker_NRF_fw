#ifndef MODEM_H_
#define MODEM_H_

// Includes
#include <modem/modem_key_mgmt.h>
#include <modem/nrf_modem_lib.h>
#include "general_include.h"


// Variables
extern struct k_sem lte_connected; 
// Constants
static K_SEM_DEFINE(time_sem, 0, 1);
// Prototypes
int modem_init(void);
static void date_time_evt_handler(const struct date_time_evt *evt);
static void lte_handler(const struct lte_lc_evt *const evt);
static void modem_thread_fn(void);
#endif /* MODEM_H_ */