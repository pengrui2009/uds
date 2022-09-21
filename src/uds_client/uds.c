/***********************************************************************
 * @file uds.c
 * @brief  
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-12  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022  Jixing. all right reserved
***********************************************************************/


#define UDS_GLOABL
#include "uds.h"


/**
 * @brief uds init
 * 
 * @param channel 
 * @return int 
            0   : success
            <0  : failed
 */
int uds_init(int channel) 
{
    int ret = 0;
    
    if (can_init(channel))
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
    uds_ap_process(&uds_ap, &uds_tp);
    uds_tp_process_out(&uds_tp, &uds_dl);
    uds_dl_process_out(&uds_dl);

    uds_dl_process_in(&uds_dl);
    uds_tp_process_in(&uds_tp, &uds_dl);
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
void uds_recv_frame(uds_q_t *q, can_std_frame_t fr) 
{
    if (fr.id == UDS_TP_FUNCTION_ADDR || fr.id == UDS_TP_PHYSICAL_ADDR) {
        uds_qenqueue(q, &fr, (uint16_t)(sizeof(can_std_frame_t)));
    }
}


/**
 * @brief 
 * 
 * @param fr 
 */
void uds_send_frame(can_std_frame_t *fr)
{   
    uint8_t i;
    /* send action */
#ifdef TEST_WIN32
    printf("can send:");
    for (i = 0; i < fr->dlc; i++) {
        printf("%02X ", fr->dt[i]);
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
 */
void uds_req_diagnostic_session(uint8_t func, uint8_t sprsp)
{
    memset(&uds_tp, 0x00, sizeof(uds_tp));
    memset(&uds_dl, 0x00, sizeof(uds_dl));

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

    uds_tp.out.buf[0] = 0x10;

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

    uds_tp_process_out(&uds_tp, &uds_dl);

    uds_dl_process_out(&uds_dl);

    uds_dl_process_in(&uds_dl);
    
    uds_tp_process_in(&uds_tp, &uds_dl);

}

/**
 * @brief Requests a reset of the ECU
 * 
 * @param func 
 * @param sprsp 
 */
void uds_req_ecu_reset(uint8_t func, uint8_t sprsp);

/**
 * @brief Requests a secure session
 * 
 * @param func 
 * @param sprsp 
 * @param key 
 * @param klen 
 */
void uds_req_security_access(uint8_t func, uint8_t sprsp, uint8_t *key, uint8_t klen);

/**
 * @brief Enable/Disable certain messages
 * 
 * @param func 
 * @param sprsp 
 * @param type 
 */
void uds_req_communication_control(uint8_t func, uint8_t sprsp, uint8_t type);

/**
 * @brief Inform server that a tester is present
 * 
 * @param sprsp 
 */
void uds_req_tester_present(uint8_t sprsp);

/**
 * @brief 
 * 
 * @param func Read and change link timing
 * @param sprsp 
 * @param key 
 * @param klen 
 */
void uds_req_access_timing_parameter(uint8_t func, uint8_t sprsp, uint8_t *key, uint8_t klen);

/**
 * @brief Transmit data in a secure manner
 * 
 * @param sd 
 * @param sdlen 
 */
void uds_req_secured_data_transmission(uint8_t *sd, uint8_t sdlen);

/**
 * @brief Halt/resume setting of DTCs
 * 
 * @param func 
 * @param sprsp 
 * @param key 
 * @param klen 
 */
void uds_req_control_dtc_setting(uint8_t func, uint8_t sprsp, uint8_t *key, uint8_t klen);

/**
 * @brief Automatically respond to certain events with a defined request
 * 
 * @param evntype 
 * @param wintime 
 * @param rec 
 * @param reclen 
 * @param rsp 
 * @param rsplen 
 */
void uds_req_response_on_event(uint8_t evntype, uint8_t wintime, uint8_t *rec, uint8_t reclen, uint8_t *rsp, uint8_t rsplen);

/**
 * @brief Check to see if transition of link baudrate to predefined rate is possible
 * 
 * @param sprsp 
 * @param baud 
 */
void uds_req_link_control_predef(uint8_t sprsp, uint8_t baud);

/**
 * @brief Check to see if transition of link baudrate to specified rate is possible
 * 
 * @param sprsp 
 * @param baud 
 */
void uds_req_link_control_user(uint8_t sprsp, uint32_t baud);

/**
 * @brief Transition to previously discussed rate
 * 
 * @param func 
 * @param sprsp 
 */
void uds_req_link_control(uint8_t func, uint8_t sprsp);

/**
 * @brief Reads data by defined dataIdentifier
 * 
 * @param did 
 * @param dlen 
 */
void uds_req_read_data_by_id(uint16_t *did, uint8_t dlen);

/**
 * @brief Read memory by address
 * 
 * @param addr 
 * @param size 
 */
void uds_req_read_memory_by_address(uint32_t addr, uint16_t size);

/**
 * @brief Read scaling data by dataIdentifier
 * 
 * @param id 
 */
void uds_req_read_scaling_by_id(uint16_t id);

/**
 * @brief Read data by periodic identifier
 * 
 * @param txmode 
 * @param did 
 * @param dlen 
 */
void uds_req_read_data_by_periodic_id(uint8_t txmode, uint16_t *did, uint8_t dlen);

/**
 * @brief Define a dataIdentifier for reading
 * 
 * @param sprsp 
 * @param ddid 
 * @param sdid 
 * @param pos 
 * @param msize 
 * @param len 
 */
void uds_req_dynamically_define_by_data_id(uint8_t sprsp, uint16_t ddid, uint16_t *sdid, 
    uint8_t *pos, uint8_t *msize, uint8_t len);

/**
 * @brief Define a dataIdentifier using a memory address for reading
 * 
 * @param sprsp 
 * @param ddid 
 * @param addr 
 * @param size 
 * @param len 
 */
void uds_req_dynamically_define_by_memory_address(uint8_t sprsp, uint16_t ddid, uint32_t *addr, 
    uint16_t *size, uint8_t len);

/**
 * @brief Clear a dynamically defined data Identifier
 * 
 * @param sprsp 
 * @param ddid 
 */
void uds_req_clear_dynamically_defined_data_id(uint8_t sprsp, uint16_t ddid);

/**
 * @brief Write data specified by dataIdentifier
 * 
 * @param did 
 * @param drec 
 * @param len 
 */
void uds_req_write_data_by_id(uint16_t did, uint8_t *drec, uint8_t len);

/**
 * @brief Write data to memory by address
 * 
 * @param addr 
 * @param size 
 * @param drec 
 */
void uds_req_write_memory_by_address(uint32_t addr, uint16_t size, uint8_t *drec);

/**
 * @brief Clear DTC information
 * 
 * @param dtc 
 */
void uds_req_clear_diagnostic_information(uint32_t dtc);

/**
 * @brief Read DTC info by status mask
 * 
 * @param func 
 * @param sprsp 
 * @param mask 
 */
void uds_req_read_dtc_info_mask(uint8_t func, uint8_t sprsp, uint8_t mask);

/**
 * @brief Request report of all DTCSnapshots
 * 
 * @param sprsp 
 */
void uds_req_read_dtc_info_report_snapshot_id(uint8_t sprsp);

/**
 * @brief Request report of DTCSnapshots related to specified DTC
 * 
 * @param sprsp 
 * @param mask 
 * @param recnum 
 */
void uds_req_read_dtc_info_report_snapshot_by_dtc(uint8_t sprsp, uint32_t mask, uint8_t recnum);

/**
 * @brief Read data from specified DTCSnapshot
 * 
 * @param sprsp 
 * @param recnum 
 */
void uds_req_read_dtc_info_report_snapshot_by_record(uint8_t sprsp, uint8_t recnum);

/**
 * @brief Read extended data about specified DTC
 * 
 * @param func 
 * @param sprsp 
 * @param mask 
 * @param recnum 
 */
void uds_req_read_dtc_info_edr_dtc(uint8_t func, uint8_t sprsp, uint32_t mask, uint8_t recnum);

/**
 * @brief Request list of DTCs which match specified severity level
 * 
 * @param func 
 * @param sprsp 
 * @param mask 
 */
void uds_req_read_dtc_info_severity_info(uint8_t func, uint8_t sprsp, uint16_t mask);

/**
 * @brief Read severity info of specified DTC
 * 
 * @param sprsp 
 * @param mask 
 */
void uds_req_read_dtc_info_severity_dtc(uint8_t sprsp, uint32_t mask);

/**
 * @brief Catchall function for remaining DTC info sub-functions
 * 
 * @param func 
 * @param sprsp 
 */
void uds_req_read_dtc_info_misc(uint8_t func, uint8_t sprsp);

/**
 * @brief Emulate value for input/output signal
 * 
 * @param did 
 * @param opt 
 * @param optlen 
 * @param mask 
 * @param masklen 
 */
void uds_req_io_control(uint16_t did, uint8_t *opt, uint8_t optlen, uint8_t *mask, uint8_t masklen);

/**
 * @brief Start/stop stored routine
 * 
 * @param rtype 
 * @param rid 
 * @param ropt 
 * @param optlen 
 */
void uds_req_routine_control(uint8_t rtype, uint16_t rid, uint8_t *ropt, uint8_t optlen);

/**
 * @brief Request a download to the server
 * 
 * @param dfmtid 
 * @param maddr 
 * @param maddrlen 
 * @param msize 
 * @param msizelen 
 */
void uds_req_download(uint8_t dfmtid, uint8_t *maddr, uint8_t maddrlen, uint8_t *msize, uint8_t msizelen);

/**
 * @brief Request an upload from the server
 * 
 * @param dfmtid 
 * @param maddr 
 * @param maddrlen 
 * @param msize 
 * @param msizelen 
 */
void uds_req_upload(uint8_t dfmtid, uint8_t *maddr, uint8_t maddrlen, uint8_t *msize, uint8_t msizelen);

/**
 * @brief Transfer data after request has been made
 * 
 * @param blckcnt 
 * @param txparam 
 * @param paramlen 
 * @return int 
 */
int uds_req_transfer_data(uint8_t blckcnt, uint8_t *txparam, uint8_t paramlen);

/**
 * @brief Exit data transfer mode
 * 
 * @param sd 
 * @param sdlen 
 */
void uds_req_transfer_exit(uint8_t *sd, uint8_t sdlen);

/**
 * @brief Automatically create secured session
 * 
 * @param level 
 */
void uds_create_secured_session(uint8_t level);

/**
 * @brief Processes seed into key for secure session creation
 * 
 * @param seed 
 * @param seedlen 
 */
void udsapp_process_seed(uint8_t *seed, uint16_t *seedlen);
