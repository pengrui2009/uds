/***********************************************************************
 * @file uds.c
 * @brief  
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-12  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022   all right reserved
***********************************************************************/


#define UDS_GLOABL
#include "uds.h"
#include <linux/can.h>

/**
 * @brief uds init
 * 
 * @param channel 
 * @param dev
 * @return int 
            0   : success
            <0  : failed
 */
int uds_init(int channel, char *dev) 
{
    int ret = 0;
    
    if (can_init(channel, dev))
    {
        printf("can init failed!\n");
        return -1;
    }
    uds_dl_init(&uds_dl);
    uds_tp_init(&uds_tp); 
    uds_ap_init(&uds_ap);

    return ret;
}

/**
 * @brief 
 * 
 */
void uds_process(void)
{
    // uds_tp_process_in(&uds_tp, &uds_dl);
    // uds_ap_process(&uds_ap, &uds_tp); 
    
    // uds_tp_process_out(&uds_tp, &uds_dl);

    // uds_dl_process_out(&uds_dl);

    // // 
    // uds_dl_process_in(&uds_dl);

    // uds_tp_process_in(&uds_tp, &uds_dl);
    
    // uds_ap_process(&uds_ap, &uds_tp);    
}

/**
 * @brief 
 * 
 */
void uds_timer_tick(void) 
{   
    uint8_t i;
    uds_timer_t *tmr;

    for (i = 0; i < UDS_TIEMR_NUM; i++) {
        tmr = &uds_timer[i];
        if (tmr->st == true) {
            // when timer is expired, execute the action and stop the timer.
            // printf("tmr->cnt:%d\n", tmr->cnt);
            if (--tmr->cnt == 0) {
                tmr->act(tmr->parg);
                tmr->cnt = tmr->val;
                tmr->st = false;
            }
        }
    }
}

/**
 * @brief 
 * 
 * @param q 
 * @param fr 
 */
void uds_recv_frame(uds_q_t *q, struct can_frame fr) 
{
    if (fr.can_id == UDS_TP_FUNCTION_ADDR || fr.can_id == UDS_TP_PHYSICAL_ADDR) {
        uds_qenqueue(q, &fr, (uint16_t)(sizeof(struct can_frame)));
    }
}

/**
 * @brief 
 * 
 * @param fr 
 */
void uds_send_frame(struct can_frame *fr)
{   
    uint8_t i;
    /* send action */
#ifdef TEST_WIN32
    printf("can send:");
    for (i = 0; i < fr->can_dlc; i++) {
        printf("%02X ", fr->data[i]);
    }
    printf("\n");
#endif
}

// add for client by rui.peng

/**
 * @brief Initializes protocol stack
 * 
 */
// void uds_init(void);

/**
 * @brief Called on startup
 * 
 */
// void udsapp_init(void);

/**
 * @brief Provides periodic time base
 * 
 */
// void uds_update(void);

/**
 * @brief Called at periodic tick rate
 * 
 */
// void udsapp_update(void);

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
 * @brief Requests a higher level diagnostic session
 * 
 * @param func  : sub function code
 * @param sprsp : suppress response from server
 * @return int 
 */
