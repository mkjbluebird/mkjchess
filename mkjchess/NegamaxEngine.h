#pragma once
#include "SearchEngine.h"
class CNegamaxEngine :
	public CSearchEngine
{
public:
	CNegamaxEngine();
	virtual ~CNegamaxEngine();

	virtual void SearchAGoodMove(BYTE position[10][9]);  //�����������ֵ���һ����
protected:
	int NegaMax(int depth);  //������ֵ��������
};
