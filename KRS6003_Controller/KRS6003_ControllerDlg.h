
// KRS6003_ControllerDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"
#include <vector>
#include "CommPort.h"
#include "IMUZ.h"
#include"GLPictureCtrl.h"
#define IMUZ_BUFF_SIZE 200

using namespace std;

// CKRS6003_ControllerDlg ダイアログ
class CKRS6003_ControllerDlg : public CDialogEx
{
// コンストラクション
public:
	CKRS6003_ControllerDlg(CWnd* pParent = NULL);	// 標準コンストラクター
	
// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KRS6003_CONTROLLER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedKrs6003C();
	CCommPort com;
	CStatic m_6003_st;
	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CScrollBar m_sbar1;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_m1a;
	CString	TextString;
	CScrollBar m_sbar2;
	CStatic m_m2a;
	afx_msg void OnBnClickedKrs6003Dc();
	CStatic m_flag0;
	CStatic m_flag1;
	CStatic m_flag2;
	CStatic m_flag3;
	CStatic m_flagnum;
	CScrollBar m_sbar3;
	CScrollBar m_sbar4;
	CStatic m_m3a;
	CStatic m_m4a;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	vector<vector<double>> gyro_now; //dps
	vector<vector<double>> acc_now; //dps
	vector<vector<double>> compass_now; //dps

	vector<vector<double>> gyro_nowL; //dps
	vector<vector<double>> acc_nowL; //dps
	vector<vector<double>> compass_nowL; //dps
	int GetGyroData(vector<vector<double>> &gyro, vector<vector<double>> &acc, vector<vector<double>> &compas);
	void GOPEN();
	CIMUZ imuz;
	CIMUZ imuzL;//second
	BYTE imuz_buff[IMUZ_BUFF_SIZE]; //生データ
	BYTE imuz_buffL[IMUZ_BUFF_SIZE]; //生データ

	bool isComGyroOpenSuccess;
	bool isComGyroOpenSuccessL;//second
	double CAX, PCAX, PAVX, PAX;
	double ROLL_ACC;
	double AX;
	CStringArray COMstrArray;
	CComboBox m_CfGcombo;
	CStatic m_GYRO_st;
	CCommPort comforgyro;
	void  GetAngleHioki();
	double k; //相補フィルタ係数
	CStatic m_Gyro;
	CGLPictureCtrl m_xc_WAKIPIC;
	vector<double> axbuff;
	int ax_pt;
	int axbuffsize;
	afx_msg void OnBnClickedButton3();
};