int uds_req_diagnostic_session(uint8_t func, uint8_t sprsp)
{    
    int result = 0;
    UdsClient client;
    memset(&client, 0x00, sizeof(client));
    // memset(&uds_ap, 0x00, sizeof(uds_ap));
    // memset(&uds_tp, 0x00, sizeof(uds_tp));
    // memset(&uds_dl, 0x00, sizeof(uds_dl));
    uds_tp.out.buf[0] = DiagnosticSessionControl;

    if (func == 0x01)
    {
        uds_tp.out.buf[1] = 0x01;
    } else if (func == 0x02) {
        uds_tp.out.buf[1] = 0x02;
    } else if (func == 0x03) {
        uds_tp.out.buf[1] = 0x03;
    } else if ((func >= 0x40) && (func <= 0x5F)) {

    } else if ((func >= 0x60) && (func <= 0x7E)) {

    }
    
    if (sprsp > 0)
    {
        uds_tp.out.buf[1] = 0x01 + 0x80;
    }
    
    uds_tp.out.pci.dl = 2;

    uds_ap.sts = A_STS_BUSY;
#if 0
    // uds_tp.in.cf_cnt;
    // uds_tp.in.cfg;
    // uds_tp.in.pci;
    // uds_tp.in.ptmr_wc;
    uds_tp.in.sts = N_STS_IDLE;
    // uds_tp.in.buf_pos;
    // uds_tp.in.buf = ;

    
    uds_tp.out.buf_pos = 0;
    uds_tp.out.cf_cnt = 0;
    // uds_tp.out.cfg = 0;
    uds_tp.out.pci.dl = 2u;
    // uds_tp.out.ptmr_wf;
    uds_tp.out.sts = N_STS_REDAY;
    // uds_tp.out.wf_cnt;
    // uds_tp.out.wf_max;

    uds_tp.out.buf[0] = DiagnosticSessionControl;

    printf("func:%d\n", func);
    if (func == 0x01)
    {
        uds_tp.out.buf[1] = 0x01;
    } else if (func == 0x02) {
        uds_tp.out.buf[1] = 0x02;
    } else if (func == 0x03) {
        uds_tp.out.buf[1] = 0x03;
    } else if ((func >= 0x40) && (func <= 0x5F)) {

    } else if ((func >= 0x60) && (func <= 0x7E)) {

    }

    if (sprsp > 0)
    {
        uds_tp.out.buf[1] = 0x01 + 0x80;
    }
#endif
    
    while((result = uds_ap_process(&uds_ap, &uds_tp, &client)) > 0)
    {
        uds_tp_process_out(&uds_tp, &uds_dl);

        uds_dl_process_out(&uds_dl);

        uds_dl_process_in(&uds_dl);

        uds_tp_process_in(&uds_tp, &uds_dl);
    }

    if (client.error == kCLIENT_OK) {
        printf("session positive response\n");
        printf("response data:");
        for (int i=0; i < client.response.buffer_len; i++)
        {
            printf("%02X ", client.response.buffer_ptr[i]);
        }
        printf("\n");
    } else {
        printf("\n");
    }

    return 0;
}

/**
 * @brief Requests a reset of the ECU
 * 
 * @param func 
 * @param sprsp 
 * @param client_ptr 
 * @return int 
 */
int uds_req_ecu_reset(uint8_t func, uint8_t sprsp)
{
    int result = 0;
    UdsClient client;
    memset(&client, 0x00, sizeof(client));
    // memset(&uds_ap, 0x00, sizeof(uds_ap));
    // memset(&uds_tp, 0x00, sizeof(uds_tp));
    // memset(&uds_dl, 0x00, sizeof(uds_dl));
    uds_tp.out.buf[0] = ECUReset;

    if (func == 0x01)
    {
        uds_tp.out.buf[1] = 0x01;
    } else if (func == 0x02) {
        uds_tp.out.buf[1] = 0x02;
    } else if (func == 0x03) {
        uds_tp.out.buf[1] = 0x03;
    } else if ((func >= 0x40) && (func <= 0x5F)) {

    } else if ((func >= 0x60) && (func <= 0x7E)) {

    }
    
    if (sprsp > 0)
    {
        uds_tp.out.buf[1] = 0x01 + 0x80;
    }
    
    uds_tp.out.pci.dl = 2;
    uds_ap.sts = A_STS_BUSY;
    
    while((result = uds_ap_process(&uds_ap, &uds_tp, &client)) > 0)
    {
        uds_tp_process_out(&uds_tp, &uds_dl);

        uds_dl_process_out(&uds_dl);

        uds_dl_process_in(&uds_dl);

        uds_tp_process_in(&uds_tp, &uds_dl);
    }

    if (client.error == kCLIENT_OK) {
        printf("session positive response\n");
        printf("response data:");
        for (int i=0; i < client.response.buffer_len; i++)
        {
            printf("%02X ", client.response.buffer_ptr[i]);
        }
        printf("\n");
    } else {
        printf("\n");
    }

    return 0;
}

