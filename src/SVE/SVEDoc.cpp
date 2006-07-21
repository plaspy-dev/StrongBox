// SVEDoc.cpp: implementación de la clase CSVEDoc
//

#include "stdafx.h"
#include "SVE.h"

#include "SVEDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSVEDoc

IMPLEMENT_DYNCREATE(CSVEDoc, CDocument)

BEGIN_MESSAGE_MAP(CSVEDoc, CDocument)
END_MESSAGE_MAP()


// Construcción o destrucción de CSVEDoc

CSVEDoc::CSVEDoc()
{
	// TODO: agregar aquí el código de construcción único

}

CSVEDoc::~CSVEDoc()
{
}

BOOL CSVEDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: agregar aquí código de reinicio
	// (los documentos SDI volverán a utilizar este documento)

	return TRUE;
}




// Serialización de CSVEDoc

void CSVEDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: agregar aquí el código de almacenamiento
	}
	else
	{
		// TODO: agregar aquí el código de carga
	}
}


// Diagnósticos de CSVEDoc

#ifdef _DEBUG
void CSVEDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSVEDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Comandos de CSVEDoc
