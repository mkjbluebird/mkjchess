#pragma once
class CMoveGenerator
{
public:
	CMoveGenerator();
	virtual ~CMoveGenerator();
	static BOOL IsValidMove(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY);  //��������Ƿ�Ϸ�
	int CreatePossibleMove(BYTE position[10][9], int nPly, int nSide);  //�������кϷ��߷�

	CHESSMOVE m_MoveList[70][80];  //��źϷ��߷�����

protected:
	int AddMove(int nFromX, int nToX, int nFromY, int nToY, int nPly); //��m_MoveList�����в���һ���߷�
	void Gen_KingMove(BYTE position[10][9], int i, int j, int nPly);  //��˧���߷�
	void Gen_RBishopMove(BYTE position[10][9], int i, int j, int nPly);  //��ʿ���߷�
	void Gen_BBishopMove(BYTE position[10][9], int i, int j, int nPly);  //��ʿ���߷�
	void Gen_ElephantMove(BYTE position[10][9], int i, int j, int nPly); //����߷�
	void Gen_HorseMove(BYTE position[10][9], int i, int j, int nPly);  //����߷�
	void Gen_CarMove(BYTE position[10][9], int i, int j, int nPly);  //�����߷�
	void Gen_RPawnMove(BYTE position[10][9], int i, int j, int nPly); //������߷�
	void Gen_BPawnMove(BYTE position[10][9], int i, int j, int nPly); //�ڱ����߷�
	void Gen_CanonMove(BYTE position[10][9], int i, int j, int nPly); //�ڵ��߷�


	int m_nMoveCount;  //��¼m_MoveList�������߷�������
	BOOL m_bIsRed;
};

