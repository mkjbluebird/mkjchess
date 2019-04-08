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
	memset(m_HistoryTable, 10, 8100 * 4);  //������ʷ��¼��
}


int CHistoryHeuristic::GetHistoryScore(CHESSMOVE *move)  //ȡ����������ʷ�÷�
{
	int nFrom, nTo;

	nFrom = move->From.y * 9 + move->From.x;  //��ʼλ��
	nTo = move->To.y * 9 + move->To.x;  //Ŀ��λ��

	return m_HistoryTable[nFrom][nTo];  //������ʷ�÷�
}


void CHistoryHeuristic::EnterHistoryScore(CHESSMOVE *move, int depth)  //��������Ӽ�����ʷ��¼
{
	int nFrom, nTo;
	nFrom = move->From.y * 9 + move->From.x;  //ԭʼλ��
	nTo = move->To.y * 9 + move->To.x;  //Ŀ��λ��

	m_HistoryTable[nFrom][nTo] += 2 << depth;  //����Ϊ2��depth�η�
}

//CHESSMOVE* sourceԭʼ����
//CHESSMOVE* targetĿ�����
//�ϲ�source[l...m]��source[m+1...r]��target[l...r]
void CHistoryHeuristic::Merge(CHESSMOVE *source, CHESSMOVE *target, int l, int m, int r)
{//��С��������
	int i = l;  //Դ�����α�
	int j = m + 1;  //Դ�����α�
	int k = l;  //Ŀ������α�

	while ((i <= m) && (j <= r))
		if (source[i].Score <= source[j].Score)  //����m�е㣬r��β
			target[k++] = source[i++];
		else
			target[k++] = source[j++];

	if (i > m)
		for (int q = j; q <= r; q++)  //���е��Ժ��������
			target[k++] = source[q];
	else
		for (int q = i; q <= m; q++)  //���е���ǰ��ʣ��������
			target[k++] = source[q];
}

//CHESSMOVE* sourceԭʼ����
//CHESSMOVE* targetĿ�����
//�ϲ�source[l...m]��source[m+1...r]��target[l...r]
void CHistoryHeuristic::Merge_A(CHESSMOVE *source, CHESSMOVE *target, int l, int m, int r)
{//�Ӵ�С����
	int i = l;  //Դ�����α�
	int j = m + 1;  //Դ�����α�
	int k = l;  //Ŀ������α�

	while ((i <= m) && (j <= r))
		if (source[i].Score >= source[j].Score)  //����m�е㣬r��β
			target[k++] = source[i++];
		else
			target[k++] = source[j++];

	if (i > m)
		for (int q = j; q <= r; q++)  //���е��Ժ��������
			target[k++] = source[q];
	else
		for (int q = i; q <= m; q++)  //���е���ǰ��ʣ��������
			target[k++] = source[q];
}

//�ϲ���СΪS������������
//n����������
//direction�Ǳ�־��ָ���ǴӴ�С���Ǵ�С��������
void CHistoryHeuristic::MergePass(CHESSMOVE *source, CHESSMOVE *target, const  int s, const  int n, const BOOL direction)
{
	int i = 0;

	while (i <= n - 2 * s)
	{
		//�ϲ���СΪs�����ڶ���������
		if (direction)
			Merge(source, target, i, i + s - 1, i + 2 * s - 1);  //��С��������
		else
			Merge_A(source, target, i, i + s - 1, i + 2 * s - 1);  //�Ӵ�С����
		i = i + 2 * s;
	}

	if (i + s < n) //ʣ���Ԫ�ظ���С�2s
	{
		if (direction)
			Merge(source, target, i, i + s - 1, n - 1);  //��С��������
		else
			Merge_A(source, target, i, i + s - 1, n - 1);   //�Ӵ�С����
	}
	else
		for (int j = i; j <= n - 1; j++)
			target[j] = source[j];
}

//���߷����й鲢������
//���Ǳ��ⲿ���õĹ鲢������
//source�Ǵ���������
//n����������
//direction�Ǳ�־��ָ���ǴӴ�С���Ǵ�С��������
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
