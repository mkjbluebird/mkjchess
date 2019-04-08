// mkjchessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mkjchess.h"
#include "mkjchessDlg.h"
//#include "NewGame.h"
#include "MoveGenerator.h"
//#include "SearchEngine.h"
//#include "Eveluation.cpp"
#include "afxdialogex.h"

#define BORDERWIDTH 31 //棋盘(左右)边缘的宽度
#define BORDERHEIGHT 31 //棋盘(上下)边缘的高度
#define GRILLEWIDTH 56  //棋盘上每个格子的高度
#define GRILLEHEIGHT 56 //棋盘上每个格子的宽度
int m_nMaxDepth;  //当前搜索深度
BOOL IsFirstTurn = TRUE;
BOOL bIsBlackFirst = FALSE;
BOOL bIsStart = FALSE;
BOOL bIsRed = TRUE;
short Gameover;
//HWND hWnd;
const BYTE InitChessBoard[10][9] =
{
	{ B_CAR,   B_HORSE, B_ELEPHANT, B_BISHOP, B_KING,  B_BISHOP, B_ELEPHANT, B_HORSE, B_CAR },
	{ NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS },
	{ NOCHESS, B_CANON, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    B_CANON, NOCHESS },
	{ B_PAWN,  NOCHESS, B_PAWN,     NOCHESS,  B_PAWN,  NOCHESS,  B_PAWN,     NOCHESS, B_PAWN },
	{ NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS },

	{ NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS },
	{ R_PAWN,  NOCHESS, R_PAWN,     NOCHESS,  R_PAWN,  NOCHESS,  R_PAWN,     NOCHESS, R_PAWN },
	{ NOCHESS, R_CANON, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    R_CANON, NOCHESS },
	{ NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS },
	{ R_CAR,   R_HORSE, R_ELEPHANT, R_BISHOP, R_KING,  R_BISHOP, R_ELEPHANT, R_HORSE, R_CAR }
};

const BYTE InitChessBoard1[10][9] =
{
	{ R_CAR,   R_HORSE, R_ELEPHANT, R_BISHOP, R_KING,  R_BISHOP, R_ELEPHANT, R_HORSE, R_CAR },
	{ NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS },
	{ NOCHESS, R_CANON, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    R_CANON, NOCHESS },
	{ R_PAWN,  NOCHESS, R_PAWN,     NOCHESS,  R_PAWN,  NOCHESS,  R_PAWN,     NOCHESS, R_PAWN },
	{ NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS },

	{ NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS },
	{ B_PAWN,  NOCHESS, B_PAWN,     NOCHESS,  B_PAWN,  NOCHESS,  B_PAWN,     NOCHESS, B_PAWN },
	{ NOCHESS, B_CANON, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    B_CANON, NOCHESS },
	{ NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS },
	{ B_CAR,   B_HORSE, B_ELEPHANT, B_BISHOP, B_KING,  B_BISHOP, B_ELEPHANT, B_HORSE, B_CAR }
};
// CmkjchessDlg 对话框

IMPLEMENT_DYNAMIC(CmkjchessDlg, CDialog)

CmkjchessDlg::CmkjchessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MKJCHESS_DIALOG, pParent)
{

}
/*
CChessDlg::~CChessDlg()
{
}
*/

void CmkjchessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_NODECOUNT, m_OutputInfo);
	DDX_Control(pDX, IDC_PLY, m_SetPly);
	DDX_Control(pDX, IDC_PLY1, m_SetPly1);
	DDX_Control(pDX, IDC_LISTENGINE, m_SearchEngineList);
	DDX_Control(pDX, IDC_LISTENGINE1, m_SearchEngineList1);
	DDX_Control(pDX, IDC_LIST1, m_ComputeList);
}


