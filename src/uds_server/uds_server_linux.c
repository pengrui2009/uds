/***********************************************************************
 * @file test.c
 * @brief  use to test the uds
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-24  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022   all right reserved
***********************************************************************/
#include "uds.h"
#include "uds_cfg.def"
#include "virtual_socketcan.h"

#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/can.h>

uint32_t timerCount = 0;
uint8_t fr_i = 0;

// can_std_frame_t fr[] = {
//     {0x18DA1EF9, 8, {0x02, 0x10, 0x03}},
//     {0x18DA1EF9, 8, {0x02, 0x27, 0x01}},
//     {0x18DA1EF9, 8, {0x04, 0x27, 0x02, 0x99, 0x11}},
//     {0x18DA1EF9, 8, {0x10, 0x09, 0x22, 0x12, 0x34, 0x12, 0x35, 0x12}},
//     {0x18DA1EF9, 8, {0x21, 0x43, 0x12, 0x37}},
//     {0x18DA1EF9, 8, {0x21, 0x43, 0x12, 0x37}},
//     {0x18DA1EF9, 8, {0x07, 0x22, 0x12, 0x34, 0x12, 0x35, 0x12, 0x43}},
//     {0x18DA1EF9, 8, {0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
//     {0x18DA1EF9, 8, {0x04, 0x2e, 0x12, 0x34, 0x55}},
//     {0x18DA1EF9, 8, {0x02, 0x10, 0x01}},
// };

struct can_frame fr[] = {
    {.can_id = 0x7e2, .can_dlc = 8, .data = {0x02, 0x10, 0x03}},
    {.can_id = 0x7e2, .can_dlc = 8, .data = {0x02, 0x27, 0x01}},
    {.can_id = 0x7e2, .can_dlc = 8, .data = {0x04, 0x27, 0x02, 0x99, 0x11}},
    {.can_id = 0x7e2, .can_dlc = 8, .data = {0x10, 0x09, 0x22, 0x12, 0x34, 0x12, 0x35, 0x12}},
    {.can_id = 0x7e2, .can_dlc = 8, .data = {0x21, 0x43, 0x12, 0x37}},
    {.can_id = 0x7e2, .can_dlc = 8, .data = {0x21, 0x43, 0x12, 0x37}},
    {.can_id = 0x7e2, .can_dlc = 8, .data = {0x07, 0x22, 0x12, 0x34, 0x12, 0x35, 0x12, 0x43}},
    {.can_id = 0x7e2, .can_dlc = 8, .data = {0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {.can_id = 0x7e2, .can_dlc = 8, .data = {0x04, 0x2e, 0x12, 0x34, 0x55}},
    {.can_id = 0x7e2, .can_dlc = 8, .data = {0x02, 0x10, 0x01}},
};


#define FR_NUM sizeof(fr) / sizeof(struct can_frame)

void* thread_entry(void *arg)
{
    while(1)
    {
        timerCount++;
        uds_timer_tick();
        // printf("timer: %ud\n", timerCount);
        usleep(10000);
    }

    return (void *)NULL;
}

static void sighand(int sig)
{
    (void)sig;
}

int main(void) 
{      
    // DWORD_PTR dwUser;
    int ret = 0;
    int time = 0;
    int timerID;
    bool stop = false;
    pthread_t tid;

    /* Allow signals to interrupt syscalls */
    // signal(SIGINT, sighand);
    // siginterrupt(SIGINT, 1);
    char *argv[] = {"server" ,"vcan0"};
    ret = can_init(2, "vcan0");
    if (ret)
    {
        printf("can_init failed!\n");
        return -1;
    }

    ret = uds_init();
    if (ret)
    {
        printf("uds init failed\n");
        return -1;
    }

    // 设置定时器
    // dwUser=(DWORD_PTR)&time;
	// timerID = timeSetEvent(10, 1, (LPTIMECALLBACK)task0_10ms, dwUser, TIME_PERIODIC);
	// timerID = timeSetEvent(10, 1, (LPTIMECALLBACK)task1_10ms, dwUser, TIME_PERIODIC);
    ret = pthread_create(&tid, NULL, &thread_entry, &time);
    if (ret)
    {
        printf("pthread_create failed!\n");
        return -1;
    }

	while (true){
        struct can_frame fr = 
        {
            .can_id = 0,
            .can_dlc = 0,
            .data = {0},
        };

        // printf("can_rx...\n");
        ret = can_rx(&fr);
        if (ret)
        {
            continue;
        }

        printf("canid:%x\n", fr.can_id);
        if (fr.can_id == UDS_TP_FUNCTION_ADDR || fr.can_id == UDS_TP_PHYSICAL_ADDR) 
        {
            uds_q_rslt result = uds_qenqueue(&uds_dl.in_qf, &fr, (uint16_t)(sizeof(struct can_frame)));
            if (result != UDS_Q_OK)
            {
                printf("uds_qenqueue result:%d\n", result);
                break;
            }
        }

        if (fr.can_id != UDS_TP_PHYSICAL_ADDR)
        {
            continue;
        }
            
        uds_process();

        if ((!uds_dl.in_qf.qentries) && stop) {
            break;
        }

    }
    pthread_join(tid, NULL);

    return 0;
}
