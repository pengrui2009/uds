/**
 * @file uds_q.h
 * @author rui.peng (pengrui2009@gmail.com)
 * @brief uds queue contain can frame
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef UDS_Q_H_
#define UDS_Q_H_

#include "cmn.h"

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UDS_Q_OK = 0,
    UDS_Q_FULL,
    UDS_Q_EMPTY,
} uds_q_rslt;

/* uds queue contain can frame */
typedef struct {
    void *qstart;
    void *qend;
    void *qin;
    void *qout;
    uint8_t qentries;   /* buf have value number */
    uint8_t qsize;      /* buf size */
} uds_q_t;


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


#ifdef __cplusplus
}
#endif

#endif /* UDS_Q_H_ */