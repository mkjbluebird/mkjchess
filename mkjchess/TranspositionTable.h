#pragma once
enum ENTRY_TYPE { exact, lower_bound, upper_bound };  //枚举类型的数据类型：精确，下边界，上边界

typedef struct HASHITEM {  //哈希表中的元素的结构定义
	LONGLONG checksum;	//64位校验码
	ENTRY_TYPE  entry_type;  //数据类型
	short depth;  //取得这个值的层次
	short eval;  //节点的值
}HashItem;
class CTranspositionTable  //置换表类
{
public:
	CTranspositionTable();
	virtual ~CTranspositionTable();

	void CalculateInitHashKey(BYTE CurPosition[10][9]);  //计算给定棋盘的哈希值
	void Hash_MakeMove(CHESSMOVE* move, BYTE CurPosition[10][9]);  //根据落子，增量生成新的哈希值
	void Hash_UnMakeMove(CHESSMOVE* move, BYTE nChessID, BYTE CurPosition[10][9]);  //恢复局面，还原哈希值

	int LookUpHashTable(int alpha, int beta, int depth, int TableNo);  //在哈希表中查询当前节点的哈希值
	void EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth, int TableNo);  //将当前节点哈希值存入哈希表

	UINT m_nHashKey32[15][10][9];  //32位随机树组，用以生成32位哈希值
	ULONGLONG m_ulHashKey64[15][10][9];  //64位随机树组，用以生成64位哈希值
	HashItem *m_pTT[10];  //置换表头指针
	void InitializeHashKey();  //初始化随机数组，创建哈希表
	UINT m_HashKey32;  //32位哈希值
	LONGLONG m_HashKey64;  //64位哈希值
};

