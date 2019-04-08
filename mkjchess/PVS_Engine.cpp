#include "stdafx.h"
#include "mkjchess.h"
#include "PVS_Engine.h"


CPVS_Engine::CPVS_Engine()
{
}


CPVS_Engine::~CPVS_Engine()
{
}

extern BOOL bIsRed;
extern int m_nMaxDepth;
void CPVS_Engine::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);  //ȡ���
	if (bIsRed) {
		m_nMaxDepth = m_nSearchDepth;  //�趨����������
	}
	else {
		m_nMaxDepth = m_nSearchDepth1;
	}

	//	for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth; m_nMaxDepth++)
	{
		PrincipalVariation(m_nMaxDepth, -20000, 20000);  //����PVS�������棬�����������
	}

	MakeMove(&m_cmBestMove);  //Ӧ���������
	memcpy(position, CurPosition, 90);  //����仯������
}

extern short Gameover;
int CPVS_Engine::PrincipalVariation(int depth, int alpha, int beta)
{
	int current = -20000;  //��ǰ�����������޸�Ϊ���ֵ��
	int score;
	int Count, i;
	BYTE type;
	int best;

	Gameover = IsGameOver(CurPosition, depth);  //��鵱ǰ��Ϸ�Ƿ����
	if (Gameover != 0)
		return Gameover;

	if (depth <= 0)	//Ҷ�ӽڵ�ȡ��ֵ
		return m_pEval->Eveluate(CurPosition, ((m_nMaxDepth - depth) + bIsRed) % 2);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, ((m_nMaxDepth - depth) + bIsRed) % 2);  //�������п����߷�
	
	type = MakeMove(&m_pMG->m_MoveList[depth][0]);  //��ʼ��һ���ڵ�
	best = -PrincipalVariation(depth - 1, -beta, -alpha);  //ʹ��ȫ����������һ���ڵ�
	UnMakeMove(&m_pMG->m_MoveList[depth][0], type);  //�ָ�ǰһ����
	if (depth == m_nMaxDepth)  //����Ǹ��ڵ㣬�򱣴��������
		m_cmBestMove = m_pMG->m_MoveList[depth][0];

	for (i = 1; i<Count; i++)  //�ӵڶ����ڵ㿪ʼ����ÿһ�ڵ�
	{

		if (best < beta)  //����Ƿ�Beta��֦
		{
			if (best > alpha)
				alpha = best;  //�޸�alpha����
			type = MakeMove(&m_pMG->m_MoveList[depth][i]);  //�����ӽڵ�
			score = -PrincipalVariation(depth - 1, -alpha - 1, -alpha);  //ʹ�ü�խ������  
			if (score > alpha && score < beta)
			{   //fail high,��������
				best = -PrincipalVariation(depth - 1, -beta, -score);
				if (depth == m_nMaxDepth)  //Ϊ���ڵ㣬��������߷�
					m_cmBestMove = m_pMG->m_MoveList[depth][i];
			}
			else if (score > best)
			{  //խ�������ɹ�
				best = score;
				if (depth == m_nMaxDepth)
					m_cmBestMove = m_pMG->m_MoveList[depth][i];  //Ϊ���ڵ㣬��������߷�
			}
			UnMakeMove(&m_pMG->m_MoveList[depth][i], type);  //�ָ�ǰһ����
		}
	}

	return best;  //�������ֵ
}