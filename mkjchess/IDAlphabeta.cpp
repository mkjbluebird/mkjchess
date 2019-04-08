#include "stdafx.h"
#include "mkjchess.h"
#include "IDAlphabeta.h"


CIDAlphabeta::CIDAlphabeta()
{
}


CIDAlphabeta::~CIDAlphabeta()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CIDAlphabeta::SearchAGoodMove(BYTE position[10][9])
{
	CHESSMOVE backupmove;  //������ӱ���
	memcpy(CurPosition, position, 90);  //ȡ��ǰ����
	m_nTimeCount = GetTickCount();  //��ǰʱ��
	if (bIsRed) {
		for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth; m_nMaxDepth++)  //���������
		{
			if (alphabeta(m_nMaxDepth, -20000, 20000) != 66666)  //�����������û����ֹ
				backupmove = m_cmBestMove;  //�����������
		}
	}
	else {
		for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth1; m_nMaxDepth++)  //���������
		{
			if (alphabeta(m_nMaxDepth, -20000, 20000) != 66666)  //�����������û����ֹ
				backupmove = m_cmBestMove;  //�����������
		}
	}	
	MakeMove(&backupmove);  //����������Ӹı����
	memcpy(position, CurPosition, 90);  //�����������
}

extern short Gameover;
int CIDAlphabeta::alphabeta(int depth, int alpha, int beta)
{
	int current = -20000;  //��ǰ�����������޸�Ϊ���ֵ��
	int score;
	int Count, i;
	BYTE type;

	Gameover = IsGameOver(CurPosition, depth);  //��Ϸ�Ƿ����
	if (Gameover != 0)
		return Gameover;  //���������ؼ�ֵ

	if (depth <= 0)	//Ҷ�ӽڵ�ȡ��ֵ
		return m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //ȡ���п����߷�
	//����if���֮�ڴ��룬���ڵ����ײ�ڵ�����˳��
	//���ĳ�߷����ϴε�������Best Move,�������ڵ�һλ
	if (depth == m_nMaxDepth && m_nMaxDepth > 1)
	{
		for (i = 1; i < Count; i++)
		{
			if (m_pMG->m_MoveList[depth][i].From.x == m_cmBestMove.From.x
				&& m_pMG->m_MoveList[depth][i].From.y == m_cmBestMove.From.y
				&& m_pMG->m_MoveList[depth][i].To.x == m_cmBestMove.To.x
				&& m_pMG->m_MoveList[depth][i].To.y == m_cmBestMove.To.y)  //���ĳ�ڵ�Ϊ�������
			{
				m_pMG->m_MoveList[depth][i] = m_pMG->m_MoveList[depth][0];  //���������߷���λ��
				m_pMG->m_MoveList[depth][0] = m_cmBestMove;
			}
		}
	}

	for (i = 0; i<Count; i++)
	{
		if (depth == m_nMaxDepth)
		{
			if (GetTickCount() - m_nTimeCount >= 1000000)  //�鿴�Ƿ��ѵ��޶�ʱ�䣬10��
				return 66666;  //������ֹ���
		}
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //�����ӽڵ�
		score = -alphabeta(depth - 1, -beta, -alpha);  //�ݹ�����
		UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //�����ӽڵ�

		if (score > alpha)
		{
			alpha = score;  //����alphaֵ
			if (depth == m_nMaxDepth)  //���Ϊ���ڵ㣬��������߷�
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
		}
		if (alpha >= beta)
			break;  //beta��֦


	}
	return alpha;  //�������ֵ
}
