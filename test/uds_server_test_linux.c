/***********************************************************************
 * @file test.c
 * @brief  use to test the uds
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-24  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022  Jixing. all right reserved
***********************************************************************/

#include "uds.h"

#include <signal.h>
#include <unistd.h>
#include <pthread.h>
// #include "uds_phycan.h"
// timer for windows

uint32_t timerCount = 0;
uint8_t fr_i = 0;

can_std_frame_t fr[] = {
    {0x7e2, 8, {0x02, 0x10, 0x03}},
    {0x7e2, 8, {0x02, 0x27, 0x01}},
    {0x7e2, 8, {0x04, 0x27, 0x02, 0x99, 0x11}},
    {0x7e2, 8, {0x10, 0x09, 0x22, 0x12, 0x34, 0x12, 0x35, 0x12}},
    {0x7e2, 8, {0x21, 0x43, 0x12, 0x37}},
    {0x7e2, 8, {0x21, 0x43, 0x12, 0x37}},
    {0x7e2, 8, {0x07, 0x22, 0x12, 0x34, 0x12, 0x35, 0x12, 0x43}},
    {0x7e2, 8, {0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x7e2, 8, {0x04, 0x2e, 0x12, 0x34, 0x55}},
    {0x7e2, 8, {0x02, 0x10, 0x01}},
};

#define FR_NUM sizeof(fr) / sizeof(can_std_frame_t)

// void WINAPI task0_10ms(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
// {
//     timerCount++;
//     uds_timer_tick();
//     printf("timer: %ld\n", timerCount);
// }

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
    bool_t stop = false;
    pthread_t tid;

    /* Allow signals to interrupt syscalls */
    // signal(SIGINT, sighand);
    // siginterrupt(SIGINT, 1);

    uds_init();
    
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
        if (timerCount > SECURITYACCESS_DELAY_TIME) {
            if (fr_i < FR_NUM) {
                // printf("fr_i:%d\n", fr_i);
                can_std_frame_t fr;
                
                uds_can_recv_frame(&uds_dl.in_qf, &fr);
                
                if (fr.id != 0x7e2)
                {
                    continue;
                }
            } else {
                stop = true;
            }
        }
        uds_process();
        // printf("main: %ud\n", timerCount);

        if ((!uds_dl.in_qf.qentries) && stop) {
            break;
        }

    }
    pthread_join(tid, NULL);

    return 0;
}