BEGIN_MESSAGE_MAP(CmkjchessDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//ON_BN_CLICKED(IDC_NEWGAME, &CmkjchessDlg::OnBnClickedNewgame)
	//ON_BN_CLICKED(IDC_START, &CmkjchessDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_AUTO, &CmkjchessDlg::OnBnClickedAuto)
	//ON_COMMAND(IDOK, &CmkjchessDlg::OnIdok)
	ON_BN_CLICKED(IDOK, &CmkjchessDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_NEWGAME, &CmkjchessDlg::OnBnClickedNewgame)
	ON_BN_CLICKED(IDC_NextStep, &CmkjchessDlg::OnBnClickedNextstep)
	ON_MESSAGE(WM_FIRSTSTEP_MSG, &CmkjchessDlg::OnFirststepMsg)
	ON_BN_CLICKED(IDC_STOP, &CmkjchessDlg::OnBnClickedStop)
END_MESSAGE_MAP()

// CmkjchessDlg 消息处理程序
//CWinThread* pWinThread;
BOOL CmkjchessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Chessman.Create(IDB_CHESSMAN, 57, 14, RGB(0, 0, 255));

	BITMAP BitMap;
	m_BoardBmp.LoadBitmap(IDB_CHESSBOARD);  //加载棋盘图片
	m_BoardBmp.GetBitmap(&BitMap);
	m_nBoardWidth = BitMap.bmWidth;  //获得棋盘宽度
	m_nBoardHeight = BitMap.bmHeight;  //获得棋盘高度
	m_BoardBmp.DeleteObject();

	memcpy(m_ChessBoard, InitChessBoard, 90);//初始化棋盘
	
	m_pSE = new CNegamaxEngine;
	m_pSE1 = new CNegamaxEngine;
	//pMG = new CMoveGenerator;
	//pMG1 = new CMoveGenerator;
	//pEvel = new CEveluation;
	//pEvel1 = new CEveluation;

	m_pSE->SetSearchDepth(3);  //设置搜索深度
	//m_pSE->SetMoveGenerator(pMG); //设置落子生成器
	//m_pSE->SetEveluator(pEvel); //设置估值核心

	m_pSE1->SetSearchDepth(3);  //设置搜索深度
	//m_pSE1->SetMoveGenerator(pMG1); //设置落子生成器
	//m_pSE1->SetEveluator(pEvel1); //设置估值核心

	m_MoveChess.nChessID = NOCHESS;
	if(IsFirstTurn)
	{
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

		m_SearchEngineList1.AddString(_T("负极大值引擎"));
		m_SearchEngineList1.AddString(_T("AlphaBeta剪枝搜索引擎"));
		m_SearchEngineList1.AddString(_T("Fail-Soft AlphaBeta剪枝搜索引擎"));
		m_SearchEngineList1.AddString(_T("渴望搜索引擎"));
		m_SearchEngineList1.AddString(_T("极小窗口搜索引擎"));
		m_SearchEngineList1.AddString(_T("迭代深化AlphaBeta搜索引擎"));
		m_SearchEngineList1.AddString(_T("AlphaBeta剪枝+置换表搜索引擎"));
		m_SearchEngineList1.AddString(_T("Alphabeta剪枝+历史启发搜索引擎"));
		m_SearchEngineList1.AddString(_T("Mdt(f)搜索引擎"));
		m_SearchEngineList1.AddString(_T("NegaScout+置换表+历史启发"));
	}
	m_SearchEngineList.SetCurSel(0);  //默认选择第一个引擎
	m_SearchEngineList1.SetCurSel(0);  //默认选择第一个引擎
	m_SetPly1.SetRange(1, 15);  //设定计算步数范围
	m_SetPly1.SetPos(3);  //默认计算步数

	m_SetPly.SetRange(1, 15);  //设定搜索深度范围
	m_SetPly.SetPos(3);  //默认搜索深度
	((CButton*)GetDlgItem(IDC_AUTO))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_STOP))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_NextStep))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_NEWGAME))->EnableWindow(FALSE);
						 //((CButton*)GetDlgItem(IDC_RADIORED))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIORED))->SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE

}


void CmkjchessDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
	CDC MemDC;
	int i, j;
	POINT pt;
	CBitmap *pOldBmp;

	MemDC.CreateCompatibleDC(&dc);
	m_BoardBmp.LoadBitmap(IDB_CHESSBOARD);
	pOldBmp = MemDC.SelectObject(&m_BoardBmp);
	for (i = 0; i < 10; i++)
		for (j = 0; j < 9; j++)
		{
			if (m_ChessBoard[i][j] == NOCHESS)
				continue;
			pt.x = j*GRILLEHEIGHT + 10;
			pt.y = i*GRILLEWIDTH + 10;
			m_Chessman.Draw(&MemDC, m_ChessBoard[i][j] - 1, pt, ILD_TRANSPARENT);
		}
	if (m_MoveChess.nChessID != NOCHESS)
		m_Chessman.Draw(&MemDC, m_MoveChess.nChessID - 1, m_MoveChess.ptMovePoint, ILD_TRANSPARENT);

	dc.BitBlt(0, 0, m_nBoardWidth, m_nBoardHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(&pOldBmp);
	MemDC.DeleteDC();
	m_BoardBmp.DeleteObject();
	// 不为绘图消息调用 CDialog::OnPaint()
}


HCURSOR CmkjchessDlg::OnQueryDragIcon()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialog::OnQueryDragIcon();
}


void CmkjchessDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int x, y;
	x = (point.x - 10) / GRILLEWIDTH;
	y = (point.y - 10) / GRILLEHEIGHT;
	memcpy(m_BackupChessBoard, m_ChessBoard, 90);
	if (point.x > 0 && point.y > 0 && point.x < m_nBoardWidth && point.y < m_nBoardHeight && IsRed(m_ChessBoard[y][x]))
	{
		memcpy(m_BackupChessBoard, m_ChessBoard, 90);
		m_ptMoveChess.x = x;
		m_ptMoveChess.y = y;
		m_MoveChess.nChessID = m_ChessBoard[y][x];
		m_ChessBoard[m_ptMoveChess.y][m_ptMoveChess.x] = NOCHESS;
		point.x -= 19;
		point.y -= 19;
		m_MoveChess.ptMovePoint = point;
		InvalidateRect(NULL, FALSE);
		UpdateWindow();
		SetCapture();
	}
	CDialog::OnLButtonDown(nFlags, point);
}

extern int count;
void CmkjchessDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	BOOL bTurnSide = FALSE;
	int x, y;
	x = (point.x - 10) / GRILLEWIDTH;  //获取棋子位置
	y = (point.y - 10) / GRILLEHEIGHT;
	if (m_MoveChess.nChessID && CMoveGenerator::IsValidMove(m_BackupChessBoard, m_ptMoveChess.x, m_ptMoveChess.y, x, y))  //检查落子是否合法
	{
		m_ChessBoard[y][x] = m_MoveChess.nChessID;  //落子
		bTurnSide = TRUE;  //信号标志，表示开始计算
		bIsRed = FALSE;
	}
	else {
		memcpy(m_ChessBoard, m_BackupChessBoard, 90);  //非法落子，恢复棋局
	}
	m_MoveChess.nChessID = NOCHESS;
	InvalidateRect(NULL, FALSE);  //刷新棋盘
	UpdateWindow();  //执行刷新
	ReleaseCapture();
	if (bTurnSide == TRUE)
	{
		calculate();
	}
			

	CDialog::OnLButtonUp(nFlags, point);
}


void CmkjchessDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_MoveChess.nChessID)
	{
		//防止将棋子拖出棋盘
		if (point.x < 10)
			point.x = 10;
		if (point.y < 10)
			point.y = 10;
		if (point.x > m_nBoardWidth - 10)
			point.x = m_nBoardWidth - 10;
		if (point.y > m_nBoardHeight - 10)
			point.y = m_nBoardHeight - 10;

		point.x -= 19;
		point.y -= 19;
		m_MoveChess.ptMovePoint = point;
		InvalidateRect(NULL, FALSE);//刷新窗口
		UpdateWindow();
	}
	CDialog::OnMouseMove(nFlags, point);
}

int stepCount = 1;
void CmkjchessDlg::calculate()
{
	int timecount;   //计算起始时间
	timecount = GetTickCount();

	if (bIsRed) {
		m_pSE->SearchAGoodMove(m_ChessBoard);
	}		
	else {
		m_pSE1->SearchAGoodMove(m_ChessBoard);
	}
		
	
	CString sNodeCount;

	if (bIsRed) {
		sNodeCount.Format(_T("第%d步: 红棋思考花费 %d ms. %d 个节点被搜索."), stepCount, GetTickCount() - timecount, count);
	}
	else {
		sNodeCount.Format(_T("第%d步: 黑棋思考花费 %d ms. %d 个节点被搜索."), stepCount, GetTickCount() - timecount, count);
	}
	

	m_ComputeList.InsertString(0, sNodeCount);  //显示信息

	stepCount++;										 //bredTurn = FALSE;
	bIsRed = bIsRed ^ 0x1;
	count = 0;
	InvalidateRect(NULL, FALSE);
	UpdateWindow();
}

void CmkjchessDlg::OnBnClickedAuto()
{
	// TODO: 在此添加控件通知处理程序代码
	bIsStart = TRUE;
	((CButton*)GetDlgItem(IDC_AUTO))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_NEWGAME))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_STOP))->EnableWindow(TRUE);
	
	while (bIsStart && Gameover ==0)
		{
				// 派发消息  
				MSG msg;

				if (PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE))
				{
					::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				}
				//Sleep(1000);
				calculate();
				if (Gameover != 0) {
					((CButton*)GetDlgItem(IDC_NEWGAME))->EnableWindow(TRUE);
					((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);
					((CButton*)GetDlgItem(IDC_AUTO))->EnableWindow(FALSE);
					((CButton*)GetDlgItem(IDC_NextStep))->EnableWindow(FALSE);
					((CButton*)GetDlgItem(IDC_STOP))->EnableWindow(FALSE);
				}	
		}	
}

void CmkjchessDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nSelectedEngine = m_SearchEngineList.GetCurSel();  //保存用户选择的搜索引擎号码
	m_nSelectedEngine1 = m_SearchEngineList1.GetCurSel();  //保存用户选择的搜索引擎号码
	if (m_pSE)
		delete m_pSE;
	switch (GetSelectedEngine())
	{
	case 0:
		m_pSE = new CNegamaxEngine;  //负极大值引擎
		break;

	case 1:
		m_pSE = new CAlphaBetaEngine;  //alpha-beta搜索引擎
		break;

	case 2:
		m_pSE = new CFAlphaBetaEngine;
		break;

	case 3:
		m_pSE = new CAspirationSearch;
		break;

	case 4:
		m_pSE = new CPVS_Engine;
		break;

	case 5:
		m_pSE = new CIDAlphabeta;
		break;

	case 6:
		m_pSE = new CAlphaBetaAndTT;
		break;

	case 7:
		m_pSE = new CAlphabeta_HH;
		break;

	case 8:
		m_pSE = new CMTD_f;
		break;

	case 9:
		m_pSE = new CNegaScout_TT_HH;
		break;

	default:
		m_pSE = new CNegamaxEngine;  //默认负极大值搜索引擎
		break;
	}

	if (m_pSE1)
		delete m_pSE1;
	switch (GetSelectedEngine1())
	{
	case 0:
		m_pSE1 = new CNegamaxEngine;  //负极大值引擎
		break;

	case 1:
		m_pSE1 = new CAlphaBetaEngine;  //alpha-beta搜索引擎
		break;

	case 2:
		m_pSE1 = new CFAlphaBetaEngine;
		break;

	case 3:
		m_pSE1 = new CAspirationSearch;
		break;

	case 4:
		m_pSE1 = new CPVS_Engine;
		break;

	case 5:
		m_pSE1 = new CIDAlphabeta;
		break;

	case 6:
		m_pSE1 = new CAlphaBetaAndTT;
		break;

	case 7:
		m_pSE1 = new CAlphabeta_HH;
		break;

	case 8:
		m_pSE1 = new CMTD_f;
		break;

	case 9:
		m_pSE1 = new CNegaScout_TT_HH;
		break;

	default:
		m_pSE1 = new CNegamaxEngine;  //默认负极大值搜索引擎
		break;
	}

	m_nSelectedPly = m_SetPly.GetPos();  //保存用户选择的搜索深度
	m_pSE->SetSearchDepth(GetSelectedPly());

	m_nSelectedPly1 = m_SetPly1.GetPos();  //保存用户选择的搜索深度	
	m_pSE1->SetSearchDepth1(GetSelectedPly1());

	if (pMG)
		delete pMG;  //删去挂在搜索引擎上的走法产生器
	pMG = new CMoveGenerator;

	if (pMG1)
		delete pMG1;  //删去挂在搜索引擎上的走法产生器
	pMG1 = new CMoveGenerator;

	if (pEvel)
		delete pEvel;  //删去挂在搜索引擎上的估值核心
	pEvel = new CEveluation;

	if (pEvel1)
		delete pEvel1;  //删去挂在搜索引擎上的估值核心	
	pEvel1 = new CEveluation;
	
	
	
	m_pSE->SetMoveGenerator(pMG);
	m_pSE->SetEveluator(pEvel);
	
	m_pSE1->SetMoveGenerator(pMG1);
	m_pSE1->SetEveluator(pEvel1);	

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

	((CButton*)GetDlgItem(IDC_AUTO))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_NextStep))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_NEWGAME))->EnableWindow(TRUE);

	InvalidateRect(NULL, FALSE);
	UpdateWindow();
}


void CmkjchessDlg::OnBnClickedNewgame()
{
	// TODO: 在此添加控件通知处理程序代码
	IsFirstTurn = false;
	stepCount = 1;
	m_ComputeList.ResetContent();
	((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	this->OnInitDialog();
	InvalidateRect(NULL, FALSE);
	UpdateWindow();
}

void CmkjchessDlg::OnBnClickedNextstep()
{
	// TODO: 在此添加控件通知处理程序代码
	
	((CButton*)GetDlgItem(IDC_NextStep))->EnableWindow(FALSE);	
	calculate();
	((CButton*)GetDlgItem(IDC_NextStep))->EnableWindow(TRUE);
}


afx_msg LRESULT CmkjchessDlg::OnFirststepMsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


void CmkjchessDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	bIsStart = FALSE;
	((CButton*)GetDlgItem(IDC_AUTO))->EnableWindow(TRUE);
	//((CButton*)GetDlgItem(IDC_NEWGAME))->EnableWindow(FALSE);
}
