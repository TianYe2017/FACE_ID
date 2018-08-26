
// FACE_ID_BaseLine1_0.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "global_var.h"

// CFACE_ID_BaseLine1_0App:
// See FACE_ID_BaseLine1_0.cpp for the implementation of this class
//

class CFACE_ID_BaseLine1_0App : public CWinApp
{
public:
	CFACE_ID_BaseLine1_0App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFACE_ID_BaseLine1_0App theApp;