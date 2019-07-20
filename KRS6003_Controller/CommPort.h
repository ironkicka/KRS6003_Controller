#pragma once

//////////////////////////////////////////////////////////////////////////
// Program: Serial port Class
// Programed by Ryu Kato
// Copyright(C)　2001-2005 Ryu Kato. All rights reserved.
//////////////////////////////////////////////////////////////////////////

class CCommPort
{
public:
	CCommPort(void);
	~CCommPort(void);
	bool KRSflag;
	bool flag;
	bool flag3;
	bool initflag;
	bool loopflag5;
	bool loopflag6;
	int scount;
	short int preP1,preP2;
	bool flag2;
	char test[1];
	int R;
	int count1;
	int count2;
	char CMD[3];
	char ReadP[2];
	char RET[6];
	char POS[4][6];

	char RETP[6];
	char RET2[6];
	char POS_H;
	char POS_L;
	char IDNUM;
	////
	char TX3[4][3];//送信時、IDを組み込みたいため、二次元にした。
	char RX6[4][3];
	char RXP[4][3];

	short int pos1,pos2,POS1,POS2,prePOS;

	double x0,y0,xx,yy,x,y,TX,TY;
	double GX[9];
	double GY[9];
	double L1,L2;
double ac1,ac2;
int th1,th2;
int pth1, pth2;
double TH1,TH2;
double a,b;
double a2, b2;
double Vmax, Vmin;

char ReadC[2];
char RETC[6];
double current;

	unsigned short int SetPosition(char ID,unsigned short int angle);
	unsigned short int SetPosition2(char ID, unsigned short int angle);
	//void SetPosition(unsigned short int angle);
	short int GetPosition(char ID);
	double GetCurrent(char ID);
	void CreateFormula(int i,int X,int Y);
	void CreateFormula2(int X, int Y, double Vmax, double Vmin);
	void CreateFormula3(int X, int Y);

	int CtoA(int x, int y);
	int CreatePoints(int i,double step);
	int CreatePoints2(float AD_DATA);
	int CreatePoints3(double AD_DATA);
	void SpeedMAX(char ID);
	void SpeedMIN(char ID);
	
	void ChangeSpeed(char ID,char SPEED);
	

	bool CommPortOpen(
		LPCTSTR nameOfCommPort,
		unsigned long baudRate,
		unsigned char stopBits,
		unsigned char parity,
		unsigned char byteSize
	);
	bool CommPortClose(void);
	int ComList(CStringArray &strArray);
	BOOL CommWriteChar(int nChar);
	BOOL CommReadChar(char* pnChar);
	BOOL CommWriteStr(char* pnChar);
	BOOL CommReadStr(char* pnChar);
	BOOL CCommPort::CommWriteStr2(char* pnChar);
	BOOL CCommPort::CommReadStr5(char* pnChar);
protected:
	LPCTSTR			commPort;
	DCB				dcb;
	HANDLE			commHandle;
	COMMTIMEOUTS	timeouts;
	LPCTSTR			*availablePortName;
	unsigned long	*availableBaudRate;
	unsigned char	*availableStopBits;
	unsigned char	*availableParity;
	unsigned char	*availableByteSize;
	bool isError;
};
