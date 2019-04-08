#include "stdafx.h"
#include "mkjchess.h"
#include "NegaScout_TT_HH.h"


CNegaScout_TT_HH::CNegaScout_TT_HH()
{
}


CNegaScout_TT_HH::~CNegaScout_TT_HH()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CNegaScout_TT_HH::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);  //取棋局
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //设定最大搜索深度
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}
	CalculateInitHashKey(CurPosition);  //初始化初始棋盘的哈希值
	ResetHistoryTable();  //初始化历史记录表
	//	m_nMaxDepth = 1;
	//	NegaScout(m_nMaxDepth, -20000, 20000);
	//	m_nMaxDepth = m_nSearchDepth;
	//	for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth; m_nMaxDepth++)
	NegaScout(m_nMaxDepth, -20000, 20000);  //使用NegaSout搜索
	MakeMove(&m_cmBestMove);  //应用最佳落子
	memcpy(position, CurPosition, 90);  //保存改变的棋局
}

//NegaScout搜索函数
extern short Gameover;
int CNegaScout_TT_HH::NegaScout(int depth, int alpha, int beta)
{
	int current = -20000;  //当前分数，不断修改为最大值。
	int Count, i;
	BYTE type;
	int a, b, t;
	int side;
	int score;

	Gameover = IsGameOver(CurPosition, depth);  //查看游戏是否结束
	if (Gameover != 0)
		return Gameover;  //游戏结束，返回极值
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	side = ((m_nMaxDepth - depth) + bIsRed) % 2;  //当前计算哪一方

	score = LookUpHashTable(alpha, beta, depth, side);  //在置换表中查看当前数据是否有效
	if (score != 66666)
		return score;  //有效，返回当前有效值

	if (depth <= 0)	//叶子节点取估值
	{
		score = m_pEval->Eveluate(CurPosition, side);  //计算当前估值
		EnterHashTable(exact, score, depth, side);  //将估值存入置换表
		return score;  //返回估值
	}
	//产生下一步所有的可能落子
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, side);

	for (i = 0; i<Count; i++)
	{
		m_pMG->m_MoveList[depth][i].Score =
			GetHistoryScore(&m_pMG->m_MoveList[depth][i]);  //取出每一个可能落子的历史得分
	}
	MergeSort(m_pMG->m_MoveList[depth], Count, 0);  //将有效走法的历史得分排序

	int bestmove = -1;

	a = alpha;
	b = beta;
	int eval_is_exact = 0;
	for (i = 0; i < Count; i++)
	{
		Hash_MakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);  //产生子节点哈希值
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //产生子节点
		//递归搜索子节点，第一个节点是全窗口，其后是空窗口探测
		t = -NegaScout(depth - 1, -b, -a);

		if (t > a && t < beta && i > 0)
		{  //第一个后的节点，如果上面搜索大于上边界
			a = -NegaScout(depth - 1, -beta, -t);     /* re-search 重新搜索*/
			eval_is_exact = 1;  //设定数据类型为精确值
			if (depth == m_nMaxDepth)  //如果为根节点
				m_cmBestMove = m_pMG->m_MoveList[depth][i];  //保存最佳值
			bestmove = i;  //记录最佳值游标
		}
		Hash_UnMakeMove(&m_pMG->m_MoveList[depth][i], type, CurPosition);  //恢复当前节点哈希值
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //撤销子节点
		if (a < t)
		{  //第一次搜索命中
			eval_is_exact = 1;  //精确值
			a = t;  //调整边界
			if (depth == m_nMaxDepth)  //如果为根节点
				m_cmBestMove = m_pMG->m_MoveList[depth][i];  //保存最佳落子
		}
		if (a >= beta)
		{
			EnterHashTable(lower_bound, a, depth, side);  //将下边界存入置换表
			EnterHistoryScore(&m_pMG->m_MoveList[depth][i], depth);  //将当前落子汇入历史记录
			return a;  //beta剪枝
		}
		b = a + 1;                      /* set new null window设定新的空窗 */
	}
	if (bestmove != -1)  //如果为有效值
		EnterHistoryScore(&m_pMG->m_MoveList[depth][bestmove], depth);  //将当前落子汇入历史记录
	if (eval_is_exact)  //如果为精确值
		EnterHashTable(exact, a, depth, side);  //将精确值保存进置换表
	else
		EnterHashTable(upper_bound, a, depth, side);  //保存上边界
	return a;  //返回最佳值
}