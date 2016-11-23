/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 14170 $ of $ 
 *
 * $Id:$
 */
/*
 * standard Net-SNMP includes 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <stdlib.h>

/*
 * include our parent header 
 */
#include "fileListingTable.h"


#include "fileListingTable_data_access.h"

/** @ingroup interface
 * @addtogroup data_access data_access: Routines to access data
 *
 * These routines are used to locate the data used to satisfy
 * requests.
 * 
 * @{
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table fileListingTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * CE-SNMP-MIB::fileListingTable is subid 6 of media.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.38010.3.4.6, length: 10
 */

/**
 * initialization for fileListingTable data access
 *
 * This function is called during startup to allow you to
 * allocate any resources you need for the data table.
 *
 * @param fileListingTable_reg
 *        Pointer to fileListingTable_registration
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : unrecoverable error.
 */
int
fileListingTable_init_data(fileListingTable_registration *
                           fileListingTable_reg)
{
    UNUSED(fileListingTable_reg);

    DEBUGMSGTL(("verbose:fileListingTable:fileListingTable_init_data",
                "called\n"));

    /*
     * TODO:303:o: Initialize fileListingTable data.
     */
    /*
     ***************************************************
     ***             START EXAMPLE CODE              ***
     ***---------------------------------------------***/
    /*
     * if you are the sole writer for the file, you could
     * open it here. However, as stated earlier, we are assuming
     * the worst case, which in this case means that the file is
     * written to by someone else, and might not even exist when
     * we start up. So we can't do anything here.
     */
    /*
     ***---------------------------------------------***
     ***              END  EXAMPLE CODE              ***
     ***************************************************/

    return MFD_SUCCESS;
}                               /* fileListingTable_init_data */

/**
 * container overview
 *
 */

/**
 * container initialization
 *
 * @param container_ptr_ptr A pointer to a container pointer. If you
 *        create a custom container, use this parameter to return it
 *        to the MFD helper. If set to NULL, the MFD helper will
 *        allocate a container for you.
 *
 *  This function is called at startup to allow you to customize certain
 *  aspects of the access method. For the most part, it is for advanced
 *  users. The default code should suffice for most cases. If no custom
 *  container is allocated, the MFD code will create one for your.
 *
 * @remark
 *  This would also be a good place to do any initialization needed
 *  for you data source. For example, opening a connection to another
 *  process that will supply the data, opening a database, etc.
 */
void
fileListingTable_container_init(netsnmp_container ** container_ptr_ptr,
                                netsnmp_cache *cache)
{
    DEBUGMSGTL(("verbose:fileListingTable:fileListingTable_container_init",
                "called\n"));

    if (NULL == container_ptr_ptr) {
        snmp_log(LOG_ERR,
                 "bad container param to fileListingTable_container_init\n");
        return;
    }

    *container_ptr_ptr = NULL;

    if (NULL == cache) {
        snmp_log(LOG_ERR,"bad cache param to fileListingTable_container_init\n");
        return;
    }

    /*
     * For advanced users, you can use a custom container. If you
     * do not create one, one will be created for you.
     */
    cache->timeout = FILELISTINGTABLE_CACHE_TIMEOUT;

}                               /* fileListingTable_container_init */

/**
 * container shutdown
 *
 * @param container_ptr A pointer to the container.
 *
 *  This function is called at shutdown to allow you to customize certain
 *  aspects of the access method. For the most part, it is for advanced
 *  users. The default code should suffice for most cases.
 *
 *  This function is called before fileListingTable_container_free().
 *
 * @remark
 *  This would also be a good place to do any cleanup needed
 *  for you data source. For example, closing a connection to another
 *  process that supplied the data, closing a database, etc.
 */
void
fileListingTable_container_shutdown(netsnmp_container * container_ptr)
{
    DEBUGMSGTL(("verbose:fileListingTable:fileListingTable_container_shutdown", "called\n"));

    if (NULL == container_ptr) {
        snmp_log(LOG_ERR,
                 "bad params to fileListingTable_container_shutdown\n");
        return;
    }

}                               /* fileListingTable_container_shutdown */

/**
 * load initial data
 *
 * TODO:350:M: Implement fileListingTable data load
 *
 * @param container container to which items should be inserted
 *
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_RESOURCE_UNAVAILABLE : Can't access data source
 * @retval MFD_ERROR                : other error.
 *
 *  This function is called to load the index(es) (and data, optionally)
 *  for the every row in the data set.
 *
 * @remark
 *  While loading the data, the only important thing is the indexes.
 *  If access to your data is cheap/fast (e.g. you have a pointer to a
 *  structure in memory), it would make sense to update the data here.
 *  If, however, the accessing the data invovles more work (e.g. parsing
 *  some other existing data, or peforming calculations to derive the data),
 *  then you can limit yourself to setting the indexes and saving any
 *  information you will need later. Then use the saved information in
 *  fileListingTable_row_prep() for populating data.
 *
 * @note
 *  If you need consistency between rows (like you want statistics
 *  for each row to be from the same time frame), you should set all
 *  data here.
 *
 */
