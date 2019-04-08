#include "stdafx.h"
#include "mkjchess.h"
#include "MoveGenerator.h"


CMoveGenerator::CMoveGenerator()
{
}


CMoveGenerator::~CMoveGenerator()
{
}

BOOL CMoveGenerator::IsValidMove(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY)  //���ж����ӺϷ��������Ϸ�����True,���򷵻�False��
{
	int i, j;
	int nMoveChessID, nTargetID;

	if (nFromY == nToY && nFromX == nToX)
		return FALSE;//Ŀ����Դ��ͬ

	nMoveChessID = position[nFromY][nFromX];
	nTargetID = position[nToY][nToX];

	if (IsSameSide(nMoveChessID, nTargetID))
		return FALSE;//���ܳ��Լ�����

	switch (nMoveChessID)
	{
	case B_KING:
			/*
		if (bIsRed)
		{
			if (nTargetID == R_KING)//�ж��Ƿ�˧����
			{
				if (nFromX != nToX)//�����겻���
					return false;//��˧����ͬһ��

				for (i = nFromY + 1; i<nToY; i++)
					if (position[i][nFromX] != NOCHESS)
						return false;//�м��������
			}
			else
			{
				if (nToY>2 || nToX>5 || nToX<3)
					return false;//Ŀ����ھŹ�֮��

				if (abs(nFromY - nToY) + abs(nFromX - nToX)>1)
					return false;//��˧ֻ��һ��ֱ��
			}
		}
		else
		{
			if (nTargetID == R_KING)//�ж��Ƿ�˧����
			{
				if (nFromX != nToX)//�����겻���
					return false;//��˧����ͬһ��

				for (i = nFromY - 1; i>nToY; i--)
					if (position[i][nFromX] != NOCHESS)
						return false;//�м��������
			}
			else
			{
				if (nToY<7 || nToX>5 || nToX<3)
					return false;//Ŀ����ھŹ�֮��

				if (abs(nFromY - nToY) + abs(nFromX - nToX)>1)
					return false;//��˧ֻ��һ��ֱ��
			}
		}
		*/
		if (nTargetID == R_KING)//�Ͻ�����?
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
				return FALSE;//Ŀ����ھŹ�֮��
			if (abs(nFromY - nToY) + abs(nToX - nFromX) > 1)
				return FALSE;//��˧ֻ��һ��ֱ��:
		}
		
		break;
	case R_BISHOP:
			/*
		if (bIsRed)
		{
			if (nToY<7 || nToX>5 || nToX<3)
				return false;//ʿ���Ź�
		}
		else
		{
			if (nToY>2 || nToX>5 || nToX<3)
				return false;//ʿ���Ź�
		}
		*/
		if (nToY < 7 || nToX > 5 || nToX < 3)
			return FALSE;//ʿ���Ź�	
			

		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//ʿ��б��

		break;

	case B_BISHOP:   //��ʿ
					 /*
		if (bIsRed)
		{
			if (nToY>2 || nToX>5 || nToX<3)
				return false;//ʿ���Ź�
		}
		else
		{
			if (nToY<7 || nToX>5 || nToX<3)
				return false;//ʿ���Ź�
		}
		*/
		if (nToY > 2 || nToX > 5 || nToX < 3)
			return FALSE;//ʿ���Ź�	
			

		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//ʿ��б��

		break;

	case R_ELEPHANT://����
					/*
		if (bIsRed)
		{
			if (nToY<5)
				return false;//�಻�ܹ���
		}
		else
		{
			if (nToY>4)
				return false;//�಻�ܹ���
		}
		*/
		if (nToY < 5)
			return FALSE;//�಻�ܹ���
			
		if (abs(nFromX - nToX) != 2 || abs(nFromY - nToY) != 2)
			return FALSE;//��������

		if (position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//���۱���ס��

		break;

	case B_ELEPHANT://���� 
			/*
		if (bIsRed)
		{
			if (nToY>4)
				return false;//���ܹ���
		}
		else
		{
			if (nToY<5)
				return false;//���ܹ���
		}
		*/
		if (nToY > 4)
			return FALSE;//�಻�ܹ���
			
		if (abs(nFromX - nToX) != 2 || abs(nFromY - nToY) != 2)
			return FALSE;//��������

		if (position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//���۱���ס��

		break;

	case B_PAWN:     //�ڱ�
			/*
		if (bIsRed)
		{
			if (nToY<nFromY)
				return false;//�䲻�ܻ�ͷ

			if (nFromY<5 && nFromY == nToY)
				return FALSE;//�����ǰֻ��ֱ��
		}
		else
		{
			if (nToY>nFromY)
				return false;//�䲻�ܻ�ͷ

			if (nFromY>4 && nFromY == nToY)
				return FALSE;//�����ǰֻ��ֱ��
		}
		*/
		if (nToY < nFromY)
			return FALSE;//������ͷ

		if (nFromY < 5 && nFromY == nToY)
			return FALSE;//������ǰֻ��ֱ��
			
		if (nToY - nFromY + abs(nToX - nFromX) > 1)
			return FALSE;//��ֻ��һ��ֱ��:

		break;

	case R_PAWN:    //���
			/*
		if (bIsRed)
		{
			if (nToY>nFromY)
				return false;//�����ܻ�ͷ

			if (nFromY>4 && nFromY == nToY)
				return FALSE;//������ǰֻ��ֱ��
		}
		else
		{
			if (nToY<nFromY)
				return false;//�����ܻ�ͷ

			if (nFromY<5 && nFromY == nToY)
				return FALSE;//������ǰֻ��ֱ��
		}
		*/
		if (nToY > nFromY)
			return FALSE;//������ͷ

		if (nFromY > 4 && nFromY == nToY)
			return FALSE;//������ǰֻ��ֱ��
			
		if (nFromY - nToY + abs(nToX - nFromX) > 1)
			return FALSE;//��ֻ��һ��ֱ��:

		break;

	case R_KING:
			/*
		if (bIsRed)
		{
			if (nTargetID == B_KING)//�ж��Ƿ�˧����
			{
				if (nFromX != nToX)//�����겻���
					return false;//��˧����ͬһ��

				for (i = nFromY - 1; i>nToY; i--)
					if (position[i][nFromX] != NOCHESS)
						return false;//�м��������
			}
			else
			{
				if (nToY<7 || nToX>5 || nToX<3)
					return false;//Ŀ����ھŹ�֮��

				if (abs(nFromY - nToY) + abs(nFromX - nToX)>1)
					return false;//��˧ֻ��һ��ֱ��
			}
		}
		else
		{
			if (nTargetID == B_KING)//�ж��Ƿ�˧����
			{
				if (nFromX != nToX)//�����겻���
					return false;//��˧����ͬһ��

				for (i = nFromY + 1; i<nToY; i++)
					if (position[i][nFromX] != NOCHESS)
						return false;//�м��������
			}
			else
			{
				if (nToY>2 || nToX>5 || nToX<3)
					return false;//Ŀ����ھŹ�֮��

				if (abs(nFromY - nToY) + abs(nFromX - nToX)>1)
					return false;//��˧ֻ��һ��ֱ��
			}
		}
		*/
		if (nTargetID == B_KING)//�Ͻ�����?
		{
			if (nFromX != nToX)
				return FALSE;//����������ͬһ��
			for (i = nFromY - 1; i > nToY; i--)
				if (position[i][nFromX] != NOCHESS)
					return FALSE;//�м��б����
		}
		else
		{
			if (nToY < 7 || nToX > 5 || nToX < 3)
				return FALSE;//Ŀ����ھŹ�֮��
			if (abs(nFromY - nToY) + abs(nToX - nFromX) > 1)
				return FALSE;//��˧ֻ��һ��ֱ��:
		}
		
		break;

	case B_CAR:
	case R_CAR:

		if (nFromY != nToY && nFromX != nToX)
			return FALSE;	//����ֱ��:

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
			return FALSE;//��������

		if (nToX - nFromX == 2 && nToX <=9)
		{  //����
			i = nFromX + 1;
			j = nFromY;
		}
		else if (nFromX - nToX == 2 && nFromX >=0)
		{  //����
			i = nFromX - 1;
			j = nFromY;
		}
		else if (nToY - nFromY == 2 && nToY <=10)
		{  //����
			i = nFromX;
			j = nFromY + 1;
		}
		else if (nFromY - nToY == 2 && nFromY >=0)
		{  //����
			i = nFromX;
			j = nFromY - 1;
		}

		if (position[j][i] != NOCHESS)
			return FALSE;//������

		break;

	case B_CANON:
	case R_CANON:

		if (nFromY != nToY && nFromX != nToX)
			return FALSE;	//����ֱ��:

							//�ڲ�����ʱ������·���в���������

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
		//�ڳ���ʱ
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

int CMoveGenerator::AddMove(int nFromX, int nFromY, int nToX, int nToY, int nPly)  //����m_MoveList�м����߷���nPly���߷���Ρ�
{
	m_MoveList[nPly][m_nMoveCount].From.x = nFromX;
	m_MoveList[nPly][m_nMoveCount].From.y = nFromY;
	m_MoveList[nPly][m_nMoveCount].To.x = nToX;
	m_MoveList[nPly][m_nMoveCount].To.y = nToY;
	//m_MoveList[nPly][m_nMoveCount].ChessID = nChessID;
	m_nMoveCount++;
	return m_nMoveCount;
}

int CMoveGenerator::CreatePossibleMove(BYTE position[10][9], int nPly, int nSide) //���������е����ӣ�nPlyΪ��Σ�nSideΪ��һ����True�췽��False�ڷ���
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
						continue;//��Ҫ���������߷�����������

					if (nSide && IsBlack(nChessID))
						continue;//��Ҫ���������߷�����������
				}
				else
				{
					if (nSide && IsRed(nChessID))
						continue;//��Ҫ���������߷�����������

					if (!nSide && IsBlack(nChessID))
						continue;//��Ҫ���������߷�����������
				}
				*/
//+++++++++++++�������巽++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				if (!nSide && IsRed(nChessID)) //���Ϊ�ڷ����������ӡ�
					continue;
				if (nSide && IsBlack(nChessID))  //���Ϊ�췽���������ӡ�
					continue;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++					
				switch (nChessID)
				{
				case R_KING: //��˧
				case B_KING:
					Gen_KingMove(position, i, j, nPly);
					break;

				case R_BISHOP:  //��ʿ
					Gen_RBishopMove(position, i, j, nPly);
					break;

				case B_BISHOP:  //��ʿ
					Gen_BBishopMove(position, i, j, nPly);
					break;

				case R_ELEPHANT: //��
				case B_ELEPHANT:
					Gen_ElephantMove(position, i, j, nPly);
					break;

				case R_HORSE:  //��
				case B_HORSE:
					Gen_HorseMove(position, i, j, nPly);
					break;

				case R_CAR:  //��
				case B_CAR:
					Gen_CarMove(position, i, j, nPly);
					break;

				case R_PAWN:  //���
					Gen_RPawnMove(position, i, j, nPly);
					break;

				case B_PAWN:  //�ڱ�
					Gen_BPawnMove(position, i, j, nPly);
					break;

				case B_CANON:  //��
				case R_CANON:
					Gen_CanonMove(position, i, j, nPly);
					break;

				default:
					break;

				}
			}
		}

	return m_nMoveCount;  //�����ܵ����ӿ���
}