/**
 * @brief Requests a secure session
 * 
 * @param func 
 * @param sprsp 
 * @param key 
 * @param klen 
 * @return int 
 */
int uds_req_security_access(uint8_t func, uint8_t sprsp, uint8_t *key, uint8_t klen)
{
    int result = 0;
    UdsClient client;
    memset(&client, 0x00, sizeof(client));
    // memset(&uds_ap, 0x00, sizeof(uds_ap));
    // memset(&uds_tp, 0x00, sizeof(uds_tp));
    // memset(&uds_dl, 0x00, sizeof(uds_dl));
    uds_tp.out.buf[0] = SecurityAccess;

    if (func == 0x01)
    {
        uds_tp.out.buf[1] = 0x01;
    } else if (func == 0x02) {
        uds_tp.out.buf[1] = 0x02;
    } else if (func == 0x03) {
        uds_tp.out.buf[1] = 0x03;
    } else if ((func >= 0x40) && (func <= 0x5F)) {

    } else if ((func >= 0x60) && (func <= 0x7E)) {

    }
    
    if (sprsp > 0)
    {
        uds_tp.out.buf[1] = 0x01 + 0x80;
    }
    
    uds_tp.out.pci.dl = 2;
    uds_ap.sts = A_STS_BUSY;
    
    while((result = uds_ap_process(&uds_ap, &uds_tp, &client)) > 0)
    {
        uds_tp_process_out(&uds_tp, &uds_dl);

        uds_dl_process_out(&uds_dl);

        uds_dl_process_in(&uds_dl);

        uds_tp_process_in(&uds_tp, &uds_dl);
    }

    if (client.error == kCLIENT_OK) {
        printf("session positive response\n");
        printf("response data:");
        for (int i=0; i < client.response.buffer_len; i++)
        {
            printf("%02X ", client.response.buffer_ptr[i]);
        }
        printf("\n");
    } else {
        printf("\n");
    }

    return 0;
}

/**
 * @brief Enable/Disable certain messages
 * 
 * @param func 
 * @param sprsp 
 * @param type 
 * @return int  
 */
int uds_req_communication_control(uint8_t func, uint8_t sprsp, uint8_t type)
{
    int result = 0;
    UdsClient client;
    memset(&client, 0x00, sizeof(client));
    // memset(&uds_ap, 0x00, sizeof(uds_ap));
    // memset(&uds_tp, 0x00, sizeof(uds_tp));
    // memset(&uds_dl, 0x00, sizeof(uds_dl));
    uds_tp.out.buf[0] = CommunicationControl;

    if (func == 0x01)
    {
        uds_tp.out.buf[1] = 0x01;
    } else if (func == 0x02) {
        uds_tp.out.buf[1] = 0x02;
    } else if (func == 0x03) {
        uds_tp.out.buf[1] = 0x03;
    } else if ((func >= 0x40) && (func <= 0x5F)) {

    } else if ((func >= 0x60) && (func <= 0x7E)) {

    }
    
    if (sprsp > 0)
    {
        uds_tp.out.buf[1] = 0x01 + 0x80;
    }
    
    uds_tp.out.pci.dl = 2;
    uds_ap.sts = A_STS_BUSY;
    
    while((result = uds_ap_process(&uds_ap, &uds_tp, &client)) > 0)
    {
        uds_tp_process_out(&uds_tp, &uds_dl);

        uds_dl_process_out(&uds_dl);

        uds_dl_process_in(&uds_dl);

        uds_tp_process_in(&uds_tp, &uds_dl);
    }

    if (client.error == kCLIENT_OK) {
        printf("session positive response\n");
        printf("response data:");
        for (int i=0; i < client.response.buffer_len; i++)
        {
            printf("%02X ", client.response.buffer_ptr[i]);
        }
        printf("\n");
    } else {
        printf("\n");
    }

    return 0;
}

