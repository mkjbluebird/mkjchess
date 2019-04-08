#include "stdafx.h"
#include "mkjchess.h"
#include "AlphaBetaEngine.h"
//#include "mkjchessDlg.h"
//#include "NewGame.h"
//#include "MoveGenerator.h"

CAlphaBetaEngine::CAlphaBetaEngine()
{
}


CAlphaBetaEngine::~CAlphaBetaEngine()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CAlphaBetaEngine::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);  //将当前局面取出到CurPosition
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //设定最大搜索深度
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}
	alphabeta(m_nMaxDepth, -20000, 20000);  //进行alphabeta搜索
	MakeMove(&m_cmBestMove);  //走出最佳步
	memcpy(position, CurPosition, 90);  //保存棋局
}

extern short Gameover;
int CAlphaBetaEngine::alphabeta(int depth, int alpha, int beta)
{
	int current = -20000;  //当前分数，不断修改为最大值。
	int score;
	int Count, i;
	BYTE type;

	Gameover = IsGameOver(CurPosition, depth);  //游戏是否结束
	if (Gameover != 0)
		return Gameover;  //结束，返回估值

	if (depth <= 0)	//叶子节点取估值
		return m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //找出当前局面所有可能走法，放入m_pMG->m_MoveList队列

	for (i = 0; i<Count; i++)
	{
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //应用当前落子，变成子节点局面
		score = -alphabeta(depth - 1, -beta, -alpha);  //递归搜索子节点
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //从子节点恢复棋局到当前节点

		if (score > alpha)
		{
			alpha = score;  //保留极大值
			if (depth == m_nMaxDepth)  //如果为根节点
				m_cmBestMove = m_pMG->m_MoveList[depth][i];  //保存最佳走法
		}
		if (alpha >= beta)
			break;  //剪枝


	}
	return alpha;  //返回极大值
}
