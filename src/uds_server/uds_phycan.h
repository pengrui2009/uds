/**
 * @file uds_phycan.h
 * @author rui.peng (pengrui_2009@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-25
 * 
 * @copyright rui.peng Copyright (c) 2022
 * 
 */

#ifndef UDS_PHY_CAN_H_
#define UDS_PHY_CAN_H_

#include "uds.h"

#define READ_WAIT_INFINITE      (unsigned long)(-1)
#define WRITE_WAIT_INFINITE     (unsigned long)(-1)

typedef struct {
    uint32_t id;
    uint8_t buf[8];
    uint16_t buf_len;
} can_t;

/**
 * @brief can init
 * 
 * @param channel 
 * @return int 
 */
int can_init(int channel);

/**
 * @brief can receive frame
 * 
 * @param q 
 * @param fr 
 * @return uint8_t 
 */
uint8_t can_rx(can_std_frame_t *fr);

/**
 * @brief can tx frame
 * 
 * @param fr 
 * @return uint8_t 
 */
uint8_t can_tx(can_std_frame_t *fr);

#endif