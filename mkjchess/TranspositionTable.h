#pragma once
enum ENTRY_TYPE { exact, lower_bound, upper_bound };  //ö�����͵��������ͣ���ȷ���±߽磬�ϱ߽�

typedef struct HASHITEM {  //��ϣ���е�Ԫ�صĽṹ����
	LONGLONG checksum;	//64λУ����
	ENTRY_TYPE  entry_type;  //��������
	short depth;  //ȡ�����ֵ�Ĳ��
	short eval;  //�ڵ��ֵ
}HashItem;
class CTranspositionTable  //�û�����
{
public:
	CTranspositionTable();
	virtual ~CTranspositionTable();

	void CalculateInitHashKey(BYTE CurPosition[10][9]);  //����������̵Ĺ�ϣֵ
	void Hash_MakeMove(CHESSMOVE* move, BYTE CurPosition[10][9]);  //�������ӣ����������µĹ�ϣֵ
	void Hash_UnMakeMove(CHESSMOVE* move, BYTE nChessID, BYTE CurPosition[10][9]);  //�ָ����棬��ԭ��ϣֵ

	int LookUpHashTable(int alpha, int beta, int depth, int TableNo);  //�ڹ�ϣ���в�ѯ��ǰ�ڵ�Ĺ�ϣֵ
	void EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth, int TableNo);  //����ǰ�ڵ��ϣֵ�����ϣ��

	UINT m_nHashKey32[15][10][9];  //32λ������飬��������32λ��ϣֵ
	ULONGLONG m_ulHashKey64[15][10][9];  //64λ������飬��������64λ��ϣֵ
	HashItem *m_pTT[10];  //�û���ͷָ��
	void InitializeHashKey();  //��ʼ��������飬������ϣ��
	UINT m_HashKey32;  //32λ��ϣֵ
	LONGLONG m_HashKey64;  //64λ��ϣֵ
};