/**
 * @brief Inform server that a tester is present
 * 
 * @param sprsp 
 * @return int  
 */
int uds_req_tester_present(uint8_t sprsp)
{
    int result = 0;
    UdsClient client;
    memset(&client, 0x00, sizeof(client));
    // memset(&uds_ap, 0x00, sizeof(uds_ap));
    // memset(&uds_tp, 0x00, sizeof(uds_tp));
    // memset(&uds_dl, 0x00, sizeof(uds_dl));
    uds_tp.out.buf[0] = TesterPresent;
    
    if (sprsp > 0)
    {
        uds_tp.out.buf[1] = 0x01 + 0x80;
    }
    
    uds_tp.out.pci.dl = 2;
    uds_ap.sts = A_STS_BUSY;
    
    while((result = uds_ap_process(&uds_ap, &uds_tp, &client)) > 0)
    {
        uds_tp_process_out(&uds_tp, &uds_dl);

        uds_dl_process_out(&uds_dl);

        uds_dl_process_in(&uds_dl);

        uds_tp_process_in(&uds_tp, &uds_dl);
    }

    if (client.error == kCLIENT_OK) {
        printf("session positive response\n");
        printf("response data:");
        for (int i=0; i < client.response.buffer_len; i++)
        {
            printf("%02X ", client.response.buffer_ptr[i]);
        }
        printf("\n");
    } else {
        printf("\n");
    }

    return 0;
}

/**
 * @brief 
 * 
 * @param func Read and change link timing
 * @param sprsp 
 * @param key 
 * @param klen 
 * @return int  
 */
int uds_req_access_timing_parameter(uint8_t func, uint8_t sprsp, uint8_t *key, uint8_t klen)
{
    int result = 0;

    return 0;
}

/**
 * @brief Transmit data in a secure manner
 * 
 * @param sd 
 * @param sdlen 
 * @return int
 */
int uds_req_secured_data_transmission(uint8_t *sd, uint8_t sdlen)
{
    int result = 0;

    return 0;
}

/**
 * @brief Halt/resume setting of DTCs
 * 
 * @param func 
 * @param sprsp 
 * @param key 
 * @param klen
 * @return int 
 */
int uds_req_control_dtc_setting(uint8_t func, uint8_t sprsp, uint8_t *key, uint8_t klen)
{
    int result = 0;

    return 0;
}

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
int uds_req_response_on_event(uint8_t evntype, uint8_t wintime, uint8_t *rec, uint8_t reclen, uint8_t *rsp, uint8_t rsplen)
{
    int result = 0;

    return 0;
}

/**
 * @brief Check to see if transition of link baudrate to predefined rate is possible
 * 
 * @param sprsp 
 * @param baud 
 * @return int 
 */
int uds_req_link_control_predef(uint8_t sprsp, uint8_t baud)
{
    int result = 0;

    return 0;
}

/**
 * @brief Check to see if transition of link baudrate to specified rate is possible
 * 
 * @param sprsp 
 * @param baud 
 * @return int  
 */
int uds_req_link_control_user(uint8_t sprsp, uint32_t baud)
{
    int result = 0;

    return 0;
}

/**
 * @brief Transition to previously discussed rate
 * 
 * @param func 
 * @param sprsp 
 * @return int  
 */
int uds_req_link_control(uint8_t func, uint8_t sprsp)
{
    int result = 0;

    return 0;
}

/**
 * @brief Reads data by defined dataIdentifier
 * 
 * @param did 
 * @param dlen 
 * @return int   
 */
