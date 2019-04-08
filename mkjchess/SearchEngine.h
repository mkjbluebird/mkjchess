#pragma once
#include "MoveGenerator.h"
#include "Eveluation.h"
class CSearchEngine
{
public:
	CSearchEngine();
	virtual ~CSearchEngine();

	virtual void SearchAGoodMove(BYTE position[10][9]) = 0; //���Ľӿڣ�Ϊ��ǰ�����߳���һ��

	void SetSearchDepth(int nDepth) { m_nSearchDepth = nDepth; }; //�趨����������

	void SetSearchDepth1(int nDepth) { m_nSearchDepth1 = nDepth; }; //�趨����������

	void SetEveluator(CEveluation *pEval) { m_pEval = pEval; }; //�趨��ֵ����

	void SetMoveGenerator(CMoveGenerator *pMG) { m_pMG = pMG; }; //�趨���Ӳ�����
	
protected:
	virtual BYTE MakeMove(CHESSMOVE* move);  //����ĳһ�߷�����֮�����
	virtual void UnMakeMove(CHESSMOVE* move, BYTE type);  //�ָ�ǰһ�������
	virtual int IsGameOver(BYTE position[10][9], int nDepth);  //�жϵ�ǰ�����ʤ��
	BYTE CurPosition[10][9];  //���ڵ�ǰ���״̬������
	CHESSMOVE m_cmBestMove;  //��������¼����߷�
	CMoveGenerator *m_pMG;  //�߷�������ָ��
	CMoveGenerator *m_pMG1;  //�߷�������ָ��
	CEveluation *m_pEval;  //��ֵ����ָ��
	CEveluation *m_pEval1;  //��ֵ����ָ��
	int m_nSearchDepth;  //����������
	int m_nSearchDepth1;  //����������
	
};

