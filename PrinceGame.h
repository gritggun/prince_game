// PrinceGame.h : main header file for the PRINCEGAME application
//

#if !defined(AFX_PRINCEGAME_H__5AA43E46_DA33_4B69_BD3B_C97A91D7E867__INCLUDED_)
#define AFX_PRINCEGAME_H__5AA43E46_DA33_4B69_BD3B_C97A91D7E867__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPrinceGameApp:
// See PrinceGame.cpp for the implementation of this class
//

class CPrinceGameApp : public CWinApp
{
public:
	CPrinceGameApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrinceGameApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPrinceGameApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINCEGAME_H__5AA43E46_DA33_4B69_BD3B_C97A91D7E867__INCLUDED_)
