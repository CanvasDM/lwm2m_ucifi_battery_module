/**
 * @file lcz_lwm2m_battery.h
 * @brief
 *
 * Copyright (c) 2022 Laird Connectivity
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __LCZ_LWM2M_BATTERY_H__
#define __LCZ_LWM2M_BATTERY_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/* Global Function Prototypes                                                                     */
/**************************************************************************************************/
/**
 * @brief Set pressure using gateway object index to obtain base instance.
 * Object is created when needed and deleted if gateway object is deleted or instance is deleted.
 *
 * @param idx into gateway device table
 * @param offset of instance (obtained from advertisement event)
 * @param voltage in volts
 * @param percentage of battery remaining
 * @return int negative error, 0 on success
 */
int lcz_lwm2m_managed_battery_set(int idx, uint8_t offset, double voltage, uint8_t percentage);

/**
 * @brief Set pressure. Requires instance to already be created.
 *
 * @param instance ID
 * @param voltage in volts
 * @param percentage of battery remaining
 * @return int negative error, 0 on success
 */
int lcz_lwm2m_battery_set(uint16_t instance, double voltage, uint8_t percentage);

/**
 * @brief Create instance
 *
 * @param instance ID
 * @return int negative error, 0 on success
 */
int lcz_lwm2m_battery_create(uint16_t instance);

#ifdef __cplusplus
}
#endif

#endif /* __LCZ_LWM2M_BATTERY_H__ */
