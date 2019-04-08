#pragma once
#include "define.h"
#include "SearchEngine.h"
#include "NegamaxEngine.h"
#include "AlphaBetaEngine.h"
#include "AlphaBetaAndTT.h"
#include "FAlphaBetaEngine.h"
#include "PVS_Engine.h"
//#include "NegaScout.h"
#include "IDAlphabeta.h"
#include "AspirationSearch.h"
#include "Alphabeta_HH.h"
#include "mtd_f.h"
#include "NegaScout_TT_HH.h"
typedef struct _movechess
{
	BYTE nChessID;
	POINT ptMovePoint;
}MOVECHESS;
/*
typedef struct tagTHREADPARMS {
	CSearchEngine* m_pSE;
	BYTE m_ChessBoard[10][9];
	//CStatic m_OutputInfo;
} THREADPARMS;
*/
// CmkjchessDlg 对话框

class CmkjchessDlg : public CDialog
{
	DECLARE_DYNAMIC(CmkjchessDlg)

public:
	CmkjchessDlg(CWnd* pParent = NULL);   // 标准构造函数
	//virtual ~CChessDlg();
	CStatic m_OutputInfo;
	CMoveGenerator *pMG;
	CMoveGenerator *pMG1;
	CEveluation *pEvel;
	CEveluation *pEvel1;
	CSearchEngine *m_pSE;
	CSearchEngine *m_pSE1;
	int GetSelectedEngine() { return m_nSelectedEngine; };
	int GetSelectedEngine1() { return m_nSelectedEngine1; };
	int GetSelectedPly() { return m_nSelectedPly; };
	int GetSelectedPly1() { return m_nSelectedPly1; };
	//afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CSpinButtonCtrl m_SetPly;
	CSpinButtonCtrl m_SetPly1;
	CListBox m_SearchEngineList;
	CListBox m_SearchEngineList1;
	CListBox m_ComputeList;
	//friend CSearchEngine::~CSearchEngine();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MKJCHESS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
protected:
	//virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnBnClickedNewgame();
	int m_nSelectedEngine;
	int m_nSelectedEngine1;
	int m_nSelectedPly;
	int m_nSelectedPly1;
	DECLARE_MESSAGE_MAP()
private:
	//BOOL m_bGameOver;
	BYTE m_ChessBoard[10][9];
	BYTE m_BackupChessBoard[10][9];
	MOVECHESS m_MoveChess;
	POINT m_ptMoveChess;
	CBitmap m_BoardBmp;
	CImageList m_Chessman;
	int m_nBoardWidth;
	int m_nBoardHeight;
	
public:
	void calculate();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedAuto();
	//UINT FuncThread(LPVOID pParam);
	//afx_msg void OnIdok();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedNewgame();
	afx_msg void OnBnClickedNextstep();
protected:
	afx_msg LRESULT OnFirststepMsg(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedStop();
};
