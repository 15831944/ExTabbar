#include "pch.h"
#include "AppConfig.h"


//global options
BOOL g_bHideRapidAccess = TRUE; //���ؿ��ٷ��ʽڵ�
BOOL g_bAddUpButton = TRUE; //�����������һ��UP��ť��win7��win8��Ҫ��win10�Ѿ��ָ���UP��ť��
BOOL g_bFixFolderScroll = FALSE; //only for win 7
BOOL g_bFolderAutoNavigate = TRUE;
/*vistaֻ�еİ汾�У�explorer�����ؼ�������һ���Զ����ؽڵ�+/-��ǵ����ԣ���explorer
ʧȥ����ʱ����Щ�����η��Ż�������ʧ */
BOOL g_bNoFadeInOutExpand = FALSE;
BOOL g_bTreeViewAutoScroll = FALSE;
BOOL g_bTreeViewSimpleMode = FALSE;
int g_nTreeViewIndent = 0;
int g_nUpButtonIconSize = 24;
BOOL g_bForceRefreshWin7 = TRUE;
BOOL g_bShowFreespaceInStatusBar = TRUE;
BOOL g_bNoWindowResizing = FALSE;

BOOL g_bUsingLargeButton = FALSE;
int g_nSmallIconSize = 16;
int g_nLargeIconSize = 24;
BOOL g_bToolbarListMode = TRUE; //��������ť����ʾ�ı�
BOOL g_bToolbarSameSize = FALSE;
BOOL g_bEnableSettings = TRUE; //��ʾ�����á���ť
BOOL g_bBandNewLine = FALSE;
BOOL g_bTabFixWidth = TRUE;
BOOL g_bTabNewButton = TRUE;
BOOL g_bTabCloseButton = TRUE;
BOOL g_bTabAutoHideButtons = TRUE;
BOOL g_bForceSysListView = TRUE;
BOOL g_bSwitchNewTab = TRUE; //�����л����´򿪵�tab��ǩ
BOOL g_bNewTabInsertBegin = FALSE; //�±�ǩ���������б�ǩ��ǰ��

int g_nDragImageTransparent = 200;

ATL::CString g_strShellNewMenuText = _T("�½�");
COLORREF g_TeStatusBkColor = RGB(200, 191, 231);
