ѡ��һ��Ŀ¼����һ���ļ���

WinExec("explorer   /n,/select,   c:\\1.txt",   SW_SHOWNORMAL   );
  

ShellExecute(NULL, _T("open"), _T("Explorer"), _T("/select, C:\\1.txt"), NULL, SW_SHOW);

HRESULT GetShellFolderViewDual(ITEMIDLIST* pidl, IShellFolderViewDual** ppIShellFolderViewDual)
{
IWebBrowserApp* pIWebBrowserApp;
IDispatch* pDoc;
HWND hWnd;
HRESULT hr;
HINSTANCE ghSHDOCVW; HRESULT (WINAPI*gpfSHGetIDispatchForFolder)(ITEMIDLIST* pidl, IWebBrowserApp** ppIWebBrowserApp); *ppIShellFolderViewDual = NULL; ghSHDOCVW = LoadLibrary(_T("SHDOCVW.DLL"));
if (ghSHDOCVW == NULL)
return FALSE; pIWebBrowserApp=NULL;
gpfSHGetIDispatchForFolder =
(HRESULT (WINAPI*)(ITEMIDLIST*, IWebBrowserApp**)) GetProcAddress(ghSHDOCVW, "SHGetIDispatchForFolder");
if (gpfSHGetIDispatchForFolder == NULL)
return FALSE;
if (SUCCEEDED(gpfSHGetIDispatchForFolder(pidl, &pIWebBrowserApp)))
{
if (SUCCEEDED(pIWebBrowserApp->get_HWND((long*)&hWnd)))
{
SetForegroundWindow(hWnd);
ShowWindow(hWnd, SW_SHOWNORMAL);
} if (SUCCEEDED(hr = pIWebBrowserApp->get_Document(&pDoc)))
{
pDoc->QueryInterface(IID_IShellFolderViewDual, (void**) ppIShellFolderViewDual);
pDoc->Release();
} pIWebBrowserApp->Release();
}
FreeLibrary(ghSHDOCVW); return TRUE;
}BOOL OpenFolderAndSelectItems(ITEMIDLIST *pidlFolder)
{
ITEMIDLIST *pidl, *pidl2;
ITEMIDLIST* pIdlFile;
USHORT cb;
IShellFolderViewDual* pIShellFolderViewDual; HRESULT (WINAPI *gpfSHOpenFolderAndSelectItems)(LPCITEMIDLIST *pidlFolder, UINT cidl, LPCITEMIDLIST *apidl, DWORD dwFlags);
HINSTANCE ghShell32; /// ֻ��WinXp�����ϼ�ϵͳ��֧��SHOpenFolderAndSelectItems() API
/// ������ϵͳ����ôʵ����������أ�ֻ�ܲ��������ķ���������
/// ������XP���ٵ�SHOpenFolderAndSelectItems()API�У���������δ���ģ�����ͬ���ķ���ȥʵ��
/// ����ϵͳ���������ʹ�ù��� VC6 .net 2003 MSDN Ollydbg v1.10���İ� ghShell32 = LoadLibrary(_T("Shell32.DLL"));
if (ghShell32 == NULL)
return FALSE; gpfSHOpenFolderAndSelectItems =
(HRESULT (WINAPI*)(LPCITEMIDLIST*, UINT, LPCITEMIDLIST*, DWORD)) GetProcAddress(ghShell32, "SHOpenFolderAndSelectItems");
if (gpfSHOpenFolderAndSelectItems != NULL)
{
/// ���Ի��SHOpenFolderAndSelectItems()������API��ַ
if (SUCCEEDED(gpfSHOpenFolderAndSelectItems((LPCITEMIDLIST*)pidlFolder,0,(LPCITEMIDLIST*)NULL,0)))
{
///ֱ�ӵ���ϵͳ�Ĺ���
FreeLibrary(ghShell32);
return TRUE;
}
FreeLibrary(ghShell32);
return FALSE;
}
FreeLibrary(ghShell32); /// ������ϵͳ��֧��SHOpenFolderAndSelectItems()������APIʱ�Ĵ���
/// ���Ѷ���дһ����ϵͳ������ͬ�Ĵ���
pidl = pidlFolder;
pIdlFile = pidl;
/// �ҳ�Ŀ���ļ����ļ�����ƫ����
while (cb = pIdlFile->mkid.cb)
{
pidl2 = pIdlFile;
pIdlFile = (ITEMIDLIST*)((BYTE*)pIdlFile + cb);
} cb = pidl2->mkid.cb;
pidl2->mkid.cb = 0; /// ��Ŀ���ļ����ڵ��ļ���
if (SUCCEEDED(GetShellFolderViewDual(pidl, &pIShellFolderViewDual)))
{
pidl2->mkid.cb = cb;
// 0 Deselect the item.
// 1 Select the item.
// 3 Put the item in edit mode.
// 4 Deselect all but the specified item.
// 8 Ensure the item is displayed in the view.
// 0x10 Give the item the focus.
COleVariant bszFile(pidl2);
if(pIShellFolderViewDual != NULL)
{
/// ѡ����Ӧ��ѡ��
pIShellFolderViewDual->SelectItem(bszFile, 0x1d);
pIShellFolderViewDual->Release();
}
return TRUE;
}
return FALSE;
}void FindTarget(CString str)
{
HRESULT hres;
IShellLink *psl;
ITEMIDLIST *pidl;
IPersistFile *ppf; // Get a pointer to the IShellLink interface.
hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
IID_IShellLink, (LPVOID*)&psl);
if (SUCCEEDED(hres))
{
// ����Ŀ���ļ�
psl->SetPath(str);
/// ���Ŀ���ļ���ITEMIDLIST
psl->GetIDList(&pidl); // Get a pointer to the IPersistFile interface.
hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
if (SUCCEEDED(hres))
{
WCHAR wsz[MAX_PATH];
#ifdef _UNICODE
wcscpy(wsz, str);
#else
// Ensure that the string is Unicode.
MultiByteToWideChar(CP_ACP, 0, str, -1, wsz, MAX_PATH);
#endif // Load the shortcut.
hres = ppf->Load(wsz, STGM_READ);
if (SUCCEEDED(hres))
{
/// ��ÿ�ݷ�ʽ��ITEMIDLIST
psl->GetIDList(&pidl);
}
ppf->Release();
} /// ���ļ��в�ѡ����Ŀ
OpenFolderAndSelectItems(pidl);
psl->Release();
}
}


