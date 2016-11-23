######################################################################
# Automatically generated by qmake (2.01a) ??? ??? 29 11:47:12 2011
######################################################################

TEMPLATE = app
TARGET = WebBrowser
CONFIG += release
DEPENDPATH += . GeneratedFiles
isEmpty(QTDIR) {
	QTDIR = $(QT_DIR)
}
# Qt base include
INCLUDEPATH += $${QTDIR}/include
INCLUDEPATH += . ../LinuxBaseLib/Include
LINUXBASELIB_DIR = ../LinuxBaseLib/Out
#SYSTEM_CMD=$$quote(echo LINUXBASELIB_DIR=$${LINUXBASELIB_DIR})
#system($${SYSTEM_CMD})
#QMAKE_RPATHDIR += $$quote($${LINUXBASELIB_DIR})
# libQtExt
INCLUDEPATH += . ../QtExt
INCLUDEPATH += . ../QtExt/Api
LIB_QTEXT_DIR = ../QtExt
# GpioBtnInput
GpioBtnInput_DIR = ../GpioBtnInput
INCLUDEPATH += $${GpioBtnInput_DIR}
# MediaPlayer
MediaPlayer_DIR = ../MediaPlayer
INCLUDEPATH += $${MediaPlayer_DIR}
# SystemService
SystemService_DIR = ../SystemService
INCLUDEPATH += $${SystemService_DIR}
# SysBaseSrv
SysBaseSrv_DIR = ../SysBaseSrv
INCLUDEPATH += $${SysBaseSrv_DIR}
INCLUDEPATH += ../../../nexus/nxclient/include/
# libGlobalStatus
libGlobalStatus_DIR = ../libGlobalStatus
INCLUDEPATH += $${libGlobalStatus_DIR}/Api
# libMp7xxCommon
libMp7xxCommon_DIR = ../libMp7xxCommon
INCLUDEPATH += $${libMp7xxCommon_DIR}/Api
# lime_english
INCLUDEPATH += . ../SoftKeyBoard
# WebBrowser
INCLUDEPATH += ./Api
# libsnmp++
Snmp_pp_DIR = ../snmp++
exists($${Snmp_pp_DIR}) {
	INCLUDEPATH += $${Snmp_pp_DIR}/include
} else {
	error("Not found libsnmp++")
}
# OpenSSL
OpenSSL_Dir = $(PRJ_TOP_DIR)/BSEAV/lib/openssl
exists($${OpenSSL_Dir}) {
	INCLUDEPATH += $${OpenSSL_Dir}/src/include
} else {
    error("Not found OpenSSL in $${OpenSSL_Dir}")
}
# tslib
TSLIB_DIR = "../tslib/src/.libs"
exists($${TSLIB_DIR}) {
	LIBS += -L$${TSLIB_DIR}
} else {
	error("Not found tslib")
}

# Input
HEADERS += browser.h PlaylistData.h WebBrowserApp.h WebAppInfoDef.h 	\
	MediaPlayerCtrl.h qplayerwindow.h web_plugin.h configuration.h    \
	qvolumewin.h NetworkManagerClient.h NetMgrEthernetClient.h DataCommService.h	\
	WebBrowserSrv.h WebBrowserAppIf.h FileDownloadItem.h FileUploadItem.h	\
	MediaVirtualChannel.h DbgLogSwitchDef.h qplaypic_html_win.h GpioFilePath.h qshowlogo.h \
	thtfit_webview.h StatusMsgBox.h BackendTransactionThread.h MessageItem.h	\
	CustomAsyncMsg.h QWbStatusBar.h AudioVideoInOutInfo.h	\
	ControlCenterApp.h QScanProgressBar.h QInputcontentdlg.h qvideolayerwnd.h	\
	GuiOsd2Ctrl.h QPowerMateClient.h UpgradeRebootConfirmDlg.h
SOURCES += browser.cpp WebBrowserApp.cpp PlaylistData.cpp main.cpp MediaPlayerCtrl.cpp 	\
	qplayerwindow.cpp web_plugin.cpp configuration.cpp qvolumewin.cpp NetworkManagerClient.cpp	\
	NetMgrEthernetClient.cpp DataCommService.cpp WebBrowserSrv.cpp WebBrowserAppIf.cpp	\
	FileDownloadItem.cpp FileUploadItem.cpp MediaVirtualChannel.cpp DbgLogSwitchDef.cpp \
	qplaypic_html_win.cpp GpioFilePath.cpp qshowlogo.cpp thtfit_webview.cpp		\
	StatusMsgBox.cpp BackendTransactionThread.cpp MessageItem.cpp	\
	CustomAsyncMsg.cpp QWbStatusBar.cpp AudioVideoInOutInfo.cpp	\
	ControlCenterApp.cpp QScanProgressBar.cpp QInputcontentdlg.cpp qvideolayerwnd.cpp	\
	GuiOsd2Ctrl.cpp QPowerMateClient.cpp UpgradeRebootConfirmDlg.cpp
FORMS += browser.ui	\
		qvolumewin.ui qplaypic_html_win.ui qshowlogo.ui qscanprogressbar.ui
#RESOURCES += browser.qrc
QT +=core \
     gui \
     webkit	\
     webkitwidgets \
     dbus \
     script \
     svg \
     xml

LIBS += -L$${LINUXBASELIB_DIR} -lLinuxBaseLib	\
	-L$${LIB_QTEXT_DIR} -lQtExt	\
	-L$${SysBaseSrv_DIR}/Out -lSysBaseSrv	\
	-L$${libGlobalStatus_DIR}/Out -lGlobalStatus	\
	-L$${libMp7xxCommon_DIR}/Out/Obj -lMp7xxCommon	\
	-L$${Snmp_pp_DIR}/lib -lsnmp++	\
	-L$${OpenSSL_Dir}/lib -lssl

ICON = qml.icns

contains(INCLUDEPATH, $${QTDIR}/qtbase/include) {
} else {
	INCLUDEPATH += $${QTDIR}/qtbase/include
}

contains(INCLUDEPATH, $${QTDIR}/qtwebkit/include) {
} else {
	INCLUDEPATH += $${QTDIR}/qtwebkit/include
}

contains(INCLUDEPATH, $${QTDIR}/qtwebkit/include/QtWebKit) {
} else {
	INCLUDEPATH += $${QTDIR}/qtwebkit/include/QtWebKit
}

contains(LIBS, $${QTDIR}/qtwebkit/lib) {
} else {
	LIBS += -L$${QTDIR}/qtwebkit/lib
}

contains(QMAKE_CXXFLAGS, -ggdb) {
} else {
	QMAKE_CXXFLAGS += -ggdb
}

contains(QMAKE_LFLAGS, -ggdb) {
} else {
	QMAKE_LFLAGS += -ggdb
}