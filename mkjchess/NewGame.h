#pragma once


// CNewGame 对话框

class CNewGame : public CDialog
{
	DECLARE_DYNAMIC(CNewGame)

public:
	CNewGame(CWnd* pParent = NULL);   // 标准构造函数
	int GetSelectedEngine() { return m_nSelectedEngine; };
	int GetSelectedPly() { return m_nSelectedPly; };
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CSpinButtonCtrl m_SetPly;
	CListBox m_SearchEngineList;
	//virtual ~CNewGame();


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWGAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	int m_nSelectedEngine;
	int m_nSelectedPly;
	DECLARE_MESSAGE_MAP()

//	BOOL REDFIRST;
};
