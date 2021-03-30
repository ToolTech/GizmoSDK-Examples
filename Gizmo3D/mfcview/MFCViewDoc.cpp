// MFCViewDoc.cpp : implementation of the CMFCViewDoc class
//

#include "stdafx.h"
#include "MFCView.h"

#include "MFCViewDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCViewDoc

IMPLEMENT_DYNCREATE(CMFCViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCViewDoc, CDocument)
END_MESSAGE_MAP()


// CMFCViewDoc construction/destruction

CMFCViewDoc::CMFCViewDoc()
{
	// TODO: add one-time construction code here

}

CMFCViewDoc::~CMFCViewDoc()
{
}

BOOL CMFCViewDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    m_csFileName = lpszPathName;

    if (!CDocument::OnOpenDocument(lpszPathName))
      return FALSE;

    return TRUE;
}




// CMFCViewDoc serialization

void CMFCViewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CMFCViewDoc diagnostics

#ifdef _DEBUG
void CMFCViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCViewDoc commands
