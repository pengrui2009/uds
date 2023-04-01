/**
 * @file socketcan.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef SOCKETCAN_H_
#define SOCKETCAN_H_

#include <stdint.h>
#include <stdbool.h>

#define USERDEBUG printf

#ifndef USERDEBUG
#define USERDEBUG(fmt, ...) ((void)fmt)
#endif

#define PRINTHEX(addr, len)                                                                        \
    {                                                                                              \
        for (int i = 0; i < len; i++) {                                                            \
            USERDEBUG("%02x,", addr[i]);                                                   \
        }                                                                                          \
        USERDEBUG("\n");                                                                   \
    }


enum CANRxStatus { 
    kCANRxNone = -1, 
    kCANRxSome = 0
};

extern bool port_should_exit;
void CanInit(int ac, char **av);
extern void isotp_user_debug(const char *fmt, ...);

enum CANRxStatus CanRxPoll(uint32_t *arb_id, uint8_t *data, uint16_t *size);
int CanSend(const uint32_t arbitration_id, const uint8_t *data, const uint16_t size);
void portYieldms(uint32_t tms);
uint32_t portGetms();

#endif /* SOCKETCAN_H_ */