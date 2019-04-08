#pragma once
#include "SearchEngine.h"
#include "TranspositionTable.h"
#include "HistoryHeuristic.h"
class CMTD_f :
	public CSearchEngine, public CTranspositionTable, public CHistoryHeuristic
{
public:
	CMTD_f();
	virtual ~CMTD_f();
	virtual void SearchAGoodMove(BYTE position[10][9]);  //计算一步好棋

protected:
	int mtdf(int firstguess, int depth);  //MTD(f)搜索函数
	int FAlphaBeta(int depth, int alpha, int beta);  //带有置换表的Fail-soft alpha-beta搜索函数
	CHESSMOVE m_cmBackupBM;  //保存最佳落子变量
};

