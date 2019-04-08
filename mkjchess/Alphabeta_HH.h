#pragma once
#include "SearchEngine.h"
#include "HistoryHeuristic.h"
class CAlphabeta_HH :
	public CSearchEngine, public CHistoryHeuristic
{
public:
	CAlphabeta_HH();
	virtual ~CAlphabeta_HH();

	virtual void SearchAGoodMove(BYTE position[10][9]);  //生成一步好棋

protected:
	int alphabeta(int depth, int alpha, int beta);  //alpha-beta搜索函数
};

