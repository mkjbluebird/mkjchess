#include "stdafx.h"
#include "mkjchess.h"
#include "TranspositionTable.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
LONGLONG rand64(void)
{

	return rand() ^ ((LONGLONG)rand() << 15) ^ ((LONGLONG)rand() << 30) ^

		((LONGLONG)rand() << 45) ^ ((LONGLONG)rand() << 60);

}
LONG rand32(void)
{

	return rand() ^ ((LONG)rand() << 15) ^ ((LONG)rand() << 30);

}


CTranspositionTable::CTranspositionTable()
{
	InitializeHashKey();  //������ϣ�������������
}


CTranspositionTable::~CTranspositionTable()
{
	//�ͷŹ�ϣ�����ÿռ�
	delete m_pTT[0];
	delete m_pTT[1];
}


void CTranspositionTable::InitializeHashKey()
{
	int i, j, k;

	srand((unsigned)time(NULL));  //�趨���������

	for (i = 0; i < 15; i++)  //����������
		for (j = 0; j < 10; j++)
			for (k = 0; k < 9; k++)
			{
				m_nHashKey32[i][j][k] = rand32();
				m_ulHashKey64[i][j][k] = rand64();
			}
	//�����û������ÿռ䣬1M*2����Ŀ������Ҳ��ָ��������С
	m_pTT[0] = new HashItem[1024 * 1024];  //���ڴ��ȡ����ֵ�Ľڵ�����
	m_pTT[1] = new HashItem[1024 * 1024];  //���ڴ��ȡ��Сֵ�Ľڵ�����
}


//���ݴ�������̼����32λ��64λ��ϣֵ
//CurPostionҪ�����ϣֵ������
void CTranspositionTable::CalculateInitHashKey(BYTE CurPosition[10][9])
{
	int j, k, nChessType;
	m_HashKey32 = 0;
	m_HashKey64 = 0;
	//���������Ӷ�Ӧ�����������
	for (j = 0; j < 10; j++)
		for (k = 0; k < 9; k++)
		{
			nChessType = CurPosition[j][k];  //ȡ����
			if (nChessType != NOCHESS)  //�����������
			{
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nChessType][j][k];  //�������ӵ�32λ��ϣֵ
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nChessType][j][k];  //�������ӵ�64λ��ϣֵ
			}
		}
}


//���ݴ�����߷����޸���Ӧ�Ĺ�ϣֵΪ�߹��Ժ��
//move��Ҫ���е��߷�
//CurPosition�ǵ�ǰ����
void CTranspositionTable::Hash_MakeMove(CHESSMOVE* move, BYTE CurPosition[10][9])
{
	BYTE nToID, nFromID;
	nFromID = CurPosition[move->From.y][move->From.x];  //ȡδ��֮ǰλ�õ���������
	nToID = CurPosition[move->To.y][move->To.x];  //ȡ����λ�õ���������
	//��ԭҪ�ƶ����ӵĹ�ϣֵ
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nFromID][move->From.y][move->From.x];
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nFromID][move->From.y][move->From.x];

	if (nToID != NOCHESS)
	{  //���Ŀ��λ�������ӣ�ҲҪ��ԭ���ӵĹ�ϣֵ
		m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nToID][move->To.y][move->To.x];
		m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nToID][move->To.y][move->To.x];
	}
	//���ºϳ����Ӻ�Ĺ�ϣֵ
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nFromID][move->To.y][move->To.x];
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nFromID][move->To.y][move->To.x];
}


//��Hash_MakeMove�෴���ָ�Hash_MakeMove�ı�Ĺ�ϣֵ
//move��Ҫȡ��������
//nChessID��Ŀ��λ�õ���������
//CurPosition�ǵ�ǰ����
void CTranspositionTable::Hash_UnMakeMove(CHESSMOVE* move, BYTE nChessID, BYTE CurPosition[10][9])
{
	BYTE nToID;
	nToID = CurPosition[move->To.y][move->To.x];  //���Ӻ����������
	//�ָ��ƶ�ǰ�Ĺ�ϣֵ
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nToID][move->From.y][move->From.x];
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nToID][move->From.y][move->From.x];
	//ɾ���ƶ���Ĺ�ϣֵ
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nToID][move->To.y][move->To.x];
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nToID][move->To.y][move->To.x];

	if (nChessID)
	{  //�ָ����Ե������ӵĹ�ϣֵ
		m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nChessID][move->To.y][move->To.x];
		m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nChessID][move->To.y][move->To.x];
	}
}

//���ҹ�ϣ��
//alpha��alpha-beta�����ϱ߽�
//beta��alpha-beta�����±߽�
//depth�ǵ�ǰ�������
//TableNo��������������ż����ı�־
int CTranspositionTable::LookUpHashTable(int alpha, int beta, int depth, int TableNo)
{
	int x;
	HashItem * pht;
	//�����ʮλ��ϣ��ַ����������趨�Ĺ�ϣ���С����1M*2,����TableSize*2,TableSizeΪ�����趨�Ĵ�С
	//����Ҫ�޸���һ��Ϊm_HashKey32%TableSize,��һ������Ҳһ��
	x = m_HashKey32 & 0xFFFFF;
	pht = &m_pTT[TableNo][x];  //ȡ�������ָ��

	if (pht->depth >= depth && pht->checksum == m_HashKey64)
	{
		switch (pht->entry_type)  //�ж���������
		{
		case exact:  //ȷ��ֵ
			return pht->eval;
		case lower_bound:  //�±߽�
			if (pht->eval >= beta)
				return (pht->eval);
			else
				break;
		case upper_bound:  //�ϱ߽�
			if (pht->eval <= alpha)
				return (pht->eval);
			else
				break;
		}
	}

	return 66666;  //û�����У�������Ч��־
}

//���û����в������ݣ�ENTRY_TYPE entry_type���������ͣ�eval��Ҫ��¼������ֵ��depth��ȡ�ø�ֵ�Ĳ��
//TableNo��������������ż����ı�־
void CTranspositionTable::EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth, int TableNo)
{
	int x;
	HashItem * pht;

	x = m_HashKey32 & 0xFFFFF;//��ʮλ��ϣ��ַ
	pht = &m_pTT[TableNo][x];  //ȡ������ı���ָ��
	//������д���ϣ��
	pht->checksum = m_HashKey64;  //64λУ����
	pht->entry_type = entry_type;  //��������
	pht->eval = eval;  //Ҫ�����ֵ
	pht->depth = depth;  //���

}
