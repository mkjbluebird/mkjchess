#include "stdafx.h"
#include "mkjchess.h"
#include "PVS_Engine.h"


CPVS_Engine::CPVS_Engine()
{
}


CPVS_Engine::~CPVS_Engine()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CPVS_Engine::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);  //取棋局
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //设定最大搜索深度
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}

	//	for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth; m_nMaxDepth++)
	{
		PrincipalVariation(m_nMaxDepth, -20000, 20000);  //调用PVS搜索引擎，搜索最佳落子
	}

	MakeMove(&m_cmBestMove);  //应用最佳落子
	memcpy(position, CurPosition, 90);  //保存变化后的棋局
}

extern short Gameover;
int CPVS_Engine::PrincipalVariation(int depth, int alpha, int beta)
{
	int current = -20000;  //当前分数，不断修改为最大值。
	int score;
	int Count, i;
	BYTE type;
	int best;

	Gameover = IsGameOver(CurPosition, depth);  //检查当前游戏是否结束
	if (Gameover != 0)
		return Gameover;

	if (depth <= 0)	//叶子节点取估值
		return m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //罗列所有可能走法
	
	type = MakeMove(&m_pMG->m_MoveList[depth][0]);  //开始第一个节点
	best = -PrincipalVariation(depth - 1, -beta, -alpha);  //使用全窗口搜索第一个节点
	UnMakeMove(&m_pMG->m_MoveList[depth][0], type);  //恢复前一局面
	if (depth == m_nMaxDepth)  //如果是根节点，则保存最佳落子
		m_cmBestMove = m_pMG->m_MoveList[depth][0];

	for (i = 1; i<Count; i++)  //从第二个节点开始，对每一节点
	{

		if (best < beta)  //如果是非Beta剪枝
		{
			if (best > alpha)
				alpha = best;  //修改alpha界限
			type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //产生子节点
			score = -PrincipalVariation(depth - 1, -alpha - 1, -alpha);  //使用极窄窗搜索  
			if (score > alpha && score < beta)
			{   //fail high,重新搜索
				best = -PrincipalVariation(depth - 1, -beta, -score);
				if (depth == m_nMaxDepth)  //为根节点，保存最佳走法
					m_cmBestMove = m_pMG->m_MoveList[depth][i];
			}
			else if (score > best)
			{  //窄窗搜索成功
				best = score;
				if (depth == m_nMaxDepth)
					m_cmBestMove = m_pMG->m_MoveList[depth][i];  //为根节点，保存最佳走法
			}
			UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //恢复前一局面
		}
	}

	return best;  //返回最佳值
}