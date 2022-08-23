/**
 * @file lcz_lwm2m_battery_get_level.c
 * @brief
 *
 * Copyright (c) 2022 Laird Connectivity
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**************************************************************************************************/
/* Includes                                                                                       */
/**************************************************************************************************/
#include <zephyr.h>

/**************************************************************************************************/
/* Global Function Definitions                                                                    */
/**************************************************************************************************/
uint8_t lcz_lwm2m_battery_get_level_bt610(double voltage)
{
	uint8_t level = 0;

	if (voltage >= 3.376) {
		level = 100;
	} else if (3.351 <= voltage && voltage <= 3.375) {
		level = 90;
	} else if (3.326 <= voltage && voltage <= 3.350) {
		level = 80;
	} else if (3.301 <= voltage && voltage <= 3.325) {
		level = 70;
	} else if (3.251 <= voltage && voltage <= 3.300) {
		level = 60;
	} else if (3.201 <= voltage && voltage <= 3.250) {
		level = 50;
	} else if (3.151 <= voltage && voltage <= 3.200) {
		level = 40;
	} else if (3.101 <= voltage && voltage <= 3.150) {
		level = 30;
	} else if (3.001 <= voltage && voltage <= 3.100) {
		level = 20;
	} else if (2.501 <= voltage && voltage <= 3.000) {
		level = 10;
	} else if (voltage <= 2.500) {
		level = 0;
	}

	return level;
}

uint8_t lcz_lwm2m_battery_get_level_bt510(double voltage)
{
	uint8_t level = 0;

	if (voltage >= 3.176) {
		level = 100;
	} else if (3.151 <= voltage && voltage <= 3.175) {
		level = 90;
	} else if (3.126 <= voltage && voltage <= 3.150) {
		level = 80;
	} else if (3.101 <= voltage && voltage <= 3.125) {
		level = 70;
	} else if (3.051 <= voltage && voltage <= 3.100) {
		level = 60;
	} else if (3.001 <= voltage && voltage <= 3.050) {
		level = 50;
	} else if (2.951 <= voltage && voltage <= 3.000) {
		level = 40;
	} else if (2.901 <= voltage && voltage <= 2.950) {
		level = 30;
	} else if (2.851 <= voltage && voltage <= 2.900) {
		level = 20;
	} else if (2.501 <= voltage && voltage <= 2.850) {
		level = 10;
	} else if (voltage <= 2.500) {
		level = 0;
	}

	return level;
}
