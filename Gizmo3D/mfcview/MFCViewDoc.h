// MFCViewDoc.h : interface of the CMFCViewDoc class
//


#pragma once


class CMFCViewDoc : public CDocument
{
protected: // create from serialization only
	CMFCViewDoc();
	DECLARE_DYNCREATE(CMFCViewDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual void Serialize(CArchive& ar);
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    CString GetFileName() const { return m_csFileName; }

// Implementation
public:
	virtual ~CMFCViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    CString m_csFileName;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


