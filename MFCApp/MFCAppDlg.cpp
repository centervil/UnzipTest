﻿
// MFCAppDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "MFCApp.h"
#include "MFCAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCAppDlg ダイアログ



CMFCAppDlg::CMFCAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCAppDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCAppDlg メッセージ ハンドラー

BOOL CMFCAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CMFCAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMFCAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMFCAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCAppDlg::OnBnClickedButton1()
{
	HRESULT hResult = E_FAIL;

	STARTUPINFO         tStartupInfo = { 0 };
	PROCESS_INFORMATION tProcessInfomation = { 0 };

	/*
		プロセスの起動(notepadを起動する)
	*/
	//wchar_t cmd[] = L"C:\\Windows\\System32\\notepad.exe";
	wchar_t cmd[] = L"..\\Modules\\UnzipNetFrm.exe";
	BOOL bResult = CreateProcess(
		NULL	  
		, cmd
		, NULL
		, NULL
		, FALSE
		, 0
		, NULL
		, NULL
		, &tStartupInfo
		, &tProcessInfomation
	);
	if (0 == bResult) {
		return;
	}


	/*
		プロセスの終了を待つ
	*/
	DWORD dwResult = ::WaitForSingleObject(
		tProcessInfomation.hProcess
		, INFINITE
	);
	if (WAIT_FAILED == dwResult) {
		hResult = HRESULT_FROM_WIN32(::GetLastError());
		goto err;
	}


	/*
		プロセスの終了コードを取得する
	*/
	DWORD dwExitCode;
	bResult = ::GetExitCodeProcess(
		tProcessInfomation.hProcess
		, &dwExitCode
	);
	if (0 == bResult) {
		hResult = HRESULT_FROM_WIN32(::GetLastError());
		goto err;
	}

	// 終了コードの表示
	//wprintf(L"dwExitCode = %d¥n", dwExitCode);


err:
	// ハンドルの解放
	::CloseHandle(tProcessInfomation.hProcess);
	::CloseHandle(tProcessInfomation.hThread);

	// 正常終了
	return;
}
