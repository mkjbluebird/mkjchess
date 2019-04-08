#include "stdafx.h"
#include "mkjchess.h"
#include "AlphaBetaEngine.h"
//#include "mkjchessDlg.h"
//#include "NewGame.h"
//#include "MoveGenerator.h"

CAlphaBetaEngine::CAlphaBetaEngine()
{
}


CAlphaBetaEngine::~CAlphaBetaEngine()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CAlphaBetaEngine::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);  //����ǰ����ȡ����CurPosition
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //�趨����������
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}
	alphabeta(m_nMaxDepth, -20000, 20000);  //����alphabeta����
	MakeMove(&m_cmBestMove);  //�߳���Ѳ�
	memcpy(position, CurPosition, 90);  //�������
}

extern short Gameover;
int CAlphaBetaEngine::alphabeta(int depth, int alpha, int beta)
{
	int current = -20000;  //��ǰ�����������޸�Ϊ���ֵ��
	int score;
	int Count, i;
	BYTE type;

	Gameover = IsGameOver(CurPosition, depth);  //��Ϸ�Ƿ����
	if (Gameover != 0)
		return Gameover;  //���������ع�ֵ

	if (depth <= 0)	//Ҷ�ӽڵ�ȡ��ֵ
		return m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //�ҳ���ǰ�������п����߷�������m_pMG->m_MoveList����

	for (i = 0; i<Count; i++)
	{
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //Ӧ�õ�ǰ���ӣ�����ӽڵ����
		score = -alphabeta(depth - 1, -beta, -alpha);  //�ݹ������ӽڵ�
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //���ӽڵ�ָ���ֵ���ǰ�ڵ�

		if (score > alpha)
		{
			alpha = score;  //��������ֵ
			if (depth == m_nMaxDepth)  //���Ϊ���ڵ�
				m_cmBestMove = m_pMG->m_MoveList[depth][i];  //��������߷�
		}
		if (alpha >= beta)
			break;  //��֦


	}
	return alpha;  //���ؼ���ֵ
}
