#pragma once
class CHistoryHeuristic
{
public:
	CHistoryHeuristic();
	virtual ~CHistoryHeuristic();

	void ResetHistoryTable();  //将历史记录表清空
	int GetHistoryScore(CHESSMOVE *move);  //取某一落子的历史得分
	void EnterHistoryScore(CHESSMOVE *move, int depth);  //将某一最佳走法汇入历史记录表
	void MergeSort(CHESSMOVE *source, int n, BOOL direction);  //对当前走法队列进行归并排序
protected:
	//从小到大排序，用于MergePass调用
	void Merge(CHESSMOVE *source, CHESSMOVE *target, int l, int m, int r);
	//用于合拼排序好的相邻数据段，MergeSort调用
	void MergePass(CHESSMOVE *source, CHESSMOVE *target, const  int s, const  int n, const BOOL direction);
	//从大到小排序，用于MergePass调用
	void Merge_A(CHESSMOVE *source, CHESSMOVE *target, int l, int m, int r);
	int m_HistoryTable[90][90];  //历史得分表
	CHESSMOVE m_TargetBuff[100];  //排序得缓冲队列

};

