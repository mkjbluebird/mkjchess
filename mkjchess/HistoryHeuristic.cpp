#include "stdafx.h"
#include "mkjchess.h"
#include "HistoryHeuristic.h"


CHistoryHeuristic::CHistoryHeuristic()
{
}


CHistoryHeuristic::~CHistoryHeuristic()
{
}


void CHistoryHeuristic::ResetHistoryTable()
{
	memset(m_HistoryTable, 10, 8100 * 4);  //清零历史记录表
}


int CHistoryHeuristic::GetHistoryScore(CHESSMOVE *move)  //取给定落子历史得分
{
	int nFrom, nTo;

	nFrom = move->From.y * 9 + move->From.x;  //起始位置
	nTo = move->To.y * 9 + move->To.x;  //目标位置

	return m_HistoryTable[nFrom][nTo];  //返回历史得分
}


void CHistoryHeuristic::EnterHistoryScore(CHESSMOVE *move, int depth)  //将最佳落子记入历史记录
{
	int nFrom, nTo;
	nFrom = move->From.y * 9 + move->From.x;  //原始位置
	nTo = move->To.y * 9 + move->To.x;  //目标位置

	m_HistoryTable[nFrom][nTo] += 2 << depth;  //增量为2的depth次方
}

//CHESSMOVE* source原始队列
//CHESSMOVE* target目标队列
//合并source[l...m]和source[m+1...r]至target[l...r]
void CHistoryHeuristic::Merge(CHESSMOVE *source, CHESSMOVE *target, int l, int m, int r)
{//从小到大排序
	int i = l;  //源队列游标
	int j = m + 1;  //源队列游标
	int k = l;  //目标队列游标

	while ((i <= m) && (j <= r))
		if (source[i].Score <= source[j].Score)  //排序，m中点，r结尾
			target[k++] = source[i++];
		else
			target[k++] = source[j++];

	if (i > m)
		for (int q = j; q <= r; q++)  //将中点以后的项拷入队列
			target[k++] = source[q];
	else
		for (int q = i; q <= m; q++)  //将中点以前的剩余项拷入队列
			target[k++] = source[q];
}

//CHESSMOVE* source原始队列
//CHESSMOVE* target目标队列
//合并source[l...m]和source[m+1...r]至target[l...r]
void CHistoryHeuristic::Merge_A(CHESSMOVE *source, CHESSMOVE *target, int l, int m, int r)
{//从大到小排序
	int i = l;  //源队列游标
	int j = m + 1;  //源队列游标
	int k = l;  //目标队列游标

	while ((i <= m) && (j <= r))
		if (source[i].Score >= source[j].Score)  //排序，m中点，r结尾
			target[k++] = source[i++];
		else
			target[k++] = source[j++];

	if (i > m)
		for (int q = j; q <= r; q++)  //将中点以后的项拷入队列
			target[k++] = source[q];
	else
		for (int q = i; q <= m; q++)  //将中点以前的剩余项拷入队列
			target[k++] = source[q];
}

//合并大小为S的相邻子数组
//n是数组项数
//direction是标志，指明是从大到小还是从小到大排序
void CHistoryHeuristic::MergePass(CHESSMOVE *source, CHESSMOVE *target, const  int s, const  int n, const BOOL direction)
{
	int i = 0;

	while (i <= n - 2 * s)
	{
		//合并大小为s的相邻二段子数组
		if (direction)
			Merge(source, target, i, i + s - 1, i + 2 * s - 1);  //从小到大排序
		else
			Merge_A(source, target, i, i + s - 1, i + 2 * s - 1);  //从大到小排序
		i = i + 2 * s;
	}

	if (i + s < n) //剩余的元素个数小於2s
	{
		if (direction)
			Merge(source, target, i, i + s - 1, n - 1);  //从小到大排序
		else
			Merge_A(source, target, i, i + s - 1, n - 1);   //从大到小排序
	}
	else
		for (int j = i; j <= n - 1; j++)
			target[j] = source[j];
}

//对走法队列归并排序函数
//这是被外部调用的归并排序函数
//source是待排序数组
//n是数组项数
//direction是标志，指明是从大到小还是从小到大排序
void CHistoryHeuristic::MergeSort(CHESSMOVE *source, int n, BOOL direction)
{
	int s = 1;
	while (s < n)
	{
		MergePass(source, m_TargetBuff, s, n, direction);
		s += s;
		MergePass(m_TargetBuff, source, s, n, direction);
		s += s;
	}
}