int uds_req_read_data_by_id(uint16_t *did, uint8_t dlen)
{
    int result = 0;

    return 0;
}

/**
 * @brief Read memory by address
 * 
 * @param addr 
 * @param size 
 * @return int  
 */
int uds_req_read_memory_by_address(uint32_t addr, uint16_t size)
{
    int result = 0;

    return 0;
}

/**
 * @brief Read scaling data by dataIdentifier
 * 
 * @param id 
 * @return int 
 */
int uds_req_read_scaling_by_id(uint16_t id)
{
    int result = 0;

    return 0;
}

/**
 * @brief Read data by periodic identifier
 * 
 * @param txmode 
 * @param did 
 * @param dlen 
 * @return int  
 */
int uds_req_read_data_by_periodic_id(uint8_t txmode, uint16_t *did, uint8_t dlen)
{
    int result = 0;

    return 0;
}

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
    uint8_t *pos, uint8_t *msize, uint8_t len)
{
    int result = 0;

    return 0;
}

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
    uint16_t *size, uint8_t len)
{
    int result = 0;

    return 0;
}

/**
 * @brief Clear a dynamically defined data Identifier
 * 
 * @param sprsp 
 * @param ddid 
 * @return int 
 */
int uds_req_clear_dynamically_defined_data_id(uint8_t sprsp, uint16_t ddid)
{
    int result = 0;

    return 0;
}

/**
 * @brief Write data specified by dataIdentifier
 * 
 * @param did 
 * @param drec 
 * @param len 
 * @return int 
 */
int uds_req_write_data_by_id(uint16_t did, uint8_t *drec, uint8_t len)
{
    int result = 0;

    return 0;
}

/**
 * @brief Write data to memory by address
 * 
 * @param addr 
 * @param size 
 * @param drec 
 * @return int  
 */
int uds_req_write_memory_by_address(uint32_t addr, uint16_t size, uint8_t *drec)
{
    int result = 0;

    return 0;
}

/**
 * @brief Clear DTC information
 * 
 * @param dtc 
 * @return int   
 */
int uds_req_clear_diagnostic_information(uint32_t dtc)
{
    int result = 0;

    return 0;
}

/**
 * @brief Read DTC info by status mask
 * 
 * @param func 
 * @param sprsp 
 * @param mask 
 * @return int  
 */
int uds_req_read_dtc_info_mask(uint8_t func, uint8_t sprsp, uint8_t mask)
{
    int result = 0;

    return 0;
}

/**
 * @brief Request report of all DTCSnapshots
 * 
 * @param sprsp 
 * @return int  
 */
int uds_req_read_dtc_info_report_snapshot_id(uint8_t sprsp)
{
    int result = 0;

    return 0;
}

/**
 * @brief Request report of DTCSnapshots related to specified DTC
 * 
 * @param sprsp 
 * @param mask 
 * @param recnum 
 * @return int  
 */
int uds_req_read_dtc_info_report_snapshot_by_dtc(uint8_t sprsp, uint32_t mask, uint8_t recnum)
{
    int result = 0;

    return 0;
}

/**
 * @brief Read data from specified DTCSnapshot
 * 
 * @param sprsp 
 * @param recnum 
 * @return int  
 */
int uds_req_read_dtc_info_report_snapshot_by_record(uint8_t sprsp, uint8_t recnum)
{
    int result = 0;

    return 0;
}

/**
 * @brief Read extended data about specified DTC
 * 
 * @param func 
 * @param sprsp 
 * @param mask 
 * @param recnum 
 * @return int  
 */
int uds_req_read_dtc_info_edr_dtc(uint8_t func, uint8_t sprsp, uint32_t mask, uint8_t recnum)
{
    int result = 0;

    return 0;
}

/**
 * @brief Request list of DTCs which match specified severity level
 * 
 * @param func 
 * @param sprsp 
 * @param mask 
 * @return int  
 */
