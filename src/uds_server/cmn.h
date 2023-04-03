/**
 * @file cmn.h
 * @author rui.peng (pengrui2009@gmail.com)
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



#ifdef UDS_GLOABL
#define UDS_EXT
#else
#define UDS_EXT extern
#endif

/* common data type defination */
typedef unsigned int                uint32_t;
typedef unsigned short              uint16_t;
typedef unsigned char               uint8_t;
typedef unsigned char               bool_t;
typedef signed   int                int32_t;
typedef signed   short              int16_t;
typedef signed   char               int8_t;
typedef float                       float32;
typedef double                      float64;

#ifdef false
#undef false
#endif 
#ifdef true
#undef true
#endif 

#define false                       0u
#define true                        1u

#define UDS_TIEMR_NUM       0x04u

typedef struct {
    uint32_t id;
    uint8_t buf[8];
    uint16_t buf_len;
} can_t;


// typedef struct {
//     void *qstart;
//     void *qend;
//     void *qin;
//     void *qout;
//     uint8_t qentries;   /* buf have value number */
//     uint8_t qsize;      /* buf size */
// } uds_q_t;

/**
 * @brief timer of uds
 * 
 */
typedef void (*uds_func_t)(void *);

typedef struct {
    bool_t st;
    uint32_t val;
    uint32_t cnt;
    uds_func_t act;
    void *parg;
} uds_timer_t;


#endif /* UDS_CMN_H_ */