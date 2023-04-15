/**
 * @file uds_ap.h
 * @author rui.peng (pengrui2009@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef UDS_AP_H_
#define UDS_AP_H_

#include "uds_tp.h"
// #include "uds_cfg.def"
// #include "cmn.h"

#ifdef __cplusplus
extern "C" {
#endif


#define suppressPosRspMsgIndicationBit  0x80u
#define exceedNumberofTrySecurity       10

#define RESPONSE_SID_OF(request_sid)    (request_sid + 0x40)
#define REQUEST_SID_OF(response_sid)    (response_sid - 0x40)

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

#define UDS_P2_IND                      0x00u
// #define UDS_A_SADELAY_IND               0x03u


/**
 * @brief uds ap layer, implementation of iso14229-1
 * 
 */
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

typedef enum {
    UDS_SEQ_ERR_MIN = INT16_MIN, // 固定范围

    // 用户可以定义流程项管故障码在这个范围内

    UDS_SEQ_ERR_UNUSED = -127,
    UDS_SEQ_ERR_TIMEOUT,       // 流程超时
    UDS_SEQ_ERR_NULL_CALLBACK, // 回调函数是NULL

    UDS_CLIENT_ERR_RESP_SCHEMA_INVALID = -12, // 数据内容或者大小不按照应用定义(如ODX)

    UDS_CLIENT_ERR_RESP_DID_MISMATCH = -11,            // 响应DID对不上期待的DID
    UDS_CLIENT_ERR_RESP_CANNOT_UNPACK = -10,           // 响应不能解析
    UDS_CLIENT_ERR_RESP_TOO_SHORT = -9,                // 响应太小
    UDS_CLIENT_ERR_RESP_NEGATIVE = -8,                 // 否定响应
    UDS_CLIENT_ERR_RESP_SID_MISMATCH = -7,             // 请求和响应SID对不上
    UDS_CLIENT_ERR_RESP_UNEXPECTED = -6,               // 突然响应
    UDS_CLIENT_ERR_REQ_TIMED_OUT = -5,                 // 请求超时
    UDS_CLIENT_ERR_REQ_NOT_SENT_TPORT_ERR = -4,        // 传输层故障、没发
    UDS_CLIENT_ERR_REQ_NOT_SENT_BUF_TOO_SMALL = -3,    // 传输层缓冲器不够大
    UDS_CLIENT_ERR_REQ_NOT_SENT_INVALID_ARGS = -2,     // 参数不对、没发
    UDS_CLIENT_ERR_REQ_NOT_SENT_SEND_IN_PROGRESS = -1, // 在忙、没发
    UDS_CLIENT_OK = 0,                                 // 流程完成
    UDS_CLIENT_SEQUENCE_RUNNING = 1,                   // 流程正在跑、还没完成
} uds_client_error_t;

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

    // timer for p2 or p2star
    uds_timer_t            *ptmr_p2;
    // uds_timer_t            *ptmr_sadelay;
    
    // application status
    uds_ap_sts_t            sts;
    // supress pos response
    bool_t                  sup_pos_rsp;
} uds_ap_layer_t;


typedef void (*uds_ap_fun_t)(uds_ap_layer_t *, uds_tp_layer_t *);

typedef struct {
    uds_ap_sid_type_t sid;
    uds_ap_session_type_t spt_ses;
    uds_ap_security_level_t spt_sec;
    uds_ap_fun_t srv_rte;
} uds_ap_service_t;


enum ClientOptions {
    SUPPRESS_POS_RESP       = 0b00000001, // 服务器不应该发送肯定响应
    FUNCTIONAL              = 0b00000010, // 发功能请求
    NEG_RESP_IS_ERR         = 0b00000100, // 否定响应是属于故障
    IGNORE_SERVER_TIMINGS   = 0b00001000, // 忽略服务器给的p2和p2_star
};

enum ClientRequestState {
    kRequestStateIdle = 0,          // 完成
    kRequestStateSending,           // 传输层现在传输数据
    kRequestStateSent,              // 传输层完成传输。可以设置等待计时器
    kRequestStateSentAwaitResponse, // 等待响应
    kRequestStateProcessResponse,   // 处理响应
};

