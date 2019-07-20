//////////////////////////////////////////////////////////////////////////
// Program: Serial port Class
// Programed by Ryu Kato
// Copyright(C)　2001-2005 Ryu Kato. All rights reserved.
//////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "commport.h"
#include <cmath>
#define OTHERWISE 5

CCommPort::CCommPort(void)
{
	//利用可能なポート名，ボーレート，ストップビットの設定
	availablePortName = new LPCTSTR[12];
	availablePortName[0]="COM1";
	availablePortName[1]="COM2";
	availablePortName[2]="COM3";
	availablePortName[3]="COM4";
	availablePortName[4]="COM5";
	availablePortName[5]="COM6";
	availablePortName[6]="COM7";
	availablePortName[7]="COM8";
	availablePortName[8]="COM9";
	availablePortName[9]= "\\\\.\\COM10";
	availablePortName[10]="COM11";
	availablePortName[11]=NULL;

	availableBaudRate = new unsigned long[8];
	availableBaudRate[0]=115200;
	availableBaudRate[1]=57600;
	availableBaudRate[2]=38400;
	availableBaudRate[3]=28800;
	availableBaudRate[4]=19200;
	availableBaudRate[5]=14400;
	availableBaudRate[6]=9600;
	availableBaudRate[7]=NULL;

	availableParity = new unsigned char[4];
	availableParity[0]=NOPARITY;
	availableParity[1]=EVENPARITY;
	availableParity[2]=ODDPARITY;
	availableParity[3]=OTHERWISE;

	availableStopBits = new unsigned char[4];
	availableStopBits[0]=ONESTOPBIT;
	availableStopBits[1]=ONE5STOPBITS;
	availableStopBits[2]=TWOSTOPBITS;
	availableStopBits[3]=OTHERWISE;

	availableByteSize = new unsigned char[5];
	availableByteSize[0]=5;
	availableByteSize[1]=6;
	availableByteSize[2]=7;
	availableByteSize[3]=8;
	availableByteSize[4]=NULL;

	initflag=1;
	
}

CCommPort::~CCommPort(void)
{
	delete [] availablePortName;
	delete [] availableBaudRate;
	delete [] availableStopBits;
	delete [] availableParity;
	delete [] availableByteSize;
}

bool CCommPort::CommPortOpen(LPCTSTR nameOfCommPort,unsigned long baudRate,unsigned char stopBits,unsigned char parity,unsigned char byteSize)
{
	isError=true;
	for(int i=0;availablePortName[i]!=NULL ;i++){
		if( lstrcmp(nameOfCommPort,availablePortName[i])==0 ){
			commPort=availablePortName[i];
			isError=false;
		}
	}
	if(isError) return false;

	isError=true;
	for(int i=0;availableBaudRate[i]!=NULL ;i++){
		if( baudRate==availableBaudRate[i] )
			isError=false;
	}
	if(isError) return false;

	isError=true;
	for(int i=0;availableStopBits[i]!=OTHERWISE ;i++){
		if( stopBits==availableStopBits[i] )
			isError=false;
	}
	if(isError) return false;

	isError=true;
	for(int i=0;availableParity[i]!=OTHERWISE ;i++){
		if( parity==availableParity[i] )
			isError=false;
	}
	if(isError) return false;
	
	isError=true;
	for(int i=0;availableByteSize[i]!=NULL;i++){
		if( byteSize==availableByteSize[i] )
			isError=false;
	}
	if(isError) return false;
    
	// ポートオープン
	commHandle = CreateFile(
		commPort,						//CommPort名 
		GENERIC_READ | GENERIC_WRITE,	//読み取り・書き取りアクセス
		0,								//ポート共有不可
		NULL,							//ハンドルを子プロセスへ継承しない
		OPEN_EXISTING,					//ファイルが存在する
		FILE_ATTRIBUTE_NORMAL,			//時間のかかる処理に対して ERROR_IO_PENDING を返す
		NULL							//NULL
	);
	if(commHandle == INVALID_HANDLE_VALUE) return false;

	if( !SetCommMask(commHandle, 0) || !SetupComm(commHandle, 4096, 4096) ) {
		CommPortClose();
		return false;
	}

	if( !GetCommState(commHandle, &dcb) ){
		CommPortClose();
		return false;
	}

	//DCB設定
	dcb.DCBlength = sizeof(DCB);
	dcb.fBinary = TRUE;
	dcb.BaudRate	= baudRate;
	dcb.ByteSize	= byteSize;
	dcb.Parity		= parity;
	dcb.StopBits	= stopBits;
	dcb.fParity = (parity == NOPARITY) ? FALSE : TRUE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fTXContinueOnXoff = TRUE;
	dcb.fInX = dcb.fOutX = FALSE;
	dcb.XonChar = 0x11;
	dcb.XoffChar = 0x13;
	dcb.XonLim = 100;
	dcb.XoffLim = 1500;

	if( !SetCommState(commHandle, &dcb) ){
		CommPortClose();
		return false;
	}

	timeouts.ReadIntervalTimeout		= 100;	// 次のキャラクタを受信するまでのタイムアウト //MAXDWORD
	timeouts.ReadTotalTimeoutMultiplier	= 100;	// Read : 1バイトに対するタイムアウト
	timeouts.ReadTotalTimeoutConstant	= 500;	// Read : 0バイト時のタイムアウト
	timeouts.WriteTotalTimeoutMultiplier= 10;	// Write : 1バイトに対するタイムアウト
	timeouts.WriteTotalTimeoutConstant	= 500;	// Write : 0バイト時のタイムアウト

	if(!SetCommTimeouts(commHandle, &timeouts)){
		CommPortClose();
		return false;
	}
	return true;
}

bool CCommPort::CommPortClose(void)
{
	CloseHandle(commHandle);
	return true;
}
///COMポート情報を引き出す．
int CCommPort::ComList(CStringArray &strArray)
{
	
	HKEY hKey = NULL;
	DWORD dwCount = 0;
	TCHAR cNameBuff[256];
	BYTE byValueBuff[256];
	DWORD dwNameBuffSize = 0;
	DWORD dwValueBuffSize = 0;
	DWORD dwType = 0;

	int i;

	// COMポートの情報が存在するキーを得る
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		;
	}

	// 要素数を得る。
	if (RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwCount, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
	{
		;
	}

	// 要素数分を繰り返す。
	for (i = 0; i < (int)dwCount; i++) {
		dwNameBuffSize = sizeof(cNameBuff);
		dwValueBuffSize = sizeof(byValueBuff);

		// 一覧を取り出す
		if (RegEnumValue(hKey, i, &cNameBuff[0], &dwNameBuffSize, NULL, &dwType, &byValueBuff[0], &dwValueBuffSize) != ERROR_SUCCESS)
		{
			;
		}

		// 名称で文字列配列を作成する。
		strArray.Add(CString(byValueBuff));
	}

	
	return 0;
}





BOOL CCommPort::CommWriteChar(int nChar)
{
	DWORD dwWritten;
	return WriteFile(commHandle, &nChar, 1, &dwWritten, NULL);
}

BOOL CCommPort::CommReadChar(char* pnChar)
{
	DWORD dwRead;

	if(!ReadFile(commHandle,pnChar, 1, &dwRead, NULL))
		return FALSE;
	
	if(dwRead == 0)
		return FALSE;
	
	return TRUE;
}

BOOL CCommPort::CommWriteStr(char* pnChar)
{
	DWORD dwWritten;
	return WriteFile(commHandle, pnChar, 3, &dwWritten, NULL);
}
BOOL CCommPort::CommWriteStr2(char* pnChar)
{
	DWORD dwWritten;
	return WriteFile(commHandle, pnChar, 2, &dwWritten, NULL);
}
BOOL CCommPort::CommReadStr(char* pnChar)
{
	DWORD dwRead;

	if(!ReadFile(commHandle,pnChar,6, &dwRead, NULL))
		return FALSE;
	
	if(dwRead == 0)
		return FALSE;
	
	return TRUE;
}
BOOL CCommPort::CommReadStr5(char* pnChar)
{
	DWORD dwRead;

	if(!ReadFile(commHandle,pnChar,5, &dwRead, NULL))
		return FALSE;
	
	if(dwRead == 0)
		return FALSE;
	
	return TRUE;
}


unsigned short int CCommPort::SetPosition(char ID,unsigned short int angle){
	//CMD[0]=(0x80|ID);//10000000と000XXXXX
	TX3[ID][0]=(0x80|ID);
	POS_H=((angle>>7)&0x007F);
	POS_L=(angle&0x007F);
	//CMD[1]=POS_H;
	//CMD[2]=POS_L;
	TX3[ID][1]=POS_H;
	TX3[ID][2]=POS_L;
	IDNUM=ID;
	CommWriteStr(TX3[ID]);
	//CommWriteStr(COM);
	CommReadStr(RET);

	return 1;
}
unsigned short int CCommPort::SetPosition2(char ID, unsigned short int angle) {
	//CMD[0]=(0x80|ID);//10000000と000XXXXX
	TX3[ID][0] = (0x80 | ID);
	POS_H = ((angle >> 7) & 0x007F);
	POS_L = (angle & 0x007F);
	//CMD[1]=POS_H;
	//CMD[2]=POS_L;
	TX3[ID][1] = POS_H;
	TX3[ID][2] = POS_L;
	IDNUM = ID;
	CommWriteStr(TX3[ID]);
	//CommWriteStr(COM);
	CommReadStr(POS[ID]);

	return 1;
}
short int CCommPort::GetPosition(char ID){
	ReadP[0]=0xA0|ID;
	ReadP[1]=0x05;
	CommWriteStr2(ReadP);
	CommReadStr(RETP);
	pos1=0;
	POS1=0;
	pos1=0x007F&(RETP[5]);
	POS1=(RETP[4]<<7)|pos1;


	return POS1;
}

 double CCommPort::GetCurrent(char ID) {
	ReadC[0] = 0xA0 | ID;
	ReadC[1] = 0x03;
	CommWriteStr2(ReadC);
	CommReadStr(RETC);
	
	current =RETC[4];

	return current;
}
void CCommPort::SpeedMAX(char ID){
	CMD[0]=0xC0|ID;
	CMD[1]=0x02;
	CMD[2]=0x7F;//1~127
	CommWriteStr(CMD);
	CommReadStr(RET2);
	
}
void CCommPort::SpeedMIN(char ID){
	CMD[0]=0xC0|ID;
	CMD[1]=0x02;
	CMD[2]=0x01;//1~127
	CommWriteStr(CMD);
	CommReadStr(RET2);
	
}
void CCommPort::ChangeSpeed(char ID,char SPEED){
	CMD[0]=0xC0|ID;
	CMD[1]=0x02;
	CMD[2]=0x00|SPEED;//1~127
	CommWriteStr(CMD);
	CommReadStr(RET2);
	
}

void CCommPort::CreateFormula(int i,int X, int Y){

	th1=(double)(8900-GetPosition(1))/30;
	th2=(double)(8900-GetPosition(2))/30;
			//ラジアンに変換
	TH1=1/180.0*3.14*th1;
	TH2=1/180.0*3.14*th2;
	x0=L1*sin(TH1)-L2*sin(TH1+TH2);
	y0=-L1*cos(TH1)+L2*cos(TH1+TH2);
			///目標地点 0が手元
	GX[i]=X;
	GY[i]=Y;
			
			a=(Y-y0)/(X-x0);
			b=y0-a*x0;

}

int CCommPort::CreatePoints(int i,double step){
	
			x=x0;
			if ((int)GX[i]==(int)x){//ゴールと現在値が同じなら何もしない．
	
			return 0;
			
			}
			if(GX[i]>x) {
				x+=step;
			}else if (GX[i]<x){
				x-=step;
			}
			//str.Format("%lf",com2.x);
				//AfxMessageBox(str);

			

			y=a*x+b;
			ac1=acos((L1*L1+x*x+y*y-L2*L2)/(2*L1*sqrt(x*x+y*y)));
			ac2=acos((L1*L1+L2*L2-x*x-y*y)/(2*L1*L2));
			th1=(3.14/2+atan(y/x)+ac1)*180/3.14;
			th2=ac2*180/3.14;
			//////例外処理
			if(th1>=180) th1=180;
			if(th2>=180) th2=180;
			if(th1<=0) th1=180;
			if(th2<=0) th2=180;
			SetPosition(1,-30*th1+8900);
			SetPosition(2,-30*th2+8900);
			
			//str.Format("%d",-30*com2.th2+8900);
			//AfxMessageBox(str);
			x0=x;//n回目の初期値はn-1回目の値にしたい。
			return 1;
}
int CCommPort::CtoA(int x, int y) {
	
	//y = a*x + b;
	ac1 = acos((L1*L1 + x*x + y*y - L2*L2) / (2 * L1*sqrt(x*x + y*y)));
	ac2 = acos((L1*L1 + L2*L2 - x*x - y*y) / (2 * L1*L2));
	th1 = (3.14 / 2 + atan(y / x) + ac1) * 180 / 3.14;
	th2 = ac2 * 180 / 3.14;
	//////例外処理
	if (th1 >= 180) th1 = 180;
	if (th2 >= 180) th2 = 180;
	if (th1 <= 0) th1 = 180;
	if (th2 <= 0) th2 = 180;
	SetPosition(1, -30 * th1 + 8900);
	SetPosition(2, -30 * th2 + 8900);

	return 1;
}
void CCommPort::CreateFormula2(int X, int Y,double max,double min) {
	///現在地を取得
	//th1 = (double)(8900 - GetPosition(1)) / 30;
	//th2 = (double)(8900 - GetPosition(2)) / 30;
	//ラジアンに変換
	TH1 = 1 / 180.0*3.14*th1;
	TH2 = 1 / 180.0*3.14*th2;
	x0 = L1*sin(TH1) - L2*sin(TH1 + TH2);
	y0 = -L1*cos(TH1) + L2*cos(TH1 + TH2);
	///目標地点 0が手元
	GX[0] = X;
	GY[0] = Y;
	Vmax = max;
	Vmin = min;
	///傾き算出
	a = (Y - y0) / (X - x0);
	b = y0 - a*x0;
	a2 = (x0-GX[0]) / (Vmax - Vmin);
	b2 = GX[0] - a2*Vmin;
}

int CCommPort::CreatePoints2(float AD_DATA) {
	if (AD_DATA >= Vmax) AD_DATA =Vmax;
	if (AD_DATA<= Vmin) AD_DATA = Vmin;
	x = a2*AD_DATA + b2;//xとアナログ値の関係

	y = a*x + b;
	ac1 = acos((L1*L1 + x*x + y*y - L2*L2) / (2 * L1*sqrt(x*x + y*y)));
	ac2 = acos((L1*L1 + L2*L2 - x*x - y*y) / (2 * L1*L2));
	th1 = (3.14 / 2 + atan(y / x) + ac1) * 180 / 3.14;
	th2 = ac2 * 180 / 3.14;
	//////例外処理
	if (th1 >= 180) th1 = 180;
	if (th2 >= 180) th2 = 180;
	if (th1 <= 0) th1 = 180;
	if (th2 <= 0) th2 = 180;
	SetPosition(1, -30 * th1 + 8900);
	SetPosition(2, -30 * th2 + 8900);

	
	return 1;
}

void CCommPort::CreateFormula3(int X, int Y){
	///現在地を取得
	//th1 = (double)(8900 - GetPosition(1)) / 30;
	//th2 = (double)(8900 - GetPosition(2)) / 30;
	//ラジアンに変換
	TH1 = 1 / 180.0*3.14*th1;
	TH2 = 1 / 180.0*3.14*th2;
	x0 = L1*sin(TH1) - L2*sin(TH1 + TH2);
	y0 = -L1*cos(TH1) + L2*cos(TH1 + TH2);
	///目標地点 0が手元
	GX[0] = X;
	GY[0] = Y;
	///傾き算出
	a = (Y - y0) / (X - x0);
	b = y0 - a*x0;

	a2 = ( GX[0]-x0) / (Vmax - Vmin);
	b2 = x0 - a2*Vmin;
}

int CCommPort::CreatePoints3(double AD_DATA) {
	//if (AD_DATA >= Vmax) AD_DATA = Vmax;
	//if (AD_DATA <= Vmin) AD_DATA = Vmin;

	x = a2*AD_DATA + b2;//xとアナログ値の関係

	y = a*x + b;
	ac1 = acos((L1*L1 + x*x + y*y - L2*L2) / (2 * L1*sqrt(x*x + y*y)));
	ac2 = acos((L1*L1 + L2*L2 - x*x - y*y) / (2 * L1*L2));
	th1 = (3.14 / 2 + atan(y / x) + ac1) * 180 / 3.14;
	th2 = ac2 * 180 / 3.14;
	
	//////例外処理
	/*if (x > GX[0] || x<x0) {
		th1 = pth1;
		th2 = pth2;
	}
	*/
	
	/*
	if (th1 >= 180) th1 = 180;
	if (th2 >= 180) th2 = 180;
	if (th1 <= 0) th1 = 180;
	if (th2 <= 0) th2 = 180;
	*/
	if (AD_DATA >= Vmax || AD_DATA <= Vmin) {
		th1 = pth1;
		th2 = pth2;
	}
	pth1 = th1;
	pth2 = th2;

	SetPosition(1, -30 * th1 + 8900);
	SetPosition(2, -30 * th2 + 8900);


	return 1;
}