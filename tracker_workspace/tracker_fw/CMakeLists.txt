#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(gnss_sample)

zephyr_library_sources(src/main.c)

add_subdirectory(src/gnss_assistance)
add_subdirectory(src/gnss)
add_subdirectory(src/modem)
add_subdirectory(src/general_lib)
add_subdirectory(src/json)
add_subdirectory(src/AWS)
add_subdirectory(src/accelerometer)
add_subdirectory(src/timer)
add_subdirectory(src/shell)
add_subdirectory(src/Battery_managment)