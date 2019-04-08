#include "stdafx.h"
#include "mkjchess.h"
#include "MTD_f.h"


CMTD_f::CMTD_f()
{
}


CMTD_f::~CMTD_f()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CMTD_f::SearchAGoodMove(BYTE position[10][9])
{
	int firstguess = 0;
	memcpy(CurPosition, position, 90);  //ȡ���
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //�趨����������
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}
	CalculateInitHashKey(CurPosition);  //�����ʼ���̵Ĺ�ϣֵ
	ResetHistoryTable();  //��ʼ����ʷ��¼��
	if (bIsRed) {
		for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth; m_nMaxDepth++)  //���������MTD(f)����
		{
			firstguess = mtdf(firstguess, m_nMaxDepth);
		}
	}
	else {
		for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth1; m_nMaxDepth++)  //���������MTD(f)����
		{
			firstguess = mtdf(firstguess, m_nMaxDepth);
		}
	}
	MakeMove(&m_cmBackupBM);  //Ӧ���������
	memcpy(position, CurPosition, 90);  //�����߹������
	//return 0;
}


int CMTD_f::mtdf(int firstguess, int depth)
{
	int g, lowerbound, upperbound, beta;

	g = firstguess;  //��ʼ�²�ֵ
	//��ʼ������Χ-20000~20000
	upperbound = 20000;
	lowerbound = -20000;
	while (lowerbound < upperbound)  //�ں����ֵ��Χ
	{
		m_cmBackupBM = m_cmBestMove;
		//����Ŀ����������
		if (g == lowerbound)
			beta = g + 1;
		else
			beta = g;
		//�մ�̽��
		g = FAlphaBeta(depth, beta - 1, beta);
		if (g<beta)
			upperbound = g;  //�����ϱ߽�
		else
			lowerbound = g;  //�����±߽�
	}
	return g;
}

extern short Gameover;
int CMTD_f::FAlphaBeta(int depth, int alpha, int beta)
{
	int current = -19999;
	int score;
	int Count, i;
	BYTE type;

	Gameover = IsGameOver(CurPosition, depth);  //�鿴��Ϸ�Ƿ����
	if (Gameover != 0)
		return Gameover;  //ʤ���ѷ֣����ؼ�ֵ

	//�鿴��ǰ�ڵ��Ƿ���ڹ�ϣ����
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	score = LookUpHashTable(alpha, beta, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);
	if (score != 66666)
		return score;  //���ڣ����ع�ϣ���е�ֵ

	if (depth <= 0)	//Ҷ�ӽڵ�ȡ��ֵ
	{
		current = m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		EnterHashTable(exact, current, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //�������û���
		return current;
	}

	//�������п��ܵ���һ��
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);

	for (i = 0; i<Count; i++)
	{
		m_pMG->m_MoveList[depth][i].Score =
			GetHistoryScore(&m_pMG->m_MoveList[depth][i]);  //ȡ��ÿһ���������ӵ���ʷ�÷�
	}
	MergeSort(m_pMG->m_MoveList[depth], Count, 0);  //����Ч�߷�����ʷ�÷�����

	int bestmove = -1;

	int eval_is_exact = 0;  //�������ͱ�־

	for (i = 0; i<Count; i++)
	{
		//�����ӽڵ�Ĺ�ϣֵ
		Hash_MakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //�����ӽڵ�
		//�ݹ������ӽڵ�
		score = -FAlphaBeta(depth - 1, -beta, -alpha);
		//�ָ���ǰ�ڵ�Ĺ�ϣֵ
		Hash_UnMakeMove(&m_pMG->m_MoveList[depth][i], type, CurPosition);
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //�����ӽڵ�


		if (score > current)
		{
			current = score;
			
			if (depth == m_nMaxDepth)  //���Ϊ���ڵ�
				m_cmBestMove = m_pMG->m_MoveList[depth][i];  //�������ֵ
			bestmove = i;  //��¼���ֵ�α�
			if (score >= beta) //beta��֦
			{
				//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				EnterHashTable(lower_bound, score, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //���±߽籣����û���
				EnterHistoryScore(&m_pMG->m_MoveList[depth][i], depth);  //����ǰ���ӻ�����ʷ��¼
				return current;
			}
			if (score > alpha)
			{
				eval_is_exact = 1;  //�趨ȷ��ֵ��־
				alpha = score;  //�������ֵ
			}
		}

	}
	if (bestmove != -1)  //���Ϊ��Чֵ
		EnterHistoryScore(&m_pMG->m_MoveList[depth][bestmove], depth);  //����ǰ���ӻ�����ʷ��¼
	if (eval_is_exact)  //���Ϊ��Чֵ
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		EnterHashTable(exact, alpha, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //��������������û���
	else
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		EnterHashTable(upper_bound, current, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //�����ϱ߽�����û���
	return current;  //�������ֵ
}
