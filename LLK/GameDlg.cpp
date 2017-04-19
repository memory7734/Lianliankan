// GameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LLK.h"
#include "GameDlg.h"
#include "afxdialogex.h"


// CGameDlg �Ի���

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAME_DIALOG, pParent)
{
	//����ϵͳͼ��
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_ptGameTop.x = MAP_LEFT;
	m_ptGameTop.y = MAP_TOP;

	//����Ԫ��ͼƬ��С������
	m_sizeElem.cx = MAP_WIDTH;
	m_sizeElem.cy = MAP_HEIGHT;

	//��ȡ����������
	int nRows = m_gameControl.gets_nRows();
	int nCols = m_gameControl.gets_nCols();

	//��ʼ����Ϸ��������
	m_rtGameRect.top = m_ptGameTop.y - m_sizeElem.cy * 1;
	m_rtGameRect.left = m_ptGameTop.x - m_sizeElem.cx * 1;
	m_rtGameRect.right = m_ptGameTop.x + m_sizeElem.cx * (nCols + 1);
	m_rtGameRect.bottom = m_ptGameTop.y + m_sizeElem.cy * (nRows + 1);

	//��ʼ��״̬
	m_bFirstPoint = true;
	m_bPlaying = false;
	m_bPause = false;

}

CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAME_TIME, m_GameProgress);
}

BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_START, &CGameDlg::OnBnClickedBtnStart)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_PROMPT, &CGameDlg::OnBnClickedBtnPrompt)
	ON_BN_CLICKED(IDC_BTN_RESET, &CGameDlg::OnBnClickedBtnReset)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STOP, &CGameDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()

// CGameDlg ��Ϣ�������
void CGameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	dc.BitBlt(0, 0, 800, 600, &m_dcMem, 0, 0, SRCCOPY);
}

