#include "stdafx.h"
#include "mkjchess.h"
#include "AspirationSearch.h"


CAspirationSearch::CAspirationSearch()
{
}


CAspirationSearch::~CAspirationSearch()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CAspirationSearch::SearchAGoodMove(BYTE position[10][9])  //搜索一步好棋
{
	int x, y;
	memcpy(CurPosition, position, 90);  //取棋局
	//m_nMaxDepth = m_nSearchDepth - 1;  //先进行浅层搜索，猜测目标值范围
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth - 1;  //设定最大搜索深度
	}
	else {
		m_nMaxDepth = m_nSearchDepth1 - 1;
	}
	x = FAlphaBeta(m_nMaxDepth, -20000, 20000);
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //设定最大搜索深度
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}
	y = FAlphaBeta(m_nMaxDepth, x - 50, x + 50);  //对目标值附近的小窗口进行搜索
	if (y < x - 50)  //如果值在x-50以下
		FAlphaBeta(m_nMaxDepth, -20000, y);  //调整搜索范围搜索
	if (y > x + 50)  //如果值在x+50以上
		FAlphaBeta(m_nMaxDepth, y, 20000);   //调整范围搜索
	MakeMove(&m_cmBestMove);  //应用最佳一步
	memcpy(position, CurPosition, 90);  //保存最新棋局
	//return 0;
}