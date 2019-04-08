// NewGame.cpp : 实现文件
//

#include "stdafx.h"
#include "mkjchess.h"
#include "NewGame.h"
#include "afxdialogex.h"


// CNewGame 对话框

IMPLEMENT_DYNAMIC(CNewGame, CDialog)

CNewGame::CNewGame(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_NEWGAME, pParent)
	
	
{

}

void CNewGame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLY, m_SetPly);
	DDX_Control(pDX, IDC_LISTENGINE, m_SearchEngineList);
	
}


BEGIN_MESSAGE_MAP(CNewGame, CDialog)
	ON_BN_CLICKED(IDOK, &CNewGame::OnBnClickedOk)
END_MESSAGE_MAP()

extern BOOL bIsBlackFirst;
extern BOOL bIsRed;
extern short Gameover;
// CNewGame 消息处理程序
void CNewGame::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nSelectedEngine = m_SearchEngineList.GetCurSel();  //保存用户选择的搜索引擎号码
	m_nSelectedPly = m_SetPly.GetPos();  //保存用户选择的计算步数
	
	if (((CButton*)GetDlgItem(IDC_RADIORED))->GetCheck())
	{
		bIsBlackFirst = FALSE;
		bIsRed = TRUE;
		Gameover = 0;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIOBLACK))->GetCheck())
	{
		bIsBlackFirst = TRUE;
		bIsRed = FALSE;
		Gameover = 0;
		HWND hWnd = ::FindWindow(NULL, _T("象棋"));
		this->PostMessage(WM_FIRSTSTEP_MSG, 0, 0);
		if (hWnd != NULL)
		{
			// 向目标窗口发送WM_CLOSE消息
			::PostMessage(hWnd, WM_FIRSTSTEP_MSG, 0, 0);
		}
		
	}
	
	CDialog::OnOK();
}


BOOL CNewGame::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_SearchEngineList.AddString(_T("负极大值引擎"));
	m_SearchEngineList.AddString(_T("AlphaBeta剪枝搜索引擎"));
	m_SearchEngineList.AddString(_T("Fail-Soft AlphaBeta剪枝搜索引擎"));
	m_SearchEngineList.AddString(_T("渴望搜索引擎"));
	m_SearchEngineList.AddString(_T("极小窗口搜索引擎"));
	m_SearchEngineList.AddString(_T("迭代深化AlphaBeta搜索引擎"));
	m_SearchEngineList.AddString(_T("AlphaBeta剪枝+置换表搜索引擎"));
	m_SearchEngineList.AddString(_T("Alphabeta剪枝+历史启发搜索引擎"));
	m_SearchEngineList.AddString(_T("Mdt(f)搜索引擎"));
	m_SearchEngineList.AddString(_T("NegaScout+置换表+历史启发"));

	m_SearchEngineList.SetCurSel(0);  //默认选择第一个引擎
	m_SetPly.SetRange(1, 15);  //设定计算步数范围
	m_SetPly.SetPos(3);  //默认计算步数
	//((CButton*)GetDlgItem(IDC_RADIORED))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIORED))->SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


