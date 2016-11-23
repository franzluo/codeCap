/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.scalar.conf 17337 2009-01-01 14:28:29Z magfr $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "subagent.h"
#include "tuner.h"

/** Initializes the tuner module */
void
init_tuner(void)
{
    const oid       playTuner_oid[] = { 1, 3, 6, 1, 4, 1, 38010, 3, 8, 1 };
    const oid       resetDuration_oid[] =
        { 1, 3, 6, 1, 4, 1, 38010, 3, 8, 2 };
    const oid       channelUp_oid[] = { 1, 3, 6, 1, 4, 1, 38010, 3, 8, 3 };
    const oid       channelDown_oid[] =
        { 1, 3, 6, 1, 4, 1, 38010, 3, 8, 4 };
    const oid       tunerMode_oid[] = { 1, 3, 6, 1, 4, 1, 38010, 3, 8, 5 };
    const oid       tunerScan_oid[] = { 1, 3, 6, 1, 4, 1, 38010, 3, 8, 6 };
    const oid       tunerChannelStrength_oid[] =
        { 1, 3, 6, 1, 4, 1, 38010, 3, 8, 8 };
    const oid       forceTxMode_oid[] =
        { 1, 3, 6, 1, 4, 1, 38010, 3, 8, 9 };
	const oid       tunerChannelMode_oid[] =
		{ 1, 3, 6, 1, 4, 1, 38010, 3, 8, 10 };

    DEBUGMSGTL(("tuner", "Initializing\n"));

    netsnmp_register_scalar(netsnmp_create_handler_registration
                            ("playTuner", handle_playTuner, playTuner_oid,
                             OID_LENGTH(playTuner_oid),
                             HANDLER_CAN_RWRITE));
    netsnmp_register_scalar(netsnmp_create_handler_registration
                            ("resetDuration", handle_resetDuration,
                             resetDuration_oid,
                             OID_LENGTH(resetDuration_oid),
                             HANDLER_CAN_RWRITE));
    netsnmp_register_scalar(netsnmp_create_handler_registration
                            ("channelUp", handle_channelUp, channelUp_oid,
                             OID_LENGTH(channelUp_oid),
                             HANDLER_CAN_RWRITE));
    netsnmp_register_scalar(netsnmp_create_handler_registration
                            ("channelDown", handle_channelDown,
                             channelDown_oid, OID_LENGTH(channelDown_oid),
                             HANDLER_CAN_RWRITE));
    netsnmp_register_scalar(netsnmp_create_handler_registration
                            ("tunerMode", handle_tunerMode, tunerMode_oid,
                             OID_LENGTH(tunerMode_oid),
                             HANDLER_CAN_RWRITE));
    netsnmp_register_scalar(netsnmp_create_handler_registration
                            ("tunerScan", handle_tunerScan, tunerScan_oid,
                             OID_LENGTH(tunerScan_oid),
                             HANDLER_CAN_RWRITE));
    netsnmp_register_scalar(netsnmp_create_handler_registration
                            ("tunerChannelStrength",
                             handle_tunerChannelStrength,
                             tunerChannelStrength_oid,
                             OID_LENGTH(tunerChannelStrength_oid),
                             HANDLER_CAN_RONLY));
    netsnmp_register_scalar(netsnmp_create_handler_registration
                            ("forceTxMode", handle_forceTxMode,
                             forceTxMode_oid, OID_LENGTH(forceTxMode_oid),
                             HANDLER_CAN_RONLY));
	netsnmp_register_scalar(netsnmp_create_handler_registration
							("tunerChannelMode", handle_tunerChannelMode,
							tunerChannelMode_oid, OID_LENGTH(tunerChannelMode_oid),
							HANDLER_CAN_RWRITE));
}

