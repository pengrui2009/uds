/**
 * @file uds_tp.h
 * @author rui.peng (pengrui2009@gmail.com)
 * @brief implementation of network layer and transport layer with
 *        reference to the standard of iso15765, 
 *        1. timeout error will include the N_TIMEOUT_Bs and N_TIMEOUT_Cr except  N_TIMEOUT_A
 *        2. half duplex
 *        3. not supported remote diagnotic
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef UDS_TP_H_
#define UDS_TP_H_

#include "uds_dl.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#define UDS_N_WAITCF_IND    0x00u
#define UDS_N_WAITFC_IND    0x01u
#define UDS_A_S3_IND        0x02u
#define UDS_A_SADELAY_IND   0x03u

#define UDS_TP_BUF_SZ           512u

#define UDS_TP_Cs               (UDS_TP_Cr - UDS_TP_As)
#define UDS_TP_Br               (UDS_TP_Bs - UDS_TP_Ar)


#define UDS_TP_WAIT_FC_TIMEOUT          (UDS_TP_As + UDS_TP_Bs)  /* when we are a sender */
#define UDS_TP_WAIT_CF_TIMEOUT          (UDS_TP_Cr)              /* when we are a receiver, and we got a cf already. */

typedef enum {
    N_PCI_SF = 0u,   /* single frame */
    N_PCI_FF,       /* first frame */
    N_PCI_CF,       /* consecutive frame */
    N_PCI_FC,       /* flow control */
} uds_tp_pci_type_t;


typedef enum {
    N_FS_CTS = 0u,   /* continue to send */
    N_FS_WAIT,      /* wait */
    N_FS_OVFLW,     /* over flow buffer */
} uds_tp_flow_sts_t;


typedef enum {
    N_OK            = 0x0u,
    N_ERROR,
    // N_TIMEOUT_A     = 0x0001u,
    // N_TIMEOUT_Bs    = 0x0002u,
    // N_TIMEOUT_Cr    = 0x0004u,
    // N_WORNG_SN      = 0x0008u,
    // N_INVALID_FS    = 0x0010u,
    // N_UNEXP_PDU     = 0x0020u,
    // N_WFT_OVRN      = 0x0040u,
    // N_BUFFER_OVFLW  = 0x0080u,
} uds_tp_rslt_t;


typedef enum {
    N_TATYPE_PHYSICAL = 0,
    N_TATYPE_FUNCTIONAL,
} uds_tp_tatype_t;


typedef struct {
    uds_tp_pci_type_t   pt;
    uds_tp_flow_sts_t   fs;     /* flow status */
    uint16_t            dl;     /* data length */
    uds_tp_tatype_t     tt;     /* target address type */
    uint8_t             sn;     /* sequence number */
    uint8_t             bs;     /* block size */
    uint8_t             stmin;  /* stmin time */
} uds_tp_pci_t;


typedef enum {
    N_STS_IDLE = 0,
    N_STS_BUSY,
    N_STS_REDAY,
    N_STS_ERROR,
    N_STS_BUSY_WAIT,
} uds_tp_stream_sts_t;


typedef struct {   
    uds_tp_flow_sts_t fs;
    uint8_t bs;
    uint8_t stmin;
} uds_tp_fc_cfg_t;


typedef struct {
    uds_tp_pci_t            pci;
    uds_tp_fc_cfg_t         cfg;        
    uds_tp_stream_sts_t     sts;
    uds_timer_t            *ptmr_wc;
    uint16_t                cf_cnt;     /* sequence number count */
    uint16_t                buf_pos;
    uint8_t                 buf[UDS_TP_BUF_SZ];
} uds_tp_instream_t;


typedef struct {
    uds_tp_pci_t            pci;
    uds_tp_fc_cfg_t         cfg;        
    uds_tp_stream_sts_t     sts;
    uds_timer_t            *ptmr_wf;
    uint16_t                cf_cnt;     /* sequence number count */
    uint16_t                wf_max;     /* fs type is wait, and the max received time, if beyond this conut give up to send the remain cf */
    uint16_t                wf_cnt;     /* if wf_cnt == wf_max, giveup send the remain cf */
    // uint16_t        buf_sz; remove it, because the pci.dl can represent this param
    uint16_t                buf_pos;
    uint8_t                 buf[UDS_TP_BUF_SZ];
} uds_tp_outstream_t;

typedef struct {
    uds_tp_instream_t   in;
    uds_tp_outstream_t  out;
} uds_tp_layer_t;

/**
 * @brief 
 * 
 * @param ptp 
 */
void uds_tp_init(uds_tp_layer_t *ptp) ;

/**
 * @brief 
 * 
 * @param ptp 
 * @param pdl 
 */
void uds_tp_process_in(uds_tp_layer_t *ptp, uds_dl_layer_t *pdl);

/**
 * @brief 
 * 
 * @param ptp 
 * @param pdl 
 * @return uds_tp_rslt_t 
 */
void uds_tp_process_out(uds_tp_layer_t *ptp, uds_dl_layer_t *pdl);

/**
 * @brief 
 * 
 * @param ptp 
 */
void uds_tp_process_to(uds_tp_layer_t *ptp);

#ifdef __cplusplus
}
#endif

#endif /* UDS_TP_H_ */