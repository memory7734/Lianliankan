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
//���ó�ʼ����ͼ������ʼ��Ϸ
void CGameControl::StartGame()
{
	CGameLogic gameLogic;
	m_pGameMap= gameLogic.InitMap();
}
//��øõ��ͼƬԪ��
int CGameControl::GetElement(int nRow,int nCol)
{
	return m_pGameMap[nRow][nCol];
}
//���õ�һ����
void CGameControl::SetFirstPoint(int nRow, int nCol)
{
	m_svSelFst.row = nRow;
	m_svSelFst.col = nCol;
}
//���õڶ�����
void CGameControl::SetSecPoint(int nRow, int nCol)
{
	m_svSelSec.row = nRow;
	m_svSelSec.col = nCol;
}
//�����ж�
bool CGameControl::Link(Vertex avPath[4],int &nVexnum)
{
	//�ж��Ƿ�Ϊͬһ��ͼƬ
	if (m_svSelFst.row == m_svSelSec.row&&m_svSelFst.col == m_svSelSec.col)
	{
		return false;
	}
	//�ж�ͼƬ�Ƿ���ͬ
	if (m_pGameMap[m_svSelFst.row][m_svSelFst.col] != m_pGameMap[m_svSelSec.row][m_svSelSec.col])
	{
		return false;
	}
	//�ж��Ƿ���ͨ
	CGameLogic m_GameLogic;
	if (m_GameLogic.IsLink(m_pGameMap, m_svSelFst, m_svSelSec))
	{
		//����
		m_GameLogic.Clear(m_pGameMap, m_svSelFst, m_svSelSec);

		//����·������
		nVexnum = m_GameLogic.GetVexPath(avPath);
		return true;
	}
	return false;
}


bool CGameControl::IsWin(int nTime)
{
	//ʱ����������Ϸʧ��
	if (nTime < 0)
	{
		return GAME_LOSE;
	}
	CGameLogic m_GameLogic;
	//ʱ��δ������Ϸ��ͼΪ�����ʤ
	if (m_GameLogic.IsBlank(m_pGameMap))
	{
		return GAME_WIN;
	}
	//��Ϸ���ڽ���
	return GAME_PLAY;
}


bool CGameControl::Help(Vertex &v1, Vertex &v2)
{

	CGameLogic m_GameLogic;
	//�����ͼ��Ϊ���������������
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
	//���ú������õ�ͼ
	CGameLogic m_GameLogic;
	m_GameLogic.ResetMap(m_pGameMap);
}
