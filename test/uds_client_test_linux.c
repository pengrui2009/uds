/**
 * @file uds_client_test_linux.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "uds.h"

#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

uint32_t timerCount = 0;
uint8_t fr_i = 0;

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


int main()
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

    ret = uds_init(0);
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
        if (timerCount > SECURITYACCESS_DELAY_TIME) {
            if (fr_i < FR_NUM) {
                // printf("fr_i:%d\n", fr_i);
                can_std_frame_t fr = {
                    .id = 0,
                    .dlc = 0,
                    .dt = {0},
                };
                
                ret = can_rx(&uds_dl.in_qf, &fr);
                if (ret)
                {
                    continue;
                }
                
                if (fr.id != 0x7e2)
                {
                    continue;
                }
            } else {
                stop = true;
            }
        }
        uds_process();

        if ((!uds_dl.in_qf.qentries) && stop) {
            break;
        }

    }
    pthread_join(tid, NULL);

    return 0;
}