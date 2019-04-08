#pragma once
#include "SearchEngine.h"
class CIDAlphabeta :
	public CSearchEngine
{
public:
	CIDAlphabeta();
	virtual ~CIDAlphabeta();
	virtual void SearchAGoodMove(BYTE position[10][9]);  //������һ��

protected:
	int alphabeta(int depth, int alpha, int beta);  //alpha-beta��������
	int m_nTimeCount;  //��ʱ����
};

