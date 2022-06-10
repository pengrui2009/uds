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
#include "uds_phycan.h"
#include <canlib.h>

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

// #include "vcanevt.h"

#define WRITE_WAIT_INFINITE     (unsigned long)(-1)

static canHandle hnd;


// static char* busStatToStr(const unsigned long flag) {
//     char* tempStr = NULL;
//     #define MACRO2STR(x) case x: tempStr = #x; break
//     switch (flag) {
//         MACRO2STR( CHIPSTAT_BUSOFF        );
//         MACRO2STR( CHIPSTAT_ERROR_PASSIVE );
//         MACRO2STR( CHIPSTAT_ERROR_WARNING );
//         MACRO2STR( CHIPSTAT_ERROR_ACTIVE  );
//         default: tempStr = ""; break;
//     }
//     #undef MACRO2STR
//     return tempStr;
// }

void notifyCallback(canNotifyData *data) {
  switch (data->eventType) {
  case canEVENT_STATUS:
    printf("CAN Status Event: %s\n", data->info.status.busStatus);
    break;
  case canEVENT_ERROR:
    printf("CAN Error Event\n");
    break;
  case canEVENT_TX:
    printf("CAN Tx Event\n");
    break;
  case canEVENT_RX:
    printf("CAN Rx Event\n");
    break;
  }
  return;
}

int uds_can_init(int channel)
{
    int ret = 0;
    canStatus stat;

    
    canInitializeLibrary();
    
    /* Open channel, set parameters and go on bus */
    hnd = canOpenChannel(channel, canOPEN_EXCLUSIVE | canOPEN_REQUIRE_EXTENDED | canOPEN_ACCEPT_VIRTUAL);
    if (hnd < 0) {
        printf("canOpenChannel %d", channel);
        return -1;
    }

    // stat = canSetNotify(hnd, notifyCallback, canNOTIFY_RX | canNOTIFY_TX | canNOTIFY_ERROR | canNOTIFY_STATUS | canNOTIFY_ENVVAR, (char*)0);
    printf("canSetNotify\n");
    stat = canSetBusParams(hnd, canBITRATE_500K, 0, 0, 0, 0, 0);
    if (stat != canOK) {
        goto ErrorExit;
    }

    stat = canBusOn(hnd);
    if (stat != canOK) {
        goto ErrorExit;
    }

    return ret;
ErrorExit:
    stat = canBusOff(hnd);
    usleep(50*1000); // Sleep just to get the last notification.
    stat = canClose(hnd);
    stat = canUnloadLibrary();

    return 0;    
}
/**
 * @brief 
 * 
 * @param q 
 * @param fr 
 */
#define READ_WAIT_INFINITE    (unsigned long)(-1)
void uds_can_recv_frame(uds_q_t *q, can_std_frame_t *fr) 
{
    canStatus stat;

    long id;
    unsigned int dlc;
    unsigned int flag;
    unsigned long time;
    long canid = 0x00;
    
    stat = canReadWait(hnd, &id, &fr->dt, &dlc, &flag, &time, READ_WAIT_INFINITE);
    if (stat == canOK)
    {
        fr->id = id;
        fr->dlc = dlc;
        if (fr->id == UDS_TP_FUNCTION_ADDR || fr->id == UDS_TP_PHYSICAL_ADDR) {
            uds_qenqueue(q, fr, (uint16_t)(sizeof(can_std_frame_t)));
        }
    }
}


/**
 * @brief 
 * 
 * @param fr 
 */
void uds_can_send_frame(can_std_frame_t *fr)
{   
//     uint8_t i;
//     /* send action */
// #ifdef TEST_WIN32
//     printf("can send:");
//     for (i = 0; i < fr->dlc; i++) {
//         printf("%02X ", fr->dt[i]);
//     }
//     printf("\n");
// #endif
    canStatus stat;

    long id = fr->id;
    unsigned int dlc = fr->dlc;

    stat = canWriteWait(hnd, id, fr->dt, dlc, canMSG_EXT, WRITE_WAIT_INFINITE);
    if (stat != canOK) {
        printf("send failed\n");
        return;
    }
}