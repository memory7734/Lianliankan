#pragma once
#include"global.h"
#include"GameControl.h"
class CGameLogic
{
	Vertex m_avPath[4];		//保存路径最多保存四个拐点
	int m_nVexNum;			//保存路径中的拐点数

public:
	CGameLogic();			//构造函数
	~CGameLogic();			//析构函数
	int** InitMap();					//初始化地图
	void ResetMap(int **pGameMap);		//重置地图
	bool IsBlank(int **pGameMap);		//判断地图是否为空
	void ReleaseMap(int** &pGameMap);	//释放地图
	int GetVexPath(Vertex avPath[4]);	//获得当前路径

	bool IsLink(int** pGameMap, Vertex v1, Vertex v2);				//判断v1和v2之间是否可以连通
	void Clear(int** pGameMap, Vertex v1, Vertex v2);				//将v1和v2设置为空
	bool SearchValidPath(int **pGameMap, Vertex &v1, Vertex &v2);	//搜索地图找连通点

protected:
	void PushVertex(Vertex V);	//将v存入路径
	void PopVertex();			//删除最后一个点
	void ClearStack();			//清空当前路径

	bool LineX(int** pGameMap, int nRow, int nCol1, int nCol2);		//判断(nRow1, nCol1)到(nRow1, nCol2)能否连通
	bool LineY(int** pGameMap, int nRow1, int nRow2, int nCol);		//判断(nRow1, nCol2)到(nRow2, nCol2)能否连通
	bool LinkInRow(int** pGameMap, Vertex v1, Vertex v2);			//判断两点之间在x轴上是否连通
	bool LinkInCol(int** pGameMap, Vertex v1, Vertex v2);			//判断两点之间在y轴上是否连通
	bool OneCornerLink(int** pGameMap, Vertex v1, Vertex v2);		//判断两点之间一个拐角是否连通
	bool TwoCornerLink(int** pGameMap, Vertex v1, Vertex v2);		//判断两点之间两个拐角是否连通

};

