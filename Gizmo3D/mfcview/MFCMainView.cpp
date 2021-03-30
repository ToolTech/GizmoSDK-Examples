// MFCMainView.cpp : implementation of the CMFCMainView class
//

#include "stdafx.h"
#include "mfcview.h"
#include "GizmoView.h"

#include "mfcviewDoc.h"
#include "MFCMainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCMainView

IMPLEMENT_DYNCREATE(CMFCMainView, CView)

BEGIN_MESSAGE_MAP(CMFCMainView, CView)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND( )
END_MESSAGE_MAP()

// CMFCMainView construction/destruction

CMFCMainView::CMFCMainView():m_GizmoView(NULL)
{
	// TODO: add construction code here

}

CMFCMainView::~CMFCMainView()
{
}

BOOL CMFCMainView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CMFCMainView drawing

void CMFCMainView::OnDraw(CDC* /*pDC*/)
{
	CMFCViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMFCMainView diagnostics

#ifdef _DEBUG
void CMFCMainView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCMainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCViewDoc* CMFCMainView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCViewDoc)));
	return (CMFCViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCMainView message handlers

int CMFCMainView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_GizmoView = CGizmoView::createInstance(m_hWnd);

    return 1;
}

void CMFCMainView::OnDestroy()
{
	CGizmoView::destroyInstance(m_GizmoView);

    // Destroy Window
    CView::OnDestroy();
}

void CMFCMainView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    // Get Filename from DocumentOpen Dialog
    CString csFileName = GetDocument()->GetFileName();

    m_GizmoView->load(csFileName);
}

void CMFCMainView::OnSize( UINT nType, int cx, int cy )
{
	m_GizmoView->resize(cx,cy);
}

void CMFCMainView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // Pass Key Presses into Gizmo

	m_GizmoView->onKeyDown(nChar);

    // Close Window on Escape Key
    if(nChar == VK_ESCAPE)
    {
        GetParent()->SendMessage(WM_CLOSE);
    }
}

void CMFCMainView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // Pass Key Presses into Gizmo
	m_GizmoView->onKeyUp(nChar);
}

BOOL CMFCMainView::OnEraseBkgnd( CDC *pDC )
{
	// Just return TRUE to skip background erase
	return TRUE;
}
