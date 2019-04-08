#include "stdafx.h"
#include "mkjchess.h"
#include "Eveluation.h"
//红卒的附加值矩阵
const int BA0[10][9] =
{
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 90,90,110,120,120,120,110,90,90 },
	{ 90,90,110,120,120,120,110,90,90 },
	{ 70,90,110,110,110,110,110,90,70 },
	{ 70,70,70, 70, 70,  70, 70,70,70 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
};
//黑兵的附加值矩阵
const int BA1[10][9] =
{
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 70,70,70, 70, 70,70, 70,70, 70 },
	{ 70,90,110,110,110,110,110,90,70 },
	{ 90,90,110,120,120,120,110,90,90 },
	{ 90,90,110,120,120,120,110,90,90 },
	{ 0,  0, 0,  0,  0,  0,  0,  0,  0 },
};

int GetBingValue(int x, int y, BYTE CurSituation[][9])  //【为每一个兵返回附加值，x,y：坐标，CurSituation:棋盘，不是兵返回0】
{
	if (CurSituation[y][x] == R_PAWN)  //红卒的附加值
		return BA0[y][x];

	if (CurSituation[y][x] == B_PAWN)  //黑兵的附加值
		return BA1[y][x];

	return 0;  //不是兵返回0
}

CEveluation::CEveluation()
{
	m_BaseValue[B_KING] = BASEVALUE_KING;
	m_BaseValue[B_CAR] = BASEVALUE_CAR;
	m_BaseValue[B_HORSE] = BASEVALUE_HORSE;
	m_BaseValue[B_BISHOP] = BASEVALUE_BISHOP;
	m_BaseValue[B_ELEPHANT] = BASEVALUE_ELEPHANT;
	m_BaseValue[B_CANON] = BASEVALUE_CANON;
	m_BaseValue[B_PAWN] = BASEVALUE_PAWN;

	m_BaseValue[R_KING] = BASEVALUE_KING;
	m_BaseValue[R_CAR] = BASEVALUE_CAR;
	m_BaseValue[R_HORSE] = BASEVALUE_HORSE;
	m_BaseValue[R_BISHOP] = BASEVALUE_BISHOP;
	m_BaseValue[R_ELEPHANT] = BASEVALUE_ELEPHANT;
	m_BaseValue[R_CANON] = BASEVALUE_CANON;
	m_BaseValue[R_PAWN] = BASEVALUE_PAWN;

	m_FlexValue[B_KING] = FLEXIBILITY_KING;
	m_FlexValue[B_CAR] = FLEXIBILITY_CAR;
	m_FlexValue[B_HORSE] = FLEXIBILITY_HORSE;
	m_FlexValue[B_BISHOP] = FLEXIBILITY_BISHOP;
	m_FlexValue[B_ELEPHANT] = FLEXIBILITY_ELEPHANT;
	m_FlexValue[B_CANON] = FLEXIBILITY_CANON;
	m_FlexValue[B_PAWN] = FLEXIBILITY_PAWN;

	m_FlexValue[R_KING] = FLEXIBILITY_KING;
	m_FlexValue[R_CAR] = FLEXIBILITY_CAR;
	m_FlexValue[R_HORSE] = FLEXIBILITY_HORSE;
	m_FlexValue[R_BISHOP] = FLEXIBILITY_BISHOP;
	m_FlexValue[R_ELEPHANT] = FLEXIBILITY_ELEPHANT;
	m_FlexValue[R_CANON] = FLEXIBILITY_CANON;
	m_FlexValue[R_PAWN] = FLEXIBILITY_PAWN;

}


CEveluation::~CEveluation()
{
}

