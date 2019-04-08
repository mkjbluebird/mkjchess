#include "stdafx.h"
#include "mkjchess.h"
#include "SearchEngine.h"


CSearchEngine::CSearchEngine()
{
}

extern short Gameover;
CSearchEngine::~CSearchEngine()
{
	if (Gameover != 0) {
		if(m_pMG)
			delete m_pMG;  //ɾȥ�������������ϵ��߷�������
		if(m_pMG1)
			delete m_pMG1;  //ɾȥ�������������ϵ��߷�������
		if(m_pEval)
			delete m_pEval;  //ɾȥ�������������ϵĹ�ֵ����
		if(m_pEval1)
			delete m_pEval1;  //ɾȥ�������������ϵĹ�ֵ����
	}	
}

BYTE CSearchEngine::MakeMove(CHESSMOVE* move) //�����ݴ�����߷��ı���֣�move�ǽ��е��߷���
{
	BYTE nChessID;
	nChessID = CurPosition[move->To.y][move->To.x];  //ȡĿ��λ������
	CurPosition[move->To.y][move->To.x] = CurPosition[move->From.y][move->From.x];  //�������ƶ���Ŀ��λ��
	CurPosition[move->From.y][move->From.x] = NOCHESS;  //��ԭλ�����
	return nChessID;  //���سԵ�������
}

void CSearchEngine::UnMakeMove(CHESSMOVE* move, BYTE nChessID) //�����ݴ�����߷��ָ���֣�move�ǽ��е��߷���nChessID��ԭ�����moveĿ��λ�õ��������͡�
{
	CurPosition[move->From.y][move->From.x] = CurPosition[move->To.y][move->To.x]; //��Ŀ��λ�õ����ӿ���ԭλ
	CurPosition[move->To.y][move->To.x] = nChessID; //�ָ�Ŀ��λ�õ�����
}
extern int m_nMaxDepth;
int CSearchEngine::IsGameOver(BYTE position[10][9], int nDepth)  //���������Ƿ������δ����������0�����������ؼ���/��Сֵ��
{
	int i, j;
	BOOL RedLive = FALSE, BlackLive = FALSE;

	for (i = 7; i < 10; i++)  //���췽�Ľ�˧
		for (j = 3; j < 6; j++)
		{
			if (position[i][j] == B_KING)
				BlackLive = TRUE;
			if (position[i][j] == R_KING)
				RedLive = TRUE;
		}

	for (i = 0; i < 3; i++)  //���ڷ��Ľ�˧
		for (j = 3; j < 6; j++)
		{
			if (position[i][j] == B_KING)
				BlackLive = TRUE;
			if (position[i][j] == R_KING)
				RedLive = TRUE;
		}

	i = (m_nMaxDepth - nDepth + 1) % 2;//ȡ��ǰ����ż��־

	if (!RedLive) //�콫�Ƿ񱻳�
		if (i)
			return 19990 + nDepth;
		else
			return -19990 - nDepth;
	if (!BlackLive)//�ڽ��Ƿ񱻳�
		if (i)
			return -19990 - nDepth;
		else
			return 19990 + nDepth;
	return 0;  //˫����δ��������0
}
