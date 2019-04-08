#include "stdafx.h"
#include "mkjchess.h"
#include "NegaScout_TT_HH.h"


CNegaScout_TT_HH::CNegaScout_TT_HH()
{
}


CNegaScout_TT_HH::~CNegaScout_TT_HH()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CNegaScout_TT_HH::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);  //ȡ���
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //�趨����������
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}
	CalculateInitHashKey(CurPosition);  //��ʼ����ʼ���̵Ĺ�ϣֵ
	ResetHistoryTable();  //��ʼ����ʷ��¼��
	//	m_nMaxDepth = 1;
	//	NegaScout(m_nMaxDepth, -20000, 20000);
	//	m_nMaxDepth = m_nSearchDepth;
	//	for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth; m_nMaxDepth++)
	NegaScout(m_nMaxDepth, -20000, 20000);  //ʹ��NegaSout����
	MakeMove(&m_cmBestMove);  //Ӧ���������
	memcpy(position, CurPosition, 90);  //����ı�����
}

//NegaScout��������
extern short Gameover;
int CNegaScout_TT_HH::NegaScout(int depth, int alpha, int beta)
{
	int current = -20000;  //��ǰ�����������޸�Ϊ���ֵ��
	int Count, i;
	BYTE type;
	int a, b, t;
	int side;
	int score;

	Gameover = IsGameOver(CurPosition, depth);  //�鿴��Ϸ�Ƿ����
	if (Gameover != 0)
		return Gameover;  //��Ϸ���������ؼ�ֵ
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	side = ((m_nMaxDepth - depth) + bIsRed) % 2;  //��ǰ������һ��

	score = LookUpHashTable(alpha, beta, depth, side);  //���û����в鿴��ǰ�����Ƿ���Ч
	if (score != 66666)
		return score;  //��Ч�����ص�ǰ��Чֵ

	if (depth <= 0)	//Ҷ�ӽڵ�ȡ��ֵ
	{
		score = m_pEval->Eveluate(CurPosition, side);  //���㵱ǰ��ֵ
		EnterHashTable(exact, score, depth, side);  //����ֵ�����û���
		return score;  //���ع�ֵ
	}
	//������һ�����еĿ�������
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, side);

	for (i = 0; i<Count; i++)
	{
		m_pMG->m_MoveList[depth][i].Score =
			GetHistoryScore(&m_pMG->m_MoveList[depth][i]);  //ȡ��ÿһ���������ӵ���ʷ�÷�
	}
	MergeSort(m_pMG->m_MoveList[depth], Count, 0);  //����Ч�߷�����ʷ�÷�����

	int bestmove = -1;

	a = alpha;
	b = beta;
	int eval_is_exact = 0;
	for (i = 0; i < Count; i++)
	{
		Hash_MakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);  //�����ӽڵ��ϣֵ
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //�����ӽڵ�
		//�ݹ������ӽڵ㣬��һ���ڵ���ȫ���ڣ�����ǿմ���̽��
		t = -NegaScout(depth - 1, -b, -a);

		if (t > a && t < beta && i > 0)
		{  //��һ����Ľڵ㣬����������������ϱ߽�
			a = -NegaScout(depth - 1, -beta, -t);     /* re-search ��������*/
			eval_is_exact = 1;  //�趨��������Ϊ��ȷֵ
			if (depth == m_nMaxDepth)  //���Ϊ���ڵ�
				m_cmBestMove = m_pMG->m_MoveList[depth][i];  //�������ֵ
			bestmove = i;  //��¼���ֵ�α�
		}
		Hash_UnMakeMove(&m_pMG->m_MoveList[depth][i], type, CurPosition);  //�ָ���ǰ�ڵ��ϣֵ
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //�����ӽڵ�
		if (a < t)
		{  //��һ����������
			eval_is_exact = 1;  //��ȷֵ
			a = t;  //�����߽�
			if (depth == m_nMaxDepth)  //���Ϊ���ڵ�
				m_cmBestMove = m_pMG->m_MoveList[depth][i];  //�����������
		}
		if (a >= beta)
		{
			EnterHashTable(lower_bound, a, depth, side);  //���±߽�����û���
			EnterHistoryScore(&m_pMG->m_MoveList[depth][i], depth);  //����ǰ���ӻ�����ʷ��¼
			return a;  //beta��֦
		}
		b = a + 1;                      /* set new null window�趨�µĿմ� */
	}
	if (bestmove != -1)  //���Ϊ��Чֵ
		EnterHistoryScore(&m_pMG->m_MoveList[depth][bestmove], depth);  //����ǰ���ӻ�����ʷ��¼
	if (eval_is_exact)  //���Ϊ��ȷֵ
		EnterHashTable(exact, a, depth, side);  //����ȷֵ������û���
	else
		EnterHashTable(upper_bound, a, depth, side);  //�����ϱ߽�
	return a;  //�������ֵ
}