#include "NetworkManagerSrv.h"
#include <BaseErrDef.h>
#include "SystemServiceAppIf.h"
#include <QtWidgets/QApplication>
#include <QDBusError>
#include <ErrPrintHelper.h>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QResolveConf.h>
#include <QDebug>
#include <QParamParser.h>
#include <inetaddr.h>
#include <SystemSrvApi.h>
#include <QNetworkAddrConverter.h>
#include <FileCreator.h>
#include "DbgLogSwitchDef.h"
#include <unistd.h>

#include "QAutoLock.h"

using namespace SystemSrv;

#define	KEY_NetMgrSrv_DefaultGatewayAddr		"NetworkManagerSrv/DefaultGatewayAddr"
#define	KEY_NetMgrSrv_Dns1				        "NetworkManagerSrv/Dns1"
#define	KEY_NetMgrSrv_Dns2				        "NetworkManagerSrv/Dns2"
#define	KEY_NetMgrSrv_DomainName				"NetworkManagerSrv/DomainName"
#define	KEY_NetMgrSrv_ActionScope           "NetworkManagerSrv/"PARAM_KEY_ActionScope
//SNTP
#define	KEY_NetMgrSrv_PriTimeServer				"SNTP/PriTimeServer"
#define	KEY_NetMgrSrv_SecTimeServer				"SNTP/SecTimeServer"
#define	INI_KEY_LoggingEnable          		    "SNMP/LoggingEnable"
#define KEY_NetMgrSrv_TIMEZONE                  "SNTP/TimeZone"

#define	PARAM_KEY_CommitType		"CommitType"
#define	PARAM_KEY_DefEthDevName		"DefEthDevName"
#define	PARAM_KEY_EthDhcp			"EthDhcp"
#define	PARAM_KEY_EthIP				"EthIP"
#define	PARAM_KEY_EthSubnetMask		"EthSubnetMask"
#define	PARAM_KEY_Gateway			"Gateway"
#define	PARAM_KEY_Dns1				"Dns1"
#define	PARAM_KEY_Dns2				"Dns2"
#define	PARAM_KEY_DomainName		"DomainName"

#define	PARAM_KEY_DefWifiDevName	"DefWifiDevName"
#define	PARAM_KEY_WifiDhcp			"WifiDhcp"
#define	PARAM_KEY_WifiIP			"WifiIP"
#define	PARAM_KEY_WifiSubnetMask	"WifiSubnetMask"

#define	SNTP_PROGRAM_NAME			"sntp"

/*Add by xuweiwei 2014-9-28 for ntpd*/
#define NTPD_PROGRAM_NAME 			"ntpd"

typedef enum {
    ACommit_TYPE_immediatelySaveWired,
    ACommit_TYPE_nextBootWired,
    ACommit_TYPE_immediatelyNoSaveWired,
    ACommit_TYPE_immediatelySaveWireless,
    ACommit_TYPE_nextBootWireless,
    ACommit_TYPE_immediatelyNoSaveWireless,
    ACommit_TYPE_invalid
} NetworkConfigCommit_TYPE, *P_NetworkConfigCommit_TYPE;

QDBusIf_NetworkManagerSrv::QDBusIf_NetworkManagerSrv(QNetworkManagerSrv * pParentObj) : QDBusAbstractAdaptor(pParentObj)
{
	m_pNetworkManagerSrv = pParentObj;
	setAutoRelaySignals(true);
}

void QDBusIf_NetworkManagerSrv::getDefaultGatewayAddr(int & iOutRet, QString & strGatewayAddr)
{
	m_pNetworkManagerSrv->getDefaultGatewayAddr(iOutRet, strGatewayAddr);
}

void QDBusIf_NetworkManagerSrv::getResolveConf(int & iOutRet, QString & strDomain, QString & strNameServer)
{
	m_pNetworkManagerSrv->getResolveConf(iOutRet, strDomain, strNameServer);
}

void QDBusIf_NetworkManagerSrv::readSettings(int & iOutRet, QString & strDefaultGatewayAddr, QString & strDns1, QString & strDns2)
{
	m_pNetworkManagerSrv->readSettings(iOutRet, strDefaultGatewayAddr, strDns1, strDns2);
}

void QDBusIf_NetworkManagerSrv::applySettings(QString strDefaultGateway, QString strDns1, QString strDns2, int & iOutRet)
{
	m_pNetworkManagerSrv->applySettings(strDefaultGateway, strDns1, strDns2, iOutRet);
}

void QDBusIf_NetworkManagerSrv::setWifiStatus(int bEnable, int & iOutRet)
{
    m_pNetworkManagerSrv->setWifiStatus(bEnable, iOutRet);
}

void QDBusIf_NetworkManagerSrv::getWifiStatus(int & iOutRet, int & iStatus)
{
    m_pNetworkManagerSrv->getWifiStatus(iOutRet, iStatus);
}

void QDBusIf_NetworkManagerSrv::getWifiSignalLevel(int & iOutRet, int & iStatus)
{
    m_pNetworkManagerSrv->getWifiSignalLevel(iOutRet, iStatus);
}

