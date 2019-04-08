#pragma once
#include "SearchEngine.h"
#include "TranspositionTable.h"
class CAlphaBetaAndTT :
	public CSearchEngine, public CTranspositionTable
{
public:
	CAlphaBetaAndTT();
	virtual ~CAlphaBetaAndTT();

	virtual void SearchAGoodMove(BYTE position[10][9]);
protected:
	int alphabeta(int depth, int alpha, int beta);
};

