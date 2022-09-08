// PrinceGameDlg.h : header file
//

#if !defined(AFX_PRINCEGAMEDLG_H__6A801155_8428_4770_BAAB_056737A651C3__INCLUDED_)
#define AFX_PRINCEGAMEDLG_H__6A801155_8428_4770_BAAB_056737A651C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPrinceGameDlg dialog

class CPrinceGameDlg : public CDialog
{
// Construction
public:
	///////////////////////////////////////////////////////////////////////
	///////////////////////     Prince Game     ///////////////////////////
	///////////////////////////////////////////////////////////////////////

	void InitGame();					// ���� �ʱ�ȭ
	void LoadSkin();					// ���̾� �α� ��Ų
	void RealDrawObject();				// ���� ��ǥ�� �׸�
	void WaitTime();					// ���ð�

	int GetOtherType(int nType);		// �ٸ� �÷��̾�� �ٲٱ�
	BOOL CheckGameOver();				// GameOver ���� üũ
	void GameOver();					// GameOver �϶� ����

	HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance);
	BOOL BitmapRgn(UINT resource, COLORREF TansColor);

	///////////////////////////////////////////////////////////////////////

	CPrinceGameDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPrinceGameDlg)
	enum { IDD = IDD_PRINCEGAME_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrinceGameDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	///////////////////////////////////////////////////////////////////////
	///////////////////////     Prince Game     ///////////////////////////
	///////////////////////////////////////////////////////////////////////

	int m_nOldX, m_nOldY;			// ���� ���� ���� ��ġ
	int m_nCurX, m_nCurY;			// ������ �̵��� ���� ��ġ
	CPoint m_pt;					// MouseMove �̺�Ʈ�� ���� ���콺 ��ǥ

	int m_nCurType;					// ���� �÷��̾�
	int m_nStats;					// Ŭ�� ����
	int m_nPlayer;					// �÷��̾� ��
	int m_nRand;					// �������� ���� ���� ��
	int m_nWait;					// �ٲ� �ð�
	BOOL m_bCheckMove;				// ���� MouseMove �̺�Ʈ ����
	BOOL m_bCheckItem;				// �������� üũ
	BOOL m_bCheckWait;				// ���ð� üũ
	BOOL m_bCheckType;				// Type�� ���ϱ��� üũ 
	BOOL m_bCheckItemCount;			// Item Count üũ
	BOOL m_bCheckWaitAni;			// ���ð� �ִϸ��̼� üũ
	BOOL m_bCheckWaitCount;			// Wait Count üũ
	BOOL m_bCheckStart;				// Start üũ
	BOOL m_bCheckStartCount;		// Start Count üũ
	BOOL m_bCheckGameOver;			// RealDraw�� ���� ���ӿ��� üũ
	BOOL m_bCheckOverCount;			// Over Count üũ
	BOOL m_bCheckFire;				// ���ӿ��� Fire üũ
	BOOL m_bCheckFireCount;			// Fire Count üũ

	BOOL m_bCurItem[5];				// �ѹ��� ������ ����
	BOOL m_bEventItem[5];			// ������ �߻�
	int m_nGame[29][21];			// ������ ����
	CRect m_rectArray[29][21];		// �� ���� �簢�� ��ǥ

	CBitmap m_bmpDlg;				// ���̾�α� ��Ų �̹���
	CBitmap m_bmpRed[4];			// RED ���� �̹���
	CBitmap m_bmpYellow[4];			// YELLOW ���� �̹���
	CBitmap m_bmpGreen[4];			// GREEN ���� �̹���
	CBitmap m_bmpCyan[4];			// CYAN ���� �̹���
	CBitmap m_bmpBlue[4];			// BLUE ���� �̹���
	CBitmap m_bmpPink[4];			// PINK ���� �̹���
	CBitmap m_bmpMask[4];			// MASK �̹���

	CBitmap m_bmpItem[4];			// ITEM �̹���
	CBitmap m_bmpItemMask[4];		// ITEM Mask �̹���
	CBitmap m_bmpWait;				// Wait Change �̹���
	CBitmap m_bmpWaitMask;			// Wait Mask Change �̹���
	CBitmap m_bmpStart;				// Strat �̹���
	CBitmap m_bmpStartMask;			// Start Mask �̹���
	CBitmap m_bmpOver;				// Game Over �̹���
	CBitmap m_bmpOverMask;			// Game Over Mask �̹���
	CBitmap m_bmpFire;				// Fire �̹���

	///////////////////////////////////////////////////////////////////////

	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPrinceGameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINCEGAMEDLG_H__6A801155_8428_4770_BAAB_056737A651C3__INCLUDED_)