�������Զ���Internet Explorer����Դ���������ڣ�

���ںܾ�֮ǰ�Ϳ�ʼ�ó����Զ���Shell���ڡ�����Ҫ������IE���ڡ���ʱ������ؼ�����MFC��CHTMLView���������ҵ���Ҫ�����Ǻܶ�ʱ������Ҫ��ͷǶ��������ؼ����Ҿ�����ģ��IE����Ϊ������ʵ��IDocHostUIHandler�������Զ���ɹ��ܡ�һ������Ȼ�����������ֱ�Ӳ���IE���ڡ�
�����µ�Internet Explorer����

��򵥵ķ����ǵ���Windows API ShellExecute (Ex)��Paul DiLascia������C++ Q&Aר������"Browser Detection Revisited, Toolbar Info, IUnknown with COM and MFC"������һ��ʾ������:

/// As I've shown in many programs... ShellExecute(0, _T("open"), pszMyHTMLFile, 0, 0, SW_SHOWNORMAL);
���ǣ�����û�������µĴ��ڣ��������û��رճ���֮�������һ��IE���ڡ�Ϊ��ɨ���Լ�����ǰѩ������Ҫ�ҵ��Ҵ����Ĵ��ڣ����ҿ�������

�ҵ���һ�������Ǵ����Ϳ���һ��InternetExplorer���󣬲����ڱ�Ҫʱ�ص�����΢��֪ʶ��������ôһƪ����"How To Automate Internet Explorer to POST Form Data" �����������ľ�������Ҫ�ģ��������Ĺرմ��ڡ��ţ��򵥵ĵ���IWebBrowser2::Quit�Ϳ���������һ��

