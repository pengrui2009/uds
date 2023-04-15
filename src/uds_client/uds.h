/***********************************************************************
 * @file uds.h
 * @brief  
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-12  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022   all right reserved
***********************************************************************/
#ifndef __UDS_H__
#define __UDS_H__

#include "uds_ap.h"
#include "cmn.h"
#include "uds_cfg.def"
#include "virtual_socketcan.h"

#include <stdio.h>
#include <string.h>
#include <linux/can.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief api of uds
 *  include 
 */
UDS_EXT uds_timer_t uds_timer[UDS_TIEMR_NUM];

UDS_EXT uint8_t rx_buffer[UDS_TP_BUF_SZ];
UDS_EXT uint8_t tx_buffer[UDS_TP_BUF_SZ];

UDS_EXT uds_dl_layer_t uds_dl;
UDS_EXT uds_tp_layer_t uds_tp;
UDS_EXT uds_ap_layer_t uds_ap;

/**
 * @brief uds_init
 * 
 * @param channel 
 * @param dev 
 * @return int 
 */
int uds_init(int channel, char *dev);

void uds_process(void);
void uds_timer_tick(void);

void uds_recv_frame(uds_q_t *q, struct can_frame fr);
void uds_send_frame(struct can_frame *fr);

/**
 * @brief uds request diagnostic session
 * 
 * @param func 
 * @param sprsp 
 * @return int 
 */
int uds_req_diagnostic_session(uint8_t func, uint8_t sprsp);

/**
 * @brief Requests a reset of the ECU
 * 
 * @param func 
 * @param sprsp 
 * @return int 
 */
int uds_req_ecu_reset(uint8_t func, uint8_t sprsp);

/**
 * @brief Requests a secure session
 * 
 * @param func 
 * @param sprsp 
 * @param key 
 * @param klen 
 * @return int 
 */
int uds_req_security_access(uint8_t func, uint8_t sprsp, uint8_t *key, uint8_t klen);

/**
 * @brief Enable/Disable certain messages
 * 
 * @param func 
 * @param sprsp 
 * @param type 
 * @return int  
 */
int uds_req_communication_control(uint8_t func, uint8_t sprsp, uint8_t type);

/**
 * @brief Inform server that a tester is present
 * 
 * @param sprsp 
 * @return int  
 */
int uds_req_tester_present(uint8_t sprsp);

#ifdef __cplusplus
}
#endif

#endif // __UDS_H__
