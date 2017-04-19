#include "stdafx.h"
#include "GameLogic.h"


CGameLogic::CGameLogic()			//构造函数
{
	m_nVexNum = 0;
}

CGameLogic::~CGameLogic()			//析构函数
{
}

int** CGameLogic::InitMap()			//初始化地图
{
	CGameControl gameControl;
	//获取地图大小和花色
	int nRows = gameControl.gets_nRows();
	int nCols = gameControl.gets_nCols();
	int nPicNum = gameControl.gets_nPicNum();

	//开辟内存区域
	int** pGameMap = new int*[nRows];
	if (pGameMap == NULL)		//开辟失败返回错误
	{
		//throw new CGameException("内存操作异常！ \n");
		return 0;
	}
	else
	{
		for (int i = 0; i < nRows; i++)
		{
			pGameMap[i] = new int[nCols];
			if (pGameMap[i] == NULL)		//开辟失败返回错误
			{
				//throw new CGameException("内存操作异常！ \n");
				return 0;
			}
			memset(pGameMap[i], NULL, sizeof(int)*nCols);	//初始化数组
		}
	}

	//多少花色
	if ((nRows*nCols) % (nPicNum * 2) != 0)
	{
		ReleaseMap(pGameMap);
		//throw new CGameException("游戏花色与地图大小不匹配！ \n");
	}
	int nRepeatNum = nRows*nCols / nPicNum;
	int nCount = 0;
	for (int i = 0; i < nPicNum; i++)
	{
		//重复数
		for (int j = 0; j < nRepeatNum; j++)
		{
			pGameMap[nCount / nCols][nCount%nCols] = i;
			nCount++;
		}
	}

	//设置种子
	srand((int)time(NULL));

	//随机任意交换两个数字
	int nVertexNum = nRows*nCols;
	for (int i = 0; i < nVertexNum; i++)
	{
		//随机得到两个坐标
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;

		//交换两个数值
		int nTmp = pGameMap[nIndex1 / nCols][nIndex1%nCols];
		pGameMap[nIndex1 / nCols][nIndex1%nCols] = pGameMap[nIndex2 / nCols][nIndex2%nCols];
		pGameMap[nIndex2 / nCols][nIndex2%nCols] = nTmp;
	}
/*	
	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
			pGameMap[i][j] = BLANK;
	pGameMap[0][0] = 1;

	pGameMap[1][0] = 1;
	*/
	//返回地图指针
	return pGameMap;
}

void CGameLogic::ResetMap(int **pGameMap)		//重置地图
{
	//设置种子
	srand((int)time(NULL));

	CGameControl gameControl;
	int nRows = gameControl.gets_nRows();
	int nCols = gameControl.gets_nCols();
	//随机任意交换两个数字
	int nVertexNum = nRows*nCols;
	for (int i = 0; i < nVertexNum; i++)
	{
		//随机得到两个坐标
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;

		//交换两个数值
		int nTmp = pGameMap[nIndex1 / nCols][nIndex1%nCols];
		pGameMap[nIndex1 / nCols][nIndex1%nCols] = pGameMap[nIndex2 / nCols][nIndex2%nCols];
		pGameMap[nIndex2 / nCols][nIndex2%nCols] = nTmp;
	}
}

void CGameLogic::ReleaseMap(int** &pGameMap)	//释放地图
{
	CGameControl gameControl;
	//释放地图空间
	for (int i = 0; i < gameControl.gets_nRows(); i++)
	{
		delete[]pGameMap[i];
	}
	delete[]pGameMap;

}

bool CGameLogic::IsBlank(int **pGameMap)		//判断地图是否为空
{
	CGameControl gameControl;
	for (int i = 0; i < gameControl.gets_nRows(); i++)
	{
		for (int j = 0; j < gameControl.gets_nCols(); j++)
		{
			//如果不为空返回false
			if (pGameMap[i][j] != BLANK)
				return false;
		}
	}
	return true;
}

int CGameLogic::GetVexPath(Vertex avPath[4])		//获得当前路径
{
	for (int i = 0; i < m_nVexNum; i++)
	{
		//把当前路径存入avPath
		avPath[i] = m_avPath[i];
	}
	return m_nVexNum;
}

void CGameLogic::PushVertex(Vertex v)	//将v存入路径
{
	m_avPath[m_nVexNum] = v;
	m_nVexNum++;
}

void CGameLogic::PopVertex()			//删除最后一个点
{
	m_nVexNum--;
}

void CGameLogic::ClearStack()			//清空当前路径
{
	m_nVexNum = 0;
}

bool CGameLogic::IsLink(int** pGameMap, Vertex v1, Vertex v2)	//判断v1和v2之间是否可以连通
{
	//将v1存入路径
	PushVertex(v1);

	//一条直线联通
	//X直连方式
	if (v1.row == v2.row)
	{
		if (LinkInRow(pGameMap, v1, v2))
		{
			PushVertex(v2);
			return true;
		}
	}

	//Y直连方式
	if (v1.col == v2.col)
	{
		if (LinkInCol(pGameMap, v1, v2))
		{
			PushVertex(v2);
			return true;
		}
	}
	
	//两条直线联通
	if (OneCornerLink(pGameMap, v1, v2))
	{
		PushVertex(v2);
		return true;
	}

	//三条直线联通
	if (TwoCornerLink(pGameMap, v1, v2))
	{
		PushVertex(v2);
		return true;
	}

	//三种方式都无法连通则把v1弹出路径
	PopVertex();
	return false;
}

void CGameLogic::Clear(int** pGameMap,Vertex v1,Vertex v2)	//将v1和v2设置为空
{
	pGameMap[v1.row][v1.col] = BLANK;
	pGameMap[v2.row][v2.col] = BLANK;
}

bool CGameLogic::LineX(int** pGameMap, int nRow, int nCol1, int nCol2)		//判断(nRow1, nCol1)到(nRow1, nCol2)能否连通
{
	if (nCol1 == nCol2)return true;	//如果是同一个点返回true
	//保证nCol1的值小于nCol2
	if (nCol1 > nCol2)
	{
		//数据交换
		int nTemp = nCol1;
		nCol1 = nCol2;
		nCol2 = nTemp;
	}
	for (int nCol = nCol1 + 1; nCol <= nCol2; nCol++)
	{
		if (nCol == nCol2)
		{
			return true;
		}
		if (pGameMap[nRow][nCol] != BLANK)
		{
			break;
		}
	}
	return false;
}

bool CGameLogic::LineY(int** pGameMap, int nRow1, int nRow2, int nCol)		//判断(nRow1, nCol2)到(nRow2, nCol2)能否连通
{
	if (nRow1 == nRow2)return true;	//如果是同一个点返回true
	//保证nRow1的值小于nRow2
	if (nRow1 > nRow2)
	{
		//数据交换
		int nTemp = nRow1;
		nRow1 = nRow2;
		nRow2 = nTemp;
	}
	for (int nRow = nRow1 + 1; nRow <= nRow2; nRow++)
	{
		if (nRow == nRow2)
		{
			return true;
		}
		if (pGameMap[nRow][nCol] != BLANK)
		{
			break;
		}
	}
	return false;
}

bool CGameLogic::LinkInRow(int** pGameMap, Vertex v1, Vertex v2)		//判断两点之间在x轴上是否连通
{
	int nRow = v1.row;
	int nCol1 = v1.col;
	int nCol2 = v2.col;

	//保证nCol1的值小于nCol2
	if (nCol1 > nCol2)
	{
		//数据交换
		int nTemp = nCol1;
		nCol1 = nCol2;
		nCol2 = nTemp;
	}

	//直通
	for (int i = nCol1 + 1; i <= nCol2; i++)
	{
		if (i == nCol2)
		{
			return true;
		}
		if (pGameMap[nRow][i] != BLANK)
		{
			break;
		}
	}
	return false;
}

bool CGameLogic::LinkInCol(int** pGameMap, Vertex v1, Vertex v2)		//判断两点之间在y轴上是否连通
{
	int nCol = v1.col;
	int nRow1 = v1.row;
	int nRow2 = v2.row;

	//保证nRow1的值小于nRow2
	if (nRow1 > nRow2)
	{
		//数据交换
		int nTemp = nRow1;
		nRow1 = nRow2;
		nRow2 = nTemp;
	}

	//直通
	for (int i = nRow1 + 1; i <= nRow2; i++)
	{
		if (i == nRow2)
		{
			return true;
		}
		if (pGameMap[i][nCol] != BLANK)
		{
			break;
		}
	}
	return false;
}

bool CGameLogic::OneCornerLink(int** pGameMap, Vertex v1, Vertex v2)		//判断两点之间一个拐角是否连通
{
	//先判断两点行列相交处的点是否已经消去
	//若消去，别讨论先x再y和先y再x两种情况

	//1.先x后y
	if (pGameMap[v1.row][v2.col]==BLANK)
	{
		if (LineY(pGameMap, v1.row, v2.row, v2.col) && LineX(pGameMap, v1.row, v1.col, v2.col))
		{
			//中间存入路径中
			Vertex v = { v1.row,v2.col,BLANK };
			PushVertex(v);
			return true;
		}
	}

	//2.先y后x
	if (pGameMap[v2.row][v1.col] == BLANK)
	{
		if (LineY(pGameMap, v1.row, v2.row, v1.col) && LineX(pGameMap, v2.row, v1.col, v2.col))
		{
			//中间存入路径中
			Vertex v = { v2.row,v1.col,BLANK };
			PushVertex(v);
			return true;
		}
	}
	return false;
}

bool CGameLogic::TwoCornerLink(int** pGameMap, Vertex v1, Vertex v2)		//判断两点之间两个拐角是否连通
{
	CGameControl gameControl;
	for (int nCol = 0; nCol < gameControl.gets_nCols(); nCol++)
	{
		//找到一条与Y轴平行的连通线段
		if (pGameMap[v1.row][nCol] == BLANK&&pGameMap[v2.row][nCol] == BLANK)
		{
			if (LineY(pGameMap, v1.row, v2.row, nCol))
			{
				if (LineX(pGameMap, v1.row, v1.col, nCol) && LineX(pGameMap, v2.row, v2.col, nCol))
				{
					//保存节点
					Vertex vx1 = { v1.row,nCol,BLANK };
					Vertex vx2 = { v2.row,nCol,BLANK };
					PushVertex(vx1);
					PushVertex(vx2);
					return true;
				}
			}
		}
	}
	for (int nRow = 0; nRow < gameControl.gets_nRows(); nRow++)
	{
		//找到一条与X轴平行的连通线段
		if (pGameMap[nRow][v1.col] == BLANK&&pGameMap[nRow][v2.col] == BLANK)
		{
			if (LineX(pGameMap, nRow, v1.col, v2.col))
			{
				if (LineY(pGameMap, nRow, v1.row, v1.col) && LineY(pGameMap, nRow, v2.row, v2.col))
				{
					//保存节点
					Vertex vx1 = { nRow,v1.col,BLANK };
					Vertex vx2 = { nRow,v2.col,BLANK };
					PushVertex(vx1);
					PushVertex(vx2);
					return true;
				}
			}
		}
	}
	//两个结点可以从左边边缘连通
	if (LineY(pGameMap, v1.row, -1, v1.col) && LineY(pGameMap, v2.row, -1, v2.col))
	{
		//保存节点
		Vertex vx1 = { -1,v1.col ,BLANK };
		Vertex vx2 = { -1,v2.col ,BLANK };
		PushVertex(vx1);
		PushVertex(vx2);
		return true;
	}
	//两个结点可以从右边边缘连通
	if (LineY(pGameMap, v1.row, gameControl.gets_nRows() , v1.col) && LineY(pGameMap, v2.row, gameControl.gets_nRows() , v2.col))
	{
		//保存节点
		Vertex vx1 = { gameControl.gets_nRows(),v1.col ,BLANK };
		Vertex vx2 = { gameControl.gets_nRows(),v2.col ,BLANK };
		PushVertex(vx1);
		PushVertex(vx2);
		return true;
	}	
	//两个结点可以从上边边缘连通
	if (LineX(pGameMap, v1.row, -1, v1.col) && LineX(pGameMap, v2.row, -1, v2.col))
	{
		//保存节点
		Vertex vx1 = { v1.row,-1 ,BLANK };
		Vertex vx2 = { v2.row,-1 ,BLANK };
		PushVertex(vx1);
		PushVertex(vx2);
		return true;
	}
	//两个结点可以从下边边缘连通
	if (LineX(pGameMap, v1.row, gameControl.gets_nCols() , v1.col) && LineX(pGameMap, v2.row, gameControl.gets_nCols() , v2.col))
	{
		//保存节点
		Vertex vx1 = { v1.row,gameControl.gets_nCols() ,BLANK };
		Vertex vx2 = { v2.row,gameControl.gets_nCols() ,BLANK };
		PushVertex(vx1);
		PushVertex(vx2);
		return true;
	}
	return false;
}


bool CGameLogic::SearchValidPath(int **pGameMap, Vertex &v1, Vertex &v2)		//搜索地图找连通点
{
	CGameControl gameControl;
	int nRows = gameControl.gets_nRows();
	int nCols = gameControl.gets_nCols();
	for (int u = 0; u < nRows; u++)
	{
		for (int v = 0; v < nCols; v++)
		{
			//若当前点为空则跳过直至找到不为空的点
			if (pGameMap[u][v] == BLANK)
				continue;
			//从不为空的点开始往右搜索当前行搜索能连通的点
			for (int i = v+1; i < nCols; i++)
			{
				if (pGameMap[u][v] == pGameMap[u][i])
				{
					v1 = { u,v,pGameMap[u][v] };
					v2 = { u,i,pGameMap[u][i] };
					if (IsLink(pGameMap, v1, v2))
					{
						return true;
					}
				}
			}
			//从下一行开始往右搜索能联通的点
			for (int i = u + 1; i < nRows; i++)
			{
				for (int j = 0; j < nCols; j++)
				{
					if (pGameMap[u][v] == pGameMap[i][j])
					{
						v1 = { u,v,pGameMap[u][v] };
						v2 = { i,j,pGameMap[i][j] };
						if (IsLink(pGameMap, v1, v2))
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
