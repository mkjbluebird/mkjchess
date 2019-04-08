#include "stdafx.h"
#include "mkjchess.h"
#include "Alphabeta_HH.h"


CAlphabeta_HH::CAlphabeta_HH()
{
}


CAlphabeta_HH::~CAlphabeta_HH()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CAlphabeta_HH::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);  //ȡ�����
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //�趨����������
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}
	ResetHistoryTable();  //��ʼ����ʷ��¼��
	alphabeta(m_nMaxDepth, -20000, 20000);  //����alpha-beta����
	MakeMove(&m_cmBestMove);  //�߳��������
	memcpy(position, CurPosition, 90);  //�������
}

extern short Gameover;
int CAlphabeta_HH::alphabeta(int depth, int alpha, int beta)  //alpha-beta����
{
	int current = -20000;  //��ǰ�����������޸�Ϊ���ֵ��
	int score;
	int Count, i;
	BYTE type;

	Gameover = IsGameOver(CurPosition, depth);  //�鿴��Ϸ�Ƿ����
	if (Gameover != 0)
		return Gameover;  //��Ϸ���������ؼ�ֵ

	if (depth <= 0)	//Ҷ�ӽڵ�ȡ��ֵ
		return m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //������һ�����п�������

	for (i = 0; i<Count; i++)
	{
		m_pMG->m_MoveList[depth][i].Score =
			GetHistoryScore(&m_pMG->m_MoveList[depth][i]);  //ȡ�����߷�����ʷ�÷�
	}
	MergeSort(m_pMG->m_MoveList[depth], Count, 0);  //����Count���߷���ʷ�÷�
	int bestmove = -1;  //��¼������Ӿ���ı���
	for (i = 0; i<Count; i++)
	{

		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //�����ӽڵ�
		score = -alphabeta(depth - 1, -beta, -alpha);  //�ݹ������ӽڵ�
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //�����ӽڵ�

		if (score > alpha)
		{
			alpha = score;  //�������ֵ�������߽�
			if (depth == m_nMaxDepth)  //���Ϊ���ڵ�
				m_cmBestMove = m_pMG->m_MoveList[depth][i];  //��������߷�
			bestmove = i;  //��¼����߷�λ��
		}
		if (alpha >= beta)
		{
			bestmove = i;  //��¼����߷�λ��
			break;  //beta��֦
		}

	}
	if (bestmove != -1)
		EnterHistoryScore(&m_pMG->m_MoveList[depth][bestmove], depth);  //������߷�������ʷ��¼��
	return alpha;
}

