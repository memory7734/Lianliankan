// GameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LLK.h"
#include "GameDlg.h"
#include "afxdialogex.h"


// CGameDlg 对话框

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAME_DIALOG, pParent)
{
	//加载系统图标
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_ptGameTop.x = MAP_LEFT;
	m_ptGameTop.y = MAP_TOP;

	//单个元素图片大小的设置
	m_sizeElem.cx = MAP_WIDTH;
	m_sizeElem.cy = MAP_HEIGHT;

	//获取行数和列数
	int nRows = m_gameControl.gets_nRows();
	int nCols = m_gameControl.gets_nCols();

	//初始化游戏更新区域
	m_rtGameRect.top = m_ptGameTop.y - m_sizeElem.cy * 1;
	m_rtGameRect.left = m_ptGameTop.x - m_sizeElem.cx * 1;
	m_rtGameRect.right = m_ptGameTop.x + m_sizeElem.cx * (nCols + 1);
	m_rtGameRect.bottom = m_ptGameTop.y + m_sizeElem.cy * (nRows + 1);

	//初始化状态
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

// CGameDlg 消息处理程序
void CGameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	dc.BitBlt(0, 0, 800, 600, &m_dcMem, 0, 0, SRCCOPY);
}

//初始化Dialog
BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	GetDlgItem(IDC_BTN_START)->SetWindowPos(NULL, m_sizeElem.cx*(MAX_COL + 2), m_sizeElem.cy*(1), 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//设置开始按钮的位置和大小
	GetDlgItem(IDC_BTN_STOP)->SetWindowPos(NULL, m_sizeElem.cx*(MAX_COL + 2), m_sizeElem.cy*(2), 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//设置开始按钮的位置和大小
	GetDlgItem(IDC_BTN_PROMPT)->SetWindowPos(NULL, m_sizeElem.cx*(MAX_COL + 2), m_sizeElem.cy*(3), 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//设置开始按钮的位置和大小
	GetDlgItem(IDC_BTN_RESET)->SetWindowPos(NULL, m_sizeElem.cx*(MAX_COL + 2), m_sizeElem.cy*(4), 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//设置开始按钮的位置和大小

	GetDlgItem(IDC_BTN_SET)->SetWindowPos(NULL, m_sizeElem.cx / 2 + m_sizeElem.cx*(MAX_COL + 2), m_sizeElem.cy*(12), 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//设置开始按钮的位置和大小
	GetDlgItem(IDC_BTN_HELP)->SetWindowPos(NULL, m_sizeElem.cx / 2 + m_sizeElem.cx*(MAX_COL + 2), m_sizeElem.cy*(13), 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//设置开始按钮的位置和大小

	GetDlgItem(IDC_GAME_TIME)->SetWindowPos(NULL, m_sizeElem.cx / 2 + m_sizeElem.cx * 2, m_sizeElem.cy / 2 + m_sizeElem.cy*12, 0, 0, SWP_NOZORDER | SWP_NOSIZE);	//设置开始按钮的位置和大小
	GetDlgItem(IDC_GAME_TIME)->ShowWindow(SW_HIDE);	//隐藏进度条

	InitBackground();				//初始化背景
	InitElement();					//初始化元素
	UpdateWindow();					//更新地图

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//初始化窗口背景与大小
void CGameDlg::InitBackground()
{
	//获得当前对话框的视频内存
	CClientDC dc(this);

	//加载BMP图片资源
	HANDLE hBmpBG = ::LoadImage(NULL, _T("theme\\picture\\white_bg.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//创建于视频内存兼容的内存DC
	m_dcBG.CreateCompatibleDC(&dc);
	//将位图资源选入DC
	m_dcBG.SelectObject(hBmpBG);

	//初始化内存DC
	m_dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, 800, 600);
	m_dcMem.SelectObject(&bmpMem);

	//绘制背景到内存DC中
	m_dcMem.BitBlt(0, 0, 800, 600, &m_dcBG, 0, 0, SRCCOPY);
}

//初始化图片元素
void CGameDlg::InitElement(void)
{
	//获得当前对话框的视频内存
	CClientDC dc(this);

	//加载BMP图片资源
	HANDLE hBmp = ::LoadImage(NULL, _T("theme\\picture\\candy_element.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//创建于视频内存兼容的内存DC
	m_dcElment.CreateCompatibleDC(&dc);
	//将位图资源选入DC
	m_dcElment.SelectObject(hBmp);

	//加载BMP图片资源
	HANDLE hPus = ::LoadImage(NULL, _T("theme\\picture\\white_pause.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//创建于视频内存兼容的内存DC
	m_dcPause.CreateCompatibleDC(&dc);
	//将位图资源选入DC
	m_dcPause.SelectObject(hPus);

	//加载BMP图片资源
	HANDLE hMask = ::LoadImage(NULL, _T("theme\\picture\\candy_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//创建于视频内存兼容的内存DC
	m_dcMask.CreateCompatibleDC(&dc);
	//将位图资源选入DC
	m_dcMask.SelectObject(hMask);
}

//更新窗口
void CGameDlg::UpdateWindow()
{
	//调整窗口大小
	CRect rtWin;
	CRect rtClient;
	this->GetWindowRect(rtWin);		//获得窗口大小
	this->GetClientRect(rtClient);	//获得客户区大小
									//标题栏和外边框的大小
	int nSpanWidth = rtWin.Width() - rtClient.Width();
	int nSpanHeight = rtWin.Height() - rtClient.Height();

	//设置窗口大小
	MoveWindow(0, 0, 800 + nSpanWidth, 600 + nSpanHeight);

	//使窗口居中
	CenterWindow();
}

//更新地图
void CGameDlg::UpdateMap()
{
	//计算图片的顶点坐标与图片大小
	int nLeft = m_ptGameTop.x;
	int nTop = m_ptGameTop.y;
	int nElemW = m_sizeElem.cx;
	int nElemH = m_sizeElem.cy;

	m_dcMem.BitBlt(m_rtGameRect.left, m_rtGameRect.top, m_rtGameRect.Width(), m_rtGameRect.Height(),
		&m_dcBG, m_rtGameRect.left, m_rtGameRect.top, SRCCOPY);

	//获取行数和列数
	int nRows = m_gameControl.gets_nRows();
	int nCols = m_gameControl.gets_nCols();

	//循环绘制图片
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			int nElemVal = m_gameControl.GetElement(i, j);
			if (nElemVal == BLANK) continue;
			//将背景与掩码相或，边保留，图像区域为1
			m_dcMem.BitBlt(nLeft + j*nElemW, nTop + i*nElemH, nElemW, nElemH, &m_dcMask, 0, nElemVal * nElemH, SRCPAINT);
			//与元素图片相与，边保留，图像元素为元素图片
			m_dcMem.BitBlt(nLeft + j*nElemW, nTop + i*nElemH, nElemW, nElemH, &m_dcElment, 0, nElemVal * nElemH, SRCAND);
		}
	}
}

//绘制提示线
void CGameDlg::DrawTipLine(Vertex avPath[4], int &nVexnum)
{
	//获得DC
	CClientDC dc(this);

	//设置画笔
	CPen penLine(PS_SOLID, 2, RGB(0, 255, 0));

	//将画笔选入DC
	CPen* pOldPen = dc.SelectObject(&penLine);

	//绘制连线
	dc.MoveTo(m_ptGameTop.x + avPath[0].col*m_sizeElem.cx + m_sizeElem.cx / 2,
		m_ptGameTop.y + avPath[0].row*m_sizeElem.cy + m_sizeElem.cy / 2);
	for (int i = 1; i < nVexnum; i++)
	{
		dc.LineTo(m_ptGameTop.x + avPath[i].col*m_sizeElem.cx + m_sizeElem.cx / 2,
			m_ptGameTop.y + avPath[i].row*m_sizeElem.cy + m_sizeElem.cy / 2);
	}

	dc.SelectObject(pOldPen);
}

//绘制提示框
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

//绘制当前秒数
void CGameDlg::DrawGameTime()
{
	//创建DC
	CClientDC dc(this);
	//创建字体
	CFont font;
	//设置字体大小字体
	font.CreatePointFont(200, _T("Courier New"));

	CFont *oldFont;
	//将字体选入dc
	oldFont = dc.SelectObject(&font);
	//设置字体颜色与背景色
	dc.SetTextColor(RGB(255, 255, 255));
	dc.SetBkColor(RGB(255, 106, 106));

	//获取进度条组件位置
	CRect rect;
	GetDlgItem(IDC_GAME_TIME)->GetWindowRect(&rect);
	ScreenToClient(rect);
	//设置输出内容为进度
	CString time;
	time.Format(_T("%d"), m_GameProgress.GetPos());
	//设置输出位置为进度条旁
	dc.TextOut(rect.left + rect.Width() + 20, rect.top, time);

	//用空格控制保证每次输出文本长度一致
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

//判断是否获胜
void CGameDlg::JudgeWin()
{
	int bGameStatus = m_gameControl.IsWin(m_GameProgress.GetPos());
	//是否继续游戏
	if (bGameStatus == GAME_PLAY)
	{
		return;
	}
	else
	{
		//将游戏标识改为false
		m_bPlaying = false;
		//关闭计时器
		KillTimer(PLAY_TIMER_ID);
		//提示获胜
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
		//还原start按钮，使用按钮可以点击
		this->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);

	}
}

//定时器
void CGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == PLAY_TIMER_ID&&m_bPlaying && !m_bPause)
	{
		//运行时间减少一个步长
		m_GameProgress.StepIt();
		DrawGameTime();    //每过一秒重绘时间
		JudgeWin();           //每过一秒判断一次输赢
	}

	CDialogEx::OnTimer(nIDEvent);
}

//开始按钮
void CGameDlg::OnBnClickedBtnStart()
{
	//初始化游戏地图
	m_gameControl.StartGame();
	
	//更新界面
	UpdateMap();

	//更新窗口
	InvalidateRect(m_rtGameRect,FALSE);

	m_bPlaying = true;
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);

	//初始化进度条
	GetDlgItem(IDC_GAME_TIME)->ShowWindow(SW_SHOW);	//显示进度条
	m_GameProgress.SetRange(0, 60 * 5);		//初始化范围
	m_GameProgress.SetStep(-1);				//初始化步长
	m_GameProgress.SetPos(60 * 5);			//设置初始值

	//启动定时器
	this->SetTimer(PLAY_TIMER_ID, 1000, NULL);
	//绘制当前秒数
	DrawGameTime();
	
}

//停止按钮
void CGameDlg::OnBnClickedBtnStop()
{
	if (!m_bPlaying)
		return;
	if (m_bPause == false)
	{
		//暂停则绘制暂停背景图
		m_dcMem.BitBlt(50, 50, 640, 400, &m_dcPause, 50, 50, SRCCOPY);
		//将按钮名称改为继续
		SetDlgItemText(IDC_BTN_STOP, _T("Continue"));
		//设置暂停标识符
		m_bPause = true;

		InvalidateRect(m_rtGameRect, FALSE);
	}
	else
	{
		//若继续游戏则恢复地图
		UpdateMap();
		//将按钮名称改为暂停
		SetDlgItemText(IDC_BTN_STOP, _T("Pause"));
		//设置标识符
		m_bPause = false;
		InvalidateRect(m_rtGameRect, FALSE);
	}
}

//提示按钮
void CGameDlg::OnBnClickedBtnPrompt()
{
	Vertex v1, v2;
	if (m_bPlaying && !m_bPause&&m_gameControl.Help(v1, v2))
	{
		//画两个点的提示框
		DrawTipFrame(v2.row, v2.col);
		DrawTipFrame(v1.row, v1.col);

		//更新地图
		Sleep(200);
		UpdateMap();
		InvalidateRect(m_rtGameRect, FALSE);
	}

}

//重置按钮
void CGameDlg::OnBnClickedBtnReset()
{
	//如果游戏未开始或在暂停状态则结束函数
	if (!m_bPlaying || m_bPause)
		return;
	//重置地图
	m_gameControl.ResetMap();
	UpdateMap();
	InvalidateRect(m_rtGameRect, FALSE);
}

//点击鼠标
void CGameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 判断鼠标点击的区域
	if (point.x < m_rtGameRect.left+ m_sizeElem.cx || point.x > m_rtGameRect.right- m_sizeElem.cx || 
		point.y < m_rtGameRect.top+ m_sizeElem.cy || point.y > m_rtGameRect.bottom- m_sizeElem.cy)
	{
		return CDialogEx::OnLButtonUp(nFlags, point);
	}

	//判断是否为游戏未开始或暂停状态
	if (!m_bPlaying || m_bPause)
	{
		return CDialogEx::OnLButtonUp(nFlags, point);
	}

	//换算点击的坐标点
	int nRow = (point.y - m_ptGameTop.y) / m_sizeElem.cy;
	int nCol = (point.x - m_ptGameTop.x) / m_sizeElem.cx;

	//判断该坐标是否为空
	if (m_gameControl.GetElement(nRow, nCol) == BLANK)
	{
		return CDialogEx::OnLButtonUp(nFlags, point);
	}

	if (m_bFirstPoint)	//第一个点
	{
		//绘制提示框
		DrawTipFrame(nRow, nCol);
		m_gameControl.SetFirstPoint(nRow, nCol);
	}
	else				//第二个点
	{
		DrawTipFrame(nRow, nCol);
		m_gameControl.SetSecPoint(nRow, nCol);

		int nVexnum;
		Vertex avPath[4];
		//连子判断
		if (m_gameControl.Link(avPath,nVexnum))
		{
			//画提示线
			DrawTipLine(avPath,nVexnum);
			//清除
			UpdateMap();

			//每次更新地图判断游戏胜负
			JudgeWin();
			Vertex v1, v2;
			if (!m_gameControl.Help(v1, v2))
			{
				//更新地图
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
