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






#endif /* UDS_CMN_H_ */