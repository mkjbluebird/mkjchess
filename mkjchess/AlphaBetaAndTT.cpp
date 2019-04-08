#include "stdafx.h"
#include "mkjchess.h"
#include "AlphaBetaAndTT.h"

extern BOOL bIsRed;
CAlphaBetaAndTT::CAlphaBetaAndTT()
{
}


CAlphaBetaAndTT::~CAlphaBetaAndTT()
{
}

extern int m_nMaxDepth;
void CAlphaBetaAndTT::SearchAGoodMove(BYTE position[10][9])  //生成下一步棋
{
	memcpy(CurPosition, position, 90);  //取棋局
	CalculateInitHashKey(CurPosition);  //计算初始棋局哈希值
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //设定最大搜索深度
	}		
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}		
	alphabeta(m_nMaxDepth, -20000, 20000);  //进行alpha-beta搜索
	MakeMove(&m_cmBestMove);  //走出最佳落子
	memcpy(position, CurPosition, 90);  //保存棋局
}

extern short Gameover;
int CAlphaBetaAndTT::alphabeta(int depth, int alpha, int beta)  //alpha-beta搜索函数
{
	int current = -20000;  //当前分数，不断修改为最大值。
	int score;
	int Count, i;
	BYTE type;
	int side;

	Gameover = IsGameOver(CurPosition, depth);  //检查游戏是否结束
	if (Gameover != 0)
		return Gameover;  //游戏结束，返回极值
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	side = ((m_nMaxDepth - depth) + bIsRed) % 2;

	score = LookUpHashTable(alpha, beta, depth, side);  //查看置换表中是否有当前节点
	if (score != 66666)
		return score;  //有当前节点，返回表中的值

	if (depth <= 0)	//叶子节点取估值
	{
		score = m_pEval->Eveluate(CurPosition, side);
		EnterHashTable(exact, score, depth, side);  //将当前节点估值保存进置换表
		return score;
	}


	//计算当前棋局下一步的所有可能落子
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, side);

	int eval_is_exact = 0;  //数据类型标志

	for (i = 0; i<Count; i++)  //对每一个下一步所有可能落子
	{
		Hash_MakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);  //产生该落子所对应子节点的哈希值
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //产生子节点

		score = -alphabeta(depth - 1, -beta, -alpha);  //递归搜索子节点

		Hash_UnMakeMove(&m_pMG->m_MoveList[depth][i], type, CurPosition);  //恢复该节点的哈希值
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //撤销子节点
		if (score >= beta)
		{  //beta剪枝
			EnterHashTable(lower_bound, score, depth, side);  //将节点下边界存入置换表
			return score;  //返回下边界
		}

		if (score > alpha)
		{
			alpha = score;  //取最大值
			eval_is_exact = 1;  //设定确切值标志
			if (depth == m_nMaxDepth)  //根节点，保存最佳走法
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
		}

	}
	//将搜索结果保存进置换表
	if (eval_is_exact)  //如果是确切值
		EnterHashTable(exact, alpha, depth, side);
	else
		EnterHashTable(upper_bound, alpha, depth, side);  //保存上边界
	return alpha;  //返回最佳值/上界
}


