
// KRS6003_Controller.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CKRS6003_ControllerApp:
// このクラスの実装については、KRS6003_Controller.cpp を参照してください。
//

class CKRS6003_ControllerApp : public CWinApp
{
public:
	CKRS6003_ControllerApp();

// オーバーライド
public:
	virtual BOOL InitInstance();
	
// 実装

	DECLARE_MESSAGE_MAP()
};

extern CKRS6003_ControllerApp theApp;