//��ʼ��Dialog
BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	GetDlgItem(IDC_BTN_START)->SetWindowPos(NULL, m_sizeElem.cx*(MAX_COL + 2), m_sizeElem.cy*(1), 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//���ÿ�ʼ��ť��λ�úʹ�С
	GetDlgItem(IDC_BTN_STOP)->SetWindowPos(NULL, m_sizeElem.cx*(MAX_COL + 2), m_sizeElem.cy*(2), 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//���ÿ�ʼ��ť��λ�úʹ�С
	GetDlgItem(IDC_BTN_PROMPT)->SetWindowPos(NULL, m_sizeElem.cx*(MAX_COL + 2), m_sizeElem.cy*(3), 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//���ÿ�ʼ��ť��λ�úʹ�С
	GetDlgItem(IDC_BTN_RESET)->SetWindowPos(NULL, m_sizeElem.cx*(MAX_COL + 2), m_sizeElem.cy*(4), 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//���ÿ�ʼ��ť��λ�úʹ�С

	GetDlgItem(IDC_BTN_SET)->SetWindowPos(NULL, m_sizeElem.cx / 2 + m_sizeElem.cx*(MAX_COL + 2), m_sizeElem.cy*(12), 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//���ÿ�ʼ��ť��λ�úʹ�С
	GetDlgItem(IDC_BTN_HELP)->SetWindowPos(NULL, m_sizeElem.cx / 2 + m_sizeElem.cx*(MAX_COL + 2), m_sizeElem.cy*(13), 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//���ÿ�ʼ��ť��λ�úʹ�С

	GetDlgItem(IDC_GAME_TIME)->SetWindowPos(NULL, m_sizeElem.cx / 2 + m_sizeElem.cx * 2, m_sizeElem.cy / 2 + m_sizeElem.cy*12, 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//���ÿ�ʼ��ť��λ�úʹ�С
	GetDlgItem(IDC_GAME_TIME)->ShowWindow(SW_HIDE);	//���ؽ�����

	InitBackground();				//��ʼ������
	InitElement();					//��ʼ��Ԫ��
	UpdateWindow();					//���µ�ͼ

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//��ʼ�����ڱ������С
void CGameDlg::InitBackground()
{
	//��õ�ǰ�Ի������Ƶ�ڴ�
	CClientDC dc(this);

	//����BMPͼƬ��Դ
	HANDLE hBmpBG = ::LoadImage(NULL, _T("theme\\picture\\white_bg.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//��������Ƶ�ڴ���ݵ��ڴ�DC
	m_dcBG.CreateCompatibleDC(&dc);
	//��λͼ��Դѡ��DC
	m_dcBG.SelectObject(hBmpBG);

	//��ʼ���ڴ�DC
	m_dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, 800, 600);
	m_dcMem.SelectObject(&bmpMem);

	//���Ʊ������ڴ�DC��
	m_dcMem.BitBlt(0, 0, 800, 600, &m_dcBG, 0, 0, SRCCOPY);
}

//��ʼ��ͼƬԪ��
void CGameDlg::InitElement(void)
{
	//��õ�ǰ�Ի������Ƶ�ڴ�
	CClientDC dc(this);

	//����BMPͼƬ��Դ
	HANDLE hBmp = ::LoadImage(NULL, _T("theme\\picture\\candy_element.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//��������Ƶ�ڴ���ݵ��ڴ�DC
	m_dcElment.CreateCompatibleDC(&dc);
	//��λͼ��Դѡ��DC
	m_dcElment.SelectObject(hBmp);

	//����BMPͼƬ��Դ
	HANDLE hPus = ::LoadImage(NULL, _T("theme\\picture\\white_pause.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//��������Ƶ�ڴ���ݵ��ڴ�DC
	m_dcPause.CreateCompatibleDC(&dc);
	//��λͼ��Դѡ��DC
	m_dcPause.SelectObject(hPus);

	//����BMPͼƬ��Դ
	HANDLE hMask = ::LoadImage(NULL, _T("theme\\picture\\candy_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//��������Ƶ�ڴ���ݵ��ڴ�DC
	m_dcMask.CreateCompatibleDC(&dc);
	//��λͼ��Դѡ��DC
	m_dcMask.SelectObject(hMask);
}

//���´���
void CGameDlg::UpdateWindow()
{
	//�������ڴ�С
	CRect rtWin;
	CRect rtClient;
	this->GetWindowRect(rtWin);		//��ô��ڴ�С
	this->GetClientRect(rtClient);	//��ÿͻ�����С
									//����������߿�Ĵ�С
	int nSpanWidth = rtWin.Width() - rtClient.Width();
	int nSpanHeight = rtWin.Height() - rtClient.Height();

	//���ô��ڴ�С
	MoveWindow(0, 0, 800 + nSpanWidth, 600 + nSpanHeight);

	//ʹ���ھ���
	CenterWindow();
}

//���µ�ͼ
void CGameDlg::UpdateMap()
{
	//����ͼƬ�Ķ���������ͼƬ��С
	int nLeft = m_ptGameTop.x;
	int nTop = m_ptGameTop.y;
	int nElemW = m_sizeElem.cx;
	int nElemH = m_sizeElem.cy;

	m_dcMem.BitBlt(m_rtGameRect.left, m_rtGameRect.top, m_rtGameRect.Width(), m_rtGameRect.Height(),
		&m_dcBG, m_rtGameRect.left, m_rtGameRect.top, SRCCOPY);

	//��ȡ����������
	int nRows = m_gameControl.gets_nRows();
	int nCols = m_gameControl.gets_nCols();

	//ѭ������ͼƬ
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			int nElemVal = m_gameControl.GetElement(i, j);
			if (nElemVal == BLANK) continue;
			//��������������򣬱߱�����ͼ������Ϊ1
			m_dcMem.BitBlt(nLeft + j*nElemW, nTop + i*nElemH, nElemW, nElemH, &m_dcMask, 0, nElemVal * nElemH, SRCPAINT);
			//��Ԫ��ͼƬ���룬�߱�����ͼ��Ԫ��ΪԪ��ͼƬ
			m_dcMem.BitBlt(nLeft + j*nElemW, nTop + i*nElemH, nElemW, nElemH, &m_dcElment, 0, nElemVal * nElemH, SRCAND);
		}
	}
}

//������ʾ��
void CGameDlg::DrawTipLine(Vertex avPath[4], int &nVexnum)
{
	//���DC
	CClientDC dc(this);

	//���û���
	CPen penLine(PS_SOLID, 2, RGB(0, 255, 0));

	//������ѡ��DC
	CPen* pOldPen = dc.SelectObject(&penLine);

	//��������
	dc.MoveTo(m_ptGameTop.x + avPath[0].col*m_sizeElem.cx + m_sizeElem.cx / 2,
		m_ptGameTop.y + avPath[0].row*m_sizeElem.cy + m_sizeElem.cy / 2);
	for (int i = 1; i < nVexnum; i++)
	{
		dc.LineTo(m_ptGameTop.x + avPath[i].col*m_sizeElem.cx + m_sizeElem.cx / 2,
			m_ptGameTop.y + avPath[i].row*m_sizeElem.cy + m_sizeElem.cy / 2);
	}

	dc.SelectObject(pOldPen);
}

//������ʾ��
void CGameDlg::DrawTipFrame(int nRow, int nCol)
{
	CClientDC dc(this);
	CBrush brush(RGB(233, 43, 43));
	CRect rtTipFrame;
	rtTipFrame.left = m_ptGameTop.x + nCol*m_sizeElem.cx;
	rtTipFrame.top = m_ptGameTop.y + nRow*m_sizeElem.cy;
	rtTipFrame.right = rtTipFrame.left + m_sizeElem.cx;
	rtTipFrame.bottom = rtTipFrame.top + m_sizeElem.cy;
	dc.FrameRect(rtTipFrame, &brush);
}

//���Ƶ�ǰ����
void CGameDlg::DrawGameTime()
{
	//����DC
	CClientDC dc(this);
	//��������
	CFont font;
	//���������С����
	font.CreatePointFont(200, _T("Courier New"));

	CFont *oldFont;
	//������ѡ��dc
	oldFont = dc.SelectObject(&font);
	//����������ɫ�뱳��ɫ
	dc.SetTextColor(RGB(255, 255, 255));
	dc.SetBkColor(RGB(255, 106, 106));

	//��ȡ���������λ��
	CRect rect;
	GetDlgItem(IDC_GAME_TIME)->GetWindowRect(&rect);
	ScreenToClient(rect);
	//�����������Ϊ����
	CString time;
	time.Format(_T("%d"), m_GameProgress.GetPos());
	//�������λ��Ϊ��������
	dc.TextOut(rect.left + rect.Width() + 20, rect.top, time);

	//�ÿո���Ʊ�֤ÿ������ı�����һ��
	if (m_GameProgress.GetPos() >= 10 && m_GameProgress.GetPos() < 100)
	{
		CString time;
		time.Format(_T("%d "), m_GameProgress.GetPos());
		dc.TextOut(rect.left + rect.Width(), rect.top, time);
	}
	if (m_GameProgress.GetPos() <10)
	{
		CString time;
		time.Format(_T("%d  "), m_GameProgress.GetPos());
		dc.TextOut(rect.left + rect.Width(), rect.top, time);
	}
}

//�ж��Ƿ��ʤ
void CGameDlg::JudgeWin()
{
	int bGameStatus = m_gameControl.IsWin(m_GameProgress.GetPos());
	//�Ƿ������Ϸ
	if (bGameStatus == GAME_PLAY)
	{
		return;
	}
	else
	{
		//����Ϸ��ʶ��Ϊfalse
		m_bPlaying = false;
		//�رռ�ʱ��
		KillTimer(PLAY_TIMER_ID);
		//��ʾ��ʤ
		InvalidateRect(m_rtGameRect, FALSE);
		CString strTitle;
		this->GetWindowTextW(strTitle);
		if (bGameStatus == GAME_WIN)
		{
			MessageBox(_T("YOU WIN!"), strTitle);

		}
		else if (bGameStatus == GAME_LOSE)
		{
			MessageBox(_T("TIME IS UP!"), strTitle);
		}
		//��ԭstart��ť��ʹ�ð�ť���Ե��
		this->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);

	}
}

//��ʱ��
void CGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == PLAY_TIMER_ID&&m_bPlaying && !m_bPause)
	{
		//����ʱ�����һ������
		m_GameProgress.StepIt();
		DrawGameTime();    //ÿ��һ���ػ�ʱ��
		JudgeWin();           //ÿ��һ���ж�һ����Ӯ
	}

	CDialogEx::OnTimer(nIDEvent);
}

//��ʼ��ť
void CGameDlg::OnBnClickedBtnStart()
{
	//��ʼ����Ϸ��ͼ
	m_gameControl.StartGame();
	
	//���½���
	UpdateMap();

	//���´���
	InvalidateRect(m_rtGameRect,FALSE);

	m_bPlaying = true;
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);

	//��ʼ��������
	GetDlgItem(IDC_GAME_TIME)->ShowWindow(SW_SHOW);	//��ʾ������
	m_GameProgress.SetRange(0, 60 * 5);		//��ʼ����Χ
	m_GameProgress.SetStep(-1);				//��ʼ������
	m_GameProgress.SetPos(60 * 5);			//���ó�ʼֵ

	//������ʱ��
	this->SetTimer(PLAY_TIMER_ID, 1000, NULL);
	//���Ƶ�ǰ����
	DrawGameTime();
	
}

//ֹͣ��ť
void CGameDlg::OnBnClickedBtnStop()
{
	if (!m_bPlaying)
		return;
	if (m_bPause == false)
	{
		//��ͣ�������ͣ����ͼ
		m_dcMem.BitBlt(50, 50, 640, 400, &m_dcPause, 50, 50, SRCCOPY);
		//����ť���Ƹ�Ϊ����
		SetDlgItemText(IDC_BTN_STOP, _T("Continue"));
		//������ͣ��ʶ��
		m_bPause = true;

		InvalidateRect(m_rtGameRect, FALSE);
	}
	else
	{
		//��������Ϸ��ָ���ͼ
		UpdateMap();
		//����ť���Ƹ�Ϊ��ͣ
		SetDlgItemText(IDC_BTN_STOP, _T("Pause"));
		//���ñ�ʶ��
		m_bPause = false;
		InvalidateRect(m_rtGameRect, FALSE);
	}
}

//��ʾ��ť
void CGameDlg::OnBnClickedBtnPrompt()
{
	Vertex v1, v2;
	if (m_bPlaying && !m_bPause&&m_gameControl.Help(v1, v2))
	{
		//�����������ʾ��
		DrawTipFrame(v2.row, v2.col);
		DrawTipFrame(v1.row, v1.col);

		//���µ�ͼ
		Sleep(200);
		UpdateMap();
		InvalidateRect(m_rtGameRect, FALSE);
	}

}

//���ð�ť
void CGameDlg::OnBnClickedBtnReset()
{
	//�����Ϸδ��ʼ������ͣ״̬���������
	if (!m_bPlaying || m_bPause)
		return;
	//���õ�ͼ
	m_gameControl.ResetMap();
	UpdateMap();
	InvalidateRect(m_rtGameRect, FALSE);
}

//������
void CGameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// �ж������������
	if (point.x < m_rtGameRect.left+ m_sizeElem.cx || point.x > m_rtGameRect.right- m_sizeElem.cx || 
		point.y < m_rtGameRect.top+ m_sizeElem.cy || point.y > m_rtGameRect.bottom- m_sizeElem.cy)
	{
		return CDialogEx::OnLButtonUp(nFlags, point);
	}

	//�ж��Ƿ�Ϊ��Ϸδ��ʼ����ͣ״̬
	if (!m_bPlaying || m_bPause)
	{
		return CDialogEx::OnLButtonUp(nFlags, point);
	}

	//�������������
	int nRow = (point.y - m_ptGameTop.y) / m_sizeElem.cy;
	int nCol = (point.x - m_ptGameTop.x) / m_sizeElem.cx;

	//�жϸ������Ƿ�Ϊ��
	if (m_gameControl.GetElement(nRow, nCol) == BLANK)
	{
		return CDialogEx::OnLButtonUp(nFlags, point);
	}

	if (m_bFirstPoint)	//��һ����
	{
		//������ʾ��
		DrawTipFrame(nRow, nCol);
		m_gameControl.SetFirstPoint(nRow, nCol);
	}
	else				//�ڶ�����
	{
		DrawTipFrame(nRow, nCol);
		m_gameControl.SetSecPoint(nRow, nCol);

		int nVexnum;
		Vertex avPath[4];
		//�����ж�
		if (m_gameControl.Link(avPath,nVexnum))
		{
			//����ʾ��
			DrawTipLine(avPath,nVexnum);
			//���
			UpdateMap();

			//ÿ�θ��µ�ͼ�ж���Ϸʤ��
			JudgeWin();
			Vertex v1, v2;
			if (!m_gameControl.Help(v1, v2))
			{
				//���µ�ͼ
				m_gameControl.ResetMap();
				UpdateMap();
				InvalidateRect(m_rtGameRect, FALSE);
			}
			
		}
		Sleep(200);
		InvalidateRect(m_rtGameRect, FALSE);
	}
	m_bFirstPoint = !m_bFirstPoint;

}