// create a new IE instance and show it //CComQIPtr m_pWebBrowser2; m_pWebBrowser2.CoCreateInstance(CLSID_InternetExplorer); HRESULT hr; hr = m_pWebBrowser2->put_StatusBar(VARIANT_TRUE); hr = m_pWebBrowser2->put_ToolBar(VARIANT_TRUE); hr = m_pWebBrowser2->put_MenuBar(VARIANT_TRUE); hr = m_pWebBrowser2->put_Visible(VARIANT_TRUE); if(!::PathIsURL(m_strFileToFind)) m_strFileToFind=_T("http://blog.joycode.com/jiangsheng"); COleVariant vaURL( ( LPCTSTR) m_strFileToFind); m_pWebBrowser2->Navigate2( &vaURL, COleVariant( (long) 0, VT_I4), COleVariant((LPCTSTR)NULL, VT_BSTR), COleSafeArray(), COleVariant((LPCTSTR)NULL, VT_BSTR) ); void CAutomationDlg::OnDestroy() { //close the IE window created by this program before exit if(m_pWebBrowser2) { if(m_bOwnIE) { m_pWebBrowser2->Quit(); m_bOwnIE=FALSE; } UnadvisesinkIE(); m_pWebBrowser2=(LPUNKNOWN)NULL; } CDialog::OnDestroy(); }
����һ�����⡣Ҫ���û����ҵ�WM_TIMER�������в�������֮ǰ�ص����µ�IE������ô�죿����IWebBrowser2�ӿڿ��Ƶ�IE�������ڲ��ٴ����ˡ��ҿ�΢���ǵ�����һ�㣬���򲻻������������û����ܹ�֪��ʲôʱ������رգ������ҿ��Ա������ⷢ����
����Internet Explorer���¼�

Internet Explorer�������˳�ʱ�ᴥ��DWebBrowserEvents2::OnQuit�¼�������һ��������ͷſ��Ƶ�ʱ������Ϊ����Ҫ�����٣�������ͬʱҲֹͣ���Ӷ�����¼�

if(m_pWebBrowser2) { UnadvisesinkIE(); m_pWebBrowser2=(LPUNKNOWN)NULL; }
���ӵ���ǰ��Internet Explorer����

��Ȼ�Ҳ��ں��һ���Ƶ��ĸ�IE���ڣ����Ǽ�Ȼ΢��֪ʶ��������"������ӵ�һ��Internet Explorer��ʵ��"����һƪ���£��Ҽٶ�һЩ�˻����"������ӵ���ǰ��Internet Explorerʵ��"����һƪ���±Ƚ�����.

�����Ļ���ʲô�ǡ���ǰ��Internet Explorerʵ������ʵ���ϣ����������һ�����IE���ڡ���ΪWindows��ѻ�Ĵ����ƶ���z-order�Ķ������������ᱣ��������IE���ڵ�z-order����ߴ����������Ҫ���ľ����ҵ��ĸ�IE���ھ�����ߵ�z-orderֵ����������Ҫ���ж��ĸ�������IE���ڡ���һЩ��Spy++�йصĵ���֮���Ҽٶ�IE���ھ���һ����ͬ�Ĵ�����"IEFrame"��Ȼ���д��һ�����������Shell���ڵĴ����ࣺ

//shell windows object will list both IE and Explorer windows //use their window class names to identify them. CString CAutomationDlg::GetWindowClassName(IWebBrowser2* pwb) { TCHAR szClassName[_MAX_PATH]; ZeroMemory( szClassName, _MAX_PATH * sizeof( TCHAR)); HWND hwnd=NULL; if (pwb) { LONG_PTR lwnd=NULL; pwb->get_HWND(&lwnd); hwnd=reinterpret_cast(lwnd); ::GetClassName( hwnd, szClassName, _MAX_PATH); } return szClassName; }
ʣ�µ�����ͺܼ��ˣ���Z��ö�ٶ��㴰�ڣ��ҵ���һ��Shell�����б��еľ��д�����"IEFrame"�ĵ�һ��ʵ����֮���Ҳ�����һ��IE��DHTML�ĵ�����ģ��(Ҳ��ΪDOM����ֻ��IE���ڴ������һ��DocumentComplete�¼�ֻ����Ч)��ȷ�ϳɹ����ӵ����ڡ�

