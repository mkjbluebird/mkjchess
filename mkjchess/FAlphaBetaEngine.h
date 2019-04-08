#pragma once
#include "SearchEngine.h"
class CFAlphaBetaEngine :
	public CSearchEngine
{
public:
	CFAlphaBetaEngine();
	virtual ~CFAlphaBetaEngine();

	virtual void SearchAGoodMove(BYTE position[10][9]);

protected:
	int FAlphaBeta(int depth, int alpha, int beta);
};