int count = 0;  //调用了估值函数的叶子节点数
int CEveluation::Eveluate(BYTE position[10][9], BOOL bIsRedTurn)  //【估值函数，position：棋盘，bIsRedTurn:True红方，False黑方】
{
	int i, j, k;
	int nChessType, nTargetType;
	count++;  //被估值过的叶子节点数
			  //初始化临时变量
	memset(m_chessValue, 0, 360);
	memset(m_AttackPos, 0, 180);
	memset(m_GuardPos, 0, 90);
	memset(m_FlexibilityPos, 0, 90);
	//扫描棋盘，找出每一个棋子，及其威胁/保护的棋子，还有其灵活性
	for (i = 0; i < 10; i++)
		for (j = 0; j < 9; j++)
		{
			if (position[i][j] != NOCHESS)  //有子
			{
				nChessType = position[i][j];  //取子
				GetRelatePiece(position, j, i);  //找出该棋子所有相关位置
				for (k = 0; k < nPosCount; k++)  //对每一目标位置
				{
					nTargetType = position[RelatePos[k].y][RelatePos[k].x];  //取目标位置棋子类型
					if (nTargetType == NOCHESS)  //无子
					{
						m_FlexibilityPos[i][j]++;  //灵活性增加
					}
					else
					{   //有子
						if (IsSameSide(nChessType, nTargetType))
						{
							m_GuardPos[RelatePos[k].y][RelatePos[k].x]++;  //己方棋子，目标受保护
						}
						else
						{
							m_AttackPos[RelatePos[k].y][RelatePos[k].x]++;  //敌方棋子，目标受威胁
							m_FlexibilityPos[i][j]++;  //灵活性增加
							switch (nTargetType)
							{
							case R_KING:  //是红将
								if (!bIsRedTurn)  //黑棋走
									return 18888;  //返回失败极值
								break;
							case B_KING:  //是黑将
								if (bIsRedTurn)  //红棋走
									return 18888;  //返回失败极值
								break;
							default:  //非将其他子
								m_AttackPos[RelatePos[k].y][RelatePos[k].x] += (30 + (m_BaseValue[nTargetType] - m_BaseValue[nChessType]) / 10) / 10;  //根据威胁棋子加上威胁分值
								break;
							}
						}
					}
				}
			}
		}
	//循环统计扫描数据
	for (i = 0; i < 10; i++)
		for (j = 0; j < 9; j++)
		{
			if (position[i][j] != NOCHESS)
			{
				nChessType = position[i][j];  //取子
				m_chessValue[i][j]++;  //如果棋子存在价值不为0
				m_chessValue[i][j] += m_FlexValue[nChessType] * m_FlexibilityPos[i][j];  //附加棋子灵活性价值
				m_chessValue[i][j] += GetBingValue(j, i, position);  //附加上兵的位置价值
			}
		}
	//循环统计扫描数据
	int nHalfvalue;
	for (i = 0; i < 10; i++)
		for (j = 0; j < 9; j++)
		{
			if (position[i][j] != NOCHESS)  //有子
			{
				nChessType = position[i][j];  //取子
				nHalfvalue = m_BaseValue[nChessType] / 16;  //棋子价值的1/16作为威胁/保护增量
				m_chessValue[i][j] += m_BaseValue[nChessType];  //基本价值计入总价值

				if (IsRed(nChessType))  //红棋
				{
					if (m_AttackPos[i][j])  //红棋被威胁
					{
						if (bIsRedTurn)
						{  //轮到红棋
							if (nChessType == R_KING)
							{  //红将
								m_chessValue[i][j] -= 20;  //价值减20
							}
							else
							{
								m_chessValue[i][j] -= nHalfvalue * 2;  //价值减2*nHalfvalue
								if (m_GuardPos[i][j])  //是否被保护
									m_chessValue[i][j] += nHalfvalue;  //加上nHalfvalue
							}
						}
						else
						{  //红棋被威胁，轮到黑棋走
							if (nChessType == R_KING)  //是否红将
								return 18888;  //返回失败的极值
											   //else if (nChessType == R_CAR)
											   //	m_chessValue[i][j] -= nHalfvalue * 10;
							m_chessValue[i][j] -= nHalfvalue * 10;  //减10*nHalfvalue,表示威胁程度高
							if (m_GuardPos[i][j])  //如果被保护
								m_chessValue[i][j] += nHalfvalue * 9;  //加上9*nHalfvalue
						}
						m_chessValue[i][j] -= m_AttackPos[i][j];  //加上被兵威胁的威胁差
					}
					else
					{  //没有受威胁
						if (m_GuardPos[i][j])
							m_chessValue[i][j] += 5;  //加一点受保护分
					}
				}
				else
				{  //黑棋
					if (m_AttackPos[i][j])
					{  //受威胁
						if (!bIsRedTurn)
						{  //黑棋走
							if (nChessType == B_KING)  //黑将
							{
								m_chessValue[i][j] -= 20;  //价值减20
							}
							else
							{
								m_chessValue[i][j] -= nHalfvalue * 2;  //非将棋子价值减2*nHalfvalue
								if (m_GuardPos[i][j])  //如果受保护
									m_chessValue[i][j] += nHalfvalue;  //棋子价值增加nHalfvalue
							}
						}
						else
						{  //红棋走
							if (nChessType == B_KING)  //黑将
								return 18888;  //返回失败极值
											   //else if (nChessType == B_CAR)
											   //	m_chessValue[i][j] -= nHalfvalue * 10;
							m_chessValue[i][j] -= nHalfvalue * 10; //棋子价值减10*nHalfvalue
							if (m_GuardPos[i][j])  //如果受保护
								m_chessValue[i][j] += nHalfvalue * 9;  //加上9*nHalfvalue
						}
						m_chessValue[i][j] -= m_AttackPos[i][j];  //加上被兵威胁的威胁差
					}
					else
					{  //没有威胁
						if (m_GuardPos[i][j])
							m_chessValue[i][j] += 5;  //加一点保护分
					}
				}
			}
		}
	//统计红黑两方总分
	int nRedValue = 0;
	int	nBlackValue = 0;

	for (i = 0; i < 10; i++)
		for (j = 0; j < 9; j++)
		{
			nChessType = position[i][j];  //取子
										  //			if (nChessType == R_KING || nChessType == B_KING)
										  //				m_chessValue[i][j] = 10000;	
			if (nChessType != NOCHESS)  //有子
			{
				if (IsRed(nChessType))
				{
					nRedValue += m_chessValue[i][j];  //红棋总分
				}
				else
				{
					nBlackValue += m_chessValue[i][j];  //黑棋总分
				}
			}
		}
	/*
	if (bIsRed == 1)
	{
		if (bIsRedTurn)
			return nRedValue - nBlackValue;//如果轮到红棋走返回估值

		return nBlackValue - nRedValue;//如果轮到黑棋走返回负估值
	}

	if (bIsRedTurn)
		return nBlackValue - nRedValue;//如果轮到黑棋走返回负估值			

	return nRedValue - nBlackValue;//如果轮到红棋走返回估值
	*/
	if (bIsRedTurn)
	{
		return nRedValue - nBlackValue;  //轮到红棋走返回估值
	}
	else
	{
		return  nBlackValue - nRedValue;  //轮到黑棋走返回估值
	}
	
}

