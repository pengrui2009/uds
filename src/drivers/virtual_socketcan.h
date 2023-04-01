#ifndef VIRTUAL_SOCKETCAN_H
#define VIRTUAL_SOCKETCAN_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern bool port_should_exit;
int portSetup(char *dev);
extern void isotp_user_debug(const char *fmt, ...);

enum Iso14229CANRxStatus portCANRxPoll(uint32_t *arb_id, uint8_t *data, uint8_t *size);
int portSendCAN(const uint32_t arbitration_id, const uint8_t *data, const uint8_t size);
void portYieldms(uint32_t tms);
uint32_t portGetms();

#ifdef __cplusplus
}
#endif

#endif /* VIRTUAL_SOCKETCAN_H */
