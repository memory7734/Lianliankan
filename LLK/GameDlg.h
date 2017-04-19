#pragma once
#include"global.h"
#include"GameControl.h"
#include "afxcmn.h"

// CGameDlg 对话框

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGameDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	HICON m_hIcon;		//系统图标
	CDC m_dcMem;		//内存DC
	CDC m_dcBG;			//背景DC
	CDC m_dcElment;		//元素内存DC
	CDC m_dcMask;		//掩码内存DC
	CDC m_dcPause;		//暂停内存DC

	CPoint m_ptGameTop;	//游戏区起始点（游戏第一张图片的顶点坐标，坐标相对于窗口客户区）
	CSize m_sizeElem;	//元素图片的大小
	CRect m_rtGameRect;	//游戏区域大小（该区域为游戏更新区域大小，考虑到后面的线，可能会超出图片一定范围）

	CGameControl m_gameControl;		//游戏控制类
	CProgressCtrl m_GameProgress;	//进度条

	bool m_bFirstPoint;	//是否为第一个点
	bool m_bPlaying;	//判断是否进行
	bool m_bPause;		//判断是否暂停

protected:
	//初始化窗口背景与大小
	void InitBackground();
	//初始化图片元素
	void InitElement();
	//更新窗口
	void UpdateWindow();
	//更新地图
	void UpdateMap();
	//绘制提示线
	void DrawTipLine(Vertex avPath[4], int &nVexnum);
	//绘制提示框
	void DrawTipFrame(int nRow, int nCol);
	//绘制当前秒数
	void DrawGameTime();
	//判断是否获胜
	void JudgeWin();

public:	
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//开始按钮
	afx_msg void OnBnClickedBtnStart();
	//停止按钮
	afx_msg void OnBnClickedBtnStop();
	//提示按钮
	afx_msg void OnBnClickedBtnPrompt();
	//重置按钮
	afx_msg void OnBnClickedBtnReset();
	//点击鼠标
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

};
