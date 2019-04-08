#include "stdafx.h"
#include "mkjchess.h"
#include "NegamaxEngine.h"


CNegamaxEngine::CNegamaxEngine()
{
}


CNegamaxEngine::~CNegamaxEngine()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CNegamaxEngine::SearchAGoodMove(BYTE position[10][9]) //【此函数针对传入的position找出最佳走法，并走一步】
{
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //设定最大搜索深度
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}

	memcpy(CurPosition, position, 90);  //将传入的棋局复制到传入成员变量中

	NegaMax(m_nMaxDepth);  //调用负极大值搜索函数找出最佳走法

	MakeMove(&m_cmBestMove); //将棋局修改为下一步
	memcpy(position, CurPosition, 90);  //将修改过的棋盘复制到传入棋局中，传出
	
}

extern short Gameover;
int CNegamaxEngine::NegaMax(int depth)  //【负极大值搜索函数，depth表示节点离叶子节点的层数】
{
	int current = -20000;  //当前分数，不断修改为最大值。
	int score;  //每一节点分数
	int Count, i;  //Count:所有可能走法数，i:是否结束标志
	BYTE type;  //

	Gameover = IsGameOver(CurPosition, depth); //检查棋局是否结束
	if (Gameover != 0)
		return Gameover;  //棋局结束，返回极大/极小值

	if (depth <= 0)	//叶子节点取估值
		return m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);  //返回估值
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //列出当前局面下一步的所有可能走法

	for (i = 0;i<Count;i++)
	{

		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //根据落子产生新局面
		score = -NegaMax(depth - 1); //递归调用负极大值搜索下一层节点
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type); //恢复当前局面

		if (score > current)  //如果score是最大的
		{
			current = score;  //修改最大值
			if (depth == m_nMaxDepth)
			{
				m_cmBestMove = m_pMG->m_MoveList[depth][i];  //在根部保存最佳走法
			}
		}

	}

	return current;  //返回极大值(分数)
}