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

	void InitGame();					// 게임 초기화
	void LoadSkin();					// 다이얼 로그 스킨
	void RealDrawObject();				// 실제 좌표에 그림
	void WaitTime();					// 대기시간

	int GetOtherType(int nType);		// 다른 플레이어로 바꾸기
	BOOL CheckGameOver();				// GameOver 인지 체크
	void GameOver();					// GameOver 일때 실행

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

	int m_nOldX, m_nOldY;			// 이전 상태 말의 위치
	int m_nCurX, m_nCurY;			// 구슬이 이동된 후의 위치
	CPoint m_pt;					// MouseMove 이벤트시 이전 마우스 좌표

	int m_nCurType;					// 현제 플레이어
	int m_nStats;					// 클릭 상태
	int m_nPlayer;					// 플레이어 수
	int m_nRand;					// 아이템을 위한 랜덤 수
	int m_nWait;					// 바뀔 시간
	BOOL m_bCheckMove;				// 말이 MouseMove 이벤트 반응
	BOOL m_bCheckItem;				// 아이템전 체크
	BOOL m_bCheckWait;				// 대기시간 체크
	BOOL m_bCheckType;				// Type이 변하근지 체크 
	BOOL m_bCheckItemCount;			// Item Count 체크
	BOOL m_bCheckWaitAni;			// 대기시간 애니메이션 체크
	BOOL m_bCheckWaitCount;			// Wait Count 체크
	BOOL m_bCheckStart;				// Start 체크
	BOOL m_bCheckStartCount;		// Start Count 체크
	BOOL m_bCheckGameOver;			// RealDraw를 위한 게임오버 체크
	BOOL m_bCheckOverCount;			// Over Count 체크
	BOOL m_bCheckFire;				// 게임오버 Fire 체크
	BOOL m_bCheckFireCount;			// Fire Count 체크

	BOOL m_bCurItem[5];				// 한번쓴 아이템 상태
	BOOL m_bEventItem[5];			// 아이템 발생
	int m_nGame[29][21];			// 말판의 상태
	CRect m_rectArray[29][21];		// 각 말판 사각형 좌표

	CBitmap m_bmpDlg;				// 다이얼로그 스킨 이미지
	CBitmap m_bmpRed[4];			// RED 구슬 이미지
	CBitmap m_bmpYellow[4];			// YELLOW 구슬 이미지
	CBitmap m_bmpGreen[4];			// GREEN 구슬 이미지
	CBitmap m_bmpCyan[4];			// CYAN 구슬 이미지
	CBitmap m_bmpBlue[4];			// BLUE 구슬 이미지
	CBitmap m_bmpPink[4];			// PINK 구슬 이미지
	CBitmap m_bmpMask[4];			// MASK 이미지

	CBitmap m_bmpItem[4];			// ITEM 이미지
	CBitmap m_bmpItemMask[4];		// ITEM Mask 이미지
	CBitmap m_bmpWait;				// Wait Change 이미지
	CBitmap m_bmpWaitMask;			// Wait Mask Change 이미지
	CBitmap m_bmpStart;				// Strat 이미지
	CBitmap m_bmpStartMask;			// Start Mask 이미지
	CBitmap m_bmpOver;				// Game Over 이미지
	CBitmap m_bmpOverMask;			// Game Over Mask 이미지
	CBitmap m_bmpFire;				// Fire 이미지

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
