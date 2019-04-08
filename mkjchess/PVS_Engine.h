#pragma once
#include "SearchEngine.h"
class CPVS_Engine :
	public CSearchEngine
{
public:
	CPVS_Engine();
	virtual ~CPVS_Engine();

	virtual void SearchAGoodMove(BYTE position[10][9]);

protected:
	int PrincipalVariation(int depth, int alpha, int beta);
};