int
handle_playTuner(netsnmp_mib_handler *handler,
                 netsnmp_handler_registration *reginfo,
                 netsnmp_agent_request_info *reqinfo,
                 netsnmp_request_info *requests)
{
    int ret, val_len;
    char *pValue;

    UNUSED(handler);
    UNUSED(reginfo);

    switch (reqinfo->mode) {
    case MODE_GET:
        if (DBus_getProperty(SPBus_ControlCenter_UiSrv, "getPlayTuner", NULL,
                             DBUS_TYPE_INT32, &ret,
                             DBUS_TYPE_STRING, &pValue,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
            break;
        }
        if (ret != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_NOSUCHOBJECT);
            break;
        }
        DEBUGMSGTL(("Interface", "%s: getString='%s'\n", __FUNCTION__, pValue));

        snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
                                 (u_char *)pValue, strlen(pValue));
        break;

    case MODE_SET_RESERVE1:
        ret = netsnmp_check_vb_type(requests->requestvb, ASN_OCTET_STR);
        if (ret != SNMP_ERR_NOERROR) {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        break;

    case MODE_SET_RESERVE2:
        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:
        DEBUGMSGTL(("Interface", "%s: MODE_SET_ACTION\n", __FUNCTION__));
        val_len = requests->requestvb->val_len;
        if (val_len > 6) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_WRONGVALUE);
            break;
        }
        pValue = netsnmp_strdup_and_null(requests->requestvb->val.string, val_len);
        DEBUGMSGTL(("Interface", "%s: setString='%s'\n", __FUNCTION__, pValue));

        if (DBus_setProperty(SPBus_ControlCenter_UiSrv, "setPlayTuner", NULL,
                             DBUS_TYPE_STRING, &pValue,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
        }
        SNMP_FREE(pValue);
        break;

    case MODE_SET_COMMIT:
        break;

    case MODE_SET_UNDO:
        break;

    default:
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_playTuner\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int
handle_resetDuration(netsnmp_mib_handler *handler,
                     netsnmp_handler_registration *reginfo,
                     netsnmp_agent_request_info *reqinfo,
                     netsnmp_request_info *requests)
{
    int ret, value;

    UNUSED(handler);
    UNUSED(reginfo);

    switch (reqinfo->mode) {
    case MODE_GET:
        if (DBus_getProperty(SPBus_ControlCenter_UiSrv, "getResetDuration", NULL,
                             DBUS_TYPE_INT32, &ret,
                             DBUS_TYPE_INT32, &value,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
            break;
        }
        if (ret != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_NOSUCHOBJECT);
            break;
        }
        DEBUGMSGTL(("Interface", "%s: getInt='%d'\n", __FUNCTION__, value));

        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                 (u_char *)&value, sizeof(value));
        break;

    case MODE_SET_RESERVE1:
        ret = netsnmp_check_vb_type(requests->requestvb, ASN_INTEGER);
        if (ret != SNMP_ERR_NOERROR) {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        break;

    case MODE_SET_RESERVE2:
        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:
        value = *requests->requestvb->val.integer;
        if (value < 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_WRONGVALUE);
            break;
        }
        DEBUGMSGTL(("Interface", "%s: setInt='%d'\n", __FUNCTION__, value));

        if (DBus_setProperty(SPBus_ControlCenter_UiSrv, "setResetDuration", NULL,
                             DBUS_TYPE_INT32, &value,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
            break;
        }
        break;

    case MODE_SET_COMMIT:
        break;

    case MODE_SET_UNDO:
        break;

    default:
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_resetDuration\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int
handle_channelUp(netsnmp_mib_handler *handler,
                 netsnmp_handler_registration *reginfo,
                 netsnmp_agent_request_info *reqinfo,
                 netsnmp_request_info *requests)
{
    int ret, value;

    UNUSED(handler);
    UNUSED(reginfo);

    switch (reqinfo->mode) {
    case MODE_GET:
		snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
                                 (u_char *) "", 0);

		/*
        if (DBus_getProperty(SPBus_ControlCenter_UiSrv, "getChannelUp", NULL,
                             DBUS_TYPE_INT32, &ret,
                             DBUS_TYPE_INT32, &value,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
            break;
        }
        if (ret != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_NOSUCHOBJECT);
            break;
        }
        DEBUGMSGTL(("Interface", "%s: getInt='%d'\n", __FUNCTION__, value));

        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                 (u_char *)&value, sizeof(value));*/
        break;

    case MODE_SET_RESERVE1:
        ret = netsnmp_check_vb_type(requests->requestvb, ASN_INTEGER);
        if (ret != SNMP_ERR_NOERROR) {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        break;

    case MODE_SET_RESERVE2:
        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:
        value = *requests->requestvb->val.integer;
#if 0
        if (value < 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_WRONGVALUE);
            break;
        }
#endif
        DEBUGMSGTL(("Interface", "%s: setInt='%d'\n", __FUNCTION__, value));

        if (DBus_setProperty(SPBus_ControlCenter_UiSrv, "setChannelUp", NULL,
                             DBUS_TYPE_INT32, &value,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
            break;
        }
        break;

    case MODE_SET_COMMIT:
        break;

    case MODE_SET_UNDO:
        break;

    default:
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_channelUp\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int
handle_channelDown(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{
    int ret, value;

    UNUSED(handler);
    UNUSED(reginfo);

    switch (reqinfo->mode) {
    case MODE_GET:
		snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
                                 (u_char *) "", 0);
        /*if (DBus_getProperty(SPBus_ControlCenter_UiSrv, "getChannelDown", NULL,
                             DBUS_TYPE_INT32, &ret,
                             DBUS_TYPE_INT32, &value,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
            break;
        }
        if (ret != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_NOSUCHOBJECT);
            break;
        }
        DEBUGMSGTL(("Interface", "%s: getInt='%d'\n", __FUNCTION__, value));

        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                 (u_char *)&value, sizeof(value));*/
        break;

    case MODE_SET_RESERVE1:
        ret = netsnmp_check_vb_type(requests->requestvb, ASN_INTEGER);
        if (ret != SNMP_ERR_NOERROR) {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        break;

    case MODE_SET_RESERVE2:
        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:
        value = *requests->requestvb->val.integer;
#if 0
        if (value < 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_WRONGVALUE);
            break;
        }
#endif
        DEBUGMSGTL(("Interface", "%s: setInt='%d'\n", __FUNCTION__, value));

        if (DBus_setProperty(SPBus_ControlCenter_UiSrv, "setChannelDown", NULL,
                             DBUS_TYPE_INT32, &value,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
            break;
        }
        break;

    case MODE_SET_COMMIT:
        break;

    case MODE_SET_UNDO:
        break;

    default:
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_channelDown\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int
handle_tunerMode(netsnmp_mib_handler *handler,
                 netsnmp_handler_registration *reginfo,
                 netsnmp_agent_request_info *reqinfo,
                 netsnmp_request_info *requests)
{
    int ret, value;

    UNUSED(handler);
    UNUSED(reginfo);

    switch (reqinfo->mode) {
    case MODE_GET:
        if (DBus_getProperty(SPBus_ControlCenter_UiSrv, "getTunerMode", NULL,
                             DBUS_TYPE_INT32, &ret,
                             DBUS_TYPE_INT32, &value,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
            break;
        }
        if (ret != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_NOSUCHOBJECT);
            break;
        }
        DEBUGMSGTL(("Interface", "%s: getInt='%d'\n", __FUNCTION__, value));

        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                 (u_char *)&value, sizeof(value));
        break;

    case MODE_SET_RESERVE1:
        ret = netsnmp_check_vb_type(requests->requestvb, ASN_INTEGER);
        if (ret != SNMP_ERR_NOERROR) {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        break;

    case MODE_SET_RESERVE2:
        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:
        value = *requests->requestvb->val.integer;
        if (value < 0 || value > 2) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_WRONGVALUE);
            break;
        }
        DEBUGMSGTL(("Interface", "%s: setInt='%d'\n", __FUNCTION__, value));

        if (DBus_setProperty(SPBus_ControlCenter_UiSrv, "setTunerMode", NULL,
                             DBUS_TYPE_INT32, &value,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
            break;
        }
        break;

    case MODE_SET_COMMIT:
        break;

    case MODE_SET_UNDO:
        break;

    default:
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_tunerMode\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int
handle_tunerScan(netsnmp_mib_handler *handler,
                 netsnmp_handler_registration *reginfo,
                 netsnmp_agent_request_info *reqinfo,
                 netsnmp_request_info *requests)
{
    int ret, value;

    UNUSED(handler);
    UNUSED(reginfo);

    switch (reqinfo->mode) {
    case MODE_GET:
        if (DBus_getProperty(SPBus_ControlCenter_UiSrv, "getTunerScan", NULL,
                             DBUS_TYPE_INT32, &ret,
                             DBUS_TYPE_INT32, &value,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
            break;
        }
        if (ret != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_NOSUCHOBJECT);
            break;
        }
        DEBUGMSGTL(("Interface", "%s: getInt='%d'\n", __FUNCTION__, value));

        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                 (u_char *)&value, sizeof(value));
        break;

    case MODE_SET_RESERVE1:
        ret = netsnmp_check_vb_type(requests->requestvb, ASN_INTEGER);
        if (ret != SNMP_ERR_NOERROR) {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        break;

    case MODE_SET_RESERVE2:
        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:
        value = *requests->requestvb->val.integer;
        if (value < 0 || value > 2) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_WRONGVALUE);
            break;
        }
        DEBUGMSGTL(("Interface", "%s: setInt='%d'\n", __FUNCTION__, value));

        if (DBus_setProperty(SPBus_ControlCenter_UiSrv, "setTunerScan", NULL,
                             DBUS_TYPE_INT32, &value,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
            break;
        }
        break;

    case MODE_SET_COMMIT:
        break;

    case MODE_SET_UNDO:
        break;

    default:
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_tunerScan\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int
handle_tunerChannelStrength(netsnmp_mib_handler *handler,
                            netsnmp_handler_registration *reginfo,
                            netsnmp_agent_request_info *reqinfo,
                            netsnmp_request_info *requests)
{
    int ret, value;
    UNUSED(handler);
    UNUSED(reginfo);
    UNUSED(requests);

    switch (reqinfo->mode) {
    case MODE_GET:
        if (DBus_getProperty(SPBus_ControlCenter_UiSrv, "getTunerChannelStrength", NULL,
                             DBUS_TYPE_INT32, &ret,
                             DBUS_TYPE_INT32, &value,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
            break;
        }
        if (ret != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
            break;
        }
        DEBUGMSGTL(("Interface", "%s: getInt='%d'\n", __FUNCTION__, value));

        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                 (u_char *)&value, sizeof(value));
        break;

    default:
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_tunerChannelStrength\n", reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int
handle_forceTxMode(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{
    int ret, value;
    UNUSED(handler);
    UNUSED(reginfo);
    UNUSED(requests);

    switch (reqinfo->mode) {
    case MODE_GET:
        if (DBus_getProperty(SPBus_ControlCenter_UiSrv, "getForceTxMode", NULL,
                             DBUS_TYPE_INT32, &ret,
                             DBUS_TYPE_INT32, &value,
                             DBUS_TYPE_INVALID) != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
            break;
        }
        if (ret != 0) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_NOSUCHOBJECT);
            break;
        }
        DEBUGMSGTL(("Interface", "%s: getInt='%d'\n", __FUNCTION__, value));

        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                 (u_char *)&value, sizeof(value));
        break;

    default:
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_forceTxMode\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int
	handle_tunerChannelMode(netsnmp_mib_handler *handler,
	netsnmp_handler_registration *reginfo,
	netsnmp_agent_request_info *reqinfo,
	netsnmp_request_info *requests)
{
	int ret, value;

	UNUSED(handler);
	UNUSED(reginfo);

	switch (reqinfo->mode) {
	case MODE_GET:
		if (DBus_getProperty(SPBus_ControlCenter_UiSrv, "getTunerChannelMode", NULL,
			DBUS_TYPE_INT32, &ret,
			DBUS_TYPE_INT32, &value,
			DBUS_TYPE_INVALID) != 0) {
				netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
				break;
		}
		if (ret != 0) {
			netsnmp_set_request_error(reqinfo, requests, SNMP_NOSUCHOBJECT);
			break;
		}
		DEBUGMSGTL(("Interface", "%s: getInt='%d'\n", __FUNCTION__, value));

		snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
			(u_char *)&value, sizeof(value));
		break;

	case MODE_SET_RESERVE1:
		ret = netsnmp_check_vb_type(requests->requestvb, ASN_INTEGER);
		if (ret != SNMP_ERR_NOERROR) {
			netsnmp_set_request_error(reqinfo, requests, ret);
		}
		break;

	case MODE_SET_RESERVE2:
		break;

	case MODE_SET_FREE:
		break;

	case MODE_SET_ACTION:
		value = *requests->requestvb->val.integer;
		if (value < 1 || value > 4) {
			netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_WRONGVALUE);
			break;
		}
		DEBUGMSGTL(("Interface", "%s: setInt='%d'\n", __FUNCTION__, value));

		if (DBus_setProperty(SPBus_ControlCenter_UiSrv, "setTunerChannelMode", NULL,
			DBUS_TYPE_INT32, &value,
			DBUS_TYPE_INVALID) != 0) {
				netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
				break;
		}

		break;

	case MODE_SET_COMMIT:
		break;

	case MODE_SET_UNDO:
		break;

	default:
		snmp_log(LOG_ERR, "unknown mode (%d) in handle_tunerChannelMode\n",
			reqinfo->mode);
		return SNMP_ERR_GENERR;
	}

	return SNMP_ERR_NOERROR;
}
