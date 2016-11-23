#include "DbgLogSwitchDef.h"
#include <DbgLogSwitch.h>
#include <BaseErrDef.h>
#include <stdio.h>
#include <ErrPrintHelper.h>

#define	DbgLogSwDesc_LogDbugCmd				"LogDbusCmd"
#define	DbgLogSwDesc_LogPlaybackStatus		"LogPlaybackStatus"
#define	DbgLogSwDesc_LogJavascript			"LogJavascript"
#define	DbgLogSwDesc_LogGpioButton			"LogGpioButton"

static BOOL_t g_Disabled = FALSE;
BOOL_t * g_pLogDbusCmdSw = &g_Disabled;
BOOL_t * g_pLogPlaybackStatus = &g_Disabled;
BOOL_t * g_pLogJavascript = &g_Disabled;
BOOL_t * g_pLogGpioButton = &g_Disabled;

INT_t RegDbgLogSwitch()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t * pbRet = NULL;

	do
	{
		iRet = DbgLogSwitch_Init();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogDbugCmd);
		if(pbRet)
		{
			g_pLogDbusCmdSw = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogDbugCmd);
		}
		//DbgLogSwDesc_LogPlaybackStatus
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogPlaybackStatus);
		if(pbRet)
		{
			g_pLogPlaybackStatus = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogPlaybackStatus);
		}
		//DbgLogSwDes_LogJavascript
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogJavascript);
		if(pbRet)
		{
			g_pLogJavascript = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogJavascript);
		}
		//LogGpioButton
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogGpioButton);
		if(pbRet)
		{
			g_pLogGpioButton = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogGpioButton);
		}
	}while(FALSE);

	return iOutRet;
}

INT_t UnregDbgLogSwitch()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		iRet = DbgLogSwitch_Uninit();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}
