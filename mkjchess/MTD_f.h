#pragma once
#include "SearchEngine.h"
#include "TranspositionTable.h"
#include "HistoryHeuristic.h"
class CMTD_f :
	public CSearchEngine, public CTranspositionTable, public CHistoryHeuristic
{
public:
	CMTD_f();
	virtual ~CMTD_f();
	virtual void SearchAGoodMove(BYTE position[10][9]);  //����һ������

protected:
	int mtdf(int firstguess, int depth);  //MTD(f)��������
	int FAlphaBeta(int depth, int alpha, int beta);  //�����û����Fail-soft alpha-beta��������
	CHESSMOVE m_cmBackupBM;  //����������ӱ���
};

