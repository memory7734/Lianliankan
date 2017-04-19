#pragma once
#include"global.h"
#include"GameControl.h"
class CGameLogic
{
	Vertex m_avPath[4];		//����·����ౣ���ĸ��յ�
	int m_nVexNum;			//����·���еĹյ���

public:
	CGameLogic();			//���캯��
	~CGameLogic();			//��������
	int** InitMap();					//��ʼ����ͼ
	void ResetMap(int **pGameMap);		//���õ�ͼ
	bool IsBlank(int **pGameMap);		//�жϵ�ͼ�Ƿ�Ϊ��
	void ReleaseMap(int** &pGameMap);	//�ͷŵ�ͼ
	int GetVexPath(Vertex avPath[4]);	//��õ�ǰ·��

	bool IsLink(int** pGameMap, Vertex v1, Vertex v2);				//�ж�v1��v2֮���Ƿ������ͨ
	void Clear(int** pGameMap, Vertex v1, Vertex v2);				//��v1��v2����Ϊ��
	bool SearchValidPath(int **pGameMap, Vertex &v1, Vertex &v2);	//������ͼ����ͨ��

protected:
	void PushVertex(Vertex V);	//��v����·��
	void PopVertex();			//ɾ�����һ����
	void ClearStack();			//��յ�ǰ·��

	bool LineX(int** pGameMap, int nRow, int nCol1, int nCol2);		//�ж�(nRow1, nCol1)��(nRow1, nCol2)�ܷ���ͨ
	bool LineY(int** pGameMap, int nRow1, int nRow2, int nCol);		//�ж�(nRow1, nCol2)��(nRow2, nCol2)�ܷ���ͨ
	bool LinkInRow(int** pGameMap, Vertex v1, Vertex v2);			//�ж�����֮����x�����Ƿ���ͨ
	bool LinkInCol(int** pGameMap, Vertex v1, Vertex v2);			//�ж�����֮����y�����Ƿ���ͨ
	bool OneCornerLink(int** pGameMap, Vertex v1, Vertex v2);		//�ж�����֮��һ���ս��Ƿ���ͨ
	bool TwoCornerLink(int** pGameMap, Vertex v1, Vertex v2);		//�ж�����֮�������ս��Ƿ���ͨ

};

