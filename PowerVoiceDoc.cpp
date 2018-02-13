// PowerVoiceDoc.cpp : implementation of the CPowerVoiceDoc class
//

#include "stdafx.h"
#include "PowerVoice.h"

#include "PowerVoiceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPowerVoiceDoc

IMPLEMENT_DYNCREATE(CPowerVoiceDoc, CDocument)

BEGIN_MESSAGE_MAP(CPowerVoiceDoc, CDocument)
	//{{AFX_MSG_MAP(CPowerVoiceDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPowerVoiceDoc construction/destruction

CPowerVoiceDoc::CPowerVoiceDoc()
{
	// TODO: add one-time construction code here

}

CPowerVoiceDoc::~CPowerVoiceDoc()
{
}


/////////////////////////////////////////////////////////////////////////////
// CPowerVoiceDoc diagnostics

#ifdef _DEBUG
void CPowerVoiceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPowerVoiceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPowerVoiceDoc commands
