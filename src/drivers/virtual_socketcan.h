#ifndef VIRTUAL_SOCKETCAN_H
#define VIRTUAL_SOCKETCAN_H

#include <stdint.h>
#include <stdbool.h>
#include <linux/can.h>

#ifdef __cplusplus
extern "C" {
#endif

// #define UDS_DL_CAN_DL                   8u

// typedef struct {
//     uint16_t    id;
//     uint16_t    dlc;
//     uint8_t     dt[UDS_DL_CAN_DL];
// } can_std_frame_t;
enum Iso14229CANRxStatus 
{
    kCANRxNone = -1, 
    kCANRxSome = 0
};

/*
provide a debug function with -DISO14229USERDEBUG=printf when compiling this
library
*/
#ifndef ISO14229USERDEBUG
#define ISO14229USERDEBUG(fmt, ...) ((void)fmt)
#endif

#define PRINTHEX(addr, len)                                                                        \
    {                                                                                              \
        for (int i = 0; i < len; i++) {                                                            \
            ISO14229USERDEBUG("%02x,", addr[i]);                                                   \
        }                                                                                          \
        ISO14229USERDEBUG("\n");                                                                   \
    }

extern bool port_should_exit;
int portSetup(char *dev);
extern void isotp_user_debug(const char *fmt, ...);

enum Iso14229CANRxStatus portCANRxPoll(uint32_t *arb_id, uint8_t *data, uint8_t *size);
int portSendCAN(const uint32_t arbitration_id, const uint8_t *data, const uint8_t size);
void portYieldms(uint32_t tms);
uint32_t portGetms();

// #define UDS_DL_CAN_DL 8u

// typedef struct {
//     uint32_t    id;
//     uint16_t    dlc;
//     uint8_t     dt[UDS_DL_CAN_DL];
// } can_std_frame_t;

/**
 * @brief can init
 * 
 * @param channel 
 * @return int 
 */
int can_init(int channel, char *dev);

/**
 * @brief can receive frame
 * 
 * @param q 
 * @param fr 
 * @return uint8_t 
 */
int can_rx(struct can_frame *frame_ptr);

/**
 * @brief can tx frame
 * 
 * @param fr 
 * @return int 
 */
int can_tx(const struct can_frame *frame_ptr);

#ifdef __cplusplus
}
#endif

#endif /* VIRTUAL_SOCKETCAN_H */
