zephyr_include_directories(.)
zephyr_library_sources_ifdef(CONFIG_GNSS_SAMPLE_ASSISTANCE_NRF_CLOUD assistance.c)
zephyr_library_sources_ifdef(CONFIG_GNSS_SAMPLE_ASSISTANCE_SUPL assistance_supl.c)
zephyr_library_sources_ifdef(CONFIG_GNSS_SAMPLE_ASSISTANCE_MINIMAL assistance_minimal.c)
zephyr_library_sources_ifdef(CONFIG_GNSS_SAMPLE_ASSISTANCE_MINIMAL mcc_location_table.c)
