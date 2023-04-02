/***********************************************************************
 * @file uds_dl.c
 * @brief  uds data link layer implement
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-24  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022   all right reserved
***********************************************************************/

#include "uds_dl.h"
#include "uds_cfg.def"
#include "virtual_socketcan.h"

#include <stdio.h>
#include <string.h>

/**
 * @brief 
 * 
 * @param pdl 
 */
void uds_dl_init(uds_dl_layer_t *pdl)
{   
    memset((uint8_t *)pdl, 0, sizeof(pdl));

    pdl->in_qf.qstart    = (struct can_frame *)&pdl->in_frs[0];
    pdl->in_qf.qend      = (struct can_frame *)&pdl->in_frs[UDS_DL_IN_SZ];
    pdl->in_qf.qin       = (struct can_frame *)&pdl->in_frs[0];
    pdl->in_qf.qout      = (struct can_frame *)&pdl->in_frs[0];
    pdl->in_qf.qentries  = 0;
    pdl->in_qf.qsize     = UDS_DL_IN_SZ;
    pdl->in.sts          = L_STS_IDLE;

    pdl->out.fr.can_id   = UDS_TP_TRANSPORT_ADDR; 
    pdl->out.fr.can_dlc  = UDS_DL_CAN_DL;
    pdl->out.sts         = L_STS_IDLE;
}


/**
 * @brief 
 * 
 * @param pdl 
 */
void uds_dl_process_in(uds_dl_layer_t *pdl)
{   
    uds_q_rslt result = UDS_Q_OK;
    
    result = uds_qdequeue(&pdl->in_qf, &pdl->in.fr, (uint16_t)(sizeof(struct can_frame)));
    if (result == UDS_Q_OK) 
    {
        pdl->in.sts = L_STS_READY;        
    }

}


/**
 * @brief 
 * 
 * @param pdl 
 */
void uds_dl_process_out(uds_dl_layer_t *pdl)
{
    int ret = 0;
    if (pdl->out.sts == L_STS_READY) {
        //ret = can_tx(&pdl->out.fr);
        ret = can_tx(&pdl->out.fr);
        if (ret)
        {
            printf("can tx frame failed\n");
            return;
        }
        pdl->out.sts = L_STS_IDLE;
    }
}