int
fileListingTable_container_load(netsnmp_container * container)
{
    fileListingTable_rowreq_ctx *rowreq_ctx;

    /*
     * temporary storage for index values
     */
    /*
     * fileIndex(1)/INTEGER/ASN_INTEGER/long(long)//l/a/w/e/R/d/h
     */
    char szFileName[256];
    int iIndex, iArray, ret;
    unsigned int iFileName;
    int64_t iFileSize;
    const char **pArray;
    char *p;

    DEBUGMSGTL(("verbose:fileListingTable:fileListingTable_container_load",
                "called\n"));

    SPArgs args;
    args.eType = SPValue_String;
    args.pString = FileTypeIndexCurrent();
    if (DBus_getProperty(SPBus_WebBrowser_DataCommSrv,
                         "getFileList",
                         &args, 
                         DBUS_TYPE_INT32, &ret,
                         DBUS_TYPE_ARRAY, DBUS_TYPE_STRING, &pArray, &iArray,
                         DBUS_TYPE_INVALID) != 0 ||
        ret != 0) {
        return MFD_RESOURCE_UNAVAILABLE;
    }

    /*
     ***---------------------------------------------***
     ***              END  EXAMPLE CODE              ***
     ***************************************************/
    /*
     * TODO:351:M: |-> Load/update data in the fileListingTable container.
     * loop over your fileListingTable data, allocate a rowreq context,
     * set the index(es) [and data, optionally] and insert into
     * the container.
     */
    for (iIndex = 0; iIndex < iArray; iIndex++) {
        iFileName = strlen(*(pArray + iIndex));
        if ((iFileName + 1) > sizeof(szFileName)) {
            DEBUGMSGTL(("Interface", "ERROR: filename too long.\n"));
            break;
        }
        strncpy(szFileName, *(pArray + iIndex), iFileName);
        szFileName[iFileName] = 0;

        p = strchr(szFileName, '\n');
        if (!p) {
            DEBUGMSGTL(("Interface", "ERROR: Cannot read the file size information.\n"));
            break;
        }
        *p++ = 0;
        iFileName = strlen(szFileName);
        iFileSize = atoll(p);
#if 0
        DEBUGMSGTL(("Interface", "Index %d: fileName='%s' fileSize='%llu'\n",
                    iIndex, szFileName, iFileSize));
#endif
        rowreq_ctx = fileListingTable_allocate_rowreq_ctx(NULL);
        if (NULL == rowreq_ctx) {
            snmp_log(LOG_ERR, "memory allocation failed\n");
            return MFD_RESOURCE_UNAVAILABLE;
        }
        if (MFD_SUCCESS != fileListingTable_indexes_set(rowreq_ctx, iIndex)) {
            snmp_log(LOG_ERR, "error setting index while loading fileListingTable data.\n");
            fileListingTable_release_rowreq_ctx(rowreq_ctx);
            continue;
        }

        /*
         * TODO:352:r: |   |-> populate fileListingTable data context.
         * Populate data context here. (optionally, delay until row prep)
         */
        /*
         * TRANSIENT or semi-TRANSIENT data:
         * copy data or save any info needed to do it in row_prep.
         */
        /*
         * setup/save data for fileName
         * fileName(2)/OCTETSTR/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/h
         */
        
        rowreq_ctx->data.fileName_len = iFileName;
        memcpy(rowreq_ctx->data.fileName, szFileName, iFileName);

        /*
         * make sure there is enough space for fileName data
         */
        if ((NULL == rowreq_ctx->data.fileName) ||
            (rowreq_ctx->data.fileName_len < (iFileName * sizeof(szFileName[0])))) {
            snmp_log(LOG_ERR, "not enough space for value\n");
            return MFD_ERROR;
        }

        /*
         * setup/save data for fileSize
         * fileSize(3)/COUNTER64/ASN_COUNTER64/U64(U64)//l/A/w/e/r/d/h
         */
        rowreq_ctx->data.fileSize.high = (iFileSize >> 32) & 0xffffffff;
        rowreq_ctx->data.fileSize.low  = iFileSize & 0xffffffff;

        // insert into table container
        CONTAINER_INSERT(container, rowreq_ctx);
    }

    DEBUGMSGT(("verbose:fileListingTable:fileListingTable_container_load",
               "inserted %d records\n", iIndex));

    return MFD_SUCCESS;
}                               /* fileListingTable_container_load */

/**
 * container clean up
 *
 * @param container container with all current items
 *
 *  This optional callback is called prior to all
 *  item's being removed from the container. If you
 *  need to do any processing before that, do it here.
 *
 * @note
 *  The MFD helper will take care of releasing all the row contexts.
 *
 */
void
fileListingTable_container_free(netsnmp_container * container)
{
    UNUSED(container);
    DEBUGMSGTL(("verbose:fileListingTable:fileListingTable_container_free",
                "called\n"));

    /*
     * TODO:380:M: Free fileListingTable container data.
     */
}                               /* fileListingTable_container_free */

/**
 * prepare row for processing.
 *
 *  When the agent has located the row for a request, this function is
 *  called to prepare the row for processing. If you fully populated
 *  the data context during the index setup phase, you may not need to
 *  do anything.
 *
 * @param rowreq_ctx pointer to a context.
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 */
int
fileListingTable_row_prep(fileListingTable_rowreq_ctx * rowreq_ctx)
{
    DEBUGMSGTL(("verbose:fileListingTable:fileListingTable_row_prep",
                "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:390:o: Prepare row for request.
     * If populating row data was delayed, this is the place to
     * fill in the row for this request.
     */

    return MFD_SUCCESS;
}                               /* fileListingTable_row_prep */

/** @} */