enum ClientError {
    kCLIENTSEQ_ERR_MIN = INT16_MIN, // 固定范围

    // 用户可以定义流程项管故障码在这个范围内

    kSEQ_ERR_UNUSED = -127,
    kSEQ_ERR_TIMEOUT,       // 流程超时
    kSEQ_ERR_NULL_CALLBACK, // 回调函数是NULL

    kCLIENT_ERR_RESP_SCHEMA_INVALID = -12, // 数据内容或者大小不按照应用定义(如ODX)

    kCLIENT_ERR_RESP_DID_MISMATCH = -11,            // 响应DID对不上期待的DID
    kCLIENT_ERR_RESP_CANNOT_UNPACK = -10,           // 响应不能解析
    kCLIENT_ERR_RESP_TOO_SHORT = -9,                // 响应太小
    kCLIENT_ERR_RESP_NEGATIVE = -8,                 // 否定响应
    kCLIENT_ERR_RESP_SID_MISMATCH = -7,             // 请求和响应SID对不上
    kCLIENT_ERR_RESP_UNEXPECTED = -6,               // 突然响应
    kCLIENT_ERR_REQ_TIMED_OUT = -5,                 // 请求超时
    kCLIENT_ERR_REQ_NOT_SENT_TPORT_ERR = -4,        // 传输层故障、没发
    kCLIENT_ERR_REQ_NOT_SENT_BUF_TOO_SMALL = -3,    // 传输层缓冲器不够大
    kCLIENT_ERR_REQ_NOT_SENT_INVALID_ARGS = -2,     // 参数不对、没发
    kCLIENT_ERR_REQ_NOT_SENT_SEND_IN_PROGRESS = -1, // 在忙、没发
    kCLIENT_OK = 0,                                 // 流程完成
    kCLIENT_SEQUENCE_RUNNING = 1,                   // 流程正在跑、还没完成
};

struct ClientRequest {
    const uint8_t *buffer_ptr;
    uint16_t buffer_len;
    uint16_t buffer_size;
};

struct ClientResponse {
    const uint8_t *buffer_ptr;
    uint16_t buffer_size;
    uint16_t buffer_len;
};

typedef struct {
    // 
    struct ClientRequest    request;
    struct ClientResponse   response;
    enum ClientRequestState state;
    enum ClientError        error;

    enum ClientOptions      options;
    enum ClientOptions      defaultOptions;
    // a copy of the options at the time a request is made
    enum ClientOptions      options_copy; 

    // response             result data
    const uint8_t           *result_data;     
    // response             result length
    uint32_t                result_len;      
} UdsClient;


// UDS_EXT uds_timer_t uds_timer[UDS_TIEMR_NUM];
/**
 * @brief 
 * 
 * @param pap 
 */
void uds_ap_init(uds_ap_layer_t *pap);


/**
 * @brief 
 * 
 * @param pap 
 * @param ptp 
 */
int uds_ap_process(uds_ap_layer_t *pap, uds_tp_layer_t *ptp, UdsClient *uds_client_ptr);


/**
 * @brief Called at periodic tick rate
 * 
 */
void udsapp_update(void);

/**
 * @brief Processes received messages.
 * 
 * @param msg 
 */
// void udsapp_process(i15765_t *msg);

/**
 * @brief Processes negative responses
 * 
 * @param svcid 
 * @param nrsp 
 */
void udsapp_nrsp_process(uint8_t svcid, uint8_t nrsp);

/**
 * @brief 
 * 
 * @param func Read and change link timing
 * @param sprsp 
 * @param key 
 * @param klen 
 * @return int  
 */
int uds_req_access_timing_parameter(uint8_t func, uint8_t sprsp, uint8_t *key, uint8_t klen);

/**
 * @brief Transmit data in a secure manner
 * 
 * @param sd 
 * @param sdlen 
 * @return int   
 */
int uds_req_secured_data_transmission(uint8_t *sd, uint8_t sdlen);

