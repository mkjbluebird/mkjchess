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
	InitializeHashKey();  //建立哈希表，创建随机数组
}


CTranspositionTable::~CTranspositionTable()
{
	//释放哈希表所用空间
	delete m_pTT[0];
	delete m_pTT[1];
}


void CTranspositionTable::InitializeHashKey()
{
	int i, j, k;

	srand((unsigned)time(NULL));  //设定随机数种子

	for (i = 0; i < 15; i++)  //填充随机数组
		for (j = 0; j < 10; j++)
			for (k = 0; k < 9; k++)
			{
				m_nHashKey32[i][j][k] = rand32();
				m_ulHashKey64[i][j][k] = rand64();
			}
	//申请置换表所用空间，1M*2个条目，读者也可指定其他大小
	m_pTT[0] = new HashItem[1024 * 1024];  //用于存放取极大值的节点数据
	m_pTT[1] = new HashItem[1024 * 1024];  //用于存放取极小值的节点数据
}


//根据传入的棋盘计算出32位及64位哈希值
//CurPostion要计算哈希值的棋盘
void CTranspositionTable::CalculateInitHashKey(BYTE CurPosition[10][9])
{
	int j, k, nChessType;
	m_HashKey32 = 0;
	m_HashKey64 = 0;
	//将所有棋子对应的随机数加总
	for (j = 0; j < 10; j++)
		for (k = 0; k < 9; k++)
		{
			nChessType = CurPosition[j][k];  //取棋子
			if (nChessType != NOCHESS)  //如果不是无子
			{
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nChessType][j][k];  //计算棋子的32位哈希值
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nChessType][j][k];  //计算棋子的64位哈希值
			}
		}
}


//根据传入的走法，修改相应的哈希值为走过以后的
//move是要进行的走法
//CurPosition是当前棋盘
void CTranspositionTable::Hash_MakeMove(CHESSMOVE* move, BYTE CurPosition[10][9])
{
	BYTE nToID, nFromID;
	nFromID = CurPosition[move->From.y][move->From.x];  //取未走之前位置的棋子类型
	nToID = CurPosition[move->To.y][move->To.x];  //取落子位置的棋子类型
	//还原要移动棋子的哈希值
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nFromID][move->From.y][move->From.x];
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nFromID][move->From.y][move->From.x];

	if (nToID != NOCHESS)
	{  //如果目标位置有棋子，也要还原棋子的哈希值
		m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nToID][move->To.y][move->To.x];
		m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nToID][move->To.y][move->To.x];
	}
	//重新合成落子后的哈希值
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nFromID][move->To.y][move->To.x];
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nFromID][move->To.y][move->To.x];
}


//与Hash_MakeMove相反，恢复Hash_MakeMove改变的哈希值
//move是要取消的落子
//nChessID是目标位置的棋子类型
//CurPosition是当前棋盘
void CTranspositionTable::Hash_UnMakeMove(CHESSMOVE* move, BYTE nChessID, BYTE CurPosition[10][9])
{
	BYTE nToID;
	nToID = CurPosition[move->To.y][move->To.x];  //落子后的棋子类型
	//恢复移动前的哈希值
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nToID][move->From.y][move->From.x];
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nToID][move->From.y][move->From.x];
	//删除移动后的哈希值
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nToID][move->To.y][move->To.x];
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nToID][move->To.y][move->To.x];

	if (nChessID)
	{  //恢复被吃掉的棋子的哈希值
		m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nChessID][move->To.y][move->To.x];
		m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nChessID][move->To.y][move->To.x];
	}
}

//查找哈希表
//alpha是alpha-beta搜索上边界
//beta是alpha-beta搜索下边界
//depth是当前搜索层次
//TableNo表明是奇数还是偶数层的标志
int CTranspositionTable::LookUpHashTable(int alpha, int beta, int depth, int TableNo)
{
	int x;
	HashItem * pht;
	//计算二十位哈希地址，如果读者设定的哈希表大小不是1M*2,而是TableSize*2,TableSize为读者设定的大小
	//则需要修改这一句为m_HashKey32%TableSize,下一个函数也一样
	x = m_HashKey32 & 0xFFFFF;
	pht = &m_pTT[TableNo][x];  //取具体表项指针

	if (pht->depth >= depth && pht->checksum == m_HashKey64)
	{
		switch (pht->entry_type)  //判断数据类型
		{
		case exact:  //确切值
			return pht->eval;
		case lower_bound:  //下边界
			if (pht->eval >= beta)
				return (pht->eval);
			else
				break;
		case upper_bound:  //上边界
			if (pht->eval <= alpha)
				return (pht->eval);
			else
				break;
		}
	}

	return 66666;  //没有命中，返回无效标志
}

//向置换表中插入数据，ENTRY_TYPE entry_type是数据类型，eval是要记录的数据值，depth是取得该值的层次
//TableNo表明是奇数还是偶数层的标志
void CTranspositionTable::EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth, int TableNo)
{
	int x;
	HashItem * pht;

	x = m_HashKey32 & 0xFFFFF;//二十位哈希地址
	pht = &m_pTT[TableNo][x];  //取到具体的表项指针
	//将数据写入哈希表
	pht->checksum = m_HashKey64;  //64位校验码
	pht->entry_type = entry_type;  //表项类型
	pht->eval = eval;  //要保存的值
	pht->depth = depth;  //层次

}
