/**
 * @file uds_q.h
 * @author rui.peng (rui.peng@tusen.ai)
 * @brief uds queue contain can frame
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdint.h>
#include "cmn.h"

#ifndef UDS_Q_H_
#define UDS_Q_H_

typedef enum {
    UDS_Q_OK = 0,
    UDS_Q_FULL,
    UDS_Q_EMPTY,
} uds_q_rslt;

/**
 * @brief get a elem from queue
 * 
 * @param q 
 * @param elem 
 * @param sz 
 * @return uds_q_rslt 
 */
uds_q_rslt uds_qdequeue(uds_q_t *q, void *elem, uint16_t sz);

/**
 * @brief put a elem in queue
 * 
 * @param q 
 * @param elem          pointer of elem
 * @param sz            sizeof elem type
 * @return uds_q_rslt 
 */
uds_q_rslt uds_qenqueue(uds_q_t *q, void *elem, uint16_t sz);

/**
 * @brief clear a queue
 * 
 * @param q 
 * @return uds_q_rslt 
 */
uds_q_rslt uds_qflush(uds_q_t *q);

#endif /* UDS_Q_H_ */