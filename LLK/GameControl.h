#pragma once
#include"global.h"
#include"GameLogic.h"
class CGameControl
{
	int s_nRows;		//��ͼ����
	int s_nCols;		//��ͼ����
	int s_nPicNum;		//��ͼͼƬ��

	int** m_pGameMap;	//��ͼָ��
	Vertex m_svSelFst;	//ѡ�еĵ�һ����
	Vertex m_svSelSec;	//ѡ�еĵڶ�����
public:
	
	CGameControl();
	~CGameControl();

	void StartGame();	//��ʼ��Ϸ
	int GetElement(int nRow, int nCol);			//���(nRow,nCol)��Ԫ��
	bool Link(Vertex avPath[4], int &nVexnum);	//�����ж�
	void SetFirstPoint(int nRow, int nCol);		//���õ�һ������
	void SetSecPoint(int nRow, int nCol);		//���õڶ�������
	bool IsWin(int nTime);						//��ѯ�Ƿ��ʤ
	bool Help(Vertex &v1, Vertex &v2);			//�����ʾ
	void ResetMap();							//���õ�ͼ

	int gets_nRows()
	{
		return s_nRows;
	}
	int gets_nCols()
	{
		return s_nCols;
	}
	int gets_nPicNum()
	{
		return s_nPicNum;
	}
};

