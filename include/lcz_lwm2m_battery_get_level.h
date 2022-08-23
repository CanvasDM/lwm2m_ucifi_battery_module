/**
 * @file lcz_lwm2m_battery_get_level.h
 * @brief
 *
 * Copyright (c) 2022 Laird Connectivity
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __LCZ_LWM2M_BATTERY_GET_LEVEL_H__
#define __LCZ_LWM2M_BATTERY_GET_LEVEL_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/* Global Function Prototypes                                                                     */
/**************************************************************************************************/
/**
 * @brief Get the battery level in percent
 *
 * @param voltage in volts
 * @return uint8_t 0-100%
 */
uint8_t lcz_lwm2m_battery_get_level_bt610(double voltage);

/**
 * @brief Get the battery level in percent
 *
 * @param voltage in volts
 * @return uint8_t 0-100%
 */
uint8_t lcz_lwm2m_battery_get_level_bt510(double voltage);

#ifdef __cplusplus
}
#endif

#endif /* __LCZ_LWM2M_BATTERY_GET_LEVEL_H__ */
