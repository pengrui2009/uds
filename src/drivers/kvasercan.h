/**
 * @file kvasercan.h
 * @author rui.peng (pengrui_2009@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-25
 * 
 * @copyright rui.peng Copyright (c) 2022
 * 
 */

#ifndef KVASER_CAN_H_
#define KVASER_CAN_H_

#include <stdint.h>
#include <linux/can.h>

#ifdef __cplusplus
extern "C" {
#endif

// #define UDS_DL_CAN_DL 8u

#define READ_WAIT_INFINITE      (unsigned long)(-1)
#define WRITE_WAIT_INFINITE     (unsigned long)(-1)

// typedef struct {
//     uint16_t    id;
//     uint16_t    dlc;
//     uint8_t     dt[UDS_DL_CAN_DL];
// } can_std_frame_t;

/**
 * @brief can init
 * 
 * @param channel 
 * @param dev
 * @return int 
 */
int can_init(int channel, char *dev);

/**
 * @brief can receive frame
 * 
 * @param q 
 * @param fr 
 * @return uint8_t 
 */
uint8_t can_rx(struct can_frame *frame_ptr);

/**
 * @brief can tx frame
 * 
 * @param fr 
 * @return uint8_t 
 */
uint8_t can_tx(const struct can_frame *frame_ptr);

#ifdef __cplusplus
}
#endif

#endif /* KVASERCAN_H_ */