void QDBusIf_NetworkManagerSrv::setNetworkCfg(QString strParam, int & iOutRet)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->setNetworkCfg(strParam, iOutRet);		
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::setWifiCfg(QString strParam, int & iOutRet)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->setWifiCfg(strParam, iOutRet);		
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::getWifiCfg(int type, QString & value, int & iOutRet)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->getWifiCfg((DBusWifiType)type, value, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::getNetworkCfg(QString strInParam, int & iOutRet, QString & strOutParam)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->getNetworkCfg(strInParam, iOutRet, strOutParam);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::ResetSnmpdV3Password(int & iOutRet)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->ResetSnmpdV3Password(iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::setSnmpdV3Only(int bEnable, int & iOutRet)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->setSnmpdV3Only(bEnable, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::getSnmpdV3Only(int & iOutRet, int & bEnable)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->getSnmpdV3Only(iOutRet, bEnable);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::setPrimaryTimeServer(QString strServerName, int & iOutRet)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->setPrimaryTimeServer(strServerName, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::getPrimaryTimeServer(int & iOutRet, QString & strServerName)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->getPrimaryTimeServer(iOutRet, strServerName);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::setSecondaryTimeServer(QString strServerName, int & iOutRet)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->setSecondaryTimeServer(strServerName, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::getSecondaryTimeServer(int & iOutRet, QString & strServerName)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->getSecondaryTimeServer(iOutRet, strServerName);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::setTrapReceiverAddress(const QString & strRecvAddr,int & iOutRet)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->setTrapReceiverAddress(strRecvAddr, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::getTrapReceiverAddress(int & iOutRet, QString & strTrapRecvAddr)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->getTrapReceiverAddress(iOutRet,strTrapRecvAddr);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::setSnmpCommunity(const QString & strComunity,int & iOutRet)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->setSnmpCommunity(strComunity, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::getSnmpCommunity(int & iOutRet, QString & strComunity)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->getSnmpCommunity( iOutRet,strComunity);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::setLoggingEnable(int iLogEnable,int & iOutRet)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->setLoggingEnable(iLogEnable, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_NetworkManagerSrv::getLoggingEnable(int & iOutRet,int & iLogEnable)
{
	if(m_pNetworkManagerSrv)
	{
		m_pNetworkManagerSrv->getLoggingEnable(iOutRet, iLogEnable);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}


//QNetworkCfgInfo

QNetworkCfgInfo::QNetworkCfgInfo()
{
	Reset();
}

QNetworkCfgInfo::~QNetworkCfgInfo()
{
}

INT_t QNetworkCfgInfo::Reset()
{
	INT_t iOutRet = ERROR_SUCCESS;

	m_bEthUseDhcp = TRUE;
	m_EthIpV4Addr = 0;
	m_EthSubnetMaskV4Addr = 0;
	m_GatewayV4Addr = 0;	
	
	return iOutRet;
}

//QNetworkManagerSrv

QNetworkManagerSrv::QNetworkManagerSrv()
{
	m_strDefaultIPv4GatewayAddr.clear();
	m_bSntpNeedRetry = FALSE;
}

INT_t QNetworkManagerSrv::InitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	bool bRet;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QDBusConnection> DBusConn_sp;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		DECLARE_CLS_STACK_BUF_STRING(strXappVersion, 32);
		iRet = SysProp_get(INI_KEY_LoggingEnable, OUT strXappVersion);
		if(ERROR_SUCCESS == iRet)
		{
			UpdateMp7xxEnableLoggingGlobalStatusData(QString(strXappVersion).toInt());
		}
		else 
		{
			UpdateMp7xxEnableLoggingGlobalStatusData(true);
		}
				
		//Read settings
		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(FALSE == SysSrvSettingsObj_sp.isNull())
		{
			m_strDefaultIPv4GatewayAddr = SysSrvSettingsObj_sp->value(KEY_NetMgrSrv_DefaultGatewayAddr).toString();
			m_strDns1IPv4Addr = SysSrvSettingsObj_sp->value(KEY_NetMgrSrv_Dns1).toString();
			m_strDns2IPv4Addr = SysSrvSettingsObj_sp->value(KEY_NetMgrSrv_Dns2).toString();
			m_strSntpPriTimerServer = SysSrvSettingsObj_sp->value(KEY_NetMgrSrv_PriTimeServer).toString();
			m_strSntpSecTimerServer = SysSrvSettingsObj_sp->value(KEY_NetMgrSrv_SecTimeServer).toString();
		}

		m_IPv4NetRouteTable_sp = QSharedPointer <CIPv4NetRouteTable> (new CIPv4NetRouteTable);
		if(m_IPv4NetRouteTable_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		m_SntpProcess_sp = QSharedPointer <QProcess> (new QProcess);
		if(m_SntpProcess_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		DBusConn_sp = pSystemServiceAppIf->getDBusConnection();
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		m_DBusIfAdaptor_NetworkManagerSrv_sp = QSharedPointer <QDBusIf_NetworkManagerSrv>(new QDBusIf_NetworkManagerSrv(this));
		if(m_DBusIfAdaptor_NetworkManagerSrv_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		bRet = DBusConn_sp->registerObject(DBUS_PATH_NetworkManagerSrv, this);
		if(false == bRet)
		{
			iOutRet = ERR_REGISTER_FAIL;
			QDBusError QDBusErr = DBusConn_sp->lastError();
			if(QDBusErr.isValid())
			{
				LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
			}
			else
			{
				LOG_BLINE("Failed to reg dbus srv %s\n", DBUS_PATH_NetworkManagerSrv);
			}
			break;
		}

		m_NetMgrEthernet_sp = QSharedPointer <QNetMgrEthernet> (new QNetMgrEthernet());
		if(m_NetMgrEthernet_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		iRet = m_NetMgrEthernet_sp->setDBusConn(DBusConn_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = m_NetMgrEthernet_sp->InitInstance();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		// Wireless manager
        m_NetMgrWifi_sp = QSharedPointer <QNetMgrWifi> (new QNetMgrWifi());
        if (m_NetMgrWifi_sp.isNull()) {
            iOutRet = ERROR_OUT_OF_MEMORY;
			PRINT_BFILE_LINENO_IRET_STR;
            break;
        }
        iRet = m_NetMgrWifi_sp->setDBusConn(DBusConn_sp);
        if (ERROR_SUCCESS != iRet) {
            iOutRet = iRet;
            break;
        }
        iRet = m_NetMgrWifi_sp->InitInstance();
        if (ERROR_SUCCESS != iRet) {
			if(iRet == ERR_NO_DEV) {
				// wifi not found.
			} else {
				iOutRet = iRet;
				PRINT_BFILE_LINENO_IRET_STR;
				break;
			}
        }

		m_SnmpdReloadCfgTimer.setSingleShot(TRUE);
		bRet = connect(&m_SnmpdReloadCfgTimer, SIGNAL(timeout()), this, SLOT(SnmpdReloadCfg()));
		if(false == bRet)
		{
			LOG_BLINE("ConnTimer(SnmpdReloadCfg) err\n");
		}

		m_SntpStartDelayTimer.setSingleShot(TRUE);
		bRet = connect(&m_SntpStartDelayTimer, SIGNAL(timeout()), this, SLOT(RestartSntpProcessIfNeeded()));
		if(false == bRet)
		{
			LOG_BLINE("ConnTimer(StartSntp) err\n");
		}

		bRet = connect(&(*m_SntpProcess_sp), SIGNAL(finished(int, QProcess::ExitStatus)), 
			this, SLOT(SntpFinished(int, QProcess::ExitStatus)));
		if(false == bRet)
		{
			LOG_BLINE("ConnFinishedSig(Sntp) err\n");
		}
		
		m_bSntpNeedRetry = TRUE;
		RestartSntpProcessIfNeeded();
	}while(FALSE);

	return iOutRet;
}

INT_t QNetworkManagerSrv::ExitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QDBusConnection> DBusConn_sp;

	do
	{
		if(qApp->inherits("CSystemServiceAppIf"))
		{
			pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		}
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		DBusConn_sp = pSystemServiceAppIf->getDBusConnection();
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		iRet = StopSntpProcess();
		if(ERR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}

		iRet = m_NetMgrEthernet_sp->ExitInstance();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = m_NetMgrWifi_sp->ExitInstance();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		m_SntpProcess_sp.clear();

		QSharedPointer <QDBusConnection> DBusConn_Null_sp;
		iRet = m_NetMgrEthernet_sp->setDBusConn(DBusConn_Null_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = m_NetMgrWifi_sp->setDBusConn(DBusConn_Null_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		if(DBusConn_sp->isConnected())
		{
			DBusConn_sp->unregisterObject(DBUS_PATH_NetworkManagerSrv);			
		}
	}while(FALSE);

	return iOutRet;
}

const QString QNetworkManagerSrv::getDefaultEthDevName()
{
	QString sDefEthDevName;

	if(m_NetMgrEthernet_sp.isNull())
	{
		LOG_BLINE("BUG,inv state.\n");
		goto EXIT_PROC;
	}

	sDefEthDevName = m_NetMgrEthernet_sp->getDefaultEthDevName();
	
EXIT_PROC:
	return sDefEthDevName;
}

INT_t QNetworkManagerSrv::applyCurSettings()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		iRet = m_NetMgrEthernet_sp->applyCurSettings();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		applySettings(m_strDefaultIPv4GatewayAddr, m_strDns1IPv4Addr, m_strDns2IPv4Addr, OUT iOutRet);
	}while(FALSE);

	return iOutRet;
}

INT_t QNetworkManagerSrv::saveCurSettings()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(SysSrvSettingsObj_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		

		SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_DefaultGatewayAddr, m_strDefaultIPv4GatewayAddr);
		SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_Dns1, m_strDns1IPv4Addr);
		SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_Dns2, m_strDns2IPv4Addr);

		iRet = m_NetMgrEthernet_sp->saveCurSettings();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

void QNetworkManagerSrv::onDhcpDone(const QString & strNetDevName, const QString & strIPv4Addr,const QString & strSubnetMask,
	const QString & strGatewayAddr,const QString & strDns)
{
	m_NetMgrEthernet_sp->onDhcpDone(strNetDevName, strIPv4Addr, strSubnetMask, strGatewayAddr, strDns);
	m_NetMgrWifi_sp->onDhcpDone(strNetDevName, strIPv4Addr, strSubnetMask, strGatewayAddr, strDns);
}

void QNetworkManagerSrv::getDefaultGatewayAddr(int & iOutRet, QString & strGatewayAddr)
{
	INT_t iRet;
	iOutRet = ERROR_SUCCESS;
	UINT32_t uiIPv4GatewayAddr;

	do
	{
		BOOL_t bForceReload = TRUE;
		iRet = m_IPv4NetRouteTable_sp->getDefaultGatewayAddrByNetIfName(&uiIPv4GatewayAddr, 
			m_NetMgrEthernet_sp->getDefaultEthDevName().toUtf8().constData(), bForceReload);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		{
			char szBuf[64];
			iRet = convertIPv4Addr_BinToStr(uiIPv4GatewayAddr, szBuf, sizeof(szBuf));
			if(ERROR_SUCCESS == iRet)
			{
				strGatewayAddr = szBuf;
			}
			else
			{
				strGatewayAddr.clear();
			}
		}
	}while(FALSE);
}

void QNetworkManagerSrv::getResolveConf(int & iOutRet, QString & strDomain, QString & strNameServer)
{
	iOutRet = ERROR_SUCCESS;
	bool bRet;
	QSharedPointer <QFile> ResolveFile_sp(new QFile(DNS_RESOLV_CONF_FILE_PATH));
	QSharedPointer <QTextStream> ResolveStream_sp;

	do
	{
		if(ResolveFile_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		bRet = ResolveFile_sp->open(QIODevice::ReadOnly);
		if(false == bRet)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}

		ResolveStream_sp = QSharedPointer <QTextStream> (new QTextStream(&(*ResolveFile_sp)));
		if(ResolveStream_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		QString OneLineContent;
		while(TRUE)
		{
			OneLineContent = ResolveStream_sp->readLine();
			if(OneLineContent.isNull())
			{
				break;
			}
			QStringList WordList = OneLineContent.split(' ');
			if(1 <= WordList.count())
			{
				if("domain" == WordList[0])
				{
					if(2 <= WordList.count())
					{
						strDomain = WordList[1];
					}
				}
				else if("nameserver" == WordList[0])
				{
					if(2 <= WordList.count())
					{
						if(strNameServer.isNull() || strNameServer.isEmpty())
						{
							strNameServer = WordList[1];
						}
						else
						{
							strNameServer += (";" + WordList[1]);
						}
					}
				}
			}
		}
	}while(FALSE);

	ResolveStream_sp.clear();
	if(FALSE == ResolveFile_sp.isNull())
	{
		ResolveFile_sp->close();
	}
	ResolveFile_sp.clear();
}

INT_t QNetworkManagerSrv::getResolveConf(QString & strDomainName, QStringList & DnsList)
{
	INT_t iOutRet = ERROR_SUCCESS;
	bool bRet;
	QSharedPointer <QFile> ResolveFile_sp(new QFile(DNS_RESOLV_CONF_FILE_PATH));
	QSharedPointer <QTextStream> ResolveStream_sp;

	do
	{
		if(ResolveFile_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		bRet = ResolveFile_sp->open(QIODevice::ReadOnly);
		if(false == bRet)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}

		ResolveStream_sp = QSharedPointer <QTextStream> (new QTextStream(&(*ResolveFile_sp)));
		if(ResolveStream_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		QString OneLineContent;
		while(TRUE)
		{
			OneLineContent = ResolveStream_sp->readLine();
			if(OneLineContent.isNull())
			{
				break;
			}
			QStringList WordList = OneLineContent.split(' ');
			if(1 <= WordList.count())
			{
				if("domain" == WordList[0])
				{
					if(2 <= WordList.count())
					{
						strDomainName = WordList[1];
					}
				}
				else if("nameserver" == WordList[0])
				{
					if(2 <= WordList.count())
					{
						DnsList << WordList[1];
					}
				}
			}
		}
	}while(FALSE);

	ResolveStream_sp.clear();
	if(FALSE == ResolveFile_sp.isNull())
	{
		ResolveFile_sp->close();
	}
	ResolveFile_sp.clear();

	return iOutRet;
}

INT_t QNetworkManagerSrv::getResolveConf(QString & strDomainName, QString & strDns1, QString & strDns2)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QStringList DnsList;

	do
	{
		iRet = getResolveConf(strDomainName, DnsList);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(2 <= DnsList.size())
		{
			strDns1 = DnsList[0];
			strDns2 = DnsList[1];
		}
		else if(1 <= DnsList.size())
		{
			strDns1 = DnsList[0];
			strDns2.clear();
		}
		else
		{
			strDns1.clear();
			strDns2.clear();
		}
	}while(FALSE);

	return iOutRet;
}

void QNetworkManagerSrv::readSettings(int & iOutRet, QString & strDefaultGatewayAddr, QString & strDns1, QString & strDns2)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		strDefaultGatewayAddr = m_strDefaultIPv4GatewayAddr;
		strDns1 = m_strDns1IPv4Addr;
		strDns2 = m_strDns2IPv4Addr;
	}while(FALSE);
}

void QNetworkManagerSrv::applySettings(QString strDefaultGateway, QString strDns1, QString strDns2, int & iOutRet)
{
	INT_t iRet;
	UINT32_t uiIPv4GatewayAddr;
	BOOL_t bForceReload = TRUE;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	BOOL_t bNoDefaultGatewayAddr = FALSE;

	iOutRet = ERROR_SUCCESS;
	
	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(SysSrvSettingsObj_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		

		//Cache the settings
		m_strDefaultIPv4GatewayAddr = strDefaultGateway;
		m_strDns1IPv4Addr = strDns1;
		m_strDns2IPv4Addr = strDns2;
		//Save settings
		SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_DefaultGatewayAddr, m_strDefaultIPv4GatewayAddr);
		SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_Dns1, m_strDns1IPv4Addr);
		SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_Dns2, m_strDns2IPv4Addr);
        if(FALSE == m_NetMgrEthernet_sp->HaveAutoIpSet())
        {
            SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_ActionScope, PARAM_VALUE_SavedCfg);
        }
        else
        {
            SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_ActionScope, PARAM_VALUE_Current);
        }
        
		SysSrvSettingsObj_sp->sync();
		sync();
		//Write the default gateway
		iRet = m_IPv4NetRouteTable_sp->getDefaultGatewayAddrByNetIfName(&uiIPv4GatewayAddr, 
			m_NetMgrEthernet_sp->getDefaultEthDevName().toUtf8().constData(), bForceReload);
		if(ERROR_NOT_FOUND == iRet)
		{
			LOG_BLINE("Not found default_gateway_addr\n");
			uiIPv4GatewayAddr = 0;
			bNoDefaultGatewayAddr = TRUE;
		}
		else if(ERROR_SUCCESS != iRet)
		{
			break;
		}

		if(FALSE == m_NetMgrEthernet_sp->HaveAutoIpSet() || bNoDefaultGatewayAddr)
		{
			iRet = DelDefaultGateway();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
				iOutRet = iRet;
				break;
			}
			iRet = AddDefaultGateway(strDefaultGateway.toLatin1().constData(), NULL);
			if(ERROR_SUCCESS != iRet)
			{
				if(bNoDefaultGatewayAddr && ERROR_SOCKET_IOCTL_FAIL == iRet)
				{
					//we treat this case as a normal one
				}
				else
				{
					if(strDefaultGateway.isEmpty())	//normal case
					{
					}
					else	//error
					{
						LOG_BLINE("ret=%d,GwAddrToSet=%s\n", iRet, strDefaultGateway.toLatin1().constData());
						iOutRet = iRet;
						break;
					}
				}
			}
		}

		//Write DNSs
		if(FALSE == m_NetMgrEthernet_sp->HaveAutoIpSet())
		{
			QResolveConf ResolveConf;
			iRet = ResolveConf.ReloadConf();
            if(ERROR_SUCCESS != iRet)
            {
                LOG_BLINE("%s(%d) load ResolveConf failed %d\n",__FUNCTION__,__LINE__,iRet);
                //iOutRet = iRet;
                //break;
            }
			QStringList DnsList;
			if(strDns1 != "" && TRUE != strDns1.isNull())
			{
				DnsList << strDns1;
			}
			if(strDns2 != "" && TRUE != strDns2.isNull())
			{
				DnsList << strDns2;
			}
			iRet = ResolveConf.setNameServer(DnsList);
			if(ERROR_SUCCESS != iRet)
			{
                LOG_BLINE("%s(%d) setNameServer ResolveConf failed %d\n",__FUNCTION__,__LINE__,iRet);
				iOutRet = iRet;
				break;
			}
			iRet = ResolveConf.SaveConf();
            if(ERROR_SUCCESS != iRet)
            {
                LOG_BLINE("%s(%d) SaveConf ResolveConf failed %d\n",__FUNCTION__,__LINE__,iRet);
                iOutRet = iRet;
                break;
            }
		}
	}while(FALSE);
}

void QNetworkManagerSrv::setNetworkCfg(const QString & strParam, int & iOutRet)
{
	BOOL_t bRet;
	INT_t iRet;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QParamParser oParamParser;
	QParamParser::PROPERTY_VALUE_MAP PropertyValueMap;
	QParamParser::PROPERTY_VALUE_MAP::iterator itValue;
	QString strIpV4Addr, strEthSubnetMask, strGateway, strDomainName, strDns1, strDns2;
	NetworkConfigCommit_TYPE eNetCfgCommitType = ACommit_TYPE_invalid;
	BOOL_t bSaveCfg = FALSE;
	BOOL_t bApplyCfg = FALSE;
	BOOL_t bEthDhcp = FALSE;
	BOOL_t bHasParam_DomainName = FALSE;
	BOOL_t bHasParam_Dns1 = FALSE;
	BOOL_t bHasParam_Dns2 = FALSE;
	
	iOutRet = ERROR_SUCCESS;

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setNetworkCfg,Param=%s\n", qPrintable(strParam));
	}

	do
	{		
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(SysSrvSettingsObj_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		

		iRet = oParamParser.ParseText(strParam, PropertyValueMap);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		itValue = PropertyValueMap.find(PARAM_KEY_CommitType);
		if(itValue != PropertyValueMap.end())
		{
			eNetCfgCommitType = (typeof(eNetCfgCommitType))(*itValue).toInt();
		}
		if(ACommit_TYPE_immediatelySaveWired == eNetCfgCommitType || ACommit_TYPE_nextBootWired == eNetCfgCommitType ||
			ACommit_TYPE_immediatelySaveWireless == eNetCfgCommitType || ACommit_TYPE_nextBootWireless == eNetCfgCommitType)
		{
			bSaveCfg = TRUE;
		}
		if(ACommit_TYPE_immediatelySaveWired == eNetCfgCommitType || 
			ACommit_TYPE_immediatelyNoSaveWired == eNetCfgCommitType || ACommit_TYPE_immediatelyNoSaveWireless == eNetCfgCommitType)
		{
			bApplyCfg = TRUE;
		}

		itValue = PropertyValueMap.find(PARAM_KEY_EthDhcp);
		if(itValue != PropertyValueMap.end())
		{
			if((*itValue).toInt())
			{
				bEthDhcp = TRUE;
			}
		}

		//Ethernet
		if(bEthDhcp)	//Use DHCP
		{
			itValue = PropertyValueMap.find(PARAM_KEY_EthIP);
			if(itValue != PropertyValueMap.end())
			{
				strIpV4Addr = (*itValue);
			}
			
			itValue = PropertyValueMap.find(PARAM_KEY_EthSubnetMask);
			if(itValue != PropertyValueMap.end())
			{
				strEthSubnetMask = (*itValue);
			}
			if(bApplyCfg)
			{
				m_NetMgrEthernet_sp->applySettingsForDefaultDevice(bEthDhcp, strIpV4Addr, strEthSubnetMask, iOutRet, bSaveCfg);
				if(ERROR_SUCCESS != iOutRet)
				{
					break;
				}
			}
		}
		else	//DHCP=0
		{
			BOOL_t bApply = FALSE;
			iRet = m_NetMgrEthernet_sp->setDhcp(bEthDhcp, bApply);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			if(bSaveCfg)
			{
				iRet = m_NetMgrEthernet_sp->saveDhcp(bEthDhcp);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
			
			itValue = PropertyValueMap.find(PARAM_KEY_EthIP);
			if(itValue != PropertyValueMap.end())
			{
				strIpV4Addr = (*itValue);
				bRet = IsValidIpV4Addr(strIpV4Addr.toLatin1(), NULL);
				if(FALSE == bRet)
				{
					LOG_BLINE("strIpV4Addr=%s\n", qPrintable(strIpV4Addr));
					iOutRet = ERR_INVALID_IPv4ADDR;
					break;
				}

				if(bApplyCfg)
				{
					iRet = m_NetMgrEthernet_sp->setIpV4Addr(strIpV4Addr);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_NEWLINE;
						iOutRet = iRet;
						break;
					}
				}
				
				if(bSaveCfg)
				{
					iRet = m_NetMgrEthernet_sp->saveIpV4Addr(strIpV4Addr);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_NEWLINE;
						iOutRet = iRet;
						break;
					}
				}			
			}

			itValue = PropertyValueMap.find(PARAM_KEY_EthSubnetMask);
			if(itValue != PropertyValueMap.end())
			{
				strEthSubnetMask = (*itValue);
				bRet = IsValidIpV4Addr(strEthSubnetMask.toLatin1(), NULL);
				if(FALSE == bRet)
				{
					LOG_BLINE("strEthSubnetMask=%s\n", qPrintable(strEthSubnetMask));
					iOutRet = ERR_INVALID_IPv4ADDR;
					break;
				}
				
				if(bApplyCfg)
				{
					iRet = m_NetMgrEthernet_sp->setSubnetMask(strEthSubnetMask);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_NEWLINE;
						iOutRet = iRet;
						break;
					}
				}

				if(bSaveCfg)
				{
					iRet = m_NetMgrEthernet_sp->saveSubnetMask(strEthSubnetMask);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_NEWLINE;
						iOutRet = iRet;
						break;
					}
				}			
			}			
		}

		if(FALSE == bEthDhcp)	//manual network setting
		{
			itValue = PropertyValueMap.find(PARAM_KEY_Gateway);
			if(itValue != PropertyValueMap.end())
			{
				strGateway = (*itValue);
				bRet = IsValidIpV4Addr(strGateway.toLatin1(), NULL);
				if(FALSE == bRet)
				{
					LOG_BLINE("strGateway=%s\n", qPrintable(strGateway));
					iOutRet = ERR_INVALID_IPv4ADDR;
					break;
				}
				
				if(bApplyCfg)
				{
					BOOL_t bForceReload = TRUE;
					UINT32_t uiIPv4GatewayAddr;
					BOOL_t bNoDefaultGatewayAddr = FALSE;
					UINT32_t IPv4GatewayToSet = 0;
					//Write the default gateway
					iRet = m_IPv4NetRouteTable_sp->getDefaultGatewayAddrByNetIfName(&uiIPv4GatewayAddr, 
						m_NetMgrEthernet_sp->getDefaultEthDevName().toUtf8().constData(), bForceReload);
					if(ERROR_NOT_FOUND == iRet)
					{
						//LOG_BLINE("Not found default_gateway_addr\n");
						uiIPv4GatewayAddr = 0;
						bNoDefaultGatewayAddr = TRUE;
					}
					else if(ERROR_SUCCESS != iRet)
					{
						break;
					}

					iRet = getIPv4AddrFromStr(strGateway.toLatin1(), &IPv4GatewayToSet);
					if(ERROR_SUCCESS != iRet)
					{
						iOutRet = iRet;
						break;
					}
					
					if(bNoDefaultGatewayAddr || IPv4GatewayToSet != uiIPv4GatewayAddr)
					{
						iRet = DelDefaultGateway();
						if(ERROR_SUCCESS != iRet)
						{
							PRINT_BFILE_LINENO_IRET_STR;
							iOutRet = iRet;
							break;
						}
						//LOG_BLINE("strGateway=%s\n", qPrintable(strGateway));
						iRet = AddDefaultGateway(strGateway.toLatin1().constData(), NULL);
						if(ERROR_SUCCESS != iRet)
						{
							if(bNoDefaultGatewayAddr && ERROR_SOCKET_IOCTL_FAIL == iRet)
							{
								//we treat this case as a normal one
							}
							else
							{
								PRINT_BFILE_LINENO_IRET_STR;
								iOutRet = iRet;
								break;
							}
						}
					}
				}
			
				if(bSaveCfg)
				{
					//Cache the settings
					m_strDefaultIPv4GatewayAddr = strGateway;
					SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_DefaultGatewayAddr, strGateway);
				}			
			}
			
			//DomainName, Dns1, Dns2
			itValue = PropertyValueMap.find(PARAM_KEY_DomainName);
			if(itValue != PropertyValueMap.end())
			{
				bHasParam_DomainName = TRUE;
				strDomainName = (*itValue);
			}			
			itValue = PropertyValueMap.find(PARAM_KEY_Dns1);
			if(itValue != PropertyValueMap.end())
			{
				bHasParam_Dns1 = TRUE;
				strDns1 = (*itValue);
				bRet = IsValidIpV4Addr(strDns1.toLatin1(), NULL);
				if(FALSE == bRet)
				{
					LOG_BLINE("strDns1=%s\n", qPrintable(strDns1));
					iOutRet = ERR_INVALID_IPv4ADDR;
					break;
				}				
			}			
			itValue = PropertyValueMap.find(PARAM_KEY_Dns2);
			if(itValue != PropertyValueMap.end())
			{
				bHasParam_Dns2 = TRUE;
				strDns2 = (*itValue);
				bRet = IsValidIpV4Addr(strDns2.toLatin1(), NULL);
				if(FALSE == bRet)
				{
					LOG_BLINE("strDns2=%s\n", qPrintable(strDns2));
					iOutRet = ERR_INVALID_IPv4ADDR;
					break;
				}				
			}			
			if(bApplyCfg)
			{
				//Write DNSs
				QResolveConf ResolveConf;
				//Read current resolv.conf
				iRet = ResolveConf.ReloadConf();
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				//DomainName
				if(bHasParam_DomainName)
				{
					iRet = ResolveConf.setDomainName(strDomainName);
					if(ERROR_SUCCESS != iRet)
					{
						iOutRet = iRet;
						break;
					}					
				}
				//Construct a DNS list
				QStringList DnsList;
				if(strDns1 != "" && TRUE != strDns1.isNull())
				{
					DnsList << strDns1;
				}
				if(strDns2 != "" && TRUE != strDns2.isNull())
				{
					DnsList << strDns2;
				}
				iRet = ResolveConf.setNameServer(DnsList);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				iRet = ResolveConf.SaveConf();
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
			
			if(bSaveCfg)
			{
				//Save them to the config ile
				if(bHasParam_Dns1)
				{
					//Cache the settings
					m_strDns1IPv4Addr = strDns1;
					SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_Dns1, m_strDns1IPv4Addr);
				}
				if(bHasParam_Dns2)
				{
					//Cache the settings
					m_strDns2IPv4Addr = strDns2;
					SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_Dns2, m_strDns2IPv4Addr);
				}
				if(bHasParam_DomainName)
				{
					m_strDomainName = strDomainName;
					SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_DomainName, strDomainName);
				}
			}			
		}
	}while(FALSE);

	SysSrvSettingsObj_sp->sync();
	sync();

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setNetworkCfg,iOutRet=%d\n", iOutRet);
	}
}

