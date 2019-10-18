/***********************************************************************************
	
	Daphne -- GPL task manager replacement for Windows
	Copyright (C) 2005  Leandro H. Fernández

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

***********************************************************************************/

// ProcList.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "ProcList.h"
#include "DaphneDlg.h"
#include "memdc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcList

CProcList::CProcList()
{
	m_rectClient.SetRectEmpty();
}

CProcList::~CProcList()
{
}


BEGIN_MESSAGE_MAP(CProcList, CListCtrl)
	//{{AFX_MSG_MAP(CProcList)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeydown)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcList message handlers


BOOL CProcList::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);

	return TRUE;
}

void CProcList::OnPaint() 
{
	CPaintDC dc(this);

	// Paint to a memory device context to reduce screen flicker.
	Keith::CMemDC memDC(&dc, &m_rectClient, CDaphneApp::rtl);

	// Let the window do its default painting...
	CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);
	//CWnd::DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);
}

void CProcList::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
	
    GetClientRect(m_rectClient);

    CHeaderCtrl* pHC;
    pHC = GetHeaderCtrl();
    if (pHC != NULL)
    {
        CRect rectHeader;
        pHC->GetItemRect( 0, &rectHeader );
        m_rectClient.top += rectHeader.bottom;
    }	
}

void CProcList::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	CDaphneDlg * parent = (CDaphneDlg *)GetParent();

	if (parent->orderColumn == pNMListView->iSubItem) {
		if (parent->orderMode)
			parent->orderMode = 0;
		else
			parent->orderMode = 1;
	}
	else {
		parent->orderColumn = pNMListView->iSubItem;
		parent->orderMode = 0;
	}

	parent->RefreshList();

	if (parent->orderColumn == 1)
		parent->ShowBalloon(_("Quick search by process name enabled."));
	else
		parent->ShowBalloon(_("Quick search by process name disabled."));

	*pResult = 0;
}

void CProcList::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	CDaphneDlg * parent = (CDaphneDlg *)GetParent();

	if (parent->orderColumn == 1) {

		if (((pLVKeyDow->wVKey >= 0x41) && (pLVKeyDow->wVKey <= 0x5A)) ||
			((pLVKeyDow->wVKey >= 0x30) && (pLVKeyDow->wVKey <= 0x39)) ||
			(pLVKeyDow->wVKey == VK_BACK)) {

			if (pLVKeyDow->wVKey == VK_BACK) {
				if (m_strSearch.GetLength())
					m_strSearch.Delete(m_strSearch.GetLength() - 1, 1);
			}
			else
				m_strSearch += (char)pLVKeyDow->wVKey;

			parent->selectProcess(m_strSearch);

			if (isWinXP()) {
				CString str;
				str.Format(_("Highlighting %s..."), m_strSearch);
				parent->ShowBalloon(str);
			}
		}
	}

  // Handle DELETE
  if (pLVKeyDow->wVKey == VK_DELETE)
    parent->killCurrentProcess();
	
	*pResult = 0;
}

void CProcList::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult) {

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	CDaphneDlg * parent = (CDaphneDlg *)GetParent();

	*pResult = CDRF_DODEFAULT;

	if ((pLVCD != NULL) && (parent != NULL)) {

		if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage ) {
			*pResult = CDRF_NOTIFYITEMDRAW;
		}
		else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage ) {

			*pResult = parent->itemLookAndFeel(pLVCD);
		}
	}
}

void CProcList::clearSearchPattern(void)
{
  m_strSearch.Empty();
}
