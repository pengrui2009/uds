/**
 * @file uds_can.h
 * @author rui.peng (pengrui_2009@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-25
 * 
 * @copyright rui.peng Copyright (c) 2022
 * 
 */

#ifndef UDS_CAN_H_
#define UDS_CAN_H_

#include "uds.h"

void uds_recv_frame(uds_q_t *q, can_std_frame_t fr);
void uds_send_frame(can_std_frame_t *fr);

#endif