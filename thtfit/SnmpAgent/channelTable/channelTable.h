/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 17337 $ of $
 *
 * $Id:$
 */
#ifndef CHANNELTABLE_H
#define CHANNELTABLE_H

#ifdef __cplusplus
extern          "C" {
#endif


/** @addtogroup misc misc: Miscellaneous routines
 *
 * @{
 */
#include <net-snmp/library/asn1.h>
#include "subagent.h"

    /*
     * other required module components 
     */
    /* *INDENT-OFF*  */
config_add_mib(CE-SNMP-MIB)
config_require(CE-SNMP-MIB/channelTable/channelTable_interface)
config_require(CE-SNMP-MIB/channelTable/channelTable_data_access)
config_require(CE-SNMP-MIB/channelTable/channelTable_data_get)
config_require(CE-SNMP-MIB/channelTable/channelTable_data_set)
    /* *INDENT-ON*  */

    /*
     * OID and column number definitions for channelTable 
     */
#include "channelTable_oids.h"

    /*
     * enum definions 
     */
#include "channelTable_enums.h"

    /*
     *********************************************************************
     * function declarations
     */
    void            init_channelTable(void);
    void            shutdown_channelTable(void);

    /*
     *********************************************************************
     * Table declarations
     */
/**********************************************************************
 **********************************************************************
 ***
 *** Table channelTable
 ***
 **********************************************************************
 **********************************************************************/
    /*
     * ce-mib::channelTable is subid 7 of tuner.
     * Its status is Current.
     * OID: .1.3.6.1.4.1.38010.3.8.7, length: 10
     */
    /*
     *********************************************************************
     * When you register your mib, you get to provide a generic
     * pointer that will be passed back to you for most of the
     * functions calls.
     *
     * TODO:100:r: Review all context structures
     */
    /*
     * TODO:101:o: |-> Review channelTable registration context.
     */
    typedef netsnmp_data_list channelTable_registration;

/**********************************************************************/
    /*
     * TODO:110:r: |-> Review channelTable data context structure.
     * This structure is used to represent the data for channelTable.
     */
    /*
     * This structure contains storage for all the columns defined in the
     * channelTable.
     */
    typedef struct channelTable_data_s {

        /*
         * channelIndex(1)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/R/d/h
         */
        long            channelIndex;

        /*
         * channelNumber(2)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/R/d/h
         */
        long            channelNumber;

        /*
         * subChannelCount(3)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/R/d/h
         */
        long            subChannelCount;

        /*
         * programID(4)/OCTETSTR/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/h
         */
        char            programID[128];
        size_t          programID_len;  /* # of char elements, not bytes */

    } channelTable_data;


    /*
     * TODO:120:r: |-> Review channelTable mib index.
     * This structure is used to represent the index for channelTable.
     */
    typedef struct channelTable_mib_index_s {

        /*
         * channelIndex(1)/INTEGER/ASN_INTEGER/long(long)//l/a/w/e/R/d/h
         */
        long            channelIndex;


    } channelTable_mib_index;

    /*
     * TODO:121:r: |   |-> Review channelTable max index length.
     * If you KNOW that your indexes will never exceed a certain
     * length, update this macro to that length.
     */
#define MAX_channelTable_IDX_LEN     1


    /*
     *********************************************************************
     * TODO:130:o: |-> Review channelTable Row request (rowreq) context.
     * When your functions are called, you will be passed a
     * channelTable_rowreq_ctx pointer.
     */
    typedef struct channelTable_rowreq_ctx_s {

    /** this must be first for container compare to work */
        netsnmp_index   oid_idx;
        oid             oid_tmp[MAX_channelTable_IDX_LEN];

        channelTable_mib_index tbl_idx;

        channelTable_data data;

        /*
         * flags per row. Currently, the first (lower) 8 bits are reserved
         * for the user. See mfd.h for other flags.
         */
        u_int           rowreq_flags;

        /*
         * TODO:131:o: |   |-> Add useful data to channelTable rowreq context.
         */

        /*
         * storage for future expansion
         */
        netsnmp_data_list *channelTable_data_list;

    } channelTable_rowreq_ctx;

    typedef struct channelTable_ref_rowreq_ctx_s {
        channelTable_rowreq_ctx *rowreq_ctx;
    } channelTable_ref_rowreq_ctx;

    /*
     *********************************************************************
     * function prototypes
     */
    int             channelTable_pre_request(channelTable_registration *
                                             user_context);
    int             channelTable_post_request(channelTable_registration *
                                              user_context, int rc);

    int             channelTable_rowreq_ctx_init(channelTable_rowreq_ctx *
                                                 rowreq_ctx,
                                                 void *user_init_ctx);
    void            channelTable_rowreq_ctx_cleanup(channelTable_rowreq_ctx
                                                    * rowreq_ctx);


    channelTable_rowreq_ctx
        *channelTable_row_find_by_mib_index(channelTable_mib_index *
                                            mib_idx);

    extern const oid channelTable_oid[];
    extern const int channelTable_oid_size;


#include "channelTable_interface.h"
#include "channelTable_data_access.h"
#include "channelTable_data_get.h"
#include "channelTable_data_set.h"

    /*
     * DUMMY markers, ignore
     *
     * TODO:099:x: *************************************************************
     * TODO:199:x: *************************************************************
     * TODO:299:x: *************************************************************
     * TODO:399:x: *************************************************************
     * TODO:499:x: *************************************************************
     */

#ifdef __cplusplus
}
#endif
#endif                          /* CHANNELTABLE_H */
/** @} */