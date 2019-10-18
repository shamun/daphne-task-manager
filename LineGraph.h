#pragma once
#include <list>

// CLineGraph

class CLineGraph : public CStatic
{
	DECLARE_DYNAMIC(CLineGraph)
private:
  std::list<int> * source;
  int maxs;
  int mins;
  int tfactor;
  CBrush fondo;
  CPen grilla;
  CPen values;
  CPen average;
  CRect area;
  HANDLE mutex;

public:
	CLineGraph();
	virtual ~CLineGraph();

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnPaint();
  void initialize(std::list<int> * s, int max, int min, HANDLE mutex, int tfactor = 1);
  void expandTime(void);
  void collapseTime(void);
  int getTFactor(void);
};


