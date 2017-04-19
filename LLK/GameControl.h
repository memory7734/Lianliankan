#pragma once
#include"global.h"
#include"GameLogic.h"
class CGameControl
{
	int s_nRows;		//地图行数
	int s_nCols;		//地图列数
	int s_nPicNum;		//地图图片数

	int** m_pGameMap;	//地图指针
	Vertex m_svSelFst;	//选中的第一个点
	Vertex m_svSelSec;	//选中的第二个点
public:
	
	CGameControl();
	~CGameControl();

	void StartGame();	//开始游戏
	int GetElement(int nRow, int nCol);			//获得(nRow,nCol)的元素
	bool Link(Vertex avPath[4], int &nVexnum);	//消子判断
	void SetFirstPoint(int nRow, int nCol);		//设置第一个定点
	void SetSecPoint(int nRow, int nCol);		//设置第二个顶点
	bool IsWin(int nTime);						//查询是否获胜
	bool Help(Vertex &v1, Vertex &v2);			//获得提示
	void ResetMap();							//重置地图

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