void CMoveGenerator::Gen_KingMove(BYTE position[10][9], int i, int j, int nPly)  //�����ļ�����Ӻ�����i,j����λ�ã�nPly����Ĳ�Ρ�
{
	int x, y;
	for (y = 0; y < 3; y++)
		for (x = 3; x < 6; x++)
			if (IsValidMove(position, j, i, x, y)) //��������Ƿ�Ϸ�
				AddMove(j, i, x, y, nPly);  //������߷�����m_MoveList

	for (y = 7; y < 10; y++)
		for (x = 3; x < 6; x++)
			if (IsValidMove(position, j, i, x, y)) //��������Ƿ�Ϸ�
				AddMove(j, i, x, y, nPly);   //������߷�����m_MoveList
}

void CMoveGenerator::Gen_RBishopMove(BYTE position[10][9], int i, int j, int nPly)  //����ʿ�ļ�����Ӻ�����i,j����λ�ã�nPly����Ĳ�Ρ�
{
	int x, y;
	for (y = 7; y < 10; y++)
		for (x = 3; x < 6; x++)
			if (IsValidMove(position, j, i, x, y)) //��������Ƿ�Ϸ�
				AddMove(j, i, x, y, nPly);  //������߷�����m_MoveList
}

void CMoveGenerator::Gen_BBishopMove(BYTE position[10][9], int i, int j, int nPly)  //����ʿ�ļ�����Ӻ�����i,j����λ�ã�nPly����Ĳ�Ρ�
{
	int x, y;
	for (y = 0; y < 3; y++)
		for (x = 3; x < 6; x++)
			if (IsValidMove(position, j, i, x, y)) //��������Ƿ�Ϸ�
				AddMove(j, i, x, y, nPly);  //������߷�����m_MoveList
}

