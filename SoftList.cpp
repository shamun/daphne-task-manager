// SoftList.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "SoftList.h"
#include "lssoftDlg.h"

// CSoftList

IMPLEMENT_DYNAMIC(CSoftList, CListCtrl)

CSoftList::CSoftList()
{

}

CSoftList::~CSoftList()
{
}


BEGIN_MESSAGE_MAP(CSoftList, CListCtrl)
  ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CSoftList::OnLvnColumnclick)
END_MESSAGE_MAP()



// CSoftList message handlers



void CSoftList::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	ClssoftDlg * parent = (ClssoftDlg *)GetParent();

	if (parent->column == pNMLV->iSubItem) {
		if (parent->inverted)
			parent->inverted = false;
		else
			parent->inverted = true;
	}
	else {
		parent->column = pNMLV->iSubItem;
		parent->inverted = false;
	}

	parent->reloadList();

  *pResult = 0;
}
