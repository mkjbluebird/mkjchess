#pragma once
#include "SearchEngine.h"

class CAlphaBetaEngine :
	public CSearchEngine
{
public:
	CAlphaBetaEngine();
	~CAlphaBetaEngine();

	virtual void SearchAGoodMove(BYTE position[10][9]);

protected:
	int alphabeta(int depth, int alpha, int beta);

};