void CMoveGenerator::Gen_ElephantMove(BYTE position[10][9], int i, int j, int nPly) //����ļ�����Ӻ�����i,j����λ�ã�nPly����Ĳ�Ρ�
{
	int x, y;

	x = j + 2;
	y = i + 2;
	if (x < 9 && y < 10 && IsValidMove(position, j, i, x, y))  //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly);  //������߷�����m_MoveList

	x = j + 2;
	y = i - 2;
	if (x < 9 && y >= 0 && IsValidMove(position, j, i, x, y)) //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly);  //������߷�����m_MoveList

	x = j - 2;
	y = i + 2;
	if (x >= 0 && y < 10 && IsValidMove(position, j, i, x, y)) //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly);  //������߷�����m_MoveList

	x = j - 2;
	y = i - 2;
	if (x >= 0 && y >= 0 && IsValidMove(position, j, i, x, y))  //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly); //������߷�����m_MoveList

}

void CMoveGenerator::Gen_HorseMove(BYTE position[10][9], int i, int j, int nPly)  //����ļ�����Ӻ�����i��Y,j��Xδ��ǰ�ߵ�λ�ã�nPly����Ĳ�Ρ�
{
	int x, y;

	x = j + 2;
	y = i + 1;
	if ((x < 9 && y < 10) && IsValidMove(position, j, i, x, y))  //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly); //������߷�����m_MoveList

	x = j + 2;
	y = i - 1;
	if ((x < 9 && y >= 0) && IsValidMove(position, j, i, x, y))  //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly); //������߷�����m_MoveList

	x = j - 2;
	y = i + 1;
	if ((x >= 0 && y < 10) && IsValidMove(position, j, i, x, y))  //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly); //������߷�����m_MoveList

	x = j - 2;
	y = i - 1;
	if ((x >= 0 && y >= 0) && IsValidMove(position, j, i, x, y))  //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly); //������߷�����m_MoveList

	x = j + 1;
	y = i + 2;
	if ((x < 9 && y < 10) && IsValidMove(position, j, i, x, y))  //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly); //������߷�����m_MoveList
	x = j - 1;
	y = i + 2;
	if ((x >= 0 && y < 10) && IsValidMove(position, j, i, x, y))  //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly); //������߷�����m_MoveList
	x = j + 1;
	y = i - 2;
	if ((x < 9 && y >= 0) && IsValidMove(position, j, i, x, y))  //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly); //������߷�����m_MoveList
	x = j - 1;
	y = i - 2;
	if ((x >= 0 && y >= 0) && IsValidMove(position, j, i, x, y)) //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly); //������߷�����m_MoveList

}

void CMoveGenerator::Gen_RPawnMove(BYTE position[10][9], int i, int j, int nPly)  //������ļ�����Ӻ�����i,j����λ�ã�nPly����Ĳ�Ρ�
{
	int x, y;
	int nChessID;

	nChessID = position[i][j];
		/*
	if (m_bIsRed)
	{
		y = i - 1;//��ǰ
		x = j;
		if (y>0 && !IsSameSide(nChessID, position[y][x]))
			AddMove(j, i, x, y, nPly, position[i][j]);//ǰ�����谭

		if (i<5)//�Ƿ��ѹ���
		{
			y = i;

			x = j + 1;//�ұ�
			if (x<9 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);

			x = j - 1;//���
			if (x >= 0 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);
		}
	}
	else
	{
		y = i + 1;//��ǰ
		x = j;
		if (y>0 && !IsSameSide(nChessID, position[y][x]))
			AddMove(j, i, x, y, nPly, position[i][j]);//ǰ�����谭

		if (i>4)//�Ƿ��ѹ���
		{
			y = i;

			x = j + 1;//�ұ�
			if (x<9 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);

			x = j - 1;//���
			if (x >= 0 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);
		}
	}
	*/
	y = i - 1; //��ǰ
	x = j;

	if (y > 0 && !IsSameSide(nChessID, position[y][x]))  //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly); //������߷�����m_MoveList 

	if (i < 5) //�Ƿ����
	{
		y = i;
		x = j + 1;//����
		if (x < 9 && !IsSameSide(nChessID, position[y][x]))  //��������Ƿ�Ϸ�
			AddMove(j, i, x, y, nPly); //������߷�����m_MoveList
		x = j - 1;//����
		if (x >= 0 && !IsSameSide(nChessID, position[y][x]))  //��������Ƿ�Ϸ�
			AddMove(j, i, x, y, nPly); //������߷�����m_MoveList
	}
	
}


void CMoveGenerator::Gen_BPawnMove(BYTE position[10][9], int i, int j, int nPly)  //���ڱ��ļ�����Ӻ�����i,j����λ�ã�nPly����Ĳ�Ρ�
{
	int x, y;
	int nChessID;

	nChessID = position[i][j];
		/*
	if (m_bIsRed)
	{
		y = i + 1;//��ǰ
		x = j;
		if (y<10 && !IsSameSide(nChessID, position[y][x]))
			AddMove(j, i, x, y, nPly, position[i][j]);//ǰ�����谭

		if (i>4)//�Ƿ��ѹ���
		{
			y = i;

			x = j + 1;//�ұ�
			if (x<9 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);

			x = j - 1;//���
			if (x >= 0 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);
		}
	}
	else
	{
		y = i - 1;//��ǰ
		x = j;
		if (y<10 && !IsSameSide(nChessID, position[y][x]))
			AddMove(j, i, x, y, nPly, position[i][j]);//ǰ�����谭

		if (i<5)//�Ƿ��ѹ���
		{
			y = i;

			x = j + 1;//�ұ�
			if (x<9 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);

			x = j - 1;//���
			if (x >= 0 && !IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly, position[i][j]);
		}
	}
	*/
	y = i + 1;//��ǰ
	x = j;

	if (y < 10 && !IsSameSide(nChessID, position[y][x]))  //��������Ƿ�Ϸ�
		AddMove(j, i, x, y, nPly); //������߷�����m_MoveList

	if (i > 4)//�Ƿ����
	{
		y = i;
		x = j + 1;//����
		if (x < 9 && !IsSameSide(nChessID, position[y][x]))  //��������Ƿ�Ϸ�
			AddMove(j, i, x, y, nPly); //������߷�����m_MoveList
		x = j - 1;//����
		if (x >= 0 && !IsSameSide(nChessID, position[y][x]))  //��������Ƿ�Ϸ�
			AddMove(j, i, x, y, nPly); //������߷�����m_MoveList
	}
	
}

void CMoveGenerator::Gen_CarMove(BYTE position[10][9], int i, int j, int nPly)  //�����ļ�����Ӻ�����i,j����λ�ã�nPly����Ĳ�Ρ�
{
	int x, y;
	int nChessID;

	nChessID = position[i][j];

	x = j + 1;//����
	y = i;
	while (x < 9)
	{
		if (NOCHESS == position[y][x])//��������
			AddMove(j, i, x, y, nPly); //������߷�����m_MoveList
		else
		{
			if (!IsSameSide(nChessID, position[y][x]))//����
				AddMove(j, i, x, y, nPly); //������߷�����m_MoveList
			break;
		}
		x++;
	}

	x = j - 1;//����
	y = i;
	while (x >= 0)
	{
		if (NOCHESS == position[y][x])//��������
			AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
		else
		{
			if (!IsSameSide(nChessID, position[y][x]))//����
				AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
			break;
		}
		x--;
	}

	x = j;
	y = i + 1;//����
	while (y < 10)
	{
		if (NOCHESS == position[y][x])//��������
			AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
		else
		{
			if (!IsSameSide(nChessID, position[y][x]))//����
				AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
			break;
		}
		y++;
	}

	x = j;
	y = i - 1;//����
	while (y >= 0)
	{
		if (NOCHESS == position[y][x])//��������
			AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
		else
		{
			if (!IsSameSide(nChessID, position[y][x]))//����
				AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
			break;
		}
		y--;
	}
}

void CMoveGenerator::Gen_CanonMove(BYTE position[10][9], int i, int j, int nPly)  //���ڵļ�����Ӻ�����i,j����λ�ã�nPly����Ĳ�Σ�flag�Ƿ���ӣ�True���ӣ�Falseδ���ӡ�
{
	int x, y;
	BOOL	flag;
	int nChessID;

	nChessID = position[i][j];

	x = j + 1;		//���ҥk
	y = i;
	flag = FALSE;
	while (x < 9)
	{
		if (NOCHESS == position[y][x])//��������
		{
			if (!flag)//δ����
				AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
		}
		else
		{
			if (!flag)
				flag = TRUE;//����
			else
			{
				if (!IsSameSide(nChessID, position[y][x]))//����
					AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
				break;
			}
		}
		x++;
	}

	x = j - 1;    //����
	flag = FALSE;
	while (x >= 0)
	{
		if (NOCHESS == position[y][x])//��������
		{
			if (!flag)//δ����
				AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
		}
		else
		{
			if (!flag)
				flag = TRUE;//����
			else
			{
				if (!IsSameSide(nChessID, position[y][x]))//����
					AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
				break;
			}
		}
		x--;
	}
	x = j;
	y = i + 1;  //����
	flag = FALSE;
	while (y < 10)
	{
		if (NOCHESS == position[y][x])//��������
		{
			if (!flag)//δ����
				AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
		}
		else
		{
			if (!flag)
				flag = TRUE;//����
			else
			{
				if (!IsSameSide(nChessID, position[y][x]))//����
					AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
				break;
			}
		}
		y++;
	}

	y = i - 1;	//���¤W
	flag = FALSE;
	while (y >= 0)
	{
		if (NOCHESS == position[y][x])//��������
		{
			if (!flag)//δ����
				AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
		}
		else
		{
			if (!flag)
				flag = TRUE;//����
			else
			{
				if (!IsSameSide(nChessID, position[y][x]))//����
					AddMove(j, i, x, y, nPly);//������߷�����m_MoveList
				break;
			}
		}
		y--;
	}

}