void QNetworkManagerSrv::getNetworkCfg(const QString & strInParam, int & iOutRet, QString & strOutParam)
{
	INT_t iRet;
	NetCfg_ActionScope_T eNetCfgActionScope = NetCfg_ActionScope_CURRENT;
	QParamParser oParamParser;
	QParamParser::PROPERTY_VALUE_MAP PropertyValueMap;
	QParamParser::PROPERTY_VALUE_MAP::iterator itValue;
	QNetworkCfgInfo oNetworkCfgInfo;
	
	iOutRet = ERROR_SUCCESS;

	do
	{
		iRet = oParamParser.ParseText(strInParam, PropertyValueMap);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		itValue = PropertyValueMap.find(PARAM_KEY_ActionScope);
		if(itValue != PropertyValueMap.end())
		{
			eNetCfgActionScope = getNetCfgActionScopeFromDesc((*itValue).toLatin1());
			if(NetCfg_ActionScope_UNKNOWN == eNetCfgActionScope)
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
		}

		if(NetCfg_ActionScope_CURRENT == eNetCfgActionScope)
		{
			iRet = getCurNetworkCfgInfo(OUT oNetworkCfgInfo);
		}
		else if(NetCfg_ActionScope_SavedCfg == eNetCfgActionScope)
		{
			iRet = getSavedNetworkCfgInfo(OUT oNetworkCfgInfo);
		}
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		QTextStream(&strOutParam) << 
			PARAM_KEY_Gateway << "=" << QNetworkAddrConverter::getIpAddrFromBin(oNetworkCfgInfo.m_GatewayV4Addr) << "\n" <<
			PARAM_KEY_DomainName << "=" << oNetworkCfgInfo.m_strDomainName << "\n" <<
			PARAM_KEY_Dns1 << "=" << oNetworkCfgInfo.m_strDns1V4Addr << "\n" <<
			PARAM_KEY_Dns2 << "=" << oNetworkCfgInfo.m_strDns2V4Addr << "\n" <<
			PARAM_KEY_DefEthDevName << "=" << oNetworkCfgInfo.m_strDefEthDevName << "\n" <<
			PARAM_KEY_EthDhcp << "=" << oNetworkCfgInfo.m_bEthUseDhcp << "\n" <<
			PARAM_KEY_EthIP << "=" << QNetworkAddrConverter::getIpAddrFromBin(oNetworkCfgInfo.m_EthIpV4Addr) << "\n" <<
			PARAM_KEY_EthSubnetMask << "=" << QNetworkAddrConverter::getIpAddrFromBin(oNetworkCfgInfo.m_EthSubnetMaskV4Addr) << "\n";
	}while(FALSE);
}

