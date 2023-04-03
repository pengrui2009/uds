/**
 * @file uds_ap.h
 * @author rui.peng (pengrui2009@gmail.com)
 * @brief uds ap layer, implementation of iso14229-1
 * @version 0.1
 * @date 2023-04-03
 * 
 * Copyright (C) 2022 . All Rights Reserved.
 * 
 */
#ifndef UDS_AP_H_
#define UDS_AP_H_

#include "uds_tp.h"

#ifdef __cplusplus
extern "C" {
#endif



#define suppressPosRspMsgIndicationBit  0x80u
#define exceedNumberofTrySecurity       10


/* SUB-FUNCTION defines */
/* 0X10 */
#define DEFAULT_SESSION                 0x01u
#define PROGRAMMING_SESSION             0x02u
#define EXTENDDIAGNOSITIC_SESSION       0x03u

/* 0x11 */
#define HARD_RESET                      0x01u
// #define KEYOFFON_RESET
#define SOFT_RESET                      0x03u

/* 0x27 */
#define REQUEST_SEED1                   0x01u
#define REQUEST_SEED2                   0x03u
#define REQUEST_SEED3                   0x05u
#define REQUEST_KEY1                    0x02u
#define REQUEST_KEY2                    0x04u
#define REQUEST_KEY3                    0x06u

/* 0x28 */
#define ENABLERX_ENABLETX               0x00u
#define ENABLERX_DISABLETX              0x01u
#define DISABLERX_ENABLETX              0x02u
#define DISABLERX_DISABLETX             0x03u

/* 0x3E */
#define zeroSubFunction                 0x00u

/* 0x85 */
#define DTC_ON                          0x01u
#define DTC_OFF                         0x02u

typedef enum {
    /* Diagnostic and Communication Management */
    DiagnosticSessionControl = 0x10u,
    ECUReset = 0x11u,
    SecurityAccess = 0x27u,
    CommunicationControl = 0x28u,
    TesterPresent = 0x3Eu,
    AccessTimingParameter = 0x83u,
    SecuredDataTransmission = 0x84u,
    ControlDTCSetting = 0x85u,
    ResponseOnEvent = 0x86u,
    LinkControl = 0x87u,

    /* Data Transmission */
    ReadDataByIdentifier = 0x22u,
    ReadMemoryByAddress = 0x23u,
    ReadScalingDataByIdentifier = 0x24u,
    ReadDataByPeriodicIdentifier = 0x2Au,
    DynamicallyDefineDataIdentifier = 0x2Cu,
    WriteDataByIdentifier = 0x2Eu,
    WriteMemoryByAddress = 0x3Du,

    /* Stored Data Transmission */
    ClearDiagnosticInformation = 0x14u,
    ReadDTCInformation = 0x19u,

    /* InputOutput Control */
    InputOutputControlByIdentifier = 0x2Fu,

    /* Routine */
    RoutineControl = 0x31u,

    /* Upload Download */
    RequestDownload = 0x34u,
    RequestUpload = 0x35u,
    TransferData = 0x36u,
    RequestTransferExit = 0x37u,
    RequestFileTransfer = 0x38u,
} uds_ap_sid_type_t;


typedef enum {
    generalReject = 0x10u,
    serviceNotSupported = 0x11u,
    subfunctionNotSupported = 0x12u,
    incorrectMessageLengthOrInvalidFormat = 0x13u,
    responseTooLong = 0x14u,
    busyRepeatRequest = 0x21u,
    conditionsNotCorrect = 0x22u,
    requestSequenceError = 0x24u,
    noResponseFromSubnetComponent = 0x25u,
    FailurePreventsExecutionOfRequestedAction = 0x26u,
    requestOutOfRange = 0x31u,
    securityAccessDenied = 0x33u,
    invalidKey = 0x35u,
    exceedNumberOfAttempts = 0x36u,
    requiredTimeDelayNotExpired = 0x37u,
    uploadDownloadNotAccepted = 0x70u,
    transferDataSuspended = 0x71u,
    generalProgrammingFailure = 0x72u,
    wrongBlockSequenceCounter = 0x73u,
    requestCorrectlyReceivedResponsePending = 0x78u,
    // subfunctionNotSupportedInActiveSession = 0x7Eu,
    serviceNotSupportedInActiveSession = 0x7Fu,
} uds_ap_nrc_type_t;


typedef enum {
    SECURITY_LEVEL_0 = 0u, /* no need to unlocked */
    SECURITY_LEVEL_1,
    SECURITY_LEVEL_2,
    SECURITY_LEVEL_3,
} uds_ap_security_level_t;


typedef enum {
    defaultSession = 0x01u,
    programmingSession = 0x02u,
    extendedDiagnosticSession = 0x04u,
} uds_ap_session_type_t;


typedef enum {
    A_STS_IDLE = 0u,
    A_STS_BUSY,
    A_STS_ERROR,
} uds_ap_sts_t;


typedef struct {
    uint8_t seed[3];
    uint8_t key[3];
    uint8_t try_cnt;
    bool_t  try_max;
    bool_t  enable;
    union {   
        struct {
            uint8_t sd1_recv:1;        /* seed1 received */
            uint8_t sd2_recv:1;        /* seed2 received */
            uint8_t sd3_recv:1;        /* seed3 received */
            uint8_t :5;
        } bit;
        uint8_t all;
    } sds_recv;
    
} uds_ap_sec_t;


typedef struct {
    struct {
        bool_t req;
        bool_t tx;
        bool_t rx;
    } nor;                      // normal frame
    struct {
        bool_t req;
        bool_t tx;
        bool_t rx;
    } net;                      // network manage frame
} uds_ap_cmm_t;



/* used for 0x22 and 0x2e */
typedef struct {
    uint16_t    id;
    void       *var;
    uint8_t     sz;
} uds_did_type_t;


typedef struct {
    uds_ap_session_type_t   cur_ses;
    uds_ap_security_level_t cur_sec;

    uds_ap_sec_t            sec_ctrl;       /* 0x27 */
    uds_ap_cmm_t            cmm_ctrl;       /* 0x28 */

    uds_timer_t            *ptmr_s3;
    uds_timer_t            *ptmr_sadelay;
    
    uds_ap_sts_t            sts;
    bool_t                  sup_pos_rsp;
} uds_ap_layer_t;

typedef void (*uds_ap_fun_t)(uds_ap_layer_t *, uds_tp_layer_t *);

typedef struct {
    uds_ap_sid_type_t sid;
    uds_ap_session_type_t spt_ses;
    uds_ap_security_level_t spt_sec;
    uds_ap_fun_t srv_rte;
} uds_ap_service_t;

/**
 * @brief uds application layer init
 * 
 * @param pap 
 */
void uds_ap_init(uds_ap_layer_t *pap);

/**
 * @brief uds applicaiton layer process
 * 
 * @param pap 
 * @param ptp 
 */
void uds_ap_process(uds_ap_layer_t *pap, uds_tp_layer_t *ptp);

#ifdef __cplusplus
}
#endif

#endif /* UDS_AP_H_ */