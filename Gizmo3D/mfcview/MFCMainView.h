// MFCMainView.h : interface of the CMFCMainView class
//

#pragma once

class CGizmoView;

class CMFCMainView : public CView
{
protected: // create from serialization only
	CMFCMainView();
	DECLARE_DYNCREATE(CMFCMainView)

// Attributes
public:
	CMFCViewDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMFCMainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	CGizmoView *m_GizmoView;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg BOOL OnEraseBkgnd( CDC *pDC );
};

#ifndef _DEBUG  // debug version in MFCMainView.cpp
inline CMFCViewDoc* CMFCMainView::GetDocument() const
   { return reinterpret_cast<CMFCViewDoc*>(m_pDocument); }
#endif

