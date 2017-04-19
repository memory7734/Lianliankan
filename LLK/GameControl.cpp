#include "stdafx.h"
#include "GameControl.h"


CGameControl::CGameControl()
{
	s_nRows = MAX_ROW;
	s_nCols = MAX_COL;
	s_nPicNum = MAX_PIC_NUM;
}


CGameControl::~CGameControl()
{
}
//调用初始化地图函数开始游戏
void CGameControl::StartGame()
{
	CGameLogic gameLogic;
	m_pGameMap= gameLogic.InitMap();
}
//获得该点的图片元素
int CGameControl::GetElement(int nRow,int nCol)
{
	return m_pGameMap[nRow][nCol];
}
//设置第一个点
void CGameControl::SetFirstPoint(int nRow, int nCol)
{
	m_svSelFst.row = nRow;
	m_svSelFst.col = nCol;
}
//设置第二个点
void CGameControl::SetSecPoint(int nRow, int nCol)
{
	m_svSelSec.row = nRow;
	m_svSelSec.col = nCol;
}
//消子判断
bool CGameControl::Link(Vertex avPath[4],int &nVexnum)
{
	//判断是否为同一张图片
	if (m_svSelFst.row == m_svSelSec.row&&m_svSelFst.col == m_svSelSec.col)
	{
		return false;
	}
	//判断图片是否相同
	if (m_pGameMap[m_svSelFst.row][m_svSelFst.col] != m_pGameMap[m_svSelSec.row][m_svSelSec.col])
	{
		return false;
	}
	//判断是否联通
	CGameLogic m_GameLogic;
	if (m_GameLogic.IsLink(m_pGameMap, m_svSelFst, m_svSelSec))
	{
		//消子
		m_GameLogic.Clear(m_pGameMap, m_svSelFst, m_svSelSec);

		//返回路径顶点
		nVexnum = m_GameLogic.GetVexPath(avPath);
		return true;
	}
	return false;
}


bool CGameControl::IsWin(int nTime)
{
	//时间用完则游戏失败
	if (nTime < 0)
	{
		return GAME_LOSE;
	}
	CGameLogic m_GameLogic;
	//时间未用完游戏地图为空则获胜
	if (m_GameLogic.IsBlank(m_pGameMap))
	{
		return GAME_WIN;
	}
	//游戏正在进行
	return GAME_PLAY;
}


bool CGameControl::Help(Vertex &v1, Vertex &v2)
{

	CGameLogic m_GameLogic;
	//如果地图不为空则调用搜索函数
	if (!m_GameLogic.IsBlank(m_pGameMap))
	{
		if (m_GameLogic.SearchValidPath(m_pGameMap, v1, v2))
		{
			return true;
		}
	}
	return false;
}


void CGameControl::ResetMap()
{
	//调用函数重置地图
	CGameLogic m_GameLogic;
	m_GameLogic.ResetMap(m_pGameMap);
}
