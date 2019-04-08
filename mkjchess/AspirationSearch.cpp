#include "stdafx.h"
#include "mkjchess.h"
#include "AspirationSearch.h"


CAspirationSearch::CAspirationSearch()
{
}


CAspirationSearch::~CAspirationSearch()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CAspirationSearch::SearchAGoodMove(BYTE position[10][9])  //����һ������
{
	int x, y;
	memcpy(CurPosition, position, 90);  //ȡ���
	//m_nMaxDepth = m_nSearchDepth - 1;  //�Ƚ���ǳ���������²�Ŀ��ֵ��Χ
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth - 1;  //�趨����������
	}
	else {
		m_nMaxDepth = m_nSearchDepth1 - 1;
	}
	x = FAlphaBeta(m_nMaxDepth, -20000, 20000);
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //�趨����������
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}
	y = FAlphaBeta(m_nMaxDepth, x - 50, x + 50);  //��Ŀ��ֵ������С���ڽ�������
	if (y < x - 50)  //���ֵ��x-50����
		FAlphaBeta(m_nMaxDepth, -20000, y);  //����������Χ����
	if (y > x + 50)  //���ֵ��x+50����
		FAlphaBeta(m_nMaxDepth, y, 20000);   //������Χ����
	MakeMove(&m_cmBestMove);  //Ӧ�����һ��
	memcpy(position, CurPosition, 90);  //�����������
	//return 0;
}