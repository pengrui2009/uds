/**
 * @file cmn.h
 * @author rui.peng (rui.peng@tusen.ai)
 * @brief 
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdint.h>

#ifndef UDS_CMN_H_
#define UDS_CMN_H_

#define UDS_DL_CAN_DL                   8u

typedef struct {
    uint32_t id;
    uint8_t buf[8];
    uint16_t buf_len;
} can_t;


typedef struct {
    void *qstart;
    void *qend;
    void *qin;
    void *qout;
    uint8_t qentries;   /* buf have value number */
    uint8_t qsize;      /* buf size */
} uds_q_t;

typedef struct {
    uint16_t    id;
    uint16_t    dlc;
    uint8_t     dt[UDS_DL_CAN_DL];
} can_std_frame_t;





#endif /* UDS_CMN_H_ */