#include "stdafx.h"
#include "mkjchess.h"
#include "SearchEngine.h"


CSearchEngine::CSearchEngine()
{
}

extern short Gameover;
CSearchEngine::~CSearchEngine()
{
	if (Gameover != 0) {
		if(m_pMG)
			delete m_pMG;  //删去挂在搜索引擎上的走法产生器
		if(m_pMG1)
			delete m_pMG1;  //删去挂在搜索引擎上的走法产生器
		if(m_pEval)
			delete m_pEval;  //删去挂在搜索引擎上的估值核心
		if(m_pEval1)
			delete m_pEval1;  //删去挂在搜索引擎上的估值核心
	}	
}

BYTE CSearchEngine::MakeMove(CHESSMOVE* move) //【根据传入的走法改变棋局，move是进行的走法】
{
	BYTE nChessID;
	nChessID = CurPosition[move->To.y][move->To.x];  //取目标位置棋子
	CurPosition[move->To.y][move->To.x] = CurPosition[move->From.y][move->From.x];  //把棋子移动到目标位置
	CurPosition[move->From.y][move->From.x] = NOCHESS;  //将原位置清空
	return nChessID;  //返回吃掉的棋子
}

void CSearchEngine::UnMakeMove(CHESSMOVE* move, BYTE nChessID) //【根据传入的走法恢复棋局，move是进行的走法，nChessID是原棋局上move目标位置的棋子类型】
{
	CurPosition[move->From.y][move->From.x] = CurPosition[move->To.y][move->To.x]; //将目标位置的棋子拷回原位
	CurPosition[move->To.y][move->To.x] = nChessID; //恢复目标位置的棋子
}
extern int m_nMaxDepth;
int CSearchEngine::IsGameOver(BYTE position[10][9], int nDepth)  //【检查棋局是否结束，未结束：返回0，结束：返回极大/极小值】
{
	int i, j;
	BOOL RedLive = FALSE, BlackLive = FALSE;

	for (i = 7; i < 10; i++)  //检查红方的将帅
		for (j = 3; j < 6; j++)
		{
			if (position[i][j] == B_KING)
				BlackLive = TRUE;
			if (position[i][j] == R_KING)
				RedLive = TRUE;
		}

	for (i = 0; i < 3; i++)  //检查黑方的将帅
		for (j = 3; j < 6; j++)
		{
			if (position[i][j] == B_KING)
				BlackLive = TRUE;
			if (position[i][j] == R_KING)
				RedLive = TRUE;
		}

	i = (m_nMaxDepth - nDepth + 1) % 2;//取当前是奇偶标志

	if (!RedLive) //红将是否被吃
		if (i)
			return 19990 + nDepth;
		else
			return -19990 - nDepth;
	if (!BlackLive)//黑将是否被吃
		if (i)
			return -19990 - nDepth;
		else
			return 19990 + nDepth;
	return 0;  //双方都未死，返回0
}
