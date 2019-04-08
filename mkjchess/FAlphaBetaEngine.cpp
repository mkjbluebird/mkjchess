#include "stdafx.h"
#include "mkjchess.h"
#include "FAlphaBetaEngine.h"


CFAlphaBetaEngine::CFAlphaBetaEngine()
{
}


CFAlphaBetaEngine::~CFAlphaBetaEngine()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CFAlphaBetaEngine::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);  //取当前局面
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //设定最大搜索深度
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}
	FAlphaBeta(m_nMaxDepth, -20000, 20000);  //开始计算
	MakeMove(&m_cmBestMove);  //走下最佳落子
	memcpy(position, CurPosition, 90);  //保存最佳落子局面
	//return 0;
}

extern short Gameover;
int CFAlphaBetaEngine::FAlphaBeta(int depth, int alpha, int beta)
{
	int current = -20000;  //初始极小值
	int score;
	int Count, i;
	BYTE type;

	Gameover = IsGameOver(CurPosition, depth);  //检查这个节点游戏是否结束
	if (Gameover != 0)
		return Gameover;   //游戏结束，返回极大/极小值

	if (depth <= 0)	//叶子节点取估值
		return m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //计算子节点

	for (i = 0; i<Count; i++)
	{  //搜索每个节点的分数
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //进入子节点
		score = -FAlphaBeta(depth - 1, -beta, -alpha);  //取子节点的分数
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //恢复到父节点

		if (score > current)
		{  //如果分数大于极小值
			current = score;  //修改分值界限
			if (depth == m_nMaxDepth)  //为根节点，保存最佳落子
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
			if (score > alpha)
				alpha = score;  //修改alpha边界
			if (score >= beta) //beta剪枝
				break;
		}

	}

	return current;  //返回极大值或边界值
}

