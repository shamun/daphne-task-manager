// LineGraph.cpp : implementation file
//

#include "stdafx.h"
#include "Daphne.h"
#include "LineGraph.h"
#include "memdc.h"


// CLineGraph

IMPLEMENT_DYNAMIC(CLineGraph, CStatic)

CLineGraph::CLineGraph()
: maxs(0)
, mins(0)
{
  fondo.CreateSolidBrush(0x00000000);
  grilla.CreatePen(PS_SOLID, 1, 0x888888);
  values.CreatePen(PS_SOLID, 1, 0x00FF00);
  average.CreatePen(PS_SOLID, 1, 0xFF0000);
}

CLineGraph::~CLineGraph()
{
}


BEGIN_MESSAGE_MAP(CLineGraph, CStatic)
  ON_WM_PAINT()
//  ON_WM_CREATE()
//  ON_WM_SHOWWINDOW()
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CLineGraph message handlers

void CLineGraph::OnPaint()
{
  int x, y;
  CPaintDC rawdc(this); // device context for painting
  GetClientRect(&area);
  Keith::CMemDC dc(&rawdc, &area);

  float dh = (float)area.Height() / 10;
  float dw = (float)area.Width() / 50;

  dc.FillRect(area, &fondo);
  dc.SelectObject(grilla);
  for(x = 1; x<=50; x++) {
    dc.MoveTo((int)(x*dw), 0);
    dc.LineTo((int)(x*dw), area.Height());
  }
  for(y = 1; y<=10; y++) {
    dc.MoveTo(0, (int)(y*dh));
    dc.LineTo(area.Width(), (int)(y*dh));
  }

  dc.MoveTo(0, area.Height());
  int sum = 0;
  int count = 0;
  int lastv = 0, lasta = 0;
  if (WaitForSingleObject(mutex, 150) == WAIT_OBJECT_0) {
    std::list<int>::iterator i = source->begin();
    for(x=0; x < (int)(source->size() - area.Width() / tfactor); ++x)
      ++i;
    for(x=0; (x < area.Width()) && (i != source->end()); x+=tfactor) {
      int d = *i;
      ++i;
      ++count;
      sum += d;
      dc.SelectObject(values);
      dc.MoveTo(x-tfactor, lastv);
      lastv = (int)((float)(d-mins) * (float)((float)area.Height() / (float)(maxs-mins)));
      dc.LineTo(x, lastv);
      dc.SelectObject(average);
      dc.MoveTo(x-tfactor, lasta);
      lasta = (int)((float)((sum/count)-mins) * (float)((float)area.Height() / (float)(maxs-mins)));
      dc.LineTo(x, lasta);
    }
    ReleaseMutex(hRefreshMutex);
	}

}

void CLineGraph::initialize(std::list<int> * s, int maxs, int mins, HANDLE mutex, int tfactor)
{
  this->source = s;
  this->maxs = maxs;
  this->mins = mins;
  this->mutex = mutex;
  this->tfactor = tfactor;
}

void CLineGraph::expandTime(void)
{
  if (tfactor < 20) ++tfactor;
  Invalidate(0);
}

void CLineGraph::collapseTime(void)
{
  if (tfactor > 1) --tfactor;
  Invalidate(0);
}

int CLineGraph::getTFactor(void)
{
  return tfactor;
}
