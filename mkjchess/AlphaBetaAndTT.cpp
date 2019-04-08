#include "stdafx.h"
#include "mkjchess.h"
#include "AlphaBetaAndTT.h"

extern BOOL bIsRed;
CAlphaBetaAndTT::CAlphaBetaAndTT()
{
}


CAlphaBetaAndTT::~CAlphaBetaAndTT()
{
}

extern int m_nMaxDepth;
void CAlphaBetaAndTT::SearchAGoodMove(BYTE position[10][9])  //������һ����
{
	memcpy(CurPosition, position, 90);  //ȡ���
	CalculateInitHashKey(CurPosition);  //�����ʼ��ֹ�ϣֵ
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //�趨����������
	}		
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}		
	alphabeta(m_nMaxDepth, -20000, 20000);  //����alpha-beta����
	MakeMove(&m_cmBestMove);  //�߳��������
	memcpy(position, CurPosition, 90);  //�������
}

extern short Gameover;
int CAlphaBetaAndTT::alphabeta(int depth, int alpha, int beta)  //alpha-beta��������
{
	int current = -20000;  //��ǰ�����������޸�Ϊ���ֵ��
	int score;
	int Count, i;
	BYTE type;
	int side;

	Gameover = IsGameOver(CurPosition, depth);  //�����Ϸ�Ƿ����
	if (Gameover != 0)
		return Gameover;  //��Ϸ���������ؼ�ֵ
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	side = ((m_nMaxDepth - depth) + bIsRed) % 2;

	score = LookUpHashTable(alpha, beta, depth, side);  //�鿴�û������Ƿ��е�ǰ�ڵ�
	if (score != 66666)
		return score;  //�е�ǰ�ڵ㣬���ر��е�ֵ

	if (depth <= 0)	//Ҷ�ӽڵ�ȡ��ֵ
	{
		score = m_pEval->Eveluate(CurPosition, side);
		EnterHashTable(exact, score, depth, side);  //����ǰ�ڵ��ֵ������û���
		return score;
	}


	//���㵱ǰ�����һ�������п�������
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, side);

	int eval_is_exact = 0;  //�������ͱ�־

	for (i = 0; i<Count; i++)  //��ÿһ����һ�����п�������
	{
		Hash_MakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);  //��������������Ӧ�ӽڵ�Ĺ�ϣֵ
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //�����ӽڵ�

		score = -alphabeta(depth - 1, -beta, -alpha);  //�ݹ������ӽڵ�

		Hash_UnMakeMove(&m_pMG->m_MoveList[depth][i], type, CurPosition);  //�ָ��ýڵ�Ĺ�ϣֵ
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //�����ӽڵ�
		if (score >= beta)
		{  //beta��֦
			EnterHashTable(lower_bound, score, depth, side);  //���ڵ��±߽�����û���
			return score;  //�����±߽�
		}

		if (score > alpha)
		{
			alpha = score;  //ȡ���ֵ
			eval_is_exact = 1;  //�趨ȷ��ֵ��־
			if (depth == m_nMaxDepth)  //���ڵ㣬��������߷�
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
		}

	}
	//���������������û���
	if (eval_is_exact)  //�����ȷ��ֵ
		EnterHashTable(exact, alpha, depth, side);
	else
		EnterHashTable(upper_bound, alpha, depth, side);  //�����ϱ߽�
	return alpha;  //�������ֵ/�Ͻ�
}


