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
#include "kvasercan.h"

#include <canlib.h>

#include <canstat.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

// can handler
static canHandle hnd;
static int inited = -1;

/**
 * @brief initialize can device
 * 
 * @param channel 
 * @return int 
             0   :init ok 
            -1   :init failed
 */
int can_init(int channel, char *dev)
{
    int ret = 0;    
    canStatus stat;

    (void)dev;
    
    canInitializeLibrary();
    
    /* Open channel, set parameters and go on bus */
    hnd = canOpenChannel(channel, canOPEN_EXCLUSIVE | canOPEN_REQUIRE_EXTENDED | canOPEN_ACCEPT_VIRTUAL);
    if (hnd < 0) {
        printf("canOpenChannel %d", channel);
        return -1;
    }

    stat = canSetBusParams(hnd, canBITRATE_500K, 0, 0, 0, 0, 0);
    if (stat != canOK) {
        goto ErrorExit;
    }

    stat = canBusOn(hnd);
    if (stat != canOK) {
        goto ErrorExit;
    }

    inited = 1;

    return ret;
ErrorExit:
    stat = canBusOff(hnd);
    usleep(50*1000); // Sleep just to get the last notification.
    stat = canClose(hnd);
    stat = canUnloadLibrary();

    return -1;    
}

/**
 * @brief receive can frame and add to queue
 * 
 * @param q : queue
 * @param fr : can frame
 * @return uint8_t 
        0: success
        1: receive failed 
        2: can device not init
 */
uint8_t can_rx(struct can_frame *frame_ptr) 
{
    uint8_t ret = 0;
    canStatus stat = canOK;

    long id = 0;
    unsigned int dlc = 0;
    unsigned int flag = 0;
    unsigned long time = 0;
    long canid = 0x00;
    
    if (inited != 1)
    {
        return 2;
    }

    stat = canReadWait(hnd, &id, &frame_ptr->can_dlc, &dlc, &flag, &time, READ_WAIT_INFINITE);
    if (stat == canOK)
    {
        frame_ptr->can_id = id;
        frame_ptr->can_dlc = dlc;
        // if (fr->id == UDS_TP_FUNCTION_ADDR || frame_ptr->id == UDS_TP_PHYSICAL_ADDR) {
        //     uds_qenqueue(q, fr, (uint16_t)(sizeof(can_std_frame_t)));
        // }
        
        ret = 0;
    } else {
        ret = 1;
    }

    return ret;
}

/**
 * @brief can tx a frame
 * 
 * @param fr : can frame
 * @return uint8_t 
            0: success send a can frame
            1: failed send a can frame
            2: can device not init
 */
uint8_t can_tx(const struct can_frame *frame_ptr)
{   
    uint8_t ret = 0;
    canStatus stat = canOK;

    if (inited != 1)
    {
        ret = 2;
        return ret;
    }

    long id = frame_ptr->can_id;
    unsigned int dlc = frame_ptr->can_dlc;

    stat = canWriteWait(hnd, id, frame_ptr->can_dlc, dlc, canMSG_EXT, WRITE_WAIT_INFINITE);
    if (stat != canOK) 
    {
        printf("send failed\n");
        ret = 1;
    } else {
        ret = 0;
    }

    return ret;
}