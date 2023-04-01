/**
 * @file uds_dl.h
 * @author rui.peng (rui.peng@tusen.ai)
 * @brief 
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef UDS_DL_H_
#define UDS_DL_H_

#include "uds_q.h"
#include "can_drv.h"

#include <stdint.h>


/** data link layer 
 *  only support the classic can and standard id
 */
#define UDS_DL_CAN_DL                   8u
#define UDS_DL_IN_SZ                    10u

// only support the standard can bus 
typedef enum {
    L_STS_IDLE = 0,
    L_STS_READY,
} uds_dl_sts_t;


typedef struct {
    uds_dl_sts_t        sts;
    can_std_frame_t     fr;
} uds_dl_iostream_t;


// typedef struct {
//     uds_dl_sts_t        sts;
//     can_std_frame_t     buf;
// } uds_dl_outstream_t;


typedef struct {
    uds_dl_iostream_t  in;
    uds_dl_iostream_t  out;
    uds_q_t            in_qf;
    can_std_frame_t    in_frs[UDS_DL_IN_SZ];
} uds_dl_layer_t;


void uds_dl_init(uds_dl_layer_t *pdl);
void uds_dl_process_in(uds_dl_layer_t *pdl);
void uds_dl_process_out(uds_dl_layer_t *pdl);

#endif /* UDS_DL_H_ */