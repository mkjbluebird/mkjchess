#include "stdafx.h"
#include "mkjchess.h"
#include "MoveGenerator.h"


CMoveGenerator::CMoveGenerator()
{
}


CMoveGenerator::~CMoveGenerator()
{
}

BOOL CMoveGenerator::IsValidMove(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY)  //【判断落子合法函数，合法返回True,否则返回False】
{
	int i, j;
	int nMoveChessID, nTargetID;

	if (nFromY == nToY && nFromX == nToX)
		return FALSE;//目的与源相同

	nMoveChessID = position[nFromY][nFromX];
	nTargetID = position[nToY][nToX];

	if (IsSameSide(nMoveChessID, nTargetID))
		return FALSE;//不能吃自己的棋

	switch (nMoveChessID)
	{
	case B_KING:
			/*
		if (bIsRed)
		{
			if (nTargetID == R_KING)//判断是否将帅见面
			{
				if (nFromX != nToX)//横坐标不相等
					return false;//将帅不在同一列

				for (i = nFromY + 1; i<nToY; i++)
					if (position[i][nFromX] != NOCHESS)
						return false;//中间隔有棋子
			}
			else
			{
				if (nToY>2 || nToX>5 || nToX<3)
					return false;//目标点在九宫之外

				if (abs(nFromY - nToY) + abs(nFromX - nToX)>1)
					return false;//将帅只走一步直线
			}
		}
		else
		{
			if (nTargetID == R_KING)//判断是否将帅见面
			{
				if (nFromX != nToX)//横坐标不相等
					return false;//将帅不在同一列

				for (i = nFromY - 1; i>nToY; i--)
					if (position[i][nFromX] != NOCHESS)
						return false;//中间隔有棋子
			}
			else
			{
				if (nToY<7 || nToX>5 || nToX<3)
					return false;//目标点在九宫之外

				if (abs(nFromY - nToY) + abs(nFromX - nToX)>1)
					return false;//将帅只走一步直线
			}
		}
		*/
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
	case R_BISHOP:
			/*
		if (bIsRed)
		{
			if (nToY<7 || nToX>5 || nToX<3)
				return false;//士出九宫
		}
		else
		{
			if (nToY>2 || nToX>5 || nToX<3)
				return false;//士出九宫
		}
		*/
		if (nToY < 7 || nToX > 5 || nToX < 3)
			return FALSE;//士出九宫	
			

		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//士走斜线

		break;

	case B_BISHOP:   //黑士
					 /*
		if (bIsRed)
		{
			if (nToY>2 || nToX>5 || nToX<3)
				return false;//士出九宫
		}
		else
		{
			if (nToY<7 || nToX>5 || nToX<3)
				return false;//士出九宫
		}
		*/
		if (nToY > 2 || nToX > 5 || nToX < 3)
			return FALSE;//士出九宫	
			

		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//士走斜线

		break;

	case R_ELEPHANT://红象
					/*
		if (bIsRed)
		{
			if (nToY<5)
				return false;//相不能过河
		}
		else
		{
			if (nToY>4)
				return false;//相不能过河
		}
		*/
		if (nToY < 5)
			return FALSE;//相不能过河
			
		if (abs(nFromX - nToX) != 2 || abs(nFromY - nToY) != 2)
			return FALSE;//相走田字

		if (position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//相眼被塞住了

		break;

	case B_ELEPHANT://黑象 
			/*
		if (bIsRed)
		{
			if (nToY>4)
				return false;//象不能过河
		}
		else
		{
			if (nToY<5)
				return false;//象不能过河
		}
		*/
		if (nToY > 4)
			return FALSE;//相不能过河
			
		if (abs(nFromX - nToX) != 2 || abs(nFromY - nToY) != 2)
			return FALSE;//相走田字

		if (position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//相眼被塞住了

		break;

	case B_PAWN:     //黑兵
			/*
		if (bIsRed)
		{
			if (nToY<nFromY)
				return false;//卒不能回头

			if (nFromY<5 && nFromY == nToY)
				return FALSE;//卒过河前只能直走
		}
		else
		{
			if (nToY>nFromY)
				return false;//卒不能回头

			if (nFromY>4 && nFromY == nToY)
				return FALSE;//卒过河前只能直走
		}
		*/
		if (nToY < nFromY)
			return FALSE;//兵不回头

		if (nFromY < 5 && nFromY == nToY)
			return FALSE;//兵过河前只能直走
			
		if (nToY - nFromY + abs(nToX - nFromX) > 1)
			return FALSE;//兵只走一步直线:

		break;

	case R_PAWN:    //红兵
			/*
		if (bIsRed)
		{
			if (nToY>nFromY)
				return false;//兵不能回头

			if (nFromY>4 && nFromY == nToY)
				return FALSE;//兵过河前只能直走
		}
		else
		{
			if (nToY<nFromY)
				return false;//兵不能回头

			if (nFromY<5 && nFromY == nToY)
				return FALSE;//兵过河前只能直走
		}
		*/
		if (nToY > nFromY)
			return FALSE;//兵不回头

		if (nFromY > 4 && nFromY == nToY)
			return FALSE;//兵过河前只能直走
			
		if (nFromY - nToY + abs(nToX - nFromX) > 1)
			return FALSE;//兵只走一步直线:

		break;

	case R_KING:
			/*
		if (bIsRed)
		{
			if (nTargetID == B_KING)//判断是否将帅见面
			{
				if (nFromX != nToX)//横坐标不相等
					return false;//将帅不在同一列

				for (i = nFromY - 1; i>nToY; i--)
					if (position[i][nFromX] != NOCHESS)
						return false;//中间隔有棋子
			}
			else
			{
				if (nToY<7 || nToX>5 || nToX<3)
					return false;//目标点在九宫之外

				if (abs(nFromY - nToY) + abs(nFromX - nToX)>1)
					return false;//将帅只走一步直线
			}
		}
		else
		{
			if (nTargetID == B_KING)//判断是否将帅见面
			{
				if (nFromX != nToX)//横坐标不相等
					return false;//将帅不在同一列

				for (i = nFromY + 1; i<nToY; i++)
					if (position[i][nFromX] != NOCHESS)
						return false;//中间隔有棋子
			}
			else
			{
				if (nToY>2 || nToX>5 || nToX<3)
					return false;//目标点在九宫之外

				if (abs(nFromY - nToY) + abs(nFromX - nToX)>1)
					return false;//将帅只走一步直线
			}
		}
		*/
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

	case B_CAR:
	case R_CAR:

		if (nFromY != nToY && nFromX != nToX)
			return FALSE;	//车走直线:

		if (nFromY == nToY)
		{
			if (nFromX < nToX && nToX <= 9)
			{
				for (i = nFromX + 1; i < nToX; i++)
					if (position[nFromY][i] != NOCHESS)
						return FALSE;
			}
			else if(nFromX >= 0)
			{
				for (i = nToX + 1; i < nFromX; i++)
					if (position[nFromY][i] != NOCHESS)
						return FALSE;
			}
		}
		else
		{
			if (nFromY < nToY && nToX <=10)
			{
				for (j = nFromY + 1; j < nToY; j++)
					if (position[j][nFromX] != NOCHESS)
						return FALSE;
			}
			else if(nFromY >= 0)
			{
				for (j = nToY + 1; j < nFromY; j++)
					if (position[j][nFromX] != NOCHESS)
						return FALSE;
			}
		}

		break;

	case B_HORSE:
	case R_HORSE:

		if (!((abs(nToX - nFromX) == 1 && abs(nToY - nFromY) == 2)
			|| (abs(nToX - nFromX) == 2 && abs(nToY - nFromY) == 1)))
			return FALSE;//马走日字

		if (nToX - nFromX == 2 && nToX <=9)
		{  //向右
			i = nFromX + 1;
			j = nFromY;
		}
		else if (nFromX - nToX == 2 && nFromX >=0)
		{  //向左
			i = nFromX - 1;
			j = nFromY;
		}
		else if (nToY - nFromY == 2 && nToY <=10)
		{  //向下
			i = nFromX;
			j = nFromY + 1;
		}
		else if (nFromY - nToY == 2 && nFromY >=0)
		{  //向上
			i = nFromX;
			j = nFromY - 1;
		}

		if (position[j][i] != NOCHESS)
			return FALSE;//绊马腿

		break;

	case B_CANON:
	case R_CANON:

		if (nFromY != nToY && nFromX != nToX)
			return FALSE;	//炮走直线:

							//炮不吃子时经过的路线中不能有棋子

		if (position[nToY][nToX] == NOCHESS)
		{
			if (nFromY == nToY)
			{
				if (nFromX < nToX  && nToX <= 9)
				{
					for (i = nFromX + 1; i < nToX; i++)
						if (position[nFromY][i] != NOCHESS)
							return FALSE;
				}
				else if (nToX >=0)
				{

				}
				{
					for (i = nToX + 1; i < nFromX; i++)
						if (position[nFromY][i] != NOCHESS)
							return FALSE;
				}
			}
			else
			{
				if (nFromY < nToY && nToY <= 10)
				{
					for (j = nFromY + 1; j < nToY; j++)
						if (position[j][nFromX] != NOCHESS)
							return FALSE;
				}
				else if (nToY >= 0)
				{

				}
				{
					for (j = nToY + 1; j < nFromY; j++)
						if (position[j][nFromX] != NOCHESS)
							return FALSE;
				}
			}
		}
		//炮吃子时
		else
		{
			int count = 0;
			if (nFromY == nToY)
			{
				if (nFromX < nToX  && nToX <= 9)
				{
					for (i = nFromX + 1;i<nToX;i++)
						if (position[nFromY][i] != NOCHESS)
							count++;
					if (count != 1)
						return FALSE;
				}
				else if (nToX >= 0)
				{
					for (i = nToX + 1;i<nFromX;i++)
						if (position[nFromY][i] != NOCHESS)
							count++;
					if (count != 1)
						return FALSE;
				}
			}
			else
			{
				if (nFromY<nToY && nToY <= 10)
				{
					for (j = nFromY + 1;j<nToY;j++)
						if (position[j][nFromX] != NOCHESS)
							count++;
					if (count != 1)
						return FALSE;
				}
				else if (nToY >= 0)
				{
					for (j = nToY + 1;j<nFromY;j++)
						if (position[j][nFromX] != NOCHESS)
							count++;
					if (count != 1)
						return FALSE;
				}
			}
		}
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

int CMoveGenerator::AddMove(int nFromX, int nFromY, int nToX, int nToY, int nPly)  //【在m_MoveList中加入走法。nPly是走法层次】
{
	m_MoveList[nPly][m_nMoveCount].From.x = nFromX;
	m_MoveList[nPly][m_nMoveCount].From.y = nFromY;
	m_MoveList[nPly][m_nMoveCount].To.x = nToX;
	m_MoveList[nPly][m_nMoveCount].To.y = nToY;
	//m_MoveList[nPly][m_nMoveCount].ChessID = nChessID;
	m_nMoveCount++;
	return m_nMoveCount;
}

int CMoveGenerator::CreatePossibleMove(BYTE position[10][9], int nPly, int nSide) //【产生所有的落子，nPly为层次，nSide为哪一方，True红方，False黑方】
{
	int     nChessID;
	int		i, j;

	m_nMoveCount = 0;
	//m_bIsRed = bIsRed;
	for (i = 0; i < 10; i++)
		for (j = 0; j < 9; j++)
		{
			if (position[i][j] != NOCHESS)
			{
				nChessID = position[i][j];
					/*
				if (bIsRed)
				{
					if (!nSide && IsRed(nChessID))
						continue;//如要产生黑棋走法，跳过红棋

					if (nSide && IsBlack(nChessID))
						continue;//如要产生红棋走法，跳过黑棋
				}
				else
				{
					if (nSide && IsRed(nChessID))
						continue;//如要产生黑棋走法，跳过红棋

					if (!nSide && IsBlack(nChessID))
						continue;//如要产生红棋走法，跳过黑棋
				}
				*/
//+++++++++++++决定走棋方++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				if (!nSide && IsRed(nChessID)) //如果为黑方，跳过红子。
					continue;
				if (nSide && IsBlack(nChessID))  //如果为红方，跳过黑子。
					continue;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++					
				switch (nChessID)
				{
				case R_KING: //将帅
				case B_KING:
					Gen_KingMove(position, i, j, nPly);
					break;

				case R_BISHOP:  //红士
					Gen_RBishopMove(position, i, j, nPly);
					break;

				case B_BISHOP:  //黑士
					Gen_BBishopMove(position, i, j, nPly);
					break;

				case R_ELEPHANT: //象
				case B_ELEPHANT:
					Gen_ElephantMove(position, i, j, nPly);
					break;

				case R_HORSE:  //马
				case B_HORSE:
					Gen_HorseMove(position, i, j, nPly);
					break;

				case R_CAR:  //车
				case B_CAR:
					Gen_CarMove(position, i, j, nPly);
					break;

				case R_PAWN:  //红兵
					Gen_RPawnMove(position, i, j, nPly);
					break;

				case B_PAWN:  //黑兵
					Gen_BPawnMove(position, i, j, nPly);
					break;

				case B_CANON:  //炮
				case R_CANON:
					Gen_CanonMove(position, i, j, nPly);
					break;

				default:
					break;

				}
			}
		}

	return m_nMoveCount;  //返回总的落子可能
}

void CMoveGenerator::Gen_KingMove(BYTE position[10][9], int i, int j, int nPly)  //【将的检测落子函数，i,j将的位置，nPly插入的层次】
{
	int x, y;
	for (y = 0; y < 3; y++)
		for (x = 3; x < 6; x++)
			if (IsValidMove(position, j, i, x, y)) //检测落子是否合法
				AddMove(j, i, x, y, nPly);  //将这个走法插入m_MoveList

	for (y = 7; y < 10; y++)
		for (x = 3; x < 6; x++)
			if (IsValidMove(position, j, i, x, y)) //检测落子是否合法
				AddMove(j, i, x, y, nPly);   //将这个走法插入m_MoveList
}

void CMoveGenerator::Gen_RBishopMove(BYTE position[10][9], int i, int j, int nPly)  //【红士的检测落子函数，i,j将的位置，nPly插入的层次】
{
	int x, y;
	for (y = 7; y < 10; y++)
		for (x = 3; x < 6; x++)
			if (IsValidMove(position, j, i, x, y)) //检测落子是否合法
				AddMove(j, i, x, y, nPly);  //将这个走法插入m_MoveList
}

void CMoveGenerator::Gen_BBishopMove(BYTE position[10][9], int i, int j, int nPly)  //【黑士的检测落子函数，i,j将的位置，nPly插入的层次】
{
	int x, y;
	for (y = 0; y < 3; y++)
		for (x = 3; x < 6; x++)
			if (IsValidMove(position, j, i, x, y)) //检测落子是否合法
				AddMove(j, i, x, y, nPly);  //将这个走法插入m_MoveList
}

void CMoveGenerator::Gen_ElephantMove(BYTE position[10][9], int i, int j, int nPly) //【象的检测落子函数，i,j将的位置，nPly插入的层次】
{
	int x, y;

	x = j + 2;
	y = i + 2;
	if (x < 9 && y < 10 && IsValidMove(position, j, i, x, y))  //检测落子是否合法
		AddMove(j, i, x, y, nPly);  //将这个走法插入m_MoveList

	x = j + 2;
	y = i - 2;
	if (x < 9 && y >= 0 && IsValidMove(position, j, i, x, y)) //检测落子是否合法
		AddMove(j, i, x, y, nPly);  //将这个走法插入m_MoveList

	x = j - 2;
	y = i + 2;
	if (x >= 0 && y < 10 && IsValidMove(position, j, i, x, y)) //检测落子是否合法
		AddMove(j, i, x, y, nPly);  //将这个走法插入m_MoveList

	x = j - 2;
	y = i - 2;
	if (x >= 0 && y >= 0 && IsValidMove(position, j, i, x, y))  //检测落子是否合法
		AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList

}

void CMoveGenerator::Gen_HorseMove(BYTE position[10][9], int i, int j, int nPly)  //【马的检测落子函数，i：Y,j：X未走前走的位置，nPly插入的层次】
{
	int x, y;

	x = j + 2;
	y = i + 1;
	if ((x < 9 && y < 10) && IsValidMove(position, j, i, x, y))  //检测落子是否合法
		AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList

	x = j + 2;
	y = i - 1;
	if ((x < 9 && y >= 0) && IsValidMove(position, j, i, x, y))  //检测落子是否合法
		AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList

	x = j - 2;
	y = i + 1;
	if ((x >= 0 && y < 10) && IsValidMove(position, j, i, x, y))  //检测落子是否合法
		AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList

	x = j - 2;
	y = i - 1;
	if ((x >= 0 && y >= 0) && IsValidMove(position, j, i, x, y))  //检测落子是否合法
		AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList

	x = j + 1;
	y = i + 2;
	if ((x < 9 && y < 10) && IsValidMove(position, j, i, x, y))  //检测落子是否合法
		AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList
	x = j - 1;
	y = i + 2;
	if ((x >= 0 && y < 10) && IsValidMove(position, j, i, x, y))  //检测落子是否合法
		AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList
	x = j + 1;
	y = i - 2;
	if ((x < 9 && y >= 0) && IsValidMove(position, j, i, x, y))  //检测落子是否合法
		AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList
	x = j - 1;
	y = i - 2;
	if ((x >= 0 && y >= 0) && IsValidMove(position, j, i, x, y)) //检测落子是否合法
		AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList

}

void CMoveGenerator::Gen_RPawnMove(BYTE position[10][9], int i, int j, int nPly)  //【红兵的检测落子函数，i,j将的位置，nPly插入的层次】
{
	int x, y;
	int nChessID;

	nChessID = position[i][j];
		/*
	if (m_bIsRed)
	{
		y = i - 1;//向前
		x = j;
		if (y>0 && !IsSameSide(nChessID, position[y][x]))
			AddMove(j, i, x, y, nPly, position[i][j]);//前方无阻碍

		if (i<5)//是否已过河
		{
			y = i;

			x = j + 1;//右边
			if (x<9 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);

			x = j - 1;//左边
			if (x >= 0 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);
		}
	}
	else
	{
		y = i + 1;//向前
		x = j;
		if (y>0 && !IsSameSide(nChessID, position[y][x]))
			AddMove(j, i, x, y, nPly, position[i][j]);//前方无阻碍

		if (i>4)//是否已过河
		{
			y = i;

			x = j + 1;//右边
			if (x<9 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);

			x = j - 1;//左边
			if (x >= 0 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);
		}
	}
	*/
	y = i - 1; //向前
	x = j;

	if (y > 0 && !IsSameSide(nChessID, position[y][x]))  //检测落子是否合法
		AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList 

	if (i < 5) //是否过河
	{
		y = i;
		x = j + 1;//向右
		if (x < 9 && !IsSameSide(nChessID, position[y][x]))  //检测落子是否合法
			AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList
		x = j - 1;//向左
		if (x >= 0 && !IsSameSide(nChessID, position[y][x]))  //检测落子是否合法
			AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList
	}
	
}


void CMoveGenerator::Gen_BPawnMove(BYTE position[10][9], int i, int j, int nPly)  //【黑兵的检测落子函数，i,j将的位置，nPly插入的层次】
{
	int x, y;
	int nChessID;

	nChessID = position[i][j];
		/*
	if (m_bIsRed)
	{
		y = i + 1;//向前
		x = j;
		if (y<10 && !IsSameSide(nChessID, position[y][x]))
			AddMove(j, i, x, y, nPly, position[i][j]);//前方无阻碍

		if (i>4)//是否已过河
		{
			y = i;

			x = j + 1;//右边
			if (x<9 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);

			x = j - 1;//左边
			if (x >= 0 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);
		}
	}
	else
	{
		y = i - 1;//向前
		x = j;
		if (y<10 && !IsSameSide(nChessID, position[y][x]))
			AddMove(j, i, x, y, nPly, position[i][j]);//前方无阻碍

		if (i<5)//是否已过河
		{
			y = i;

			x = j + 1;//右边
			if (x<9 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);

			x = j - 1;//左边
			if (x >= 0 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);
		}
	}
	*/
	y = i + 1;//向前
	x = j;

	if (y < 10 && !IsSameSide(nChessID, position[y][x]))  //检测落子是否合法
		AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList

	if (i > 4)//是否过河
	{
		y = i;
		x = j + 1;//向右
		if (x < 9 && !IsSameSide(nChessID, position[y][x]))  //检测落子是否合法
			AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList
		x = j - 1;//向左
		if (x >= 0 && !IsSameSide(nChessID, position[y][x]))  //检测落子是否合法
			AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList
	}
	
}

void CMoveGenerator::Gen_CarMove(BYTE position[10][9], int i, int j, int nPly)  //【车的检测落子函数，i,j将的位置，nPly插入的层次】
{
	int x, y;
	int nChessID;

	nChessID = position[i][j];

	x = j + 1;//向右
	y = i;
	while (x < 9)
	{
		if (NOCHESS == position[y][x])//无子阻拦
			AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList
		else
		{
			if (!IsSameSide(nChessID, position[y][x]))//吃子
				AddMove(j, i, x, y, nPly); //将这个走法插入m_MoveList
			break;
		}
		x++;
	}

	x = j - 1;//向左
	y = i;
	while (x >= 0)
	{
		if (NOCHESS == position[y][x])//无子阻拦
			AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
		else
		{
			if (!IsSameSide(nChessID, position[y][x]))//吃子
				AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
			break;
		}
		x--;
	}

	x = j;
	y = i + 1;//向上
	while (y < 10)
	{
		if (NOCHESS == position[y][x])//无子阻拦
			AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
		else
		{
			if (!IsSameSide(nChessID, position[y][x]))//吃子
				AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
			break;
		}
		y++;
	}

	x = j;
	y = i - 1;//向下
	while (y >= 0)
	{
		if (NOCHESS == position[y][x])//无子阻拦
			AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
		else
		{
			if (!IsSameSide(nChessID, position[y][x]))//吃子
				AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
			break;
		}
		y--;
	}
}

void CMoveGenerator::Gen_CanonMove(BYTE position[10][9], int i, int j, int nPly)  //【炮的检测落子函数，i,j将的位置，nPly插入的层次，flag是否隔子，True隔子，False未隔子】
{
	int x, y;
	BOOL	flag;
	int nChessID;

	nChessID = position[i][j];

	x = j + 1;		//向右
	y = i;
	flag = FALSE;
	while (x < 9)
	{
		if (NOCHESS == position[y][x])//无子阻拦
		{
			if (!flag)//未隔子
				AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
		}
		else
		{
			if (!flag)
				flag = TRUE;//隔子
			else
			{
				if (!IsSameSide(nChessID, position[y][x]))//吃子
					AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
				break;
			}
		}
		x++;
	}

	x = j - 1;    //向左
	flag = FALSE;
	while (x >= 0)
	{
		if (NOCHESS == position[y][x])//无子阻拦
		{
			if (!flag)//未隔子
				AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
		}
		else
		{
			if (!flag)
				flag = TRUE;//隔子
			else
			{
				if (!IsSameSide(nChessID, position[y][x]))//吃子
					AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
				break;
			}
		}
		x--;
	}
	x = j;
	y = i + 1;  //向上
	flag = FALSE;
	while (y < 10)
	{
		if (NOCHESS == position[y][x])//无子阻拦
		{
			if (!flag)//未隔子
				AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
		}
		else
		{
			if (!flag)
				flag = TRUE;//隔子
			else
			{
				if (!IsSameSide(nChessID, position[y][x]))//吃子
					AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
				break;
			}
		}
		y++;
	}

	y = i - 1;	//向下
	flag = FALSE;
	while (y >= 0)
	{
		if (NOCHESS == position[y][x])//无子阻拦
		{
			if (!flag)//未隔子
				AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
		}
		else
		{
			if (!flag)
				flag = TRUE;//隔子
			else
			{
				if (!IsSameSide(nChessID, position[y][x]))//吃子
					AddMove(j, i, x, y, nPly);//将这个走法插入m_MoveList
				break;
			}
		}
		y--;
	}

}
