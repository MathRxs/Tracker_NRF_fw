#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <modem/modem_info.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/printk.h>

#include <modem/lte_lc.h>
#include <modem/modem_key_mgmt.h>
#include <modem/nrf_modem_lib.h>

LOG_MODULE_DECLARE(app, LOG_LEVEL_DBG);

static int cmd_provision_certs(const struct shell *shell, size_t argc, char **argv)
{
    int err = 0;
    char *mem_ptr;
    mem_ptr = k_malloc(2048);
    if(mem_ptr == NULL)
    {
        shell_print(shell, "Failed to allocate heap memory for key.");
        return(0);
    }
    memset(mem_ptr, 0, 2048);

    /*
     * Write to the key buffer
     */
    for(size_t cnt = 1; cnt < argc; cnt++) {
        strncat(mem_ptr, argv[cnt], 2048);
        strncat(mem_ptr, "\n", 2048);
    }

    size_t lengthOfData = strlen(mem_ptr);

    shell_print(shell, "Received a certificate, Length (ptr): %d bytes.", lengthOfData);

    nrf_sec_tag_t tag = CONFIG_AWS_IOT_SEC_TAG;
    switch(argv[0][0])
    {
        case 'a': // Authority
            err = modem_key_mgmt_delete(tag, MODEM_KEY_MGMT_CRED_TYPE_CA_CHAIN);
            err = modem_key_mgmt_write(tag, MODEM_KEY_MGMT_CRED_TYPE_CA_CHAIN, mem_ptr, lengthOfData);
            if(err)
            {
                shell_print(shell, "Could not write Certificate Authority.");
            }
            else
            {
                shell_print(shell, "Writing a new Certificate authority");
            }

            break;

        case 'c': // public cert

            err = modem_key_mgmt_delete(tag, MODEM_KEY_MGMT_CRED_TYPE_PUBLIC_CERT);
            err = modem_key_mgmt_write(tag, MODEM_KEY_MGMT_CRED_TYPE_PUBLIC_CERT, mem_ptr, lengthOfData);
            if(err)
            {
                shell_print(shell, "Could not write public certificate.");
            }
            else
            {
                shell_print(shell, "Writing a new public certificate");
            }
            break;

        case 'r': // private cert

            err = modem_key_mgmt_delete(tag, MODEM_KEY_MGMT_CRED_TYPE_PRIVATE_CERT);
            err = modem_key_mgmt_write(tag, MODEM_KEY_MGMT_CRED_TYPE_PRIVATE_CERT, mem_ptr, lengthOfData);
            if(err)
            {
                shell_print(shell, "Could not write private certificate.");
            }
            else
            {
                shell_print(shell, "Success writing Private Cert");
            }
            break;

        case 'd':

            lte_lc_offline();
            err = modem_key_mgmt_delete(tag, MODEM_KEY_MGMT_CRED_TYPE_PRIVATE_CERT);
            err = modem_key_mgmt_delete(tag, MODEM_KEY_MGMT_CRED_TYPE_PUBLIC_CERT);
            err = modem_key_mgmt_delete(tag, MODEM_KEY_MGMT_CRED_TYPE_CA_CHAIN);
            err = modem_key_mgmt_delete(tag, MODEM_KEY_MGMT_CRED_TYPE_CA_CHAIN);

            //err = at_cmd_write("AT%CMNG=3,0,6", NULL, 0, NULL);

            // err = at_cmd_write("AT%CMNG=1", NULL, 0, NULL);

            break;

        case 's':
            lte_lc_offline();
            shell_print(shell, "LTE link stopped.");
            break;

        default:
            break;
    }

    // Clear from memory for security.
    memset(mem_ptr, 0, 2048);
    k_free(mem_ptr);
    return(0);
}


SHELL_STATIC_SUBCMD_SET_CREATE(modem_cmd,
    SHELL_CMD(c, NULL, "Loads a public cert into the KMU.", cmd_provision_certs),
    SHELL_CMD(r, NULL, "Loads a private cert into the KMU.", cmd_provision_certs),
    SHELL_CMD(a, NULL, "Loads a cert authority into the KMU.", cmd_provision_certs),
    SHELL_CMD(d, NULL, "Clears all keys in the KMU.", cmd_provision_certs),
    SHELL_CMD(s, NULL, "Stops LTE Link.", cmd_provision_certs),
    SHELL_SUBCMD_SET_END
    );



/* Creating root (level 0) command "demo" without a handler */
SHELL_CMD_REGISTER(modem, &modem_cmd, "Modem Commands", NULL);
