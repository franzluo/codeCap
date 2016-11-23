#include "TransactionMsgData.h"

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_setDataSource);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_Play);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

CMsgData_Play::CMsgData_Play()
{
	LoopCount = 1;
#if 1/*added by lxj 2013-1-8*/
	bPauseAfterBuffering = FALSE;
#endif
}


BEGIN_IMPL_RUNTIME_CLASS(CMsgData_PlaybackStart);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_PlaybackEos);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_setVideoPosition);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_setVideoInputWindowSize);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_ChangeVidOutMode);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_setSpdifMode);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_setPlaySpeedCtrl);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_getPlaySpeedCtrl);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_DispOnLedScr);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();
#ifdef ENABLE_DTV
BEGIN_IMPL_RUNTIME_CLASS(CMsgData_GetTunerMode);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_PropmtMsg);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_GetStrength);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_GetFirstChNum);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();


BEGIN_IMPL_RUNTIME_CLASS(CMsgData_SetTuMode);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_ScanProgress);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_GetEntryInfo);
BASE_IMPL_RUNTIME_CLASS(CMsgData_GetEntryInfo);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_SetTuChMode);
BASE_IMPL_RUNTIME_CLASS(CMsgData_SetTuChMode);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_GetTuChMode);
BASE_IMPL_RUNTIME_CLASS(CMsgData_GetTuChMode);
END_IMPL_RUNTIME_CLASS();


BEGIN_IMPL_RUNTIME_CLASS(CMsgData_setVideoRotation);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

BEGIN_IMPL_RUNTIME_CLASS(CMsgData_setEnableAudio6Ch);
BASE_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();


CMsgData_QuitPlay::CMsgData_QuitPlay()
{
	m_PlaybackId = 0;	//invalid
}

#endif

CMsgData_StopPlayback::CMsgData_StopPlayback()
{
	m_PlaybackId = 0;
}

