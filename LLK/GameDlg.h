#pragma once
#include"global.h"
#include"GameControl.h"
#include "afxcmn.h"

// CGameDlg �Ի���

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	HICON m_hIcon;		//ϵͳͼ��
	CDC m_dcMem;		//�ڴ�DC
	CDC m_dcBG;			//����DC
	CDC m_dcElment;		//Ԫ���ڴ�DC
	CDC m_dcMask;		//�����ڴ�DC
	CDC m_dcPause;		//��ͣ�ڴ�DC

	CPoint m_ptGameTop;	//��Ϸ����ʼ�㣨��Ϸ��һ��ͼƬ�Ķ������꣬��������ڴ��ڿͻ�����
	CSize m_sizeElem;	//Ԫ��ͼƬ�Ĵ�С
	CRect m_rtGameRect;	//��Ϸ�����С��������Ϊ��Ϸ���������С�����ǵ�������ߣ����ܻᳬ��ͼƬһ����Χ��

	CGameControl m_gameControl;		//��Ϸ������
	CProgressCtrl m_GameProgress;	//������

	bool m_bFirstPoint;	//�Ƿ�Ϊ��һ����
	bool m_bPlaying;	//�ж��Ƿ����
	bool m_bPause;		//�ж��Ƿ���ͣ

protected:
	//��ʼ�����ڱ������С
	void InitBackground();
	//��ʼ��ͼƬԪ��
	void InitElement();
	//���´���
	void UpdateWindow();
	//���µ�ͼ
	void UpdateMap();
	//������ʾ��
	void DrawTipLine(Vertex avPath[4], int &nVexnum);
	//������ʾ��
	void DrawTipFrame(int nRow, int nCol);
	//���Ƶ�ǰ����
	void DrawGameTime();
	//�ж��Ƿ��ʤ
	void JudgeWin();

public:	
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//��ʼ��ť
	afx_msg void OnBnClickedBtnStart();
	//ֹͣ��ť
	afx_msg void OnBnClickedBtnStop();
	//��ʾ��ť
	afx_msg void OnBnClickedBtnPrompt();
	//���ð�ť
	afx_msg void OnBnClickedBtnReset();
	//������
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

};
