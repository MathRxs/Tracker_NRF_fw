#include "modem.h"

static void date_time_evt_handler(const struct date_time_evt *evt);
K_SEM_DEFINE(lte_connected, 0, 3);
LOG_MODULE_REGISTER(modem, CONFIG_GNSS_SAMPLE_LOG_LEVEL);
K_THREAD_DEFINE(modem_thread, 0x1000, modem_thread_fn, NULL, NULL, NULL,
		6, 0, 0);
static bool private_checkCertificateProvisioned(void)
{
    int err;
    /*
     * Key management
     */
    nrf_sec_tag_t tag = CONFIG_AWS_IOT_SEC_TAG;
    bool existsPubCert = false;
    bool existsPrivCert = false;
    bool existsCA = false;
    err = modem_key_mgmt_exists(tag, MODEM_KEY_MGMT_CRED_TYPE_PUBLIC_CERT, &existsPubCert);
    err = modem_key_mgmt_exists(tag, MODEM_KEY_MGMT_CRED_TYPE_PRIVATE_CERT, &existsPrivCert);
    err = modem_key_mgmt_exists(tag, MODEM_KEY_MGMT_CRED_TYPE_CA_CHAIN, &existsCA);
    if(err)
    {
        LOG_ERR("Could not interact with secure key unit.");
        return(false);
    }
    else
    {
        if(false == existsPubCert)
        {
            LOG_WRN("It seems no (public) certificate is provisioned.");
        }
        if(false == existsPrivCert)
        {
            LOG_WRN("It seems no (private) certificate is provisioned.");
        }
        if(false == existsCA)
        {
            LOG_WRN("It seems no certificate authority is provisioned.");
        }

        if((existsPubCert == true) && (existsPrivCert == true) && (existsCA == true))
        {
            return(true);
        }
        {
            return(false);
        }
    }
}

int modem_init(void)
{
	while(!private_checkCertificateProvisioned()){
		LOG_INF("Waiting for certificates to be provisioned");
		k_sleep(K_SECONDS(1));
	}
	if (IS_ENABLED(CONFIG_DATE_TIME)) {
		date_time_register_handler(date_time_evt_handler);
	}

	if (lte_lc_init() != 0) {
		LOG_ERR("Failed to initialize LTE link controller");
		return -1;
	}

#if defined(CONFIG_GNSS_SAMPLE_LTE_ON_DEMAND)
	lte_lc_register_handler(lte_lc_event_handler);
#elif !defined(CONFIG_GNSS_SAMPLE_ASSISTANCE_NONE)
	lte_lc_psm_req(true);

	LOG_INF("Connecting to LTE network");

	if (lte_lc_connect_async(lte_handler) != 0) {
		LOG_ERR("Failed to connect to LTE network");
		return -1;
	}

	k_sem_take(&lte_connected, K_FOREVER);
	LOG_INF("Connected to LTE network");

	if (IS_ENABLED(CONFIG_DATE_TIME)) {
		LOG_INF("Waiting for current time");

		/* Wait for an event from the Date Time library. */
		k_sem_take(&time_sem, K_MINUTES(10));

		if (!date_time_is_valid()) {
			LOG_WRN("Failed to get current time, continuing anyway");
		}
	}


#endif

	return 0;
}

static void date_time_evt_handler(const struct date_time_evt *evt)
{
	k_sem_give(&time_sem);
}

static void lte_handler(const struct lte_lc_evt *const evt)
{
	switch (evt->type) {
	case LTE_LC_EVT_NW_REG_STATUS:
		if ((evt->nw_reg_status != LTE_LC_NW_REG_REGISTERED_HOME) &&
		     (evt->nw_reg_status != LTE_LC_NW_REG_REGISTERED_ROAMING)) {
			break;
		}

		printk("Network registration status: %s\n",
			evt->nw_reg_status == LTE_LC_NW_REG_REGISTERED_HOME ?
			"Connected - home network" : "Connected - roaming\n");
		k_sem_give(&lte_connected);
		k_sem_give(&lte_connected);
		k_sem_give(&lte_connected);
		break;
	case LTE_LC_EVT_PSM_UPDATE:
		printk("PSM parameter update: TAU: %d, Active time: %d\n",
			evt->psm_cfg.tau, evt->psm_cfg.active_time);
		break;
	case LTE_LC_EVT_EDRX_UPDATE: {
		char log_buf[60];
		ssize_t len;

		len = snprintf(log_buf, sizeof(log_buf),
			       "eDRX parameter update: eDRX: %f, PTW: %f\n",
			       evt->edrx_cfg.edrx, evt->edrx_cfg.ptw);
		if (len > 0) {
			printk("%s\n", log_buf);
		}
		break;
	}
	case LTE_LC_EVT_RRC_UPDATE:
		printk("RRC mode: %s\n",
			evt->rrc_mode == LTE_LC_RRC_MODE_CONNECTED ?
			"Connected" : "Idle\n");
		break;
	case LTE_LC_EVT_CELL_UPDATE:
		printk("LTE cell changed: Cell ID: %d, Tracking area: %d\n",
		       evt->cell.id, evt->cell.tac);
		break;
	default:
		break;
	}
}

static void modem_thread_fn(void)
{
	int err;

	LOG_INF("Initializing modem");

	err = modem_init();
	if (err) {
		LOG_ERR("Failed to initialize modem, error: %d", err);
		return;
	}

	LOG_INF("Modem initialized");
	k_sleep(K_FOREVER);
}