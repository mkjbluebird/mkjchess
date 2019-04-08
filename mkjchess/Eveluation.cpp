#include "stdafx.h"
#include "mkjchess.h"
#include "Eveluation.h"
//����ĸ���ֵ����
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
//�ڱ��ĸ���ֵ����
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

int GetBingValue(int x, int y, BYTE CurSituation[][9])  //��Ϊÿһ�������ظ���ֵ��x,y�����꣬CurSituation:���̣����Ǳ�����0��
{
	if (CurSituation[y][x] == R_PAWN)  //����ĸ���ֵ
		return BA0[y][x];

	if (CurSituation[y][x] == B_PAWN)  //�ڱ��ĸ���ֵ
		return BA1[y][x];

	return 0;  //���Ǳ�����0
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

int count = 0;  //�����˹�ֵ������Ҷ�ӽڵ���
int CEveluation::Eveluate(BYTE position[10][9], BOOL bIsRedTurn)  //����ֵ������position�����̣�bIsRedTurn:True�췽��False�ڷ���
{
	int i, j, k;
	int nChessType, nTargetType;
	count++;  //����ֵ����Ҷ�ӽڵ���
			  //��ʼ����ʱ����
	memset(m_chessValue, 0, 360);
	memset(m_AttackPos, 0, 180);
	memset(m_GuardPos, 0, 90);
	memset(m_FlexibilityPos, 0, 90);
	//ɨ�����̣��ҳ�ÿһ�����ӣ�������в/���������ӣ������������
	for (i = 0; i < 10; i++)
		for (j = 0; j < 9; j++)
		{
			if (position[i][j] != NOCHESS)  //����
			{
				nChessType = position[i][j];  //ȡ��
				GetRelatePiece(position, j, i);  //�ҳ��������������λ��
				for (k = 0; k < nPosCount; k++)  //��ÿһĿ��λ��
				{
					nTargetType = position[RelatePos[k].y][RelatePos[k].x];  //ȡĿ��λ����������
					if (nTargetType == NOCHESS)  //����
					{
						m_FlexibilityPos[i][j]++;  //���������
					}
					else
					{   //����
						if (IsSameSide(nChessType, nTargetType))
						{
							m_GuardPos[RelatePos[k].y][RelatePos[k].x]++;  //�������ӣ�Ŀ���ܱ���
						}
						else
						{
							m_AttackPos[RelatePos[k].y][RelatePos[k].x]++;  //�з����ӣ�Ŀ������в
							m_FlexibilityPos[i][j]++;  //���������
							switch (nTargetType)
							{
							case R_KING:  //�Ǻ콫
								if (!bIsRedTurn)  //������
									return 18888;  //����ʧ�ܼ�ֵ
								break;
							case B_KING:  //�Ǻڽ�
								if (bIsRedTurn)  //������
									return 18888;  //����ʧ�ܼ�ֵ
								break;
							default:  //�ǽ�������
								m_AttackPos[RelatePos[k].y][RelatePos[k].x] += (30 + (m_BaseValue[nTargetType] - m_BaseValue[nChessType]) / 10) / 10;  //������в���Ӽ�����в��ֵ
								break;
							}
						}
					}
				}
			}
		}
	//ѭ��ͳ��ɨ������
	for (i = 0; i < 10; i++)
		for (j = 0; j < 9; j++)
		{
			if (position[i][j] != NOCHESS)
			{
				nChessType = position[i][j];  //ȡ��
				m_chessValue[i][j]++;  //������Ӵ��ڼ�ֵ��Ϊ0
				m_chessValue[i][j] += m_FlexValue[nChessType] * m_FlexibilityPos[i][j];  //������������Լ�ֵ
				m_chessValue[i][j] += GetBingValue(j, i, position);  //�����ϱ���λ�ü�ֵ
			}
		}
	//ѭ��ͳ��ɨ������
	int nHalfvalue;
	for (i = 0; i < 10; i++)
		for (j = 0; j < 9; j++)
		{
			if (position[i][j] != NOCHESS)  //����
			{
				nChessType = position[i][j];  //ȡ��
				nHalfvalue = m_BaseValue[nChessType] / 16;  //���Ӽ�ֵ��1/16��Ϊ��в/��������
				m_chessValue[i][j] += m_BaseValue[nChessType];  //������ֵ�����ܼ�ֵ

				if (IsRed(nChessType))  //����
				{
					if (m_AttackPos[i][j])  //���屻��в
					{
						if (bIsRedTurn)
						{  //�ֵ�����
							if (nChessType == R_KING)
							{  //�콫
								m_chessValue[i][j] -= 20;  //��ֵ��20
							}
							else
							{
								m_chessValue[i][j] -= nHalfvalue * 2;  //��ֵ��2*nHalfvalue
								if (m_GuardPos[i][j])  //�Ƿ񱻱���
									m_chessValue[i][j] += nHalfvalue;  //����nHalfvalue
							}
						}
						else
						{  //���屻��в���ֵ�������
							if (nChessType == R_KING)  //�Ƿ�콫
								return 18888;  //����ʧ�ܵļ�ֵ
											   //else if (nChessType == R_CAR)
											   //	m_chessValue[i][j] -= nHalfvalue * 10;
							m_chessValue[i][j] -= nHalfvalue * 10;  //��10*nHalfvalue,��ʾ��в�̶ȸ�
							if (m_GuardPos[i][j])  //���������
								m_chessValue[i][j] += nHalfvalue * 9;  //����9*nHalfvalue
						}
						m_chessValue[i][j] -= m_AttackPos[i][j];  //���ϱ�����в����в��
					}
					else
					{  //û������в
						if (m_GuardPos[i][j])
							m_chessValue[i][j] += 5;  //��һ���ܱ�����
					}
				}
				else
				{  //����
					if (m_AttackPos[i][j])
					{  //����в
						if (!bIsRedTurn)
						{  //������
							if (nChessType == B_KING)  //�ڽ�
							{
								m_chessValue[i][j] -= 20;  //��ֵ��20
							}
							else
							{
								m_chessValue[i][j] -= nHalfvalue * 2;  //�ǽ����Ӽ�ֵ��2*nHalfvalue
								if (m_GuardPos[i][j])  //����ܱ���
									m_chessValue[i][j] += nHalfvalue;  //���Ӽ�ֵ����nHalfvalue
							}
						}
						else
						{  //������
							if (nChessType == B_KING)  //�ڽ�
								return 18888;  //����ʧ�ܼ�ֵ
											   //else if (nChessType == B_CAR)
											   //	m_chessValue[i][j] -= nHalfvalue * 10;
							m_chessValue[i][j] -= nHalfvalue * 10; //���Ӽ�ֵ��10*nHalfvalue
							if (m_GuardPos[i][j])  //����ܱ���
								m_chessValue[i][j] += nHalfvalue * 9;  //����9*nHalfvalue
						}
						m_chessValue[i][j] -= m_AttackPos[i][j];  //���ϱ�����в����в��
					}
					else
					{  //û����в
						if (m_GuardPos[i][j])
							m_chessValue[i][j] += 5;  //��һ�㱣����
					}
				}
			}
		}
	//ͳ�ƺ�������ܷ�
	int nRedValue = 0;
	int	nBlackValue = 0;

	for (i = 0; i < 10; i++)
		for (j = 0; j < 9; j++)
		{
			nChessType = position[i][j];  //ȡ��
										  //			if (nChessType == R_KING || nChessType == B_KING)
										  //				m_chessValue[i][j] = 10000;	
			if (nChessType != NOCHESS)  //����
			{
				if (IsRed(nChessType))
				{
					nRedValue += m_chessValue[i][j];  //�����ܷ�
				}
				else
				{
					nBlackValue += m_chessValue[i][j];  //�����ܷ�
				}
			}
		}
	/*
	if (bIsRed == 1)
	{
		if (bIsRedTurn)
			return nRedValue - nBlackValue;//����ֵ������߷��ع�ֵ

		return nBlackValue - nRedValue;//����ֵ������߷��ظ���ֵ
	}

	if (bIsRedTurn)
		return nBlackValue - nRedValue;//����ֵ������߷��ظ���ֵ			

	return nRedValue - nBlackValue;//����ֵ������߷��ع�ֵ
	*/
	if (bIsRedTurn)
	{
		return nRedValue - nBlackValue;  //�ֵ������߷��ع�ֵ
	}
	else
	{
		return  nBlackValue - nRedValue;  //�ֵ������߷��ع�ֵ
	}
	
}

void CEveluation::AddPoint(int  x, int y)  //��������һ��λ�ü�������RelatePos���С�
{
	RelatePos[nPosCount].x = x;
	RelatePos[nPosCount].y = y;
	nPosCount++;
}

int CEveluation::GetRelatePiece(BYTE position[10][9], int j, int i)  //��ö�ٸ���λ���ϵ����ӵ��������λ�ã��������ߵ�λ�úͿɱ���λ�ã�position�����̣�x,y�������꡿
{
	nPosCount = 0;
	BYTE nChessID;
	BYTE flag;
	int x, y;

	nChessID = position[i][j];
	switch (nChessID)
	{
	case R_KING:  //�콫
	case B_KING:  //�ڽ�
				  //ѭ��ɨ��Ź�
		for (y = 0; y < 3; y++)
			for (x = 3; x < 6; x++)
				if (CanTouch(position, j, i, x, y))  //�ܷ��ߵ�
					AddPoint(x, y);  //�ɵ���/������λ�ü�������
		for (y = 7; y < 10; y++)
			for (x = 3; x < 6; x++)
				if (CanTouch(position, j, i, x, y))  //�ܷ��ߵ�
					AddPoint(x, y);  //�ɵ���/������λ�ü�������
		break;

	case R_BISHOP:  //��ʿ
					//ѭ��ɨ��Ź�
		for (y = 7; y < 10; y++)
			for (x = 3; x < 6; x++)
				if (CanTouch(position, j, i, x, y))  //�ܷ��ߵ�
					AddPoint(x, y);  //�ɵ���/������λ�ü�������
		break;

	case B_BISHOP:  //��ʿ
					//ѭ��ɨ��Ź�
		for (y = 0; y < 3; y++)
			for (x = 3; x < 6; x++)
				if (CanTouch(position, j, i, x, y))  //�ܷ��ߵ�
					AddPoint(x, y);  //�ɵ���/������λ�ü�������
		break;

	case R_ELEPHANT:  //����
	case B_ELEPHANT:  //����
					  //����
		x = j + 2;
		y = i + 2;
		if (x < 9 && y < 10 && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//����
		x = j + 2;
		y = i - 2;
		if (x < 9 && y >= 0 && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//����
		x = j - 2;
		y = i + 2;
		if (x >= 0 && y < 10 && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//����
		x = j - 2;
		y = i - 2;
		if (x >= 0 && y >= 0 && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		break;

	case R_HORSE:  //����
	case B_HORSE:  //����
				   //������·��Ƿ�����/����
		x = j + 2;
		y = i + 1;
		if ((x < 9 && y < 10) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//������Ϸ��Ƿ�����/����
		x = j + 2;
		y = i - 1;
		if ((x < 9 && y >= 0) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//������·��Ƿ�����/����
		x = j - 2;
		y = i + 1;
		if ((x >= 0 && y < 10) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//������Ϸ��Ƿ�����/����
		x = j - 2;
		y = i - 1;
		if ((x >= 0 && y >= 0) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//������·��Ƿ�����/����
		x = j + 1;
		y = i + 2;
		if ((x < 9 && y < 10) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//������·��Ƿ�����/����
		x = j - 1;
		y = i + 2;
		if ((x >= 0 && y < 10) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//������Ϸ��Ƿ�����/����
		x = j + 1;
		y = i - 2;
		if ((x < 9 && y >= 0) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		//������Ϸ��Ƿ�����/����
		x = j - 1;
		y = i - 2;
		if ((x >= 0 && y >= 0) && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		break;

	case R_CAR:  //�쳵
	case B_CAR:  //�ڳ� 
				 //����ҷ��Ƿ�����/����
		x = j + 1;
		y = i;
		while (x < 9)
		{
			if (NOCHESS == position[y][x])  //����
				AddPoint(x, y);
			else
			{  //����
				AddPoint(x, y);
				break;  //���治������
			}
			x++;
		}
		//������Ƿ�����/����
		x = j - 1;
		y = i;
		while (x >= 0)
		{
			if (NOCHESS == position[y][x])  //����
				AddPoint(x, y);
			else
			{  //����
				AddPoint(x, y);
				break;  //���治������
			}
			x--;
		}
		//����·��Ƿ�����/����
		x = j;
		y = i + 1;//
		while (y < 10)
		{
			if (NOCHESS == position[y][x])  //����
				AddPoint(x, y);
			else
			{  //����
				AddPoint(x, y);
				break;  //���治������
			}
			y++;
		}
		//����Ϸ��Ƿ�����/����
		x = j;
		y = i - 1;//�W��
		while (y >= 0)
		{
			if (NOCHESS == position[y][x])  //����
				AddPoint(x, y);
			else
			{  //����
				AddPoint(x, y);
				break;  //���治������
			}
			y--;
		}
		break;

	case R_PAWN:  //���
				  //�鿴��ǰ�������Ƿ񵽵�
		y = i - 1;
		x = j;

		if (y >= 0)
			AddPoint(x, y);  //û���ף�����

		if (i < 5)
		{  //�ѹ���
			y = i;
			x = j + 1;  //����
			if (x < 9)
				AddPoint(x, y);  //δ���ң�����
			x = j - 1;  //����
			if (x >= 0)
				AddPoint(x, y);  //δ���󣬿���
		}
		break;

	case B_PAWN:  //�ڱ�
				  //�鿴��ǰ�������Ƿ񵽵�
		y = i + 1;
		x = j;

		if (y < 10)
			AddPoint(x, y);  //û���ף�����

		if (i > 4)
		{  //�ѹ���
			y = i;
			x = j + 1;  //����
			if (x < 9)
				AddPoint(x, y);  //δ���ң�����
			x = j - 1;  //����
			if (x >= 0)
				AddPoint(x, y);  //δ���󣬿���
		}
		break;

	case B_CANON:  //����
	case R_CANON:  //����
				   //����ҷ��Ƿ�����/����
		x = j + 1;
		y = i;
		flag = FALSE;
		while (x < 9)
		{
			if (NOCHESS == position[y][x])
			{  //����
				if (!flag)
					AddPoint(x, y);
			}
			else
			{  //����
				if (!flag)
					flag = TRUE;  //�ǵ�1������
				else
				{  //�ǵ�2������
					AddPoint(x, y);
					break;
				}
			}
			x++;  //��������
		}
		//������Ƿ�����/����
		x = j - 1;
		flag = FALSE;
		while (x >= 0)
		{
			if (NOCHESS == position[y][x])
			{  //����
				if (!flag)
					AddPoint(x, y);
			}
			else
			{  //����
				if (!flag)
					flag = TRUE;  //�ǵ�1������
				else
				{  //�ǵ�2������
					AddPoint(x, y);
					break;
				}
			}
			x--;  //��������
		}
		//����·��Ƿ�����/����
		x = j;
		y = i + 1;
		flag = FALSE;
		while (y < 10)
		{
			if (NOCHESS == position[y][x])
			{  //����
				if (!flag)
					AddPoint(x, y);
			}
			else
			{  //����
				if (!flag)
					flag = TRUE;  //�ǵ�1������
				else
				{  //�ǵ�2������
					AddPoint(x, y);
					break;
				}
			}
			y++;  //��������
		}
		//����Ϸ��Ƿ�����/����
		y = i - 1;	//�W
		flag = FALSE;
		while (y >= 0)
		{
			if (NOCHESS == position[y][x])
			{  //����
				if (!flag)
					AddPoint(x, y);
			}
			else
			{  //����
				if (!flag)
					flag = TRUE;  //�ǵ�1������
				else
				{  //�ǵ�2������
					AddPoint(x, y);
					break;
				}
			}
			y--;  //��������
		}
		break;

	default:
		break;

	}
	return nPosCount;
}

BOOL CEveluation::CanTouch(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY)  //���ж�positionλ��From�������ܷ��ߵ�λ��To,�ܣ�True,��False��
{
	int i, j;
	int nMoveChessID, nTargetID;

	if (nFromY == nToY && nFromX == nToX)
		return FALSE;//Ŀ����Դ��ͬ

	nMoveChessID = position[nFromY][nFromX];
	nTargetID = position[nToY][nToX];

	switch (nMoveChessID)
	{
	case B_KING:  //�ڽ�
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
	case R_BISHOP:  //��ʿ

		if (nToY < 7 || nToX > 5 || nToX < 3)
			return FALSE;//ʿ���Ź�	

		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//ʿ��б��

		break;

	case B_BISHOP:   //��ʿ

		if (nToY > 2 || nToX > 5 || nToX < 3)
			return FALSE;//ʿ���Ź�	

		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//ʿ��б��

		break;

	case R_ELEPHANT://����

		if (nToY < 5)
			return FALSE;//�಻�ܹ���

		if (abs(nFromX - nToX) != 2 || abs(nFromY - nToY) != 2)
			return FALSE;//��������

		if (position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//���۱���ס��

		break;

	case B_ELEPHANT://���� 

		if (nToY > 4)
			return FALSE;//�಻�ܹ���

		if (abs(nFromX - nToX) != 2 || abs(nFromY - nToY) != 2)
			return FALSE;//��������

		if (position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//���۱���ס��

		break;

	case B_PAWN:     //�ڱ�

		if (nToY < nFromY)
			return FALSE;//������ͷ

		if (nFromY < 5 && nFromY == nToY)
			return FALSE;//������ǰֻ��ֱ��

		if (nToY - nFromY + abs(nToX - nFromX) > 1)
			return FALSE;//��ֻ��һ��ֱ��:

		break;

	case R_PAWN:    //���

		if (nToY > nFromY)
			return FALSE;//������ͷ

		if (nFromY > 4 && nFromY == nToY)
			return FALSE;//������ǰֻ��ֱ��

		if (nFromY - nToY + abs(nToX - nFromX) > 1)
			return FALSE;//��ֻ��һ��ֱ��:

		break;

	case R_KING:  //�콫
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

	case B_CAR:  //�ڳ�
	case R_CAR:  //�쳵

		if (nFromY != nToY && nFromX != nToX)
			return FALSE;	//����ֱ��:

		if (nFromY == nToY)
		{
			if (nFromX < nToX)
			{  //����
				for (i = nFromX + 1; i < nToX; i++)
					if (position[nFromY][i] != NOCHESS)
						return FALSE;
			}
			else
			{  //����
				for (i = nToX + 1; i < nFromX; i++)
					if (position[nFromY][i] != NOCHESS)
						return FALSE;
			}
		}
		else
		{
			if (nFromY < nToY)
			{  //����
				for (j = nFromY + 1; j < nToY; j++)
					if (position[j][nFromX] != NOCHESS)
						return FALSE;
			}
			else
			{  //����
				for (j = nToY + 1; j < nFromY; j++)
					if (position[j][nFromX] != NOCHESS)
						return FALSE;
			}
		}

		break;

	case B_HORSE:  //����
	case R_HORSE:  //����

		if (!((abs(nToX - nFromX) == 1 && abs(nToY - nFromY) == 2)
			|| (abs(nToX - nFromX) == 2 && abs(nToY - nFromY) == 1)))
			return FALSE;//��������

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
			return FALSE;//������

		break;
	case B_CANON:  //����
	case R_CANON:  //����

		if (nFromY != nToY && nFromX != nToX)
			return FALSE;	//����ֱ��:

							//�ڲ�����ʱ������·���в���������:------------------

		if (position[nToY][nToX] == NOCHESS)
		{
			if (nFromY == nToY)
			{
				if (nFromX < nToX)
				{  //����
					for (i = nFromX + 1; i < nToX; i++)
						if (position[nFromY][i] != NOCHESS)
							return FALSE;
				}
				else
				{  //����
					for (i = nToX + 1; i < nFromX; i++)
						if (position[nFromY][i] != NOCHESS)
							return FALSE;
				}
			}
			else
			{
				if (nFromY < nToY)
				{  //����
					for (j = nFromY + 1; j < nToY; j++)
						if (position[j][nFromX] != NOCHESS)
							return FALSE;
				}
				else
				{  //����
					for (j = nToY + 1; j < nFromY; j++)
						if (position[j][nFromX] != NOCHESS)
							return FALSE;
				}
			}
		}
		//�������ڲ�����-------------------------------------
		//����ʱ:=======================================
		else
		{
			int nCount = 0;
			if (nFromY == nToY)
			{
				if (nFromX < nToX)
				{  //����
					for (i = nFromX + 1;i<nToX;i++)
						if (position[nFromY][i] != NOCHESS)
							nCount++;
					if (nCount != 1)  //���Ǹ�һ����
						return FALSE;
				}
				else
				{  //����
					for (i = nToX + 1;i<nFromX;i++)
						if (position[nFromY][i] != NOCHESS)
							nCount++;
					if (nCount != 1)  //���Ǹ�һ����
						return FALSE;
				}
			}
			else
			{
				if (nFromY<nToY)
				{  //����
					for (j = nFromY + 1;j<nToY;j++)
						if (position[j][nFromX] != NOCHESS)
							nCount++;
					if (nCount != 1)  //���Ǹ�һ����
						return FALSE;
				}
				else
				{  //����
					for (j = nToY + 1;j<nFromY;j++)
						if (position[j][nFromX] != NOCHESS)
							nCount++;
					if (nCount != 1)  //���Ǹ�һ����
						return FALSE;
				}
			}
		}
		//�������ڳ���ʱ================================
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