/**
 * @brief Halt/resume setting of DTCs
 * 
 * @param func 
 * @param sprsp 
 * @param key 
 * @param klen
 * @return int 
 */
int uds_req_control_dtc_setting(uint8_t func, uint8_t sprsp, uint8_t *key, uint8_t klen);

/**
 * @brief Automatically respond to certain events with a defined request
 * 
 * @param evntype 
 * @param wintime 
 * @param rec 
 * @param reclen 
 * @param rsp 
 * @param rsplen 
 * @return int  
 */
int uds_req_response_on_event(uint8_t evntype, uint8_t wintime, uint8_t *rec, uint8_t reclen, uint8_t *rsp, uint8_t rsplen);

/**
 * @brief Check to see if transition of link baudrate to predefined rate is possible
 * 
 * @param sprsp 
 * @param baud 
 * @return int 
 */
int uds_req_link_control_predef(uint8_t sprsp, uint8_t baud);

/**
 * @brief Check to see if transition of link baudrate to specified rate is possible
 * 
 * @param sprsp 
 * @param baud 
 * @return int  
 */
int uds_req_link_control_user(uint8_t sprsp, uint32_t baud);

/**
 * @brief Transition to previously discussed rate
 * 
 * @param func 
 * @param sprsp 
 * @return int  
 */
int uds_req_link_control(uint8_t func, uint8_t sprsp);

/**
 * @brief Reads data by defined dataIdentifier
 * 
 * @param did 
 * @param dlen 
 * @return int   
 */
int uds_req_read_data_by_id(uint16_t *did, uint8_t dlen);

/**
 * @brief Read memory by address
 * 
 * @param addr 
 * @param size 
 * @return int  
 */
int uds_req_read_memory_by_address(uint32_t addr, uint16_t size);

/**
 * @brief Read scaling data by dataIdentifier
 * 
 * @param id 
 * @return int 
 */
int uds_req_read_scaling_by_id(uint16_t id);

/**
 * @brief Read data by periodic identifier
 * 
 * @param txmode 
 * @param did 
 * @param dlen 
 * @return int  
 */
int uds_req_read_data_by_periodic_id(uint8_t txmode, uint16_t *did, uint8_t dlen);

/**
 * @brief Define a dataIdentifier for reading
 * 
 * @param sprsp 
 * @param ddid 
 * @param sdid 
 * @param pos 
 * @param msize 
 * @param len 
 * @return int  
 */
int uds_req_dynamically_define_by_data_id(uint8_t sprsp, uint16_t ddid, uint16_t *sdid, 
    uint8_t *pos, uint8_t *msize, uint8_t len);

/**
 * @brief Define a dataIdentifier using a memory address for reading
 * 
 * @param sprsp 
 * @param ddid 
 * @param addr 
 * @param size 
 * @param len 
 * @return int
 */
int uds_req_dynamically_define_by_memory_address(uint8_t sprsp, uint16_t ddid, uint32_t *addr, 
    uint16_t *size, uint8_t len);

/**
 * @brief Clear a dynamically defined data Identifier
 * 
 * @param sprsp 
 * @param ddid 
 * @return int 
 */
int uds_req_clear_dynamically_defined_data_id(uint8_t sprsp, uint16_t ddid);

/**
 * @brief Write data specified by dataIdentifier
 * 
 * @param did 
 * @param drec 
 * @param len 
 * @return int 
 */
int uds_req_write_data_by_id(uint16_t did, uint8_t *drec, uint8_t len);

/**
 * @brief Write data to memory by address
 * 
 * @param addr 
 * @param size 
 * @param drec 
 * @return int  
 */
int uds_req_write_memory_by_address(uint32_t addr, uint16_t size, uint8_t *drec);

/**
 * @brief Clear DTC information
 * 
 * @param dtc 
 * @return int   
 */
int uds_req_clear_diagnostic_information(uint32_t dtc);

/**
 * @brief Read DTC info by status mask
 * 
 * @param func 
 * @param sprsp 
 * @param mask 
 * @return int  
 */
int uds_req_read_dtc_info_mask(uint8_t func, uint8_t sprsp, uint8_t mask);