void CEveluation::AddPoint(int  x, int y)  //【函数将一个位置加入数组RelatePos当中】
{
	RelatePos[nPosCount].x = x;
	RelatePos[nPosCount].y = y;
	nPosCount++;
}

int CEveluation::GetRelatePiece(BYTE position[10][9], int j, int i)  //【枚举给定位置上的棋子的所有相关位置，包括可走到位置和可保护位置，position是棋盘，x,y棋子坐标】
{
	nPosCount = 0;
	BYTE nChessID;
	BYTE flag;
	int x, y;

	nChessID = position[i][j];
	switch (nChessID)
	{
	case R_KING:  //红将
	case B_KING:  //黑将
				  //循环扫描九宫
		for (y = 0; y < 3; y++)
			for (x = 3; x < 6; x++)
				if (CanTouch(position, j, i, x, y))  //能否走到
					AddPoint(x, y);  //可到达/保护的位置加入数组
		for (y = 7; y < 10; y++)
			for (x = 3; x < 6; x++)
				if (CanTouch(position, j, i, x, y))  //能否走到
					AddPoint(x, y);  //可到达/保护的位置加入数组
		break;

	case R_BISHOP:  //红士
					//循环扫描九宫
		for (y = 7; y < 10; y++)
			for (x = 3; x < 6; x++)
				if (CanTouch(position, j, i, x, y))  //能否走到
					AddPoint(x, y);  //可到达/保护的位置加入数组
		break;

	case B_BISHOP:  //黑士
					//循环扫描九宫
		for (y = 0; y < 3; y++)
			for (x = 3; x < 6; x++)
				if (CanTouch(position, j, i, x, y))  //能否走到
					AddPoint(x, y);  //可到达/保护的位置加入数组
		break;

	case R_ELEPHANT:  //红象
	case B_ELEPHANT:  //黑象
					  //右下
		x = j + 2;
		y = i + 2;
		if (x < 9 && y < 10 && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//右上
		x = j + 2;
		y = i - 2;
		if (x < 9 && y >= 0 && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//左下
		x = j - 2;
		y = i + 2;
		if (x >= 0 && y < 10 && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//左上
		x = j - 2;
		y = i - 2;
		if (x >= 0 && y >= 0 && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		break;

	case R_HORSE:  //红马
	case B_HORSE:  //黑马
				   //检查右下方是否能走/保护
		x = j + 2;
		y = i + 1;
		if ((x < 9 && y < 10) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//检查右上方是否能走/保护
		x = j + 2;
		y = i - 1;
		if ((x < 9 && y >= 0) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//检查左下方是否能走/保护
		x = j - 2;
		y = i + 1;
		if ((x >= 0 && y < 10) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//检查左上方是否能走/保护
		x = j - 2;
		y = i - 1;
		if ((x >= 0 && y >= 0) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//检查右下方是否能走/保护
		x = j + 1;
		y = i + 2;
		if ((x < 9 && y < 10) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//检查左下方是否能走/保护
		x = j - 1;
		y = i + 2;
		if ((x >= 0 && y < 10) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//检查右上方是否能走/保护
		x = j + 1;
		y = i - 2;
		if ((x < 9 && y >= 0) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//检查左上方是否能走/保护
		x = j - 1;
		y = i - 2;
		if ((x >= 0 && y >= 0) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		break;

	case R_CAR:  //红车
	case B_CAR:  //黑车 
				 //检查右方是否能走/保护
		x = j + 1;
		y = i;
		while (x < 9)
		{
			if (NOCHESS == position[y][x])  //无子
				AddPoint(x, y);
			else
			{  //有子
				AddPoint(x, y);
				break;  //后面不能走了
			}
			x++;
		}
		//检查左方是否能走/保护
		x = j - 1;
		y = i;
		while (x >= 0)
		{
			if (NOCHESS == position[y][x])  //无子
				AddPoint(x, y);
			else
			{  //有子
				AddPoint(x, y);
				break;  //后面不能走了
			}
			x--;
		}
		//检查下方是否能走/保护
		x = j;
		y = i + 1;//
		while (y < 10)
		{
			if (NOCHESS == position[y][x])  //无子
				AddPoint(x, y);
			else
			{  //有子
				AddPoint(x, y);
				break;  //后面不能走了
			}
			y++;
		}
		//检查上方是否能走/保护
		x = j;
		y = i - 1;//よ
		while (y >= 0)
		{
			if (NOCHESS == position[y][x])  //无子
				AddPoint(x, y);
			else
			{  //有子
				AddPoint(x, y);
				break;  //后面不能走了
			}
			y--;
		}
		break;

	case R_PAWN:  //红兵
				  //查看向前，还有是否到底
		y = i - 1;
		x = j;

		if (y >= 0)
			AddPoint(x, y);  //没到底，可走

		if (i < 5)
		{  //已过河
			y = i;
			x = j + 1;  //向右
			if (x < 9)
				AddPoint(x, y);  //未到右，可走
			x = j - 1;  //向左
			if (x >= 0)
				AddPoint(x, y);  //未到左，可走
		}
		break;

	case B_PAWN:  //黑兵
				  //查看向前，还有是否到底
		y = i + 1;
		x = j;

		if (y < 10)
			AddPoint(x, y);  //没到底，可走

		if (i > 4)
		{  //已过河
			y = i;
			x = j + 1;  //向右
			if (x < 9)
				AddPoint(x, y);  //未到右，可走
			x = j - 1;  //向左
			if (x >= 0)
				AddPoint(x, y);  //未到左，可走
		}
		break;

	case B_CANON:  //黑炮
	case R_CANON:  //红炮
				   //检查右方是否能走/保护
		x = j + 1;
		y = i;
		flag = FALSE;
		while (x < 9)
		{
			if (NOCHESS == position[y][x])
			{  //无子
				if (!flag)
					AddPoint(x, y);
			}
			else
			{  //有子
				if (!flag)
					flag = TRUE;  //是第1个棋子
				else
				{  //是第2个棋子
					AddPoint(x, y);
					break;
				}
			}
			x++;  //继续向右
		}
		//检查左方是否能走/保护
		x = j - 1;
		flag = FALSE;
		while (x >= 0)
		{
			if (NOCHESS == position[y][x])
			{  //无子
				if (!flag)
					AddPoint(x, y);
			}
			else
			{  //有子
				if (!flag)
					flag = TRUE;  //是第1个棋子
				else
				{  //是第2个棋子
					AddPoint(x, y);
					break;
				}
			}
			x--;  //继续向左
		}
		//检查下方是否能走/保护
		x = j;
		y = i + 1;
		flag = FALSE;
		while (y < 10)
		{
			if (NOCHESS == position[y][x])
			{  //无子
				if (!flag)
					AddPoint(x, y);
			}
			else
			{  //有子
				if (!flag)
					flag = TRUE;  //是第1个棋子
				else
				{  //是第2个棋子
					AddPoint(x, y);
					break;
				}
			}
			y++;  //继续向下
		}
		//检查上方是否能走/保护
		y = i - 1;	//
		flag = FALSE;
		while (y >= 0)
		{
			if (NOCHESS == position[y][x])
			{  //无子
				if (!flag)
					AddPoint(x, y);
			}
			else
			{  //有子
				if (!flag)
					flag = TRUE;  //是第1个棋子
				else
				{  //是第2个棋子
					AddPoint(x, y);
					break;
				}
			}
			y--;  //继续向上
		}
		break;

	default:
		break;

	}
	return nPosCount;
}

BOOL CEveluation::CanTouch(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY)  //【判断position位置From的棋子能否走到位置To,能：True,否：False】
{
	int i, j;
	int nMoveChessID, nTargetID;

	if (nFromY == nToY && nFromX == nToX)
		return FALSE;//目的与源相同

	nMoveChessID = position[nFromY][nFromX];
	nTargetID = position[nToY][nToX];

	switch (nMoveChessID)
	{
	case B_KING:  //黑将
		if (nTargetID == R_KING)//老将见面?
		{
			if (nFromX != nToX)
				return FALSE;
			for (i = nFromY + 1; i < nToY; i++)
				if (position[i][nFromX] != NOCHESS)
					return FALSE;
		}
		else
		{
			if (nToY > 2 || nToX > 5 || nToX < 3)
				return FALSE;//目标点在九宫之外
			if (abs(nFromY - nToY) + abs(nToX - nFromX) > 1)
				return FALSE;//将帅只走一步直线:
		}
		break;
	case R_BISHOP:  //红士

		if (nToY < 7 || nToX > 5 || nToX < 3)
			return FALSE;//士出九宫	

		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//士走斜线

		break;

	case B_BISHOP:   //黑士

		if (nToY > 2 || nToX > 5 || nToX < 3)
			return FALSE;//士出九宫	

		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//士走斜线

		break;

	case R_ELEPHANT://红象

		if (nToY < 5)
			return FALSE;//相不能过河

		if (abs(nFromX - nToX) != 2 || abs(nFromY - nToY) != 2)
			return FALSE;//相走田字

		if (position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//相眼被塞住了

		break;

	case B_ELEPHANT://黑象 

		if (nToY > 4)
			return FALSE;//相不能过河

		if (abs(nFromX - nToX) != 2 || abs(nFromY - nToY) != 2)
			return FALSE;//相走田字

		if (position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//相眼被塞住了

		break;

	case B_PAWN:     //黑兵

		if (nToY < nFromY)
			return FALSE;//兵不回头

		if (nFromY < 5 && nFromY == nToY)
			return FALSE;//兵过河前只能直走

		if (nToY - nFromY + abs(nToX - nFromX) > 1)
			return FALSE;//兵只走一步直线:

		break;

	case R_PAWN:    //红兵

		if (nToY > nFromY)
			return FALSE;//兵不回头

		if (nFromY > 4 && nFromY == nToY)
			return FALSE;//兵过河前只能直走

		if (nFromY - nToY + abs(nToX - nFromX) > 1)
			return FALSE;//兵只走一步直线:

		break;

	case R_KING:  //红将
		if (nTargetID == B_KING)//老将见面?
		{
			if (nFromX != nToX)
				return FALSE;//两个将不在同一列
			for (i = nFromY - 1; i > nToY; i--)
				if (position[i][nFromX] != NOCHESS)
					return FALSE;//中间有别的子
		}
		else
		{
			if (nToY < 7 || nToX > 5 || nToX < 3)
				return FALSE;//目标点在九宫之外
			if (abs(nFromY - nToY) + abs(nToX - nFromX) > 1)
				return FALSE;//将帅只走一步直线:
		}
		break;

	case B_CAR:  //黑车
	case R_CAR:  //红车

		if (nFromY != nToY && nFromX != nToX)
			return FALSE;	//车走直线:

		if (nFromY == nToY)
		{
			if (nFromX < nToX)
			{  //向右
				for (i = nFromX + 1; i < nToX; i++)
					if (position[nFromY][i] != NOCHESS)
						return FALSE;
			}
			else
			{  //向左
				for (i = nToX + 1; i < nFromX; i++)
					if (position[nFromY][i] != NOCHESS)
						return FALSE;
			}
		}
		else
		{
			if (nFromY < nToY)
			{  //向下
				for (j = nFromY + 1; j < nToY; j++)
					if (position[j][nFromX] != NOCHESS)
						return FALSE;
			}
			else
			{  //向上
				for (j = nToY + 1; j < nFromY; j++)
					if (position[j][nFromX] != NOCHESS)
						return FALSE;
			}
		}

		break;

	case B_HORSE:  //黑马
	case R_HORSE:  //红马

		if (!((abs(nToX - nFromX) == 1 && abs(nToY - nFromY) == 2)
			|| (abs(nToX - nFromX) == 2 && abs(nToY - nFromY) == 1)))
			return FALSE;//马走日字

		if (nToX - nFromX == 2)
		{
			i = nFromX + 1;
			j = nFromY;
		}
		else if (nFromX - nToX == 2)
		{
			i = nFromX - 1;
			j = nFromY;
		}
		else if (nToY - nFromY == 2)
		{
			i = nFromX;
			j = nFromY + 1;
		}
		else if (nFromY - nToY == 2)
		{
			i = nFromX;
			j = nFromY - 1;
		}

		if (position[j][i] != (BYTE)NOCHESS)
			return FALSE;//绊马腿

		break;
	case B_CANON:  //黑炮
	case R_CANON:  //红炮

		if (nFromY != nToY && nFromX != nToX)
			return FALSE;	//炮走直线:

							//炮不吃子时经过的路线中不能有棋子:------------------

		if (position[nToY][nToX] == NOCHESS)
		{
			if (nFromY == nToY)
			{
				if (nFromX < nToX)
				{  //向右
					for (i = nFromX + 1; i < nToX; i++)
						if (position[nFromY][i] != NOCHESS)
							return FALSE;
				}
				else
				{  //向左
					for (i = nToX + 1; i < nFromX; i++)
						if (position[nFromY][i] != NOCHESS)
							return FALSE;
				}
			}
			else
			{
				if (nFromY < nToY)
				{  //向下
					for (j = nFromY + 1; j < nToY; j++)
						if (position[j][nFromX] != NOCHESS)
							return FALSE;
				}
				else
				{  //向上
					for (j = nToY + 1; j < nFromY; j++)
						if (position[j][nFromX] != NOCHESS)
							return FALSE;
				}
			}
		}
		//以上是炮不吃子-------------------------------------
		//吃子时:=======================================
		else
		{
			int nCount = 0;
			if (nFromY == nToY)
			{
				if (nFromX < nToX)
				{  //向右
					for (i = nFromX + 1;i<nToX;i++)
						if (position[nFromY][i] != NOCHESS)
							nCount++;
					if (nCount != 1)  //不是隔一个子
						return FALSE;
				}
				else
				{  //向左
					for (i = nToX + 1;i<nFromX;i++)
						if (position[nFromY][i] != NOCHESS)
							nCount++;
					if (nCount != 1)  //不是隔一个子
						return FALSE;
				}
			}
			else
			{
				if (nFromY<nToY)
				{  //向下
					for (j = nFromY + 1;j<nToY;j++)
						if (position[j][nFromX] != NOCHESS)
							nCount++;
					if (nCount != 1)  //不是隔一个子
						return FALSE;
				}
				else
				{  //向上
					for (j = nToY + 1;j<nFromY;j++)
						if (position[j][nFromX] != NOCHESS)
							nCount++;
					if (nCount != 1)  //不是隔一个子
						return FALSE;
				}
			}
		}
		//以上是炮吃子时================================
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

