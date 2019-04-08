#pragma once
//���������Ӽ�ֵ��
#define BASEVALUE_PAWN 100
#define BASEVALUE_BISHOP 250
#define BASEVALUE_ELEPHANT 250
#define BASEVALUE_CAR 500
#define BASEVALUE_HORSE 350
#define BASEVALUE_CANON 350
#define BASEVALUE_KING 10000
//��������������Լ�ֵ��
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

	virtual int Eveluate(BYTE position[10][9], BOOL bIsRedTurn);  //����ֵ����������ִ�֣�bIsRedTurn�����ֵ�˭�ߡ�
protected:
	int GetRelatePiece(BYTE position[10][9], int j, int i);  //�о�ָ��λ�����ӵ��������
	BOOL CanTouch(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY);  //�ж�Fromλ�õ������ܷ��ߵ�λ��To
	void AddPoint(int x, int y);  //��һ��λ�üӵ����λ�õĶ���
								  //int GetBingValue(int x, int y, BYTE CurSituation[][9]);  //
	int m_BaseValue[15];  //������ӻ�����ֵ������
	int m_FlexValue[15];  //�����������Է���������
	short m_AttackPos[10][9];  //���ÿһλ�ñ���в����Ϣ
	BYTE m_GuardPos[10][9];  //���ÿһλ�ñ���������Ϣ
	BYTE m_FlexibilityPos[10][9];  //���ÿһλ���ϵ����ӵ�����Է���
	int m_chessValue[10][9];  //���ÿһλ���ϵ����ӵ��ܼ�ֵ
	int nPosCount;  //��¼һ���ӵ����λ�ø���
	POINT RelatePos[20];  //��¼һ���������λ�õ�����
};