void QNetworkManagerSrv::setWifiCfg(const QString & strParam, int & iOutRet)
{
    iOutRet = ERROR_SUCCESS;
    QSharedPointer <QSettings> SysSrvSettingsObj_sp;
    CSystemServiceAppIf * pSystemServiceAppIf = NULL;

    QParamParser oParamParser;
    QParamParser::PROPERTY_VALUE_MAP PropertyValueMap;
    QParamParser::PROPERTY_VALUE_MAP::iterator itValue;

    NetworkConfigCommit_TYPE eNetCfgCommitType = ACommit_TYPE_invalid;

    if (Sw_LogDbusCmd) {
        LOG_BLINE("setWifiCfg, Param=%s\n", qPrintable(strParam));
    }

    do {
        pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
        if (NULL == pSystemServiceAppIf) {
            iOutRet = ERROR_INVALID_STATE;
            break;
        }
        SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
        if (SysSrvSettingsObj_sp.isNull()) {
            iOutRet = ERROR_INVALID_STATE;
            break;
        }

#if 0
        qDebug() << "---strParam---";
        qDebug() << strParam;
        qDebug() << "--------------";
#endif
        // analytic parameter
        WifiCfg cfg;
		cfg.bEnable 		= 0;
        cfg.bDhcp 			= 0;
        cfg.bSaveCfg 		= 0;
        cfg.strIPv4Addr 	= "";
        cfg.strSubnetMask 	= "";
        cfg.strGateway 		= "";
        cfg.strDomainName 	= "";
        cfg.strDns1 		= "";
        cfg.strDns2 		= "";
        cfg.strSSID 		= "";
        cfg.strKey 			= "";
        cfg.strSecurity 	= "";
		cfg.strType 		= "";
        cfg.iWepKeyIndex 	= 0;
		cfg.iChannel 		= 0;

        QStringList lines = strParam.split(QRegExp("\\n"));
        for (QStringList::Iterator it = lines.begin(); it != lines.end(); it++) {
            // split: name=value
            QStringList cols = (*it).split(QRegExp("="));
            if (cols.count() != 2)
                continue;
            if (cols[0] == "CommitType")
                eNetCfgCommitType = (typeof(eNetCfgCommitType))cols[1].toInt();
            else if (cols[0] == "WifiDhcp")
                cfg.bDhcp = cols[1].toInt();
            else if (cols[0] == "WifiIP")
                cfg.strIPv4Addr = cols[1];
            else if (cols[0] == "WifiSubnetMask")
                cfg.strSubnetMask = cols[1];
            else if (cols[0] == "WifiGateway")
                cfg.strGateway = cols[1];
            else if (cols[0] == "WifiDomainName")
                cfg.strDomainName = cols[1];
            else if (cols[0] == "Dns1")
                cfg.strDns1 = cols[1];
            else if (cols[0] == "Dns2")
                cfg.strDns2= cols[1];
            else if (cols[0] == "ssid")
                cfg.strSSID = cols[1];
            else if (cols[0] == "key")
                cfg.strKey = cols[1];
            else if (cols[0] == "channel")
                cfg.iChannel = cols[1].toInt();
            else if (cols[0] == "security")
                cfg.strSecurity = cols[1];
            else if (cols[0] == "wepKeyIndex")
                cfg.iWepKeyIndex = cols[1].toInt();
            else if (cols[0] == "type")
                cfg.strType = cols[1];
        }
        if (eNetCfgCommitType == ACommit_TYPE_immediatelySaveWireless ||
            eNetCfgCommitType == ACommit_TYPE_nextBootWireless) {
            cfg.bSaveCfg = TRUE;
		}
        if (eNetCfgCommitType == ACommit_TYPE_immediatelySaveWireless ||
			eNetCfgCommitType == ACommit_TYPE_immediatelyNoSaveWireless) {
            cfg.bEnable = TRUE;
		}
		m_NetMgrWifi_sp->UpdateNextCfg(cfg);

        // check
        if (!cfg.bDhcp) {
            if (!IsValidIpV4Addr(cfg.strIPv4Addr.toLatin1(), NULL)) {
                LOG_BLINE("strWifiIpV4Addr=%s\n", qPrintable(cfg.strIPv4Addr));
                iOutRet = ERR_INVALID_IPv4ADDR;
                return;
            }
            if (!IsValidIpV4Addr(cfg.strSubnetMask.toLatin1(), NULL)) {
                LOG_BLINE("strWifiSubnetMask=%s\n", qPrintable(cfg.strSubnetMask));
                iOutRet = ERR_INVALID_IPv4ADDR;
                return;
            }
            if (!IsValidIpV4Addr(cfg.strGateway.toLatin1(), NULL)) {
                LOG_BLINE("strWifiGateway=%s\n", qPrintable(cfg.strGateway));
                iOutRet = ERR_INVALID_IPv4ADDR;
                return;
            }
            if (IsValidIpV4Addr(cfg.strDns1.toLatin1(), NULL) == FALSE) {
                LOG_BLINE("strWifiDns1=%s\n", qPrintable(cfg.strDns1));
                iOutRet = ERR_INVALID_IPv4ADDR;
                return;
            }
            if (IsValidIpV4Addr(cfg.strDns2.toLatin1(), NULL) == FALSE) {
                LOG_BLINE("strWifiDns2=%s\n", qPrintable(cfg.strDns2));
                iOutRet = ERR_INVALID_IPv4ADDR;
                return;
            }
        }
		//m_NetMgrWifi_sp->UpdateNextCfg(cfg);
       
        if (cfg.bEnable) {
			if(Sw_LogWifiConnection)
			{
				LOG_BLINE("ApplyCfg=true, so ConnectToAp\n");
			}

            m_NetMgrWifi_sp->setStatus(TRUE, iOutRet);

            if (iOutRet != 0) {
                LOG_BLINE("Failed enable wifi device!\n");
                break;
            }

            iOutRet = m_NetMgrWifi_sp->ConnectToAp();
            if (iOutRet != 0) {
				LOG_BLINE("Failed to Connect AP: err=%d\n", iOutRet);
                m_NetMgrWifi_sp->SendFinishedState(iOutRet, "WifiConnectToApERR");
            }
		}

        if (cfg.bSaveCfg) {
			if(Sw_LogWifiConnection)
			{
				LOG_BLINE("bSaveCfg=true, so saveCurSettings\n");
			}
			m_NetMgrWifi_sp->saveCurSettings();
		}

    } while(FALSE);

    SysSrvSettingsObj_sp->sync();
    sync();

    if (Sw_LogDbusCmd) {
        LOG_BLINE("setNetworkCfg,iOutRet=%d\n", iOutRet);
    }
}

