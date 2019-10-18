// ScheduledTasksDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "ScheduledTasksDlg.h"


// CScheduledTasksDlg dialog

IMPLEMENT_DYNAMIC(CScheduledTasksDlg, CDialog)

CScheduledTasksDlg::CScheduledTasksDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScheduledTasksDlg::IDD, pParent)
{

}

CScheduledTasksDlg::~CScheduledTasksDlg()
{
}

void CScheduledTasksDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TASKS, tasks);
}


BEGIN_MESSAGE_MAP(CScheduledTasksDlg, CDialog)
	ON_NOTIFY(NM_RCLICK, IDC_TASKS, OnRclickList)
END_MESSAGE_MAP()


// CScheduledTasksDlg message handlers

BOOL CScheduledTasksDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

	SetWindowText(_(" Scheduled tasks"));

	tasks.InsertColumn(0, L"Text", LVCFMT_LEFT, 230, -1);

	tasks.SetExtendedStyle(LVS_EX_FULLROWSELECT);

  reload();

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CScheduledTasksDlg::reload()
{
  tasks.DeleteAllItems();

  CString str;
  TaskMap::iterator now = schedule->begin();
  while (now != schedule->end()) {
    std::list<Task*>::iterator it;
    for (it = now->second.begin(); it != now->second.end(); it++) {
      int index = tasks.InsertItem(0, (*it)->toString());
      tasks.SetItemData(index, (DWORD_PTR)(*it));
    }
    ++now;
  }

  tasks.SortItems(compare, NULL);
}

int CALLBACK CScheduledTasksDlg::compare(LPARAM left, LPARAM right, LPARAM sort)
{
  Task * l = (Task*)left;
  Task * r = (Task*)right;

  if (l->time < r->time)
    return -1;
  else if (l->time == r->time)
    return 0;
  else if (l->time > r->time)
    return 1;

  return 0;
}

void CScheduledTasksDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu menu;
	POINT point;

  if (GetSelectedTask() != NULL) {
	  menu.CreatePopupMenu();
	  menu.InsertMenu(0, 0, IDM_TASK_REMOVE, _("Remove task"));
	  menu.SetDefaultItem(IDM_TASK_REMOVE);

	  GetCursorPos(&point);

	  menu.TrackPopupMenu(0, point.x, point.y, this);
  }

	*pResult = 0;
}

Task * CScheduledTasksDlg::GetSelectedTask()
{
	POSITION pos = tasks.GetFirstSelectedItemPosition();

  return (Task*)((pos>0)?tasks.GetItemData((int)(pos-1)):NULL);
}

BOOL CScheduledTasksDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	WORD item = LOWORD(wParam);
  CString msg;

	switch(item) {
	case IDM_TASK_REMOVE:
    (*schedule)[GetSelectedTask()->time].remove(GetSelectedTask());
    delete GetSelectedTask();
    MessageBox(_("Task removed."));
    reload();
		break;
  }
	return CDialog::OnCommand(wParam, lParam);
}
