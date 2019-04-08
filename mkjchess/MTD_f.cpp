#include "stdafx.h"
#include "mkjchess.h"
#include "MTD_f.h"


CMTD_f::CMTD_f()
{
}


CMTD_f::~CMTD_f()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CMTD_f::SearchAGoodMove(BYTE position[10][9])
{
	int firstguess = 0;
	memcpy(CurPosition, position, 90);  //取棋局
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //设定最大搜索深度
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}
	CalculateInitHashKey(CurPosition);  //计算初始棋盘的哈希值
	ResetHistoryTable();  //初始化历史记录表
	if (bIsRed) {
		for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth; m_nMaxDepth++)  //迭代深化调用MTD(f)搜索
		{
			firstguess = mtdf(firstguess, m_nMaxDepth);
		}
	}
	else {
		for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth1; m_nMaxDepth++)  //迭代深化调用MTD(f)搜索
		{
			firstguess = mtdf(firstguess, m_nMaxDepth);
		}
	}
	MakeMove(&m_cmBackupBM);  //应用最佳落子
	memcpy(position, CurPosition, 90);  //保存走过的棋局
	//return 0;
}


int CMTD_f::mtdf(int firstguess, int depth)
{
	int g, lowerbound, upperbound, beta;

	g = firstguess;  //初始猜测值
	//初始搜索范围-20000~20000
	upperbound = 20000;
	lowerbound = -20000;
	while (lowerbound < upperbound)  //在合理估值范围
	{
		m_cmBackupBM = m_cmBestMove;
		//趋向目标收缩窗口
		if (g == lowerbound)
			beta = g + 1;
		else
			beta = g;
		//空窗探测
		g = FAlphaBeta(depth, beta - 1, beta);
		if (g<beta)
			upperbound = g;  //调整上边界
		else
			lowerbound = g;  //调整下边界
	}
	return g;
}

extern short Gameover;
int CMTD_f::FAlphaBeta(int depth, int alpha, int beta)
{
	int current = -19999;
	int score;
	int Count, i;
	BYTE type;

	Gameover = IsGameOver(CurPosition, depth);  //查看游戏是否结束
	if (Gameover != 0)
		return Gameover;  //胜负已分，返回极值

	//查看当前节点是否存在哈希表中
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	score = LookUpHashTable(alpha, beta, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);
	if (score != 66666)
		return score;  //存在，返回哈希表中的值

	if (depth <= 0)	//叶子节点取估值
	{
		current = m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		EnterHashTable(exact, current, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //保存入置换表
		return current;
	}

	//产生所有可能的下一步
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);

	for (i = 0; i<Count; i++)
	{
		m_pMG->m_MoveList[depth][i].Score =
			GetHistoryScore(&m_pMG->m_MoveList[depth][i]);  //取出每一个可能落子的历史得分
	}
	MergeSort(m_pMG->m_MoveList[depth], Count, 0);  //将有效走法的历史得分排序

	int bestmove = -1;

	int eval_is_exact = 0;  //数据类型标志

	for (i = 0; i<Count; i++)
	{
		//产生子节点的哈希值
		Hash_MakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //产生子节点
		//递归搜索子节点
		score = -FAlphaBeta(depth - 1, -beta, -alpha);
		//恢复当前节点的哈希值
		Hash_UnMakeMove(&m_pMG->m_MoveList[depth][i], type, CurPosition);
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //撤销子节点


		if (score > current)
		{
			current = score;
			
			if (depth == m_nMaxDepth)  //如果为根节点
				m_cmBestMove = m_pMG->m_MoveList[depth][i];  //保存最佳值
			bestmove = i;  //记录最佳值游标
			if (score >= beta) //beta剪枝
			{
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				EnterHashTable(lower_bound, score, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //将下边界保存进置换表
				EnterHistoryScore(&m_pMG->m_MoveList[depth][i], depth);  //将当前落子汇入历史记录
				return current;
			}
			if (score > alpha)
			{
				eval_is_exact = 1;  //设定确切值标志
				alpha = score;  //保留最大值
			}
		}

	}
	if (bestmove != -1)  //如果为有效值
		EnterHistoryScore(&m_pMG->m_MoveList[depth][bestmove], depth);  //将当前落子汇入历史记录
	if (eval_is_exact)  //如果为有效值
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		EnterHashTable(exact, alpha, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //将搜索结果放入置换表
	else
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		EnterHashTable(upper_bound, current, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //否则将上边界放入置换表
	return current;  //返回最佳值
}
