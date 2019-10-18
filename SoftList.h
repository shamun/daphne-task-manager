#pragma once


// CSoftList

class CSoftList : public CListCtrl
{
	DECLARE_DYNAMIC(CSoftList)

public:
	CSoftList();
	virtual ~CSoftList();

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
};