/**
 * @brief Request report of all DTCSnapshots
 * 
 * @param sprsp 
 * @return int  
 */
int uds_req_read_dtc_info_report_snapshot_id(uint8_t sprsp);

/**
 * @brief Request report of DTCSnapshots related to specified DTC
 * 
 * @param sprsp 
 * @param mask 
 * @param recnum 
 * @return int  
 */
int uds_req_read_dtc_info_report_snapshot_by_dtc(uint8_t sprsp, uint32_t mask, uint8_t recnum);

/**
 * @brief Read data from specified DTCSnapshot
 * 
 * @param sprsp 
 * @param recnum 
 * @return int  
 */
int uds_req_read_dtc_info_report_snapshot_by_record(uint8_t sprsp, uint8_t recnum);

/**
 * @brief Read extended data about specified DTC
 * 
 * @param func 
 * @param sprsp 
 * @param mask 
 * @param recnum 
 * @return int  
 */
int uds_req_read_dtc_info_edr_dtc(uint8_t func, uint8_t sprsp, uint32_t mask, uint8_t recnum);

/**
 * @brief Request list of DTCs which match specified severity level
 * 
 * @param func 
 * @param sprsp 
 * @param mask 
 * @return int  
 */
int uds_req_read_dtc_info_severity_info(uint8_t func, uint8_t sprsp, uint16_t mask);

/**
 * @brief Read severity info of specified DTC
 * 
 * @param sprsp 
 * @param mask 
 * @return int  
 */
int uds_req_read_dtc_info_severity_dtc(uint8_t sprsp, uint32_t mask);

/**
 * @brief Catchall function for remaining DTC info sub-functions
 * 
 * @param func 
 * @param sprsp 
 * @return int  
 */
int uds_req_read_dtc_info_misc(uint8_t func, uint8_t sprsp);


/**
 * @brief Emulate value for input/output signal
 * 
 * @param did 
 * @param opt 
 * @param optlen 
 * @param mask 
 * @param masklen 
 * @return int  
 */
int uds_req_io_control(uint16_t did, uint8_t *opt, uint8_t optlen, uint8_t *mask, uint8_t masklen);

/**
 * @brief Start/stop stored routine
 * 
 * @param rtype 
 * @param rid 
 * @param ropt 
 * @param optlen 
 * @return int  
 */
int uds_req_routine_control(uint8_t rtype, uint16_t rid, uint8_t *ropt, uint8_t optlen);

/**
 * @brief Request a download to the server
 * 
 * @param dfmtid 
 * @param maddr 
 * @param maddrlen 
 * @param msize 
 * @param msizelen 
 * @return int  
 */
int uds_req_download(uint8_t dfmtid, uint8_t *maddr, uint8_t maddrlen, uint8_t *msize, uint8_t msizelen);

/**
 * @brief Request an upload from the server
 * 
 * @param dfmtid 
 * @param maddr 
 * @param maddrlen 
 * @param msize 
 * @param msizelen 
 * @return int  
 */
int uds_req_upload(uint8_t dfmtid, uint8_t *maddr, uint8_t maddrlen, uint8_t *msize, uint8_t msizelen);

/**
 * @brief Transfer data after request has been made
 * 
 * @param blckcnt 
 * @param txparam 
 * @param paramlen 
 * @return int 
 */
int uds_req_transfer_data(uint8_t blckcnt, uint8_t *txparam, uint32_t paramlen);

/**
 * @brief Exit data transfer mode
 * 
 * @param sd 
 * @param sdlen 
 * @return int  
 */
int uds_req_transfer_exit(uint8_t *sd, uint8_t sdlen);

/**
 * @brief Automatically create secured session
 * 
 * @param level 
 * @return int  
 */
int uds_create_secured_session(uint8_t level);

/**
 * @brief Processes seed into key for secure session creation
 * 
 * @param seed 
 * @param seedlen 
 * @return int  
 */
int udsapp_process_seed(uint8_t *seed, uint16_t *seedlen);





#ifdef __cplusplus
}
#endif

#endif /* UDS_AP_H_ */