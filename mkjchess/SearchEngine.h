#pragma once
#include "MoveGenerator.h"
#include "Eveluation.h"
class CSearchEngine
{
public:
	CSearchEngine();
	virtual ~CSearchEngine();

	virtual void SearchAGoodMove(BYTE position[10][9]) = 0; //博弈接口，为当前局面走出下一步

	void SetSearchDepth(int nDepth) { m_nSearchDepth = nDepth; }; //设定最大搜索深度

	void SetSearchDepth1(int nDepth) { m_nSearchDepth1 = nDepth; }; //设定最大搜索深度

	void SetEveluator(CEveluation *pEval) { m_pEval = pEval; }; //设定估值引擎

	void SetMoveGenerator(CMoveGenerator *pMG) { m_pMG = pMG; }; //设定落子产生器
	
protected:
	virtual BYTE MakeMove(CHESSMOVE* move);  //根据某一走法产生之后棋局
	virtual void UnMakeMove(CHESSMOVE* move, BYTE type);  //恢复前一步的棋局
	virtual int IsGameOver(BYTE position[10][9], int nDepth);  //判断当前局面的胜负
	BYTE CurPosition[10][9];  //用于当前棋局状态的数组
	CHESSMOVE m_cmBestMove;  //变量：记录最佳走法
	CMoveGenerator *m_pMG;  //走法产生器指针
	CMoveGenerator *m_pMG1;  //走法产生器指针
	CEveluation *m_pEval;  //估值核心指针
	CEveluation *m_pEval1;  //估值核心指针
	int m_nSearchDepth;  //最大搜索深度
	int m_nSearchDepth1;  //最大搜索深度
	
};

