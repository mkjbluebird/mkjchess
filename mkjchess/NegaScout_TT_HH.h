#pragma once
#include "SearchEngine.h"
#include "TranspositionTable.h"
#include "HistoryHeuristic.h"
class CNegaScout_TT_HH :
	public CSearchEngine, public CHistoryHeuristic, public CTranspositionTable
{
public:
	CNegaScout_TT_HH();
	virtual ~CNegaScout_TT_HH();

	virtual void SearchAGoodMove(BYTE position[10][9]);  //计算一步好棋，博弈接口

protected:
	int NegaScout(int depth, int alpha, int beta);  //搜索函数
};

