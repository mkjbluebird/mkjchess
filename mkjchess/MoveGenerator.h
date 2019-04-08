#pragma once
class CMoveGenerator
{
public:
	CMoveGenerator();
	virtual ~CMoveGenerator();
	static BOOL IsValidMove(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY);  //检查落子是否合法
	int CreatePossibleMove(BYTE position[10][9], int nPly, int nSide);  //产生所有合法走法

	CHESSMOVE m_MoveList[70][80];  //存放合法走法队列

protected:
	int AddMove(int nFromX, int nToX, int nFromY, int nToY, int nPly); //在m_MoveList队列中插入一个走法
	void Gen_KingMove(BYTE position[10][9], int i, int j, int nPly);  //将帅的走法
	void Gen_RBishopMove(BYTE position[10][9], int i, int j, int nPly);  //红士的走法
	void Gen_BBishopMove(BYTE position[10][9], int i, int j, int nPly);  //黑士的走法
	void Gen_ElephantMove(BYTE position[10][9], int i, int j, int nPly); //象的走法
	void Gen_HorseMove(BYTE position[10][9], int i, int j, int nPly);  //马的走法
	void Gen_CarMove(BYTE position[10][9], int i, int j, int nPly);  //车的走法
	void Gen_RPawnMove(BYTE position[10][9], int i, int j, int nPly); //红兵的走法
	void Gen_BPawnMove(BYTE position[10][9], int i, int j, int nPly); //黑兵的走法
	void Gen_CanonMove(BYTE position[10][9], int i, int j, int nPly); //炮的走法


	int m_nMoveCount;  //记录m_MoveList队列中走法的数量
	BOOL m_bIsRed;
};