void CAutomationDlg::DocumentComplete(IDispatch *pDisp, VARIANT *URL) { //HTML DOM is available AFTER the DocumentComplete event is fired. //For more information, please visit KB article //"How To Determine When a Page Is Done Loading in WebBrowser Control" //http://support.microsoft.com/kb/q180366/ CComQIPtr pWBUK(m_pWebBrowser2); CComQIPtr pSenderUK( pDisp); USES_CONVERSION; TRACE( _T( "Page downloading complete:/r/n")); CComBSTR bstrName; m_pWebBrowser2->get_LocationName(&bstrName); CComBSTR bstrURL; m_pWebBrowser2->get_LocationURL(&bstrURL); TRACE( _T( "Name:[ %s ]/r/nURL: [ %s ]/r/n"), OLE2T(bstrName), OLE2T(bstrURL)); if (pWBUK== pSenderUK) { CComQIPtr pHTMLDocDisp; m_pWebBrowser2->get_Document(&pHTMLDocDisp); CComQIPtr pHTMLDoc(pHTMLDocDisp); CComQIPtr ecAll; CComPtr pTagLineDisp; if(pHTMLDoc) { CComBSTR bstrNewTitle(_T("Sheng Jiang's Automation Test")); pHTMLDoc->put_title(bstrNewTitle); pHTMLDoc->get_all(&ecAll); } if(ecAll) { ecAll->item(COleVariant(_T("tagline")),COleVariant((long)0),&pTagLineDisp); } CComQIPtr eTagLine(pTagLineDisp); if(eTagLine) { eTagLine->put_innerText( CComBSTR(_T("Command what is yours, conquer what is not. --Kane"))); } } }
���ڿ��ƵĴ��ں�IE���ļ�ʱѡ���һ���ˡ�
����Ʒ: ���ӵ���ǰ��Windows Explorer����

���о�ShellWindows�����shell�����б�ʱ���һ��һ������Ʒ��������Windows Explorer����Ҳ�й�ͬ�Ĵ�������������ͬ���Ļ����ڰѴ������"IEFrame"�ĳ�"ExploreWClass"֮���Windows Explorer����Ҳ���á���Ϊû��DHTML DOM�ɹ���������֪ͨWindows Explorer ���ڴ�һ���ִ�·��������־�ҽӹ���������ڡ�

//show the folder bar COleVariant clsIDFolderBar(_T("{EFA24E64-B078-11d0-89E4-00C04FC9E26E}")); COleVariant FolderBarShow(VARIANT_TRUE,VT_BOOL); COleVariant dummy; if(m_pWebBrowser2) m_pWebBrowser2->ShowBrowserBar(&clsIDFolderBar,&FolderBarShow,&dummy); //browse to a given folder CComQIPtr psp(m_pWebBrowser2); CComPtr psb; if(psp) psp->QueryService(SID_STopLevelBrowser,IID_IShellBrowser,(LPVOID*)&psb); if(psb) { USES_CONVERSION; LPITEMIDLIST pidl=NULL; SFGAOF sfgao; SHParseDisplayName (T2OLE(m_strFileToFind),NULL,&pidl,0, &sfgao); if(pidl==NULL) ::SHGetSpecialFolderLocation(m_hWnd,CSIDL_DRIVES,&pidl); m_pidlToNavigate=NULL; if(pidl) { //if the start address is a folder, then browse it. //otherwise browse to its parent folder, and select it in the folder view. LPCITEMIDLIST pidlChild=NULL; CComPtr psf; HRESULT hr = SHBindToParent(pidl, IID_IShellFolder, (LPVOID*)&psf, &pidlChild); if (SUCCEEDED(hr)){ SFGAOF rgfInOut=SFGAO_FOLDER; hr=psf->GetAttributesOf(1,&pidlChild,&rgfInOut); if (SUCCEEDED(hr)){ m_pidlToNavigate=ILClone(pidl); if(rgfInOut&SFGAO_FOLDER){//this is a folder psb->BrowseObject(pidl,SBSP_SAMEBROWSER); } else { //this is a file, browse to the parent folder LPITEMIDLIST pidlParent=ILClone(pidl); ::ILRemoveLastID(pidlParent); psb->BrowseObject( pidlParent, SBSP_SAMEBROWSER); ILFree(pidlParent); } } } //clean up ILFree(pidl); } }:
������е㳤����Ϊ��������Դ��ļ����ļ��С���������IShellBrowser::BrowseObject���Ҹ������������һ���ļ�pidl����ôWindows Explorer����ʾ���Ƿ������ļ�����������Դ�������ĵ�ַ��������·��֮�󰴻س�һ��������ģ��"Explorer.exe /select"����Ϊ�����ļ�����ͼ��ѡ��ָ�����ļ�����������DocumentComplete�¼��������м�����һЩ���룺

if(m_pidlToNavigate) { //If the start address is a file, browse to the parent folder //and then select it CComQIPtr psp(m_pWebBrowser2); CComPtr psb; CComPtr psv; if(psp) psp->QueryService(SID_STopLevelBrowser,IID_IShellBrowser,(LPVOID*)&psb); if(psb) psb->QueryActiveShellView(&psv); if(psv) { LPCITEMIDLIST pidlChild=NULL; CComPtr psf; SFGAOF rgfInOut=SHCIDS_ALLFIELDS; HRESULT hr = SHBindToParent(m_pidlToNavigate, IID_IShellFolder, (LPVOID*)&psf, &pidlChild); if (SUCCEEDED(hr)){ hr=psf->GetAttributesOf(1,&pidlChild,&rgfInOut); if (SUCCEEDED(hr)){ if((rgfInOut&SFGAO_FOLDER)==0){ //a file, select it hr=psv->SelectItem(ILFindLastID(m_pidlToNavigate) ,SVSI_SELECT|SVSI_ENSUREVISIBLE|SVSI_FOCUSED| SVSI_POSITIONITEM); } } } } //clean up ILFree(m_pidlToNavigate); m_pidlToNavigate=NULL; }
����Explorer����

�������ô�����⣬�����½������ˡ���Ȼ�ҿ����Ժ͵�ǰ��Internet Explorer���ڻ�����ͬ�ķ�ʽ���ӵ���ǰ��Windows Explorer���ڣ���ô���Ƿ�����Ժʹ���Internet Explorer���ڻ�����ͬ�ķ�ʽ����Windows Explorer���ڣ��ź����ǣ��ⲻ���С�������Windows Explorer��Ӧ����ID������һ��COM������Ȼ���Ծɿ��Դ���IE���ڣ�������ļ��У���ʾ�ļ��в������ʹ��������������һ��Windows Explorer���ڣ������Ҳ��ܸı䴰����"IEFrame"����˽��Ѱ�������������ʾHTML��ҳ�ͻ�ĵ���IE�������ֿ�����

�ðɣ���Ȼ�Ҳ�����COM�ķ�ʽ�����������һ����Գ����ô�ͳ�ķ�ʽ���ҿ��Դ���һ��explorer.exe����֮������������ڣ�����Paul DiLascia ����������"Get the Main Window, Get EXE Name"����ʾ�����������ҷ���δ�ĵ�������ϢWM_GETISHELLBROWSER����ô��ڵ�IShellBrowser�ӿ�:

//start the new process STARTUPINFO si; PROCESS_INFORMATION pi; ZeroMemory( &si, sizeof(si) ); si.cb = sizeof(si); ZeroMemory( &pi, sizeof(pi) ); // Start the child process. if( !CreateProcess( NULL, // No module name (use command line). _T("explorer.exe"), // Command line. NULL, // Process handle not inheritable. NULL, // Thread handle not inheritable. FALSE, // Set handle inheritance to FALSE. 0, // No creation flags. NULL, // Use parent's environment block. NULL, // Use parent's starting directory. &si, // Pointer to STARTUPINFO structure. &pi ) // Pointer to PROCESS_INFORMATION structure. ) //wait a graceful time //so the window is created and is ready to answer messages. ::WaitForInputIdle(pi.hProcess,1000); //m_hExplorerProcess=(DWORD)pi.hProcess; EnumWindows(EnumWindowsProc,(LPARAM)this); BOOL CALLBACK CAutomationDlg::EnumWindowsProc(HWND hwnd,LPARAM lParam) { CAutomationDlg* pdlg=(CAutomationDlg*)lParam; DWORD pidwin; GetWindowThreadProcessId(hwnd, &pidwin); if (pidwin==pdlg->m_hExplorerProcess) { IShellBrowser* psb=(IShellBrowser*)::SendMessage(hwnd,WM_USER+7,0,0); CComQIPtr pwb(psb); return FALSE; } return TRUE; }
��ร������ҵļ������Ҳû��Ч������ô���£����ҵ���Դ���������ļ���ѡ���У�����ͬһ�����д�ÿһ���ļ��С���ѡ�У������µ�Windows Explorer���ڱ����������е�Windows Explorer�����С�����������������ͬ��

��һ�£�����ͷ������һ��ShellWindows���������Ը���һ��Shell���ڵ��б�����ÿһ��Windows Explorer���ں�ÿ�����ڶ�Ӧ��IWebBrowser2�ӿڣ����ǵ�IShellBrowser�ӿڵ���ڡ�.��������Ҫ�������shell�����б�����explorer.exe����֮ǰ��֮���һ�ݣ�֮��Ҫ�Ƚ��������ҵ��µ�shell���ڣ�

m_pShellWindows.CoCreateInstance(CLSID_ShellWindows); if(m_pShellWindows) { //get the list of running IE windows //using the ShellWindows collection //For more information, please visit //http://support.microsoft.com/kb/176792 long lCount=0; m_pShellWindows->get_Count(&lCount); for(long i=0;i pdispShellWindow; m_pShellWindows->Item(COleVariant(i),&pdispShellWindow); if(pdispShellWindow) { m_listShellWindows.AddTail(new CComQIPtrIDispatch(pdispShellWindow)); } } } //enumerate through the new shell window list long lCount=0; m_pShellWindows->get_Count(&lCount); for(long i=0;i//search the new window //using the ShellWindows collection //For more information, please visit //http://support.microsoft.com/kb/176792 BOOL bFound=FALSE; CComPtr pdispShellWindow; m_pShellWindows->Item(COleVariant(i),&pdispShellWindow); //search it in the old shell window list POSITION pos=m_listShellWindows.GetHeadPosition(); while(pos) { CComQIPtrIDispatch* pDispatch=m_listShellWindows.GetNext(pos); if(pDispatch&&pdispShellWindow.p==pDispatch->p) { bFound=TRUE;break; } } if(!bFound)//new window found { //attach to it m_pWebBrowser2=pdispShellWindow; m_bOwnIE=TRUE; //sink for the Quit and DocumentComplete events AdviseSinkIE(); NavigateToSamplePage(FALSE); } }
��һ�£����"����explorer.exe����֮��"��ʲô��˼?һ����֮�󣿻��������ӣ�ʵ���ϣ�һ��WindowRegistered�¼��ᱻShellWindows ���󴥷������������¼������м���һЩ���룺.

//sink DShellWindowsEvents events LPUNKNOWN pUnkSink = GetIDispatch(FALSE); m_pShellWindows.CoCreateInstance(CLSID_ShellWindows); AfxConnectionAdvise((LPUNKNOWN)m_pShellWindows, DIID_DShellWindowsEvents,pUnkSink,FALSE,&m_dwCookieShellWindows); void CAutomationDlg::WindowRegistered(long lCookie) { //ok, a new shell window is created if(m_pShellWindows) { //enumerate through the new shell window list long lCount=0; m_pShellWindows->get_Count(&lCount); for(long i=0;i//search the new window //using the ShellWindows collection //For more information, please visit //http://support.microsoft.com/kb/176792 BOOL bFound=FALSE; CComPtr pdispShellWindow; m_pShellWindows->Item(COleVariant(i),&pdispShellWindow); //search it in the old shell window list POSITION pos=m_listShellWindows.GetHeadPosition(); while(pos) { CComQIPtrIDispatch* pDispatch=m_listShellWindows.GetNext(pos); if(pDispatch&&pdispShellWindow.p==pDispatch->p) { bFound=TRUE;break; } } if(!bFound)//new window { //attach to it m_pWebBrowser2=pdispShellWindow; m_bOwnIE=TRUE; //sink for the Quit and DocumentComplete events AdviseSinkIE(); NavigateToSamplePage(FALSE); } } //clean up if(m_dwCookieShellWindows!= 0) { LPUNKNOWN pUnkSink = GetIDispatch(FALSE); AfxConnectionUnadvise((LPUNKNOWN)m_pShellWindows, DIID_DShellWindowsEvents, pUnkSink, FALSE, m_dwCookieShellWindows); m_dwCookieShellWindows= 0; } POSITION pos=m_listShellWindows.GetHeadPosition(); while(pos) { CComQIPtrIDispatch* pDispatch=m_listShellWindows.GetNext(pos); delete pDispatch; } m_listShellWindows.RemoveAll(); m_pShellWindows=(LPUNKNOWN)NULL; } }
Ϊʲô����Browser Helper Objects?

��Ϊ�µĴ����ڽ����⣬���Կ�����м�COM���ú������������Զ������������ܶ��COM���ã���ô�����Ҫ�Ѵ��뱾�ػ��������дһ���������������(BHO)�����ǣ�BHO�ᱻÿһ��Windows Explorer��Internet Explorer��ʵ�����أ������Ҳ�����������ϵͳ��������ɨ����˪��һЩ�˵���ʹ��������������ӵ���ǰ��Internet Explorer����.
��֪����

ShellWindows������explorer.exe process����ֹ������δ����ʱ���ɷ��ʡ�BHO����������¿�����Ϊ���������
����

������һ��������Ժ��Ĵ��룬���ҿ��ܻ����㲻��Ϥ��COM��Windows API������ϵ��á�ϣ�������ñ������ã����Ҳ��ᱻ�ҵĴ�����ͷ�����͡��Զ���Internet Explorer��Windows Explorers���ڿ��Խ�ʡ��ģ��ϵͳĬ����Ϊ��ʱ�䣬���Ҹ��û��ṩһ����Ϥ�Ľ��档
�ο�

    Browser Detection Revisited, Toolbar Info, IUnknown with COM and MFC
    Connecting to Running Instances of Internet Explorer
    Connecting to a running instance of Internet Explorer
    Get the Main Window, Get EXE Name
    How to connect to a running instance of Internet Explorer
    How to Connect to IExplorer in Citrix Environments
    PRB: IShellFolder::CreateViewObject() causes access violation
    Querying information from an Explorer window
    ShellWindows Object (Windows Explorer and Controls)



    ATL����ҳ�����¼�����:
    ��ǰ����Ϊ�˱�дһ��Ư���ĳ�����Ҫ��VC���пͻ����ǿͻ����ػ棬��Ҫά��ͼƬ���롢GDI���ơ���Ϣ�������ظ��ֿؼ���������Ȼ�ܶ��˶�����ô���ģ����������������Ŀ�У���Ȼʹ��GDI�����ܶ�����ϵ����顣���ǣ�Ŀǰ�ƺ�������Webҳ�棬��ͬȥ��Щ����·�Խ��Խ���С���Ȼ�������̻����벻��GDI��GDI+��������ֻ�Ǽ�¼һЩ����Ϊδ����д���������Ŀ�п��ܻ�ʹ�õ�һЩ����������Ȼ�����ּ����Ѿ��ܶ��˶������ˣ����ҿ϶��õı��Һã�ϣ������ıȽ���Ķ�������еĴ���Ͳ��㡣

�ӽ��쿪ʼ���Ҽƻ���1��2���µ�ʱ��½��дһЩ����ʹ��ATL/WTL����ҳ���������֣�����ʹ��COM��������ҳԪ�أ��Լ�����ҳԪ���¼�����Ӧ�� 

��������MFC/ATL/WTL��������Insertһ��Microsoft Web Browser��ActiveX�ؼ������Ƕ�̬������һ��Browserʵ������������Ҫֱ�Ӳ���Windowsϵͳ��IE����������Ҫ����������ǻ��IE Browser�Ľӿڡ�

������������дһ��WTL��ʾ������ʼ˵����

��δ�������

2008��1��16��
���һ��ʱ��ʵ����̫æ�ˣ��ոհ��²�Ʒ���������Ū�ꡣ��һ����Ʒ���������������Ҫ���ˡ����ơ�
���ڱ���Ĺ���������������ȥ�ˣ��Լ��Ѿ�û�ж���ʱ������ˣ��ܶ��µļ�����ϸ�����Ѿ����ó��ˣ���֪�����Ǻ��ǻ������������Ŀ��������鵹��ѧ���˲��١�

�ϻ���˵���һ��Ǿ�����հ��������д�ꡣ

ʹ��WTL����һ����׼�ĶԻ������������ǵ�Ҫ��Enable ActiveX Control Hosting��ѡ��ϡ�Ȼ��һ��Microsoft Web Browser��ActiveX�ؼ���ȥ��ID�ȷ�˵��IDC_EXPLORER��

���öԻ����ܹ�����WebBrowser�ؼ����¼�����ҪΪ�Ի�������һЩ�̳��ࡣ�������Ƕ�������һ��������
#define WebBrowser IDispEventImpl<IDC_EXPLORER, CMainDlg, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 0>

Ȼ����ӵ��Ի���ļ̳��б��У����£�
class CMainDlg : public CAxDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
        public CMessageFilter, public CIdleHandler, public WebBrowser

��stdafx.h������⼸����Ҫ��ͷ�ļ���
#include <exdisp.h>
#include <exdispid.h>
#include <mshtml.h>
#include <mshtmdid.h>
OK������ϣ��������ǶԻ������Ǹ�WebBrowser�ؼ����¼�����ˣ���Ҫά��һ���ÿؼ��ĳ�Ա������
CComPtr<IWebBrowser2> m_pWeb2;
��������ѡ������ϣ���������ҳ�¼������������Ϣ��Ӧ�꣬Ȼ����������¼������£�
BEGIN_SINK_MAP(CMainDlg)
        SINK_ENTRY_EX(IDC_EXPLORER, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete) 
        SINK_ENTRY_EX(IDC_EXPLORER, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2)
    END_SINK_MAP()
��������ϣ�����������¼���OnBeforeNavigate2��OnDocumentComplete���������������¼���ʵ���壺
void __stdcall OnDocumentComplete(IDispatch *, VARIANT* )
{
    MessageBox(_T("Current document complete!"),_T("Info"),MB_OK|MB_ICONEXCLAMATION);
}

void __stdcall CMainDlg::OnBeforeNavigate2 (
    IDispatch* pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName,
    VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel )
{
    MessageBox(_T("Before navigate!"),_T("Info"),MB_OK|MB_ICONEXCLAMATION);
};

���������öԻ�������ʱ����Ƕ���Web Browser ActiveX�ؼ�ֱ�Ӵ�һ����ҳ����OnInitDialog���������Ĵ��룺
CAxWindow AxWin = GetDlgItem(IDC_EXPLORER);
AxWin.QueryControl(&m_pWeb2);

if (m_pWeb2)
{
    WebBrowser::DispEventAdvise(m_pWeb2);
    m_pWeb2->Navigate(CComBSTR("http://blog.csdn.net/epubcn/"),NULL,NULL,NULL,NULL);
}
�ǵ���OnDestroy���ͷ���ҳ�¼���أ�
if (m_pWeb2)
{
    WebBrowser::DispEventUnadvise(m_pWeb2);
    m_pWeb2.Release();
}

OK�����ڱ�������һ���Ƿ�ᵯ��Ԥ�ڵ���Ϣ�Ի���

ע���������ʱ����ATLASSERT(!InlineIsEqualGUID(*m_plibid,GUID_NULL) && "Did you forget to pass the LIBID to CComModule::Init?");�����뽫���������еġ�ATLʹ�á���Ϊ����̬���ӵ�ATL�����ɡ�

�����Ƕ���ҳ�¼�����Ӧ����������������������ҳ��ĳ��Ԫ�ص��¼�������һ��BUTTON�ĵ������������ӵĵ����һ����ѡ��ť�Ƿ�ѡ�С��ı���������ݵĸı�ͻ�ȡ�ȡ�




















