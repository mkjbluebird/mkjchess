#include "stdafx.h"
#include "mkjchess.h"
#include "IDAlphabeta.h"


CIDAlphabeta::CIDAlphabeta()
{
}


CIDAlphabeta::~CIDAlphabeta()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CIDAlphabeta::SearchAGoodMove(BYTE position[10][9])
{
	CHESSMOVE backupmove;  //最佳落子备份
	memcpy(CurPosition, position, 90);  //取当前局面
	m_nTimeCount = GetTickCount();  //求当前时间
	if (bIsRed) {
		for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth; m_nMaxDepth++)  //迭代深化搜索
		{
			if (alphabeta(m_nMaxDepth, -20000, 20000) != 66666)  //如果本次搜索没有终止
				backupmove = m_cmBestMove;  //保存最佳落子
		}
	}
	else {
		for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth1; m_nMaxDepth++)  //迭代深化搜索
		{
			if (alphabeta(m_nMaxDepth, -20000, 20000) != 66666)  //如果本次搜索没有终止
				backupmove = m_cmBestMove;  //保存最佳落子
		}
	}	
	MakeMove(&backupmove);  //根据最佳落子改变棋局
	memcpy(position, CurPosition, 90);  //保存最佳落子
}

extern short Gameover;
int CIDAlphabeta::alphabeta(int depth, int alpha, int beta)
{
	int current = -20000;  //当前分数，不断修改为最大值。
	int score;
	int Count, i;
	BYTE type;

	Gameover = IsGameOver(CurPosition, depth);  //游戏是否结束
	if (Gameover != 0)
		return Gameover;  //结束，返回极值

	if (depth <= 0)	//叶子节点取估值
		return m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //取所有可能走法
	//此下if语句之内代码，用于调整底层节点排列顺序
	//如果某走法在上次迭代中是Best Move,将其排在第一位
	if (depth == m_nMaxDepth && m_nMaxDepth > 1)
	{
		for (i = 1; i < Count; i++)
		{
			if (m_pMG->m_MoveList[depth][i].From.x == m_cmBestMove.From.x
				&& m_pMG->m_MoveList[depth][i].From.y == m_cmBestMove.From.y
				&& m_pMG->m_MoveList[depth][i].To.x == m_cmBestMove.To.x
				&& m_pMG->m_MoveList[depth][i].To.y == m_cmBestMove.To.y)  //如果某节点为最佳落子
			{
				m_pMG->m_MoveList[depth][i] = m_pMG->m_MoveList[depth][0];  //交换两个走法的位置
				m_pMG->m_MoveList[depth][0] = m_cmBestMove;
			}
		}
	}

	for (i = 0; i<Count; i++)
	{
		if (depth == m_nMaxDepth)
		{
			if (GetTickCount() - m_nTimeCount >= 1000000)  //查看是否已到限定时间，10秒
				return 66666;  //返回中止标记
		}
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //产生子节点
		score = -alphabeta(depth - 1, -beta, -alpha);  //递归搜索
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //撤销子节点

		if (score > alpha)
		{
			alpha = score;  //调整alpha值
			if (depth == m_nMaxDepth)  //如果为根节点，保存最佳走法
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
		}
		if (alpha >= beta)
			break;  //beta剪枝


	}
	return alpha;  //返回最大值
}
