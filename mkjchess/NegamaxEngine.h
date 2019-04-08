#pragma once
#include "SearchEngine.h"
class CNegamaxEngine :
	public CSearchEngine
{
public:
	CNegamaxEngine();
	virtual ~CNegamaxEngine();

	virtual void SearchAGoodMove(BYTE position[10][9]);  //【计算给定棋局的下一步】
protected:
	int NegaMax(int depth);  //负极大值搜索引擎
};

