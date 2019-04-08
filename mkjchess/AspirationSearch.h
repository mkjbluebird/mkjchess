#pragma once
#include "FAlphaBetaEngine.h"
class CAspirationSearch :
	public CFAlphaBetaEngine
{
public:
	CAspirationSearch();
	virtual ~CAspirationSearch();

	virtual void SearchAGoodMove(BYTE position[10][9]);
};

