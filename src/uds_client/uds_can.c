/**
 * @file uds_can.c
 * @author rui.peng (pengrui_2009@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-25
 * 
 * @copyright rui.peng Copyright (c) 2022
 * 
 */
#include "uds_can.h"

/**
 * @brief 
 * 
 * @param q 
 * @param fr 
 */
void uds_recv_frame(uds_q_t *q, can_std_frame_t fr) 
{
    if (fr.id == UDS_TP_FUNCTION_ADDR || fr.id == UDS_TP_PHYSICAL_ADDR) {
        uds_qenqueue(q, &fr, (uint16_t)(sizeof(can_std_frame_t)));
    }
}


/**
 * @brief 
 * 
 * @param fr 
 */
void uds_send_frame(can_std_frame_t *fr)
{   
    uint8_t i;
    /* send action */
#ifdef TEST_WIN32
    printf("can send:");
    for (i = 0; i < fr->dlc; i++) {
        printf("%02X ", fr->dt[i]);
    }
    printf("\n");
#endif
}