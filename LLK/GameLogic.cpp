#include "stdafx.h"
#include "GameLogic.h"


CGameLogic::CGameLogic()			//���캯��
{
	m_nVexNum = 0;
}

CGameLogic::~CGameLogic()			//��������
{
}

int** CGameLogic::InitMap()			//��ʼ����ͼ
{
	CGameControl gameControl;
	//��ȡ��ͼ��С�ͻ�ɫ
	int nRows = gameControl.gets_nRows();
	int nCols = gameControl.gets_nCols();
	int nPicNum = gameControl.gets_nPicNum();

	//�����ڴ�����
	int** pGameMap = new int*[nRows];
	if (pGameMap == NULL)		//����ʧ�ܷ��ش���
	{
		//throw new CGameException("�ڴ�����쳣�� \n");
		return 0;
	}
	else
	{
		for (int i = 0; i < nRows; i++)
		{
			pGameMap[i] = new int[nCols];
			if (pGameMap[i] == NULL)		//����ʧ�ܷ��ش���
			{
				//throw new CGameException("�ڴ�����쳣�� \n");
				return 0;
			}
			memset(pGameMap[i], NULL, sizeof(int)*nCols);	//��ʼ������
		}
	}

	//���ٻ�ɫ
	if ((nRows*nCols) % (nPicNum * 2) != 0)
	{
		ReleaseMap(pGameMap);
		//throw new CGameException("��Ϸ��ɫ���ͼ��С��ƥ�䣡 \n");
	}
	int nRepeatNum = nRows*nCols / nPicNum;
	int nCount = 0;
	for (int i = 0; i < nPicNum; i++)
	{
		//�ظ���
		for (int j = 0; j < nRepeatNum; j++)
		{
			pGameMap[nCount / nCols][nCount%nCols] = i;
			nCount++;
		}
	}

	//��������
	srand((int)time(NULL));

	//������⽻����������
	int nVertexNum = nRows*nCols;
	for (int i = 0; i < nVertexNum; i++)
	{
		//����õ���������
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;

		//����������ֵ
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
	//���ص�ͼָ��
	return pGameMap;
}

void CGameLogic::ResetMap(int **pGameMap)		//���õ�ͼ
{
	//��������
	srand((int)time(NULL));

	CGameControl gameControl;
	int nRows = gameControl.gets_nRows();
	int nCols = gameControl.gets_nCols();
	//������⽻����������
	int nVertexNum = nRows*nCols;
	for (int i = 0; i < nVertexNum; i++)
	{
		//����õ���������
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;

		//����������ֵ
		int nTmp = pGameMap[nIndex1 / nCols][nIndex1%nCols];
		pGameMap[nIndex1 / nCols][nIndex1%nCols] = pGameMap[nIndex2 / nCols][nIndex2%nCols];
		pGameMap[nIndex2 / nCols][nIndex2%nCols] = nTmp;
	}
}

void CGameLogic::ReleaseMap(int** &pGameMap)	//�ͷŵ�ͼ
{
	CGameControl gameControl;
	//�ͷŵ�ͼ�ռ�
	for (int i = 0; i < gameControl.gets_nRows(); i++)
	{
		delete[]pGameMap[i];
	}
	delete[]pGameMap;

}

bool CGameLogic::IsBlank(int **pGameMap)		//�жϵ�ͼ�Ƿ�Ϊ��
{
	CGameControl gameControl;
	for (int i = 0; i < gameControl.gets_nRows(); i++)
	{
		for (int j = 0; j < gameControl.gets_nCols(); j++)
		{
			//�����Ϊ�շ���false
			if (pGameMap[i][j] != BLANK)
				return false;
		}
	}
	return true;
}

int CGameLogic::GetVexPath(Vertex avPath[4])		//��õ�ǰ·��
{
	for (int i = 0; i < m_nVexNum; i++)
	{
		//�ѵ�ǰ·������avPath
		avPath[i] = m_avPath[i];
	}
	return m_nVexNum;
}

void CGameLogic::PushVertex(Vertex v)	//��v����·��
{
	m_avPath[m_nVexNum] = v;
	m_nVexNum++;
}

void CGameLogic::PopVertex()			//ɾ�����һ����
{
	m_nVexNum--;
}

void CGameLogic::ClearStack()			//��յ�ǰ·��
{
	m_nVexNum = 0;
}

bool CGameLogic::IsLink(int** pGameMap, Vertex v1, Vertex v2)	//�ж�v1��v2֮���Ƿ������ͨ
{
	//��v1����·��
	PushVertex(v1);

	//һ��ֱ����ͨ
	//Xֱ����ʽ
	if (v1.row == v2.row)
	{
		if (LinkInRow(pGameMap, v1, v2))
		{
			PushVertex(v2);
			return true;
		}
	}

	//Yֱ����ʽ
	if (v1.col == v2.col)
	{
		if (LinkInCol(pGameMap, v1, v2))
		{
			PushVertex(v2);
			return true;
		}
	}
	
	//����ֱ����ͨ
	if (OneCornerLink(pGameMap, v1, v2))
	{
		PushVertex(v2);
		return true;
	}

	//����ֱ����ͨ
	if (TwoCornerLink(pGameMap, v1, v2))
	{
		PushVertex(v2);
		return true;
	}

	//���ַ�ʽ���޷���ͨ���v1����·��
	PopVertex();
	return false;
}

void CGameLogic::Clear(int** pGameMap,Vertex v1,Vertex v2)	//��v1��v2����Ϊ��
{
	pGameMap[v1.row][v1.col] = BLANK;
	pGameMap[v2.row][v2.col] = BLANK;
}

bool CGameLogic::LineX(int** pGameMap, int nRow, int nCol1, int nCol2)		//�ж�(nRow1, nCol1)��(nRow1, nCol2)�ܷ���ͨ
{
	if (nCol1 == nCol2)return true;	//�����ͬһ���㷵��true
	//��֤nCol1��ֵС��nCol2
	if (nCol1 > nCol2)
	{
		//���ݽ���
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

bool CGameLogic::LineY(int** pGameMap, int nRow1, int nRow2, int nCol)		//�ж�(nRow1, nCol2)��(nRow2, nCol2)�ܷ���ͨ
{
	if (nRow1 == nRow2)return true;	//�����ͬһ���㷵��true
	//��֤nRow1��ֵС��nRow2
	if (nRow1 > nRow2)
	{
		//���ݽ���
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

bool CGameLogic::LinkInRow(int** pGameMap, Vertex v1, Vertex v2)		//�ж�����֮����x�����Ƿ���ͨ
{
	int nRow = v1.row;
	int nCol1 = v1.col;
	int nCol2 = v2.col;

	//��֤nCol1��ֵС��nCol2
	if (nCol1 > nCol2)
	{
		//���ݽ���
		int nTemp = nCol1;
		nCol1 = nCol2;
		nCol2 = nTemp;
	}

	//ֱͨ
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

bool CGameLogic::LinkInCol(int** pGameMap, Vertex v1, Vertex v2)		//�ж�����֮����y�����Ƿ���ͨ
{
	int nCol = v1.col;
	int nRow1 = v1.row;
	int nRow2 = v2.row;

	//��֤nRow1��ֵС��nRow2
	if (nRow1 > nRow2)
	{
		//���ݽ���
		int nTemp = nRow1;
		nRow1 = nRow2;
		nRow2 = nTemp;
	}

	//ֱͨ
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

bool CGameLogic::OneCornerLink(int** pGameMap, Vertex v1, Vertex v2)		//�ж�����֮��һ���ս��Ƿ���ͨ
{
	//���ж����������ཻ���ĵ��Ƿ��Ѿ���ȥ
	//����ȥ����������x��y����y��x�������

	//1.��x��y
	if (pGameMap[v1.row][v2.col]==BLANK)
	{
		if (LineY(pGameMap, v1.row, v2.row, v2.col) && LineX(pGameMap, v1.row, v1.col, v2.col))
		{
			//�м����·����
			Vertex v = { v1.row,v2.col,BLANK };
			PushVertex(v);
			return true;
		}
	}

	//2.��y��x
	if (pGameMap[v2.row][v1.col] == BLANK)
	{
		if (LineY(pGameMap, v1.row, v2.row, v1.col) && LineX(pGameMap, v2.row, v1.col, v2.col))
		{
			//�м����·����
			Vertex v = { v2.row,v1.col,BLANK };
			PushVertex(v);
			return true;
		}
	}
	return false;
}

bool CGameLogic::TwoCornerLink(int** pGameMap, Vertex v1, Vertex v2)		//�ж�����֮�������ս��Ƿ���ͨ
{
	CGameControl gameControl;
	for (int nCol = 0; nCol < gameControl.gets_nCols(); nCol++)
	{
		//�ҵ�һ����Y��ƽ�е���ͨ�߶�
		if (pGameMap[v1.row][nCol] == BLANK&&pGameMap[v2.row][nCol] == BLANK)
		{
			if (LineY(pGameMap, v1.row, v2.row, nCol))
			{
				if (LineX(pGameMap, v1.row, v1.col, nCol) && LineX(pGameMap, v2.row, v2.col, nCol))
				{
					//����ڵ�
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
		//�ҵ�һ����X��ƽ�е���ͨ�߶�
		if (pGameMap[nRow][v1.col] == BLANK&&pGameMap[nRow][v2.col] == BLANK)
		{
			if (LineX(pGameMap, nRow, v1.col, v2.col))
			{
				if (LineY(pGameMap, nRow, v1.row, v1.col) && LineY(pGameMap, nRow, v2.row, v2.col))
				{
					//����ڵ�
					Vertex vx1 = { nRow,v1.col,BLANK };
					Vertex vx2 = { nRow,v2.col,BLANK };
					PushVertex(vx1);
					PushVertex(vx2);
					return true;
				}
			}
		}
	}
	//���������Դ���߱�Ե��ͨ
	if (LineY(pGameMap, v1.row, -1, v1.col) && LineY(pGameMap, v2.row, -1, v2.col))
	{
		//����ڵ�
		Vertex vx1 = { -1,v1.col ,BLANK };
		Vertex vx2 = { -1,v2.col ,BLANK };
		PushVertex(vx1);
		PushVertex(vx2);
		return true;
	}
	//���������Դ��ұ߱�Ե��ͨ
	if (LineY(pGameMap, v1.row, gameControl.gets_nRows() , v1.col) && LineY(pGameMap, v2.row, gameControl.gets_nRows() , v2.col))
	{
		//����ڵ�
		Vertex vx1 = { gameControl.gets_nRows(),v1.col ,BLANK };
		Vertex vx2 = { gameControl.gets_nRows(),v2.col ,BLANK };
		PushVertex(vx1);
		PushVertex(vx2);
		return true;
	}	
	//���������Դ��ϱ߱�Ե��ͨ
	if (LineX(pGameMap, v1.row, -1, v1.col) && LineX(pGameMap, v2.row, -1, v2.col))
	{
		//����ڵ�
		Vertex vx1 = { v1.row,-1 ,BLANK };
		Vertex vx2 = { v2.row,-1 ,BLANK };
		PushVertex(vx1);
		PushVertex(vx2);
		return true;
	}
	//���������Դ��±߱�Ե��ͨ
	if (LineX(pGameMap, v1.row, gameControl.gets_nCols() , v1.col) && LineX(pGameMap, v2.row, gameControl.gets_nCols() , v2.col))
	{
		//����ڵ�
		Vertex vx1 = { v1.row,gameControl.gets_nCols() ,BLANK };
		Vertex vx2 = { v2.row,gameControl.gets_nCols() ,BLANK };
		PushVertex(vx1);
		PushVertex(vx2);
		return true;
	}
	return false;
}


bool CGameLogic::SearchValidPath(int **pGameMap, Vertex &v1, Vertex &v2)		//������ͼ����ͨ��
{
	CGameControl gameControl;
	int nRows = gameControl.gets_nRows();
	int nCols = gameControl.gets_nCols();
	for (int u = 0; u < nRows; u++)
	{
		for (int v = 0; v < nCols; v++)
		{
			//����ǰ��Ϊ��������ֱ���ҵ���Ϊ�յĵ�
			if (pGameMap[u][v] == BLANK)
				continue;
			//�Ӳ�Ϊ�յĵ㿪ʼ����������ǰ����������ͨ�ĵ�
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
			//����һ�п�ʼ������������ͨ�ĵ�
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
