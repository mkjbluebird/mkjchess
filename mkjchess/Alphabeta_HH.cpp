#include "stdafx.h"
#include "mkjchess.h"
#include "Alphabeta_HH.h"


CAlphabeta_HH::CAlphabeta_HH()
{
}


CAlphabeta_HH::~CAlphabeta_HH()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CAlphabeta_HH::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);  //取出棋局
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //设定最大搜索深度
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}
	ResetHistoryTable();  //初始化历史记录表
	alphabeta(m_nMaxDepth, -20000, 20000);  //进行alpha-beta搜索
	MakeMove(&m_cmBestMove);  //走出最佳落子
	memcpy(position, CurPosition, 90);  //保存棋局
}

extern short Gameover;
int CAlphabeta_HH::alphabeta(int depth, int alpha, int beta)  //alpha-beta搜索
{
	int current = -20000;  //当前分数，不断修改为最大值。
	int score;
	int Count, i;
	BYTE type;

	Gameover = IsGameOver(CurPosition, depth);  //查看游戏是否结束
	if (Gameover != 0)
		return Gameover;  //游戏结束，返回极值

	if (depth <= 0)	//叶子节点取估值
		return m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //产生下一步所有可能落子

	for (i = 0; i<Count; i++)
	{
		m_pMG->m_MoveList[depth][i].Score =
			GetHistoryScore(&m_pMG->m_MoveList[depth][i]);  //取所有走法的历史得分
	}
	MergeSort(m_pMG->m_MoveList[depth], Count, 0);  //排序Count种走法历史得分
	int bestmove = -1;  //记录最佳落子局面的变量
	for (i = 0; i<Count; i++)
	{

		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //生成子节点
		score = -alphabeta(depth - 1, -beta, -alpha);  //递归搜索子节点
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //撤销子节点

		if (score > alpha)
		{
			alpha = score;  //保存最大值，调整边界
			if (depth == m_nMaxDepth)  //如果为根节点
				m_cmBestMove = m_pMG->m_MoveList[depth][i];  //保存最佳走法
			bestmove = i;  //记录最佳走法位置
		}
		if (alpha >= beta)
		{
			bestmove = i;  //记录最佳走法位置
			break;  //beta剪枝
		}

	}
	if (bestmove != -1)
		EnterHistoryScore(&m_pMG->m_MoveList[depth][bestmove], depth);  //将最佳走法汇入历史记录表
	return alpha;
}

