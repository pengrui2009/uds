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

int uds_can_init(int channel);
void uds_can_recv_frame(uds_q_t *q, can_std_frame_t *fr);
void uds_can_send_frame(can_std_frame_t *fr);

#endif