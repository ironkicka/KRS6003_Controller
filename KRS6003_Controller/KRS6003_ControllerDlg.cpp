
// KRS6003_ControllerDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KRS6003_Controller.h"
#include "KRS6003_ControllerDlg.h"
#include "afxdialogex.h"
#include "CommPort.h"
#include "IMUZ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKRS6003_ControllerDlg �_�C�A���O



CKRS6003_ControllerDlg::CKRS6003_ControllerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_KRS6003_CONTROLLER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKRS6003_ControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_6003_ST, m_6003_st);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_sbar1);
	DDX_Control(pDX, IDC_M1A, m_m1a);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_sbar2);
	DDX_Control(pDX, IDC_M2A, m_m2a);
	DDX_Control(pDX, IDC_flagst0, m_flag0);
	DDX_Control(pDX, IDC_flagst1, m_flag1);
	DDX_Control(pDX, IDC_flagst2, m_flag2);
	DDX_Control(pDX, IDC_flagst3, m_flag3);
	DDX_Control(pDX, IDC_flagnum, m_flagnum);
	DDX_Control(pDX, IDC_SCROLLBAR3, m_sbar3);
	DDX_Control(pDX, IDC_SCROLLBAR4, m_sbar4);
	DDX_Control(pDX, IDC_M3A, m_m3a);
	DDX_Control(pDX, IDC_M4A, m_m4a);
	DDX_Control(pDX, IDC_COMBO1, m_CfGcombo);
	DDX_Control(pDX, IDC_gyrost, m_GYRO_st);
	DDX_Control(pDX, IDC_gyrost2, m_Gyro);
	DDX_Control(pDX, IDC_WAKIPIC, m_xc_WAKIPIC);
}

BEGIN_MESSAGE_MAP(CKRS6003_ControllerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_KRS6003_C, &CKRS6003_ControllerDlg::OnBnClickedKrs6003C)
	
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_KRS6003_DC, &CKRS6003_ControllerDlg::OnBnClickedKrs6003Dc)
	ON_BN_CLICKED(IDC_BUTTON1, &CKRS6003_ControllerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CKRS6003_ControllerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CKRS6003_ControllerDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CKRS6003_ControllerDlg ���b�Z�[�W �n���h���[

BOOL CKRS6003_ControllerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	ShowWindow(SW_MAXIMIZE);
	/////////////
	

	m_sbar1.SetScrollRange(0,180);
	m_sbar2.SetScrollRange(0,180);
	m_sbar3.SetScrollRange(0, 180);
	m_sbar4.SetScrollRange(0, 180);

	m_sbar1.SetScrollPos(90);
	m_sbar2.SetScrollPos(90);
	m_sbar3.SetScrollPos(90);
	m_sbar4.SetScrollPos(90);

	com.flag3 = 0;
	//com.ChangeSpeed(3,10);
	

	ax_pt = 0;
	axbuffsize = 1000 ;
	axbuff.clear();
	axbuff.resize(axbuffsize);

	///////////////////
	//COM�|�[�g���X�g�쐬 �E���p
	comforgyro.ComList(COMstrArray);
	//�R���{�{�b�N�X������
	m_CfGcombo.ResetContent();//�����ƃR���g���[���ϐ���ǉ����܂��傤�D
	//�����Ƀ��X�g��ǉ�
	for (int i = 0; i<COMstrArray.GetSize(); i++) { m_CfGcombo.InsertString(-1, COMstrArray[i]); }
	//for (int i = 0; i<COMstrArray.GetSize(); i++) { m_CfGcombo.AddString(COMstrArray[i]); }
	//�R���{�{�b�N�X�����l�I��
	m_CfGcombo.SetCurSel(0);


	imuz.IMUflag =0;

	////////////////
	SetTimer(1, 10, NULL);//�^�C�}�[�J�n

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

void CKRS6003_ControllerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CKRS6003_ControllerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CKRS6003_ControllerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKRS6003_ControllerDlg::OnTimer(UINT_PTR nIDEvent)
{
	int angle;
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	//TextString.Format(_T("%d"), m_sbar1.GetScrollPos());
	//m_m1a.SetWindowText(TextString);

	TextString.Format(_T("%d"), m_sbar2.GetScrollPos());
	m_m2a.SetWindowText(TextString);

	TextString.Format(_T("%d"), m_sbar3.GetScrollPos());
	m_m3a.SetWindowText(TextString);

	TextString.Format(_T("%d"), m_sbar4.GetScrollPos());
	m_m4a.SetWindowText(TextString);

	com.SetPosition2(1, -30 * m_sbar1.GetScrollPos() + 8900);
	//com.SetPosition2(2, -30 * m_sbar2.GetScrollPos() + 8900);
	//com.SetPosition2(3, -30 * m_sbar3.GetScrollPos() + 8900);
	//com.SetPosition2(4, -30 * m_sbar4.GetScrollPos() + 8900);

	TextString.Format(_T("%lf"), com.GetCurrent(1));
	m_m1a.SetWindowText(TextString);

	///���ؗp�T�[�{��]
	TextString.Format(_T("%d"), com.flag3);
	m_flagnum.SetWindowText(TextString);
	/*if (com.flag == 1) {
		//if (com.flag3 == 0) {
		//	com.SetPosition(3, -30 * 0 + 8900);
		//	com.flag3 = 1;
		//}

		angle = (8900 - com.GetPosition(3)) / 30;
		TextString.Format(_T("%d"), angle);

		if (angle >= 85 && angle <= 90) {
			com.SetPosition(3, -30 * 0 + 8900);

		}
		if (angle >= 0 && angle <= 5) {
			com.SetPosition(3, -30 * 90 + 8900);

		}
		
	}*/
	/*
	if(com.flag==1){
		
		m_6003_st.SetWindowText(TextString);
	if (com.flag3 == 0) {//0-90�̂Ƃ�
		com.SetPosition(3, -30 * 90 + 8900);
		com.flag3 = 1;
		//AfxMessageBox("a");
		m_flag0.SetWindowText("IN");
		m_flag1.SetWindowText("OUT");
		m_flag2.SetWindowText("OUT");
		m_flag3.SetWindowText("OUT");
	}
	if (com.flag3 == 2) {//90-0�̂Ƃ�
		com.SetPosition(3, -30 * 0 + 8900);
		com.flag3 = 3;
		AfxMessageBox("2");
		m_flag0.SetWindowText("OUT");
		m_flag1.SetWindowText("OUT");
		m_flag2.SetWindowText("IN");
		m_flag3.SetWindowText("OUT");
	}


	if (com.flag3 == 1) {//0-90
		m_flag0.SetWindowText("OUT");
		m_flag1.SetWindowText("IN");
		m_flag2.SetWindowText("OUT");
		m_flag3.SetWindowText("OUT");
		if (85<angle && angle<= 95) {
			com.flag3 = 2;
			TextString.Format(_T("%d"), com.flag3);
			m_flagnum.SetWindowText(TextString);
			//AfxMessageBox("b");
		}
		//ok
	}

	if (com.flag3 == 3) {//90-0
		m_flag0.SetWindowText("OUT");
		m_flag1.SetWindowText("OUT");
		m_flag2.SetWindowText("OUT");
		m_flag3.SetWindowText("IN");
		if (angle>= 0 && angle < 10) {
			com.flag3 = 0;
		}

	}
	}*/
	if (imuz.IMUflag == 1) {
		GetGyroData(gyro_now, acc_now, compass_now);
		GetAngleHioki();
		//m_xc_WAKIPIC.DrawEOGMAP(0.1,0,0,true,0);
		m_xc_WAKIPIC.DrawSignal(axbuff,axbuffsize,180,-180) ;
	}


	CDialogEx::OnTimer(nIDEvent);
}

void CKRS6003_ControllerDlg::OnBnClickedKrs6003C()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	com.flag = com.CommPortOpen("\\\\.\\COM10", 115200, ONESTOPBIT, EVENPARITY, 8);

	if (com.flag == 1) {
		m_6003_st.SetWindowText(_T("CONNECTED"));
		//com.SetPosition(3, 3500);
		//com.ChangeSpeed(3, 50);
	}
	else {
		m_6003_st.SetWindowText(_T("FAILED"));

	}

}


void CKRS6003_ControllerDlg::OnBnClickedKrs6003Dc()
{
	if (com.CommPortClose() == 1) {
		m_6003_st.SetWindowText(_T("DISCONNECTED"));
	}
}

void CKRS6003_ControllerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B


	int pos = pScrollBar->GetScrollPos();
	switch (nSBCode) {
	case SB_LEFT:
		pos = 0;	break;
	case SB_RIGHT:
		pos = 180;	break;
	case SB_LINELEFT:
		pos = pos - 1;	break;
	case SB_LINERIGHT:
		pos = pos + 1;	break;
	case SB_PAGELEFT:
		pos = pos -10;	
		break; 
	case SB_PAGERIGHT:
		pos = pos + 10;	
		break;
	case SB_THUMBPOSITION:
		pos = nPos;	break; 
	case SB_THUMBTRACK:
		pos = nPos;	
		
		break;
	case SB_ENDSCROLL:
		     break;
	}

	if (pos < 0) pos = 0;
	if (pos > 180) pos = 180;
	pScrollBar->SetScrollPos(pos);
	
	
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}







void CKRS6003_ControllerDlg::OnBnClickedButton1()
{
	GOPEN();
	imuz.IMUflag = 1;
}


void CKRS6003_ControllerDlg::OnBnClickedButton2()
{
	if (imuz.CommPortClose()) {
		m_GYRO_st.SetWindowText(_T("DISCONNECTED"));
	}
	imuz.IMUflag = 0;
}

int CKRS6003_ControllerDlg::GetGyroData(vector<vector<double>> &gyro, vector<vector<double>> &acc, vector<vector<double>> &compas)
{
	int num = 0;
	int i, j;

	//IMU���[�h

	MeasurementData mess;
	if (isComGyroOpenSuccess) {
		//AfxMessageBox("a");
		//�f�[�^�T�C�Y��1�p�P�b�g(18�o�C�g)�����̎��͕Ԃ�
		num = imuz.ReceiveDataSize();//�f�[�^�T�C�Y�擾�@�o�C�g����Ԃ�

		if (num<PACKET_SIZE) {//
			CString str;
			str.Format("%d", num);
			//AfxMessageBox(str); 
			return 0;
		}
		else { num = num / 18; }//�@�@num/18�̓T���v�����O��


								//�T���v�����O�����̔z���p��

								//�e��f�[�^��xyz�O�����̔z���p��
		gyro.clear();
		gyro.resize(num);//�T���v�����O�񐔕��z����m��
		for (i = 0; i<num; i++) { gyro[i].resize(3); }//���ɂ�xyz�̎O�����O�����m��

		acc.clear();
		acc.resize(num);
		for (i = 0; i<num; i++) { acc[i].resize(3); }

		compas.clear();
		compas.resize(num);
		for (i = 0; i<num; i++) { compas[i].resize(3); }

		//�p�ӂ����z��ɒl���i�[
		//CString str;
		//str.Format("%d",num);
		//AfxMessageBox(str);

		for (i = 0; i<num; i++) {
			//�o�b�t�@��������
			memset(imuz_buff, 0x00, sizeof(imuz_buff));
			// �p�P�b�g1��ǂ݂܂�
			if (imuz.com_ReadPacket(imuz_buff, IMUZ_BUFF_SIZE)) {

				mess = imuz.IMUZBinaryDataDecode(imuz_buff);

				//�W���C���f�[�^�擾�@double
				gyro[i][0] = mess.gyro[0];//x
				gyro[i][1] = mess.gyro[1];//y
				gyro[i][2] = mess.gyro[2];//z

										  //�����x
				acc[i][0] = mess.acc[0];
				acc[i][1] = mess.acc[1];
				acc[i][2] = mess.acc[2];

				//���C
				compas[i][0] = mess.comp[0];
				compas[i][1] = mess.comp[1];
				compas[i][2] = mess.comp[2];

			}
			//else { ; }

		}//return 0;
	}
	//return 0;
	return num;

}

// GYRO�|�[�g�I�[�v��
void CKRS6003_ControllerDlg::GOPEN()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CString str;
	int test;
	PAVX = 0;
	PAX = 0;
	CAX = 0;
	AX = 0;

	

	UpdateData();
	//�|�[�g�i���o�[�擾
	str = COMstrArray[m_CfGcombo.GetCurSel()];
	//IMU
	if (!imuz.CommPortOpen(atoi(str.Mid(3, str.GetLength() - 3)), 9600, ONESTOPBIT, NOPARITY, 8)) {
		//AfxMessageBox(str + "�͊J���܂���",MB_OK);
		str.Format("COM%d", str.Mid(3, str.GetLength() - 3));
		AfxMessageBox(str + "�͊J���܂���", MB_OK);
	}
	else {
		//AfxMessageBox("GYRO_OPEN");
		isComGyroOpenSuccess = true;
		m_GYRO_st.SetWindowText(_T("CONNECTED"));
		//test = imuz.ReceiveDataSize();//�f�[�^�T�C�Y�擾
		//if (test !=0) {
		//	m_GYRO_st.SetWindowText(_T("CONNECTED"));
		//}
		//m_xc_ComGyroOpen.EnableWindow(false);
		//m_xc_ComGyroClose.EnableWindow(true);
		UpdateData(false);
		//m_CfGcombo.EnableWindow(true);
		//m_CfGcombo.EnableWindow(false);

		//IMU ������ 
		//XAngle = 0;


		//�f�[�^�ϊ��p�̌W���@������ �Ɛݒ�
		imuz.conv_Init();
		imuz.conv_SetupFactors(31, HARDWARE_IMUZ_2, Range2g, Range500dps, Range1_3Ga, FALSE);

		gyro_now.clear();
		gyro_now.resize(1);
		gyro_now[0].resize(3);

		acc_now.clear();
		acc_now.resize(1);
		acc_now[0].resize(3);

		compass_now.clear();
		compass_now.resize(1);
		compass_now[0].resize(3);


	}

}

void  CKRS6003_ControllerDlg::GetAngleHioki()
{

	////////////////////////     X           ///////////////////////////////////////////////////////
	CAX = PCAX - (PAVX + gyro_now[0][0]) * 0.01 / 2;//100ms��180���炢�܂ł����@10ms 60���炢�܂�
	if (CAX > 180) CAX = CAX - 360;
	if (CAX < -180) CAX = CAX + 360;
	//TextString.Format(_T("%lf"), CAX);
	//m_AGL_G.SetWindowText(TextString);


	ROLL_ACC = atan2(acc_now[0][2], acc_now[0][1]) * 180 / 3.14 - 90;//atan2(az,ay)
	if (ROLL_ACC > 180) ROLL_ACC = ROLL_ACC - 360;
	if (ROLL_ACC < -180) ROLL_ACC = ROLL_ACC + 360;
	//TextString.Format(_T("%lf"), ROLL_ACC);
	//m_AGL_A.SetWindowText(TextString);


	k = 0.98;
	AX = k*(PAX - (PAVX + gyro_now[0][0]) * 0.01 / 2) + (1 - k)*ROLL_ACC;
	if (AX > 180) AX = AX - 360;
	if (AX < -180) AX = AX + 360;
	TextString.Format(_T("%lf"), AX);
	m_Gyro.SetWindowText(TextString);

	//n-1��ڂ��X�V
	PAVX = gyro_now[0][0];
	PCAX = CAX;
	PAX = AX;

	//�o�b�t�@�Ɋi�[
	axbuff[ax_pt] = AX;
	ax_pt = (++ax_pt) % axbuffsize;

	/*
	////////////////////////     Y           ///////////////////////////////////////////////////////
	CAY = PCAY - (PAVY + gyro_now[0][1]) * 0.01 / 2;//100ms��180���炢�܂ł����@10ms 60���炢�܂�
	if (CAY > 180) CAY = CAY - 360;
	if (CAY < -180) CAY = CAY + 360;
	TextString.Format(_T("%lf"), CAY);
	m_AGL_GY.SetWindowText(TextString);


	PITCH_ACC = atan2(acc_now[0][2], acc_now[0][0]) * 180 / 3.14;//atan2(az,ax)
	if (PITCH_ACC > 180) PITCH_ACC = PITCH_ACC - 360;
	if (PITCH_ACC < -180) PITCH_ACC = PITCH_ACC + 360;
	TextString.Format(_T("%lf"), PITCH_ACC);
	m_AGL_AY.SetWindowText(TextString);



	k = 0.98;
	AY = k*(PAY - (PAVY + gyro_now[0][1]) * 0.01 / 2) + (1 - k)*PITCH_ACC;
	if (AY > 180) AY = AY - 360;
	if (AY < -180) AY = AY + 360;
	TextString.Format(_T("%lf"), AY);
	m_AGL_Y.SetWindowText(TextString);

	//n-1��ڂ��X�V
	PAVY = gyro_now[0][1];
	PCAY = CAY;
	PAY = AY;
	*/
}

void CKRS6003_ControllerDlg::OnBnClickedButton3()
{

	
	TextString.Format(_T("%d"), com.GetCurrent(1));
	m_m1a.SetWindowText(TextString);
}
