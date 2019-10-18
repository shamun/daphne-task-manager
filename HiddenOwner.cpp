// HiddenOwner.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "HiddenOwner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHiddenOwner dialog


CHiddenOwner::CHiddenOwner(CWnd* pParent /*=NULL*/)
	: CDialog(CHiddenOwner::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHiddenOwner)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHiddenOwner::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHiddenOwner)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHiddenOwner, CDialog)
	//{{AFX_MSG_MAP(CHiddenOwner)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHiddenOwner message handlers
