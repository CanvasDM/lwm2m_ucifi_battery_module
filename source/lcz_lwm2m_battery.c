/**
 * @file lcz_lwm2m_battery.c
 * @brief
 *
 * Copyright (c) 2022 Laird Connectivity
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <logging/log.h>
LOG_MODULE_REGISTER(lwm2m_battery, CONFIG_LCZ_LWM2M_BATTERY_LOG_LEVEL);

/**************************************************************************************************/
/* Includes                                                                                       */
/**************************************************************************************************/
#include <zephyr.h>
#include <init.h>
#include <lcz_lwm2m.h>

#include "ucifi_battery.h"

#include "lcz_snprintk.h"
#include "lcz_lwm2m.h"
#include "lcz_lwm2m_util.h"
#include "lcz_lwm2m_battery.h"

/**************************************************************************************************/
/* Local Constant, Macro and Type Definitions                                                     */
/**************************************************************************************************/
#define MAX_INSTANCES CONFIG_LCZ_LWM2M_UCIFI_BATTERY_INSTANCE_COUNT

/**************************************************************************************************/
/* Local Data Definitions                                                                         */
/**************************************************************************************************/
static struct lwm2m_obj_agent battery_agent;

struct battery_voltage_data {
	double volts;
	uint16_t instance;
	bool in_use;
};

static struct battery_voltage_data battery_voltage_data[MAX_INSTANCES];

static bool max_batteries;

/**************************************************************************************************/
/* Local Function Prototypes                                                                      */
/**************************************************************************************************/
static int create_battery_sensor(int idx, uint16_t type, uint16_t instance, void *context);
static int remove_battery_callback(uint16_t instance);

/**************************************************************************************************/
/* SYS INIT                                                                                       */
/**************************************************************************************************/
static int lcz_lwm2m_battery_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	battery_agent.type = UCIFI_OBJECT_BATTERY_ID;
	battery_agent.create = create_battery_sensor;
	battery_agent.context = battery_voltage_data;
	lcz_lwm2m_util_register_agent(&battery_agent);

	return 0;
}

SYS_INIT(lcz_lwm2m_battery_init, APPLICATION, LCZ_LWM2M_UTIL_USER_INIT_PRIORITY);

/**************************************************************************************************/
/* Global Function Definitions                                                                    */
/**************************************************************************************************/
#if defined(CONFIG_LCZ_LWM2M_UTIL_MANAGE_OBJ_INST)
int lcz_lwm2m_managed_battery_set(int idx, uint8_t offset, double voltage, uint8_t percentage)
{
	uint16_t type = UCIFI_OBJECT_BATTERY_ID;
	int instance;
	int r;

	instance = lcz_lwm2m_util_manage_obj_instance(type, idx, offset);
	if (instance < 0) {
		return instance;
	}

	r = lcz_lwm2m_battery_set(instance, voltage, percentage);
	lcz_lwm2m_util_manage_obj_deletion(r, type, idx, instance);
	return r;
}
#endif

int lcz_lwm2m_battery_set(uint16_t instance, double voltage, uint8_t percentage)
{
	uint16_t type = UCIFI_OBJECT_BATTERY_ID;
	char path[LWM2M_MAX_PATH_STR_LEN];
	int r;

	LCZ_SNPRINTK(path, "%u/%u/%u", type, instance, UCIFI_BATTERY_LEVEL_RID);
	r = lwm2m_engine_set_u8(path, percentage);
	if (r < 0) {
		return r;
	}

	LCZ_SNPRINTK(path, "%u/%u/%u", type, instance, UCIFI_BATTERY_VOLTAGE_RID);
	r = lwm2m_engine_set_float(path, &voltage);

	return r;
}

int lcz_lwm2m_battery_create(uint16_t instance)
{
	return lcz_lwm2m_util_create_obj_inst(UCIFI_OBJECT_BATTERY_ID, instance);
}

/**************************************************************************************************/
/* Local Function Definitions                                                                     */
/**************************************************************************************************/
static int create_battery_sensor(int idx, uint16_t type, uint16_t instance, void *context)
{
	ARG_UNUSED(idx);
	ARG_UNUSED(context);
	char path[LWM2M_MAX_PATH_STR_LEN];
	struct battery_voltage_data *bvd;
	int r;
	int i;

	if (max_batteries) {
		return -ENOMEM;
	}

	do {
		/* If object instance is deleted, then the resource data must be marked as free. */
		r = lwm2m_engine_register_delete_callback(type, remove_battery_callback);
		if (r < 0) {
			break;
		}

		/* Set the battery voltage resource data.
		 * It is an optional resource and doesn't have storage.
		 */
		LCZ_SNPRINTK(path, "%u/%u/%u", type, instance, UCIFI_BATTERY_VOLTAGE_RID);
		bvd = NULL;
		for (i = 0; i < MAX_INSTANCES; i++) {
			if (!battery_voltage_data[i].in_use) {
				bvd = &battery_voltage_data[i];
				break;
			}
		}
		if (bvd == NULL) {
			LOG_WRN("All battery voltage instances used");
			max_batteries = true;
			r = -ENOMEM;
			break;
		}

		r = lwm2m_engine_set_res_data(path, &bvd->volts, sizeof(bvd->volts), 0);
		if (r < 0) {
			break;
		}

		bvd->instance = instance;
		bvd->in_use = true;

	} while (0);

	return r;
}

static int remove_battery_callback(uint16_t instance)
{
	int i;

	for (i = 0; i < MAX_INSTANCES; i++) {
		if (instance == battery_voltage_data[i].instance) {
			battery_voltage_data[i].in_use = false;
			max_batteries = false;
		}
	}

	return 0;
}
