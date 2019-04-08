#pragma once
class CHistoryHeuristic
{
public:
	CHistoryHeuristic();
	virtual ~CHistoryHeuristic();

	void ResetHistoryTable();  //����ʷ��¼�����
	int GetHistoryScore(CHESSMOVE *move);  //ȡĳһ���ӵ���ʷ�÷�
	void EnterHistoryScore(CHESSMOVE *move, int depth);  //��ĳһ����߷�������ʷ��¼��
	void MergeSort(CHESSMOVE *source, int n, BOOL direction);  //�Ե�ǰ�߷����н��й鲢����
protected:
	//��С������������MergePass����
	void Merge(CHESSMOVE *source, CHESSMOVE *target, int l, int m, int r);
	//���ں�ƴ����õ��������ݶΣ�MergeSort����
	void MergePass(CHESSMOVE *source, CHESSMOVE *target, const  int s, const  int n, const BOOL direction);
	//�Ӵ�С��������MergePass����
	void Merge_A(CHESSMOVE *source, CHESSMOVE *target, int l, int m, int r);
	int m_HistoryTable[90][90];  //��ʷ�÷ֱ�
	CHESSMOVE m_TargetBuff[100];  //����û������

};