void QNetworkManagerSrv::getWifiCfg(DBusWifiType type, QString & value, int & iOutRet)
{
	if(m_NetMgrWifi_sp)
	{
		m_NetMgrWifi_sp->getDefaultDeviceInfo(type, value, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

INT_t QNetworkManagerSrv::getCurNetworkCfgInfo(OUT QNetworkCfgInfo & oNetworkCfgInfo)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bForceReload = TRUE;

	do
	{
		iRet = oNetworkCfgInfo.Reset();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = m_IPv4NetRouteTable_sp->getDefaultGatewayAddrByNetIfName(&(oNetworkCfgInfo.m_GatewayV4Addr), 
			m_NetMgrEthernet_sp->getDefaultEthDevName().toUtf8().constData(), bForceReload);
		if(ERROR_NOT_FOUND == iRet)
		{
			oNetworkCfgInfo.m_GatewayV4Addr = 0;
		}
		else if(ERROR_SUCCESS != iRet)
		{
			//LOG_BLINE("iRet=%d\n", iRet);
			iOutRet = iRet;
			break;
		}

		iRet = getResolveConf(oNetworkCfgInfo.m_strDomainName, oNetworkCfgInfo.m_strDns1V4Addr, oNetworkCfgInfo.m_strDns2V4Addr);
		if(ERROR_SUCCESS != iRet)
		{
			//LOG_BLINE("iRet=%d\n", iRet);
			iOutRet = iRet;
			break;
		}

		QString strEthIPv4Addr, strEthSubnetMask;
		m_NetMgrEthernet_sp->getDefaultDeviceInfo(iRet, oNetworkCfgInfo.m_strDefEthDevName, strEthIPv4Addr, strEthSubnetMask,
			&(oNetworkCfgInfo.m_bEthUseDhcp), &(oNetworkCfgInfo.m_EthIpV4Addr), &(oNetworkCfgInfo.m_EthSubnetMaskV4Addr));
		if(ERROR_SUCCESS != iRet)
		{
			//LOG_BLINE("iRet=%d\n", iRet);
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QNetworkManagerSrv::getSavedNetworkCfgInfo(OUT QNetworkCfgInfo & oNetworkCfgInfo)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		iRet = oNetworkCfgInfo.Reset();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = getIPv4AddrFromStr(m_strDefaultIPv4GatewayAddr.toLatin1(), &(oNetworkCfgInfo.m_GatewayV4Addr));
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		oNetworkCfgInfo.m_strDomainName = m_strDomainName;
		oNetworkCfgInfo.m_strDns1V4Addr = m_strDns1IPv4Addr;
		oNetworkCfgInfo.m_strDns2V4Addr = m_strDns2IPv4Addr;

		iRet = m_NetMgrEthernet_sp->getSavedNetworkCfgInfo(oNetworkCfgInfo.m_strDefEthDevName, &(oNetworkCfgInfo.m_bEthUseDhcp), 
			&(oNetworkCfgInfo.m_EthIpV4Addr), &(oNetworkCfgInfo.m_EthSubnetMaskV4Addr));
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
	
	return iOutRet;
}

void QNetworkManagerSrv::ResetSnmpdV3Password(int & iOutRet)
{
	INT_t iRet;
	iOutRet = ERROR_SUCCESS;

	do
	{
		iRet = ChangeSnmpdV3PwdDynamically();
		if(ERROR_SUCCESS == iRet)
		{
			break;
		}
		iRet = ChangeSnmpdV3Pwd_RestartSrv();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
}

INT_t QNetworkManagerSrv::ChangeSnmpdV3PwdDynamically()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	QString strCmd;

	do
	{
		strCmd.sprintf("net-snmp-config --create-snmpv3-user -a %s -A MD5 CeLabs", SNMPD_DEFAULT_PASSWORD);
		iRet = system(strCmd.toLatin1());
		if(0 != iRet)
		{
			iOutRet = ERR_CREATE_OBJ_FAIL;
			LOG_BLINE("CreateSnmpV3User err %d\n", iRet);
			break;
		}
		strCmd.sprintf("snmpset -v 1 -c public localhost UCD-SNMP-MIB::versionUpdateConfig.0 i 1");
		iRet = system(strCmd.toLatin1());
		if(0 != iRet)
		{
			iOutRet = ERR_CREATE_OBJ_FAIL;
			LOG_BLINE("SnmpSet(ReloadSnmpdConf) err %d\n", iRet);
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QNetworkManagerSrv::ChangeSnmpdV3Pwd_RestartSrv()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	QString strCmd;

	do
	{
		strCmd.sprintf("/etc/Service/SnmpdSrv stop");
		iRet = system(strCmd.toLatin1());
		if(0 != iRet)
		{
			iOutRet = ERR_CREATE_OBJ_FAIL;
			LOG_BLINE("RestartSrv(Snmpd) err %d\n", iRet);
			break;
		}
		strCmd.sprintf("net-snmp-config --create-snmpv3-user -a %s -A MD5 CeLabs", SNMPD_DEFAULT_PASSWORD);
		iRet = system(strCmd.toLatin1());
		if(0 != iRet)
		{
			iOutRet = ERR_CREATE_OBJ_FAIL;
			LOG_BLINE("CreateSnmpV3User err %d\n", iRet);
			break;
		}
		strCmd.sprintf("/etc/Service/SnmpdSrv start");
		iRet = system(strCmd.toLatin1());
		if(0 != iRet)
		{
			iOutRet = ERR_CREATE_OBJ_FAIL;
			LOG_BLINE("RestartSrv(Snmpd) err %d\n", iRet);
			break;
		}
	}while(FALSE);

	return iOutRet;
}

void QNetworkManagerSrv::setSnmpdV3Only(int bEnable, int & iOutRet)
{
	INT_t iRet = ERROR_SUCCESS;
	bool bRet;
	QString strCmd;
	QFile SnmpdConfFile(SNMPD_CONF_FILE_PATH);
	QString strNewFileContent;
	BOOL_t bFileContentChanged = FALSE;

	iOutRet = ERROR_SUCCESS;

	do
	{
		strNewFileContent.reserve(64*1024);
		bRet = SnmpdConfFile.open(QIODevice::ReadOnly | QIODevice::Text);
		if(FALSE == bRet)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		//Scan per line
		{
			QString strOneLine;
			QTextStream SnmpdConfFile_TextStream(&SnmpdConfFile);
			while(TRUE)
			{
				strOneLine = SnmpdConfFile_TextStream.readLine();
				if(strOneLine.isNull())
				{
					break;
				}
				if("com2sec" == strOneLine.left(7) || "#com2sec" == strOneLine.left(8))
				{
					QString strNewLine;
					QStringList FieldList = strOneLine.split(" ", QString::SkipEmptyParts);
					if(4 == FieldList.size() && ("com2sec" == FieldList[0] || "#com2sec" == FieldList[0]) &&
						"notConfigUser" == FieldList[1] && "localhost" != FieldList[2] && "public" != FieldList[3])
					{
						if(bEnable)
						{
							if("com2sec" == FieldList[0])
							{
								strNewLine = "#" + strOneLine;
								bFileContentChanged = TRUE;
							}
							else
							{
								strNewLine = strOneLine;
							}
						}
						else
						{
							if("#com2sec" == FieldList[0])
							{
								strNewLine = strOneLine.mid(1);
								bFileContentChanged = TRUE;
							}
							else
							{
								strNewLine = strOneLine;
							}
						}
						strNewFileContent += strNewLine + "\n";
					}
					else
					{
						strNewFileContent += strOneLine + "\n";
					}
			} 
			else
			{
				strNewFileContent += strOneLine + "\n";
			}
		}
			if(strNewFileContent.isEmpty())
			{
				iOutRet = ERROR_FILE_READ_FAIL;
				break;
			}
		}
		if(FALSE == bFileContentChanged)
		{
			break;
		}
		//Save the new Contents to the file
		{
			QByteArray FileContent_Ascii = strNewFileContent.toLatin1();
			iRet = BufferDataToFile(SNMPD_CONF_FILE_PATH, (PBYTE)(FileContent_Ascii.constData()), FileContent_Ascii.size());
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		//schedule reloading the conf
		m_SnmpdReloadCfgTimer.start(1*1000);
	}while(FALSE);

	SnmpdConfFile.close();
}

void QNetworkManagerSrv::getSnmpdV3Only(int & iOutRet, int & bEnable)
{
	iOutRet = ERROR_SUCCESS;
	bool bRet;
	QFile SnmpdConfFile(SNMPD_CONF_FILE_PATH);

	bEnable = TRUE;
	
	do
	{
		bRet = SnmpdConfFile.open(QIODevice::ReadOnly | QIODevice::Text);
		if(FALSE == bRet)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		//Scan per line
		{
			QString strOneLine;
			QTextStream SnmpdConfFile_TextStream(&SnmpdConfFile);
			while(TRUE)
			{
				strOneLine = SnmpdConfFile_TextStream.readLine();
				if(strOneLine.isNull())
				{
					break;
				}
				if("com2sec" == strOneLine.left(7))
				{
					QStringList FieldList = strOneLine.split(" ", QString::SkipEmptyParts);
					if(4 == FieldList.size() && ("com2sec" == FieldList[0]) &&
						"notConfigUser" == FieldList[1] && "default" == FieldList[2])
					{
						bEnable = FALSE;
						break;
					}
				}
			}
		}
	}while(FALSE);
	
	SnmpdConfFile.close();
}

void QNetworkManagerSrv::setPrimaryTimeServer(const QString & strServerName, int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(SysSrvSettingsObj_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		
		m_strSntpPriTimerServer = strServerName;
		SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_PriTimeServer, m_strSntpPriTimerServer);
		SysSrvSettingsObj_sp->sync();

		//schedule reloading the conf
		m_SntpStartDelayTimer.start(1*1000);
	}while(FALSE);
}

void QNetworkManagerSrv::getPrimaryTimeServer(int & iOutRet, QString & strServerName)
{
	iOutRet = ERROR_SUCCESS;

	strServerName = m_strSntpPriTimerServer;
}

void QNetworkManagerSrv::setSecondaryTimeServer(const QString & strServerName, int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(SysSrvSettingsObj_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		
		m_strSntpSecTimerServer = strServerName;
		SysSrvSettingsObj_sp->setValue(KEY_NetMgrSrv_SecTimeServer, m_strSntpSecTimerServer);

		//schedule reloading the conf
		m_SntpStartDelayTimer.start(1*1000);
	}while(FALSE);
}

void QNetworkManagerSrv::getSecondaryTimeServer(int & iOutRet, QString & strServerName)
{
	iOutRet = ERROR_SUCCESS;

	strServerName = m_strSntpSecTimerServer;
}

int QNetworkManagerSrv::SnmpdReloadCfg()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QString strCmd;

	do
	{
		m_SnmpdReloadCfgTimer.stop();
		
		strCmd.sprintf("snmpset -v 1 -c public localhost UCD-SNMP-MIB::versionUpdateConfig.0 i 1");
		iRet = system(strCmd.toLatin1());
		if(0 != iRet)
		{
			LOG_BLINE("SnmpSet(ReloadSnmpdConf) err %d\n", iRet);
			strCmd.sprintf("/etc/Service/SnmpdSrv restart");
			iRet = system(strCmd.toLatin1());
			if(0 != iRet)
			{
				LOG_BLINE("RestartSrv(Snmpd) err %d\n", iRet);
				iOutRet = ERROR_CALL_SYSTEM_FAILED;
				break;
			}
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QNetworkManagerSrv::StopSntpProcess()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	int TimeoutMs;
	bool bRet;

	do
	{
		iRet = system("killall " NTPD_PROGRAM_NAME);
		if(0 != iRet)
		{
			//LOG_BLINE("KillSntp err %d\n", iRet);
		}
		if(FALSE == m_SntpProcess_sp.isNull())
		{
			if(QProcess::NotRunning == m_SntpProcess_sp->state())
			{
				break;
			}
			m_SntpProcess_sp->terminate();
			TimeoutMs = (5*1000);
			bRet = m_SntpProcess_sp->waitForFinished(TimeoutMs);
			if(false == bRet)
			{
				LOG_BLINE("SntpProcess temination timeout\n");
				iOutRet = ERR_TIMEOUT;
			}
		}
	}while(FALSE);
	
	return iOutRet;
}

INT_t QNetworkManagerSrv::StartSntpProcess()
{
	INT_t iOutRet = ERROR_SUCCESS;
	QStringList CmdLineParams;

	do
	{
		CmdLineParams << ((QString)("-p"));
		if(FALSE == m_strSntpPriTimerServer.isEmpty())
		{
			CmdLineParams << m_strSntpPriTimerServer;
		}
		if(FALSE == m_strSntpSecTimerServer.isEmpty() && m_strSntpSecTimerServer != m_strSntpPriTimerServer)
		{
			CmdLineParams << m_strSntpSecTimerServer;
		}
		//qDebug() << CmdLineParams;
		m_SntpProcess_sp->setProcessChannelMode(QProcess::ForwardedChannels);
		m_SntpProcess_sp->start(NTPD_PROGRAM_NAME, CmdLineParams, QIODevice::NotOpen);
	}while(FALSE);
	
	return iOutRet;
}

INT_t QNetworkManagerSrv::RestartSntpProcess()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		iRet = StopSntpProcess();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = StartSntpProcess();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
	
	return iOutRet;
}

void QNetworkManagerSrv::RestartSntpProcessIfNeeded()
{
	INT_t iRet;

	do
	{
		m_SntpStartDelayTimer.stop();
		
		if(m_strSntpPriTimerServer.isEmpty() && m_strSntpSecTimerServer.isEmpty())
		{
			break;
		}
		
		iRet = RestartSntpProcess();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(FALSE);
}

void QNetworkManagerSrv::SntpFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	//LOG_BLINE("Sntp:exitCode=%d,exitStatus=%d\n", exitCode, exitStatus);

	if(QProcess::NormalExit == exitStatus)
	{
		if(0 == exitCode)
		{
			emit SntpUpdatedSysTime();
		}
		else
		{
			if(m_bSntpNeedRetry)
			{
				m_bSntpNeedRetry = FALSE;
				RestartSntpProcessIfNeeded();
			}
		}
	}
}

INT_t QNetworkManagerSrv::setTrapReceiverAddress(const QString & strRecvAddr,int & iOutRet)
{
	INT_t iRet = ERROR_SUCCESS;
	bool bRet;
	QString strCmd;
	QFile SnmpdConfFile(SNMPD_CONF_FILE_PATH);
	QString strNewFileContent;
	BOOL_t bFileContentHavetrap2sink = false;

	iOutRet = ERROR_SUCCESS;

	do
	{
		strNewFileContent.reserve(64*1024);
		bRet = SnmpdConfFile.open(QIODevice::ReadOnly | QIODevice::Text);
		if(FALSE == bRet)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		//Scan per line
		{
			QString strOneLine;
			QTextStream SnmpdConfFile_TextStream(&SnmpdConfFile);
			while(TRUE)
			{
				strOneLine = SnmpdConfFile_TextStream.readLine();
				if(strOneLine.isNull())
				{
					break;
				}
				if("trap2sink" == strOneLine.left(9) || "#trap2sink" == strOneLine.left(10))
				{
					bFileContentHavetrap2sink = true;
					QString strNewLine;
					QStringList FieldList = strOneLine.split(" ", QString::SkipEmptyParts);
					if(FieldList.size() == 2)
					{
						strNewFileContent += QString("trap2sink %1").arg(strRecvAddr)  + "\n";;
					}						
				}
				else
				{
					strNewFileContent +=  strOneLine + "\n";
				}	
			}
			if(strNewFileContent.isEmpty())
			{
				iOutRet = ERROR_FILE_READ_FAIL;
				break;
			}
		}
		if(FALSE == bFileContentHavetrap2sink)
		{
			strNewFileContent += QString("\ntrap2sink %1").arg(strRecvAddr) + "\n";
		}
		//Save the new Contents to the file
		{
			QByteArray FileContent_Ascii = strNewFileContent.toLatin1();
			iRet = BufferDataToFile(SNMPD_CONF_FILE_PATH, (PBYTE)(FileContent_Ascii.constData()), FileContent_Ascii.size());
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		//schedule reloading the conf
		m_SnmpdReloadCfgTimer.start(1*1000);
	}while(FALSE);

	SnmpdConfFile.close();
	
	return iOutRet;
}

void QNetworkManagerSrv::getTrapReceiverAddress(int & iOutRet, QString & strTrapRecvAddr)
{
	iOutRet = ERROR_SUCCESS;
	bool bRet;
	QFile SnmpdConfFile(SNMPD_CONF_FILE_PATH);

	do
	{
		bRet = SnmpdConfFile.open(QIODevice::ReadOnly | QIODevice::Text);
		if(FALSE == bRet)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		//Scan per line
		{
			QString strOneLine;
			QTextStream SnmpdConfFile_TextStream(&SnmpdConfFile);
			while(TRUE)
			{
				strOneLine = SnmpdConfFile_TextStream.readLine();
				if(strOneLine.isNull())
				{
					break;
				}
				if("trap2sink" == strOneLine.left(9))
				{
					QStringList FieldList = strOneLine.split(" ", QString::SkipEmptyParts);
					if(2 == FieldList.size())
					{
						strTrapRecvAddr = FieldList [1];
					}
				}
			}
		}
	}while(FALSE);
	
	SnmpdConfFile.close();
}

void QNetworkManagerSrv::setSnmpCommunity(const QString & strCommunity,int & iOutRet)
{
	INT_t iRet = ERROR_SUCCESS;
	bool bRet;
	QString strCmd;
	QFile SnmpdConfFile(SNMPD_CONF_FILE_PATH);
	QString strNewFileContent;
	BOOL_t bFileContentChanged = false;
	BOOL_t bFoundSnmpCommunity = false;

	iOutRet = ERROR_SUCCESS;

	do
	{
		strNewFileContent.reserve(64*1024);
		bRet = SnmpdConfFile.open(QIODevice::ReadOnly | QIODevice::Text);
		if(FALSE == bRet)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		//Scan per line
		{
			QString strOneLine;
			QTextStream SnmpdConfFile_TextStream(&SnmpdConfFile);
			while(TRUE)
			{
				strOneLine = SnmpdConfFile_TextStream.readLine();
				if(strOneLine.isNull())
				{
					break;
				}				
				QString strNewLine;
				QStringList FieldList = strOneLine.split(" ", QString::SkipEmptyParts);
				if(4 == FieldList.size() && (("com2sec" == FieldList[0] || "#com2sec" == FieldList[0]) &&
					"notConfigUser" == FieldList[1] && "default" == FieldList[2]))
				{					
					//LOG_BLINE("Old:\"%s\",New:\"%s\"\n", qPrintable(FieldList[3]), qPrintable(strCommunity));
					bFoundSnmpCommunity = TRUE;
					if(strCommunity != FieldList[3])
					{
						bFileContentChanged = true;
						strNewFileContent += FieldList[0] + " " +  FieldList[1] + " "
							+ FieldList[2] + " " + strCommunity + "\n";
					}
					else
					{
						strNewFileContent += strOneLine + "\n";
					}
				}
				else
				{
					strNewFileContent += strOneLine + "\n";
				}
					
			} 
			if(strNewFileContent.isEmpty())
			{
				iOutRet = ERROR_FILE_READ_FAIL;
				break;
			}
		}
		
		if(FALSE == bFoundSnmpCommunity)
		{
			strNewFileContent += QString("\ncom2sec notConfigUser    default   %1").arg(strCommunity) + "\n";
			bFileContentChanged = true;
		}

		if(FALSE == bFileContentChanged)
		{
			break;
		}
		
		//Save the new Contents to the file
		{
			QByteArray FileContent_Ascii = strNewFileContent.toLatin1();
			iRet = BufferDataToFile(SNMPD_CONF_FILE_PATH, (PBYTE)(FileContent_Ascii.constData()), FileContent_Ascii.size());
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		//schedule reloading the conf
		m_SnmpdReloadCfgTimer.start(1*1000);
	}while(FALSE);

	SnmpdConfFile.close();		
}

void QNetworkManagerSrv::getSnmpCommunity(int & iOutRet, QString & strCommunity)
{
	iOutRet = ERROR_SUCCESS;
	bool bRet;
	QFile SnmpdConfFile(SNMPD_CONF_FILE_PATH);

	strCommunity = "";
	
	do
	{
		bRet = SnmpdConfFile.open(QIODevice::ReadOnly | QIODevice::Text);
		if(FALSE == bRet)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		//Scan per line
		{
			QString strOneLine;
			QTextStream SnmpdConfFile_TextStream(&SnmpdConfFile);
			while(TRUE)
			{
				strOneLine = SnmpdConfFile_TextStream.readLine();
				if(strOneLine.isNull())
				{
					break;
				}
				if("com2sec" == strOneLine.left(7))
				{
					QStringList FieldList = strOneLine.split(" ", QString::SkipEmptyParts);
					if(4 == FieldList.size() && ("com2sec" == FieldList[0]) &&
						"notConfigUser" == FieldList[1] && "default" == FieldList[2])
					{
						strCommunity = FieldList[3];
						break;
					}
				}
			}
		}
	}while(FALSE);
	
	SnmpdConfFile.close();
}

void QNetworkManagerSrv::setLoggingEnable(int iLogEnable,int & iOutRet)
{
	int iRet ;
	iRet = ERROR_SUCCESS;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(iLogEnable < 0 || iLogEnable > 1)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
	
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(SysSrvSettingsObj_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		
		iOutRet = SysProp_setPersistent(INI_KEY_LoggingEnable, QString::number(iLogEnable).toUtf8().constData());
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;	
		}
		else
		{
			UpdateMp7xxEnableLoggingGlobalStatusData(iLogEnable);
		}
	}while(FALSE);
}

INT_t QNetworkManagerSrv::UpdateMp7xxEnableLoggingGlobalStatusData(bool bEnable)
{
	INT_t iOutRet = ERROR_SUCCESS;
	INT_t iRet =ERROR_SUCCESS ;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	
	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);	
		if(NULL == pSystemServiceAppIf)
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}

		CMp7xxGlobalStatus & Mp7xxGlobalStatus = pSystemServiceAppIf->getMp7xxGlobalStatusObj();
		iRet = Mp7xxGlobalStatus.setEnableLogging(bEnable);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		iOutRet = iRet;
	}while(FALSE);

	return iRet;
}

void QNetworkManagerSrv::getLoggingEnable(int & iOutRet,int & iLogEnable)
{
	INT_t iRet =ERROR_SUCCESS ;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	
	iOutRet = ERROR_SUCCESS;
	
	do
	{
		BOOL_t bEnableWriteLoggin = TRUE;
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);	
		if(NULL == pSystemServiceAppIf)
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}

		CMp7xxGlobalStatus & Mp7xxGlobalStatus = pSystemServiceAppIf->getMp7xxGlobalStatusObj();
		iRet = Mp7xxGlobalStatus.getEnableLogging(OUT bEnableWriteLoggin);
		if(ERROR_SUCCESS != iRet)
		{
			iLogEnable = 0 ;
			break;
		}			
		if(bEnableWriteLoggin)	
		{
			iLogEnable = 1;
		}		
	}while(FALSE);
}

void QNetworkManagerSrv::setWifiStatus(int bEnable, int & iOutRet)
{
	if(m_NetMgrWifi_sp)
	{
		m_NetMgrWifi_sp->setStatus(bEnable, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QNetworkManagerSrv::getWifiStatus(int & iOutRet, int & iStatus)
{
	if(m_NetMgrWifi_sp)
	{
		m_NetMgrWifi_sp->getStatus(iOutRet, iStatus);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QNetworkManagerSrv::getWifiSignalLevel(int & iOutRet, int & iStatus)
{
	if(m_NetMgrWifi_sp)
	{
		m_NetMgrWifi_sp->getSignalLevel(iOutRet, iStatus);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}
