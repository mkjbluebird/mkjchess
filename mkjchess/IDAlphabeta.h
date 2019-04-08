#pragma once
#include "SearchEngine.h"
class CIDAlphabeta :
	public CSearchEngine
{
public:
	CIDAlphabeta();
	virtual ~CIDAlphabeta();
	virtual void SearchAGoodMove(BYTE position[10][9]);  //搜索下一步

protected:
	int alphabeta(int depth, int alpha, int beta);  //alpha-beta搜索函数
	int m_nTimeCount;  //计时变量
};

