#include "stdafx.h"
#include "mkjchess.h"
#include "FAlphaBetaEngine.h"


CFAlphaBetaEngine::CFAlphaBetaEngine()
{
}


CFAlphaBetaEngine::~CFAlphaBetaEngine()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CFAlphaBetaEngine::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);  //ȡ��ǰ����
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //�趨����������
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}
	FAlphaBeta(m_nMaxDepth, -20000, 20000);  //��ʼ����
	MakeMove(&m_cmBestMove);  //�����������
	memcpy(position, CurPosition, 90);  //����������Ӿ���
	//return 0;
}

extern short Gameover;
int CFAlphaBetaEngine::FAlphaBeta(int depth, int alpha, int beta)
{
	int current = -20000;  //��ʼ��Сֵ
	int score;
	int Count, i;
	BYTE type;

	Gameover = IsGameOver(CurPosition, depth);  //�������ڵ���Ϸ�Ƿ����
	if (Gameover != 0)
		return Gameover;   //��Ϸ���������ؼ���/��Сֵ

	if (depth <= 0)	//Ҷ�ӽڵ�ȡ��ֵ
		return m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //�����ӽڵ�

	for (i = 0; i<Count; i++)
	{  //����ÿ���ڵ�ķ���
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //�����ӽڵ�
		score = -FAlphaBeta(depth - 1, -beta, -alpha);  //ȡ�ӽڵ�ķ���
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //�ָ������ڵ�

		if (score > current)
		{  //����������ڼ�Сֵ
			current = score;  //�޸ķ�ֵ����
			if (depth == m_nMaxDepth)  //Ϊ���ڵ㣬�����������
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
			if (score > alpha)
				alpha = score;  //�޸�alpha�߽�
			if (score >= beta) //beta��֦
				break;
		}

	}

	return current;  //���ؼ���ֵ��߽�ֵ
}