int uds_req_read_dtc_info_severity_info(uint8_t func, uint8_t sprsp, uint16_t mask)
{
    int result = 0;

    return 0;
}

/**
 * @brief Read severity info of specified DTC
 * 
 * @param sprsp 
 * @param mask 
 * @return int  
 */
int uds_req_read_dtc_info_severity_dtc(uint8_t sprsp, uint32_t mask)
{
    int result = 0;

    return 0;
}

/**
 * @brief Catchall function for remaining DTC info sub-functions
 * 
 * @param func 
 * @param sprsp 
 * @return int  
 */
int uds_req_read_dtc_info_misc(uint8_t func, uint8_t sprsp)
{
    int result = 0;

    return 0;
}

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
int uds_req_io_control(uint16_t did, uint8_t *opt, uint8_t optlen, uint8_t *mask, uint8_t masklen)
{
    int result = 0;

    return 0;
}

/**
 * @brief Start/stop stored routine
 * 
 * @param rtype 
 * @param rid 
 * @param ropt 
 * @param optlen 
 * @return int  
 */
int uds_req_routine_control(uint8_t rtype, uint16_t rid, uint8_t *ropt, uint8_t optlen)
{
    int result = 0;

    return 0;
}

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
int uds_req_download(uint8_t dfmtid, uint8_t *maddr, uint8_t maddrlen, uint8_t *msize, uint8_t msizelen)
{
    int result = 0;

    return 0;
}

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
int uds_req_upload(uint8_t dfmtid, uint8_t *maddr, uint8_t maddrlen, uint8_t *msize, uint8_t msizelen)
{
    int result = 0;

    return 0;
}

/**
 * @brief Transfer data after request has been made
 * 
 * @param blckcnt 
 * @param txparam 
 * @param paramlen 
 * @return int 
 */
int uds_req_transfer_data(uint8_t blckcnt, uint8_t *txparam, uint32_t paramlen)
{
    int result = 0;
    UdsClient client;
    memset(&client, 0x00, sizeof(client));
    // memset(&uds_ap, 0x00, sizeof(uds_ap));
    // memset(&uds_tp, 0x00, sizeof(uds_tp));
    // memset(&uds_dl, 0x00, sizeof(uds_dl));
    uds_tp.out.buf[0] = TransferData;
    uds_tp.out.buf[1] = blckcnt;

    memcpy(&uds_tp.out.buf[2], txparam, paramlen);

    uds_tp.out.pci.dl = 2;
    uds_ap.sts = A_STS_BUSY;
    
    while((result = uds_ap_process(&uds_ap, &uds_tp, &client)) > 0)
    {
        uds_tp_process_out(&uds_tp, &uds_dl);

        uds_dl_process_out(&uds_dl);

        uds_dl_process_in(&uds_dl);

        uds_tp_process_in(&uds_tp, &uds_dl);
    }

    if (client.error == kCLIENT_OK) {
        printf("session positive response\n");
        printf("response data:");
        for (int i=0; i < client.response.buffer_len; i++)
        {
            printf("%02X ", client.response.buffer_ptr[i]);
        }
        printf("\n");
    } else {
        printf("session negative response\n");
    }

    return 0;
}

/**
 * @brief Exit data transfer mode
 * 
 * @param sd 
 * @param sdlen 
 * @return int  
 */
int uds_req_transfer_exit(uint8_t *sd, uint8_t sdlen)
{
    int result = 0;

    return 0;
}

/**
 * @brief Automatically create secured session
 * 
 * @param level 
 * @return int  
 */
int uds_create_secured_session(uint8_t level)
{
    int result = 0;

    return 0;
}

/**
 * @brief Processes seed into key for secure session creation
 * 
 * @param seed 
 * @param seedlen 
 * @return int  
 */
int udsapp_process_seed(uint8_t *seed, uint16_t *seedlen)
{
    int result = 0;

    return 0;
}