/**
 * @file cmn.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef CMN_H_
#define CMN_H_

#ifdef __cplusplus
extern "C" {
#endif

enum Iso14229CANRxStatus {
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

#ifdef __cplusplus
}
#endif

#endif /* CMN_H_ */
