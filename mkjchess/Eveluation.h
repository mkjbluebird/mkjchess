#pragma once
//【定义棋子价值】
#define BASEVALUE_PAWN 100
#define BASEVALUE_BISHOP 250
#define BASEVALUE_ELEPHANT 250
#define BASEVALUE_CAR 500
#define BASEVALUE_HORSE 350
#define BASEVALUE_CANON 350
#define BASEVALUE_KING 10000
//【定义棋子灵活性价值】
#define FLEXIBILITY_PAWN 15
#define FLEXIBILITY_BISHOP 1
#define FLEXIBILITY_ELEPHANT 1
#define FLEXIBILITY_CAR 6
#define FLEXIBILITY_HORSE 12
#define FLEXIBILITY_CANON 6
#define FLEXIBILITY_KING 0

class CEveluation
{
public:
	CEveluation();
	virtual ~CEveluation();

	virtual int Eveluate(BYTE position[10][9], BOOL bIsRedTurn);  //【估值函数，对棋局打分，bIsRedTurn标明轮到谁走】
protected:
	int GetRelatePiece(BYTE position[10][9], int j, int i);  //列举指定位置棋子的相关棋子
	BOOL CanTouch(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY);  //判断From位置的棋子能否走到位置To
	void AddPoint(int x, int y);  //将一个位置加到相关位置的队列
								  //int GetBingValue(int x, int y, BYTE CurSituation[][9]);  //
	int m_BaseValue[15];  //存放棋子基本价值的数组
	int m_FlexValue[15];  //存放棋子灵活性分数的数组
	short m_AttackPos[10][9];  //存放每一位置被威胁的信息
	BYTE m_GuardPos[10][9];  //存放每一位置被保护的信息
	BYTE m_FlexibilityPos[10][9];  //存放每一位置上的棋子的灵活性分数
	int m_chessValue[10][9];  //存放每一位置上的棋子的总价值
	int nPosCount;  //记录一棋子的相关位置个数
	POINT RelatePos[20];  //记录一个棋子相关位置的数组
};

