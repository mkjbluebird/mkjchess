#include "stdafx.h"
#include "mkjchess.h"
#include "NegamaxEngine.h"


CNegamaxEngine::CNegamaxEngine()
{
}


CNegamaxEngine::~CNegamaxEngine()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CNegamaxEngine::SearchAGoodMove(BYTE position[10][9]) //���˺�����Դ����position�ҳ�����߷�������һ����
{
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //�趨����������
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}

	memcpy(CurPosition, position, 90);  //���������ָ��Ƶ������Ա������

	NegaMax(m_nMaxDepth);  //���ø�����ֵ���������ҳ�����߷�

	MakeMove(&m_cmBestMove); //������޸�Ϊ��һ��
	memcpy(position, CurPosition, 90);  //���޸Ĺ������̸��Ƶ���������У�����
	
}

extern short Gameover;
int CNegamaxEngine::NegaMax(int depth)  //��������ֵ����������depth��ʾ�ڵ���Ҷ�ӽڵ�Ĳ�����
{
	int current = -20000;  //��ǰ�����������޸�Ϊ���ֵ��
	int score;  //ÿһ�ڵ����
	int Count, i;  //Count:���п����߷�����i:�Ƿ������־
	BYTE type;  //

	Gameover = IsGameOver(CurPosition, depth); //�������Ƿ����
	if (Gameover != 0)
		return Gameover;  //��ֽ��������ؼ���/��Сֵ

	if (depth <= 0)	//Ҷ�ӽڵ�ȡ��ֵ
		return m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);  //���ع�ֵ
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //�г���ǰ������һ�������п����߷�

	for (i = 0;i<Count;i++)
	{

		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //�������Ӳ����¾���
		score = -NegaMax(depth - 1); //�ݹ���ø�����ֵ������һ��ڵ�
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type); //�ָ���ǰ����

		if (score > current)  //���score������
		{
			current = score;  //�޸����ֵ
			if (depth == m_nMaxDepth)
			{
				m_cmBestMove = m_pMG->m_MoveList[depth][i];  //�ڸ�����������߷�
			}
		}

	}

	return current;  //���ؼ���ֵ(����)
}