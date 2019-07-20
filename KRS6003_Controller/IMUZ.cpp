#include "StdAfx.h"
#include "IMUZ.h"


CIMUZ::CIMUZ(void)
{
	//imuz パケット通信用
	s_cnt=0;
	s_bRecover=false;

	//com
	availableBaudRate.resize(8,0);
	availableBaudRate[0]=115200;
	availableBaudRate[1]=57600;
	availableBaudRate[2]=38400;
	availableBaudRate[3]=28800;
	availableBaudRate[4]=19200;
	availableBaudRate[5]=14400;
	availableBaudRate[6]=9600;
	availableBaudRate[7]=NULL;

	availableParity.resize(5,0);
	availableParity[0]=NOPARITY;
	availableParity[1]=EVENPARITY;
	availableParity[2]=ODDPARITY;
	availableParity[3]=OTHERWISE;
	availableParity[4]=NULL;

	availableStopBits.resize(5,0);
	availableStopBits[0]=ONESTOPBIT;
	availableStopBits[1]=ONE5STOPBITS;
	availableStopBits[2]=TWOSTOPBITS;
	availableStopBits[3]=OTHERWISE;
	availableStopBits[4]=NULL;

	availableByteSize.resize(5,5);
	availableByteSize[0]=5;
	availableByteSize[1]=6;
	availableByteSize[2]=7;
	availableByteSize[3]=8;
	availableByteSize[4]=NULL;
}


CIMUZ::~CIMUZ(void)
{
}

bool CIMUZ::CommPortOpen(unsigned int NumberOfCommPort, unsigned long baudRate, unsigned char stopBits, unsigned char parity, unsigned char byteSize)
{
	CString temp;
	CString nametemp = _T("");
	
	if(NumberOfCommPort >= 10){
		nametemp = _T("\\\\.\\COM");
	}
	else{
		nametemp = _T("COM");
	}

	//COMポート名
	if(NumberOfCommPort > MAXPORT || NumberOfCommPort < 1){
		return false;
	}
	temp.Format(_T("%d"),NumberOfCommPort);
	nametemp = nametemp + temp;
	commPort = (LPCTSTR)nametemp;

	//転送レート
	isError=true;
	for(int i=0;availableBaudRate[i]!=NULL ;i++){
		if( baudRate==availableBaudRate[i] )
			isError=false;
	}
	if(isError) return false;

	//書き込み設定
	isError=true;
	for(int i=0;availableStopBits[i]!=OTHERWISE ;i++){
		if( stopBits==availableStopBits[i] )
			isError=false;
	}
	if(isError) return false;

	//パリティ設定
	isError=true;
	for(int i=0;availableParity[i]!=OTHERWISE ;i++){
		if( parity==availableParity[i] )
			isError=false;
	}
	if(isError) return false;

	//バイト数
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
	dcb.DCBlength	= sizeof(DCB);
	dcb.fBinary		= TRUE;
	dcb.BaudRate	= baudRate;
	dcb.ByteSize	= byteSize;
	dcb.Parity		= parity;
	dcb.StopBits	= stopBits;
	dcb.fParity		= (parity == NOPARITY) ? FALSE : TRUE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fOutxCtsFlow= FALSE;
	dcb.fTXContinueOnXoff	= TRUE;
	dcb.fInX		= dcb.fOutX = FALSE;
	dcb.XonChar		= 0x11;
	dcb.XoffChar	= 0x13;
	dcb.XonLim		= 100;
	dcb.XoffLim		= 1500;


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



bool CIMUZ::CommPortClose(void)
{
	CloseHandle(commHandle);
	return true;
}




//1パケット読みだす
bool CIMUZ::com_ReadPacket(PBYTE lpBuff, DWORD readMax)
{
	DWORD dw;
	BYTE c;
	PBYTE p = lpBuff + s_cnt;
	
	if (readMax < PACKET_SIZE) {
		return false;
	}

	while (ReadFile(commHandle, &c, 1, &dw, NULL) && dw == 1) {
		if (s_bRecover) {
			if (c == 'B' || c == 'E') {
				s_bRecover = FALSE;
				p = lpBuff;
				*p++ = c;
				s_cnt = 1;
			} else {
			}
		} else {
			*p++ = c;
			s_cnt++;
			if (s_cnt >= PACKET_SIZE) {
				int i;
				BYTE sum = 0;
				for (i = 0; i < PACKET_SIZE - 1; i++) {
					sum += lpBuff[i];
				}
				if (sum == lpBuff[PACKET_SIZE - 1]) {
					s_cnt = 0;
					return true;
					//return false;
				} else {
					s_bRecover = TRUE;
					s_cnt = 0;
					return false;
				}
			}
		}
	}
	return false;

}


int CIMUZ::ReceiveDataSize(void)
{
	DWORD dwErrors;  /* エラー情報 */
	COMSTAT ComStat; /* デバイスの状態 */
	DWORD dwCount;   /* 受信データのバイト数 */

	ClearCommError(commHandle,&dwErrors, &ComStat);
	dwCount = ComStat.cbInQue;

	return dwCount;
}


MeasurementData CIMUZ::IMUZBinaryDataDecode(PBYTE data)
{
	MeasurementData mess;
	if (data[0] == 'B') {
		// 計測データ
		int node_no;
		int time_stamp;
		int raw_acc[3];
		int raw_gyro[3];
		int raw_comp[3];
		int i;

		node_no = (UINT)data[1] >> 3;
		time_stamp = ((UINT)data[1] << 12) & 0x7000 | ((UINT)data[2] << 4) & 0x0ff0 | ((UINT)data[3] >> 4) & 0x000f;

		raw_acc[0] = ((int)data[3] << 8) & 0x0f00 | data[4];
		raw_acc[1] = ((int)data[5] << 4) | (data[6] >> 4) & 0x0f;
		raw_acc[2] = ((int)data[6] << 8) & 0x0f00 | data[7];
		raw_gyro[0] = ((int)data[8] << 4) | (data[9] >> 4) & 0x0f;
		raw_gyro[1] = ((int)data[9] << 8) & 0x0f00 | data[10];
		raw_gyro[2] = ((int)data[11] << 4) | (data[12] >> 4) & 0x0f;
		raw_comp[0] = ((int)data[12] << 8) & 0x0f00 | data[13];
		raw_comp[1] = ((int)data[14] << 4) | (data[15] >> 4) & 0x0f;
		raw_comp[2] = ((int)data[15] << 8) & 0x0f00 | data[16];
		for (i = 0; i < 3; i++){
			raw_acc[i] = (short)(unsigned short)(raw_acc[i] << 4);
			raw_gyro[i] = (short)(unsigned short)(raw_gyro[i] << 4);
			raw_comp[i] = (short)(unsigned short)(raw_comp[i] << 4);
		}

		 mess= conv_Measurement(node_no, time_stamp, raw_acc, raw_gyro, raw_comp);


	} 
		//エコーモードの時
		/*
	else if (data[0] == 'E') {
		if (data[1] == 'E' && data[2] == 'C') {
			// エコーの応答
			int node_no = data[3];
			display_Echo(node_no);
		} else if (data[1] == 'S' && data[2] == 'T') {
			// ステータス情報
			int node_no = data[3];
			int role = data[4];
			int period = (int)data[5] << 8 | data[6];
			int range_acc = data[7];
			int range_gyro = data[8];
			int raw_batt = data[9];
			int range_comp = data[10];
			int binary = data[11];

			{
				StatusData stat = conv_Status(node_no, role, period, range_acc, range_gyro, range_comp, raw_batt, binary);
				display_Status(stat);
			}
		} else if (data[1] == 'D' && data[2] == 'P') {
			// デバイスプロファイル情報
			ProfileData prof;
			prof.node_no = data[3];
			prof.hardware = data[4];
			prof.firmware = data[5];
			_stprintf(prof.bd_addr, _T("%02x%02x%02x%02x%02x%02x"), 
				data[6], data[7], data[8], data[9], data[10], data[11]);
			display_ProfileData(prof);
		}
	}
	*/
	return mess;
}


MeasurementData CIMUZ::conv_Measurement(int node_no, int timestamp, int acc[3], int gyro[3], int comp[3])
{
	int i;
	MeasurementData mdata;

	mdata.node_no = node_no;
	mdata.time = conv_Timestamp(node_no, timestamp);
	for (i = 0; i < 3; i++) {
		mdata.acc[i] = conv_Accelerometer(node_no, acc[i]);
	}
	for (i = 0; i < 3; i++) {
		mdata.gyro[i] = conv_Gyroscope(node_no, gyro[i]);
	}
	for (i = 0; i < 3; i++) {
		mdata.comp[i] = conv_Compass(node_no, comp[i]);
	}
	return mdata;	
}

int CIMUZ::conv_Init(void)
{
	int i;

	for (i = 1;i <= 29; i++) {
		s_accRatio[i] = 1.0f;
		s_gyroRatio[i] = 1.0f;
		s_compRatio[i] = 1.0f / 1090;
		s_battRatio[i] = ((16.0f) * 3.3f / 4096) / (3.3f / (3.3f + 18));
		s_periodScale[i] = 10;
		s_timestamp_scale[i] = 10;
	}


	return 0;
}
//レンジをそろえる
int CIMUZ::conv_SetupFactors(UINT node_no, UINT hardware, AccelerometerRange range_acc, GyroscopeRange range_gyro, CompassRange range_comp, BOOL extend)
{
	int beg, end;
	int i;
	
	if (node_no == BROADCAST_NODE_NO) {
		beg = 1;
		end = 28;
	} else {
		beg = (int)node_no;
		end = (int)node_no;
	}

	if (hardware == HARDWARE_IMUZ_2) {
		for (i = beg; i <= end; i++) {
			s_accRatio[i] = (1) / (1.0f * 1000.0f) * imuz2_rangeRatioAcc(range_acc) / 16.0f;
			s_gyroRatio[i] = (1) / (1.0f * 1000.0f) * imuz2_rangeRatioGyro(range_gyro);
			s_compRatio[i] = imuz2_rangeRatioComp(range_comp) / 16.0f;
			s_battRatio[i] = ((16.0f) * 3.3f / 4096) / (3.3f / (3.3f + 18));
			s_periodScale[i] = 10;
			if (extend) {
				s_timestamp_scale[i] = 1;
			} else {
				s_timestamp_scale[i] = 10;
			}

		}
	}


	return 0;
}


int CIMUZ::conv_Timestamp(int node_no, int raw_time)
{
	return raw_time * s_timestamp_scale[node_no];
}

float CIMUZ::conv_Accelerometer(int node_no, int raw_acc)
{
	return raw_acc * s_accRatio[node_no];
}


float CIMUZ::conv_Gyroscope(int node_no, int raw_gyro)
{
	return raw_gyro * s_gyroRatio[node_no];
}


float CIMUZ::conv_Compass(int node_no, int raw_comp)
{
	return raw_comp* s_compRatio[node_no];
}


float CIMUZ::imuz2_rangeRatioAcc(AccelerometerRange range_acc)
{
	float r = 1.0;
	switch (range_acc) {
	case Range2g: r = 1.0f; break;
	case Range4g: r = 2.0f; break;
	case Range8g: r = 4.0f; break;
	case Range16g: r = 12.0f; break;
	}
	return r;
}
float CIMUZ::imuz2_rangeRatioGyro(GyroscopeRange range_gyro)
{
	float r = 1.0;
	switch (range_gyro) {
	case Range250dps: r = 8.75f; break;
	case Range500dps: r = 17.5f; break;
	case Range2000dps: r = 70.0f; break;
	}
	return r;

}
float CIMUZ::imuz2_rangeRatioComp(CompassRange range_comp)
{
	float r = 1.0;
	switch (range_comp) {
	case Range0_88Ga: r = 1.0f / 1370; break;
	case Range1_3Ga: r = 1.0f / 1090; break;
	case Range1_9Ga: r = 1.0f / 820; break;
	case Range2_5Ga: r = 1.0f / 660; break;
	case Range4_0Ga: r = 1.0f / 440; break;
	case Range4_7Ga: r = 1.0f / 390; break;
	case Range5_6Ga: r = 1.0f / 330; break;
	case Range8_1Ga: r = 1.0f / 230; break;
	}
	return r;
}

double CIMUZ::GyroAngleEstimation(double PreAngle,double GyroX,double gyroth,bool gyroComparison,double dt,double accX,double accY,double accZ,bool IsMovement,double c1,double c2,int mode)
{
	double Angle=0;
	double AngleAcc;

	//エラー処理
	//Gyroの値は最大レンジでも2000
	if(abs(GyroX)>30000){return PreAngle;}

	//モード毎にpreAngleを修正
	if(mode==0){
		if(PreAngle>180){PreAngle=PreAngle-360;}
		if(PreAngle<-180){PreAngle=PreAngle+360;}
	}
	else if(mode==1){
		if(PreAngle>360){PreAngle=PreAngle-360;}
		if(PreAngle<0){PreAngle=PreAngle+360;}
	}

	//ACCによる角度推定
	AngleAcc=AccAngleEsitimation(accX,accY,accZ);
	if(mode==0){
		if(AngleAcc>180){AngleAcc=AngleAcc-360;}
		if(AngleAcc<-180){AngleAcc=AngleAcc+360;}
	}
	else if(mode==1){
		if(AngleAcc>360){AngleAcc=AngleAcc-360;}
		if(AngleAcc<0){AngleAcc=AngleAcc+360;}
	}
	//ACCのみ
	//Angle=AngleAcc;
	//ジャイロのみ
	//Angle=(PreAngle+GyroX*dt);

	//相補フィルタ
	//運動中
	if(IsMovement){
		if(gyroth<abs(GyroX) && gyroComparison){Angle=PreAngle+GyroX*dt;}
		else{Angle=PreAngle;}
	}
	//未運動
	else{
		Angle=c1*(PreAngle+GyroX*dt)+c2*AngleAcc;
	}
	if(mode==0){
		if(Angle>180){Angle=Angle-360;}
		if(Angle<-180){Angle=Angle+360;}
	}
	else if(mode==1){
		if(Angle>360){Angle=Angle-360;}
		if(Angle<0){Angle=Angle+360;}
	}
	
	return Angle;
}


double CIMUZ::AccAngleEsitimation(double accX,double accY,double accZ)
{
	double AngleAcc=0;

	AngleAcc = atan2(accY,sqrt(accX*accX+accZ*accZ))/3.141592*180.0;

	if(accZ>0){}
	//±180°に対応
	//else{AngleAcc= -AngleAcc + 180;}
	
	if(AngleAcc>180){AngleAcc = AngleAcc - 360; }
	if(AngleAcc < -180){AngleAcc = AngleAcc + 360;}
	
	return AngleAcc;
}


double CIMUZ::NormAcc(double AccX,double AccY,double AccZ)
{
	double AccNorm;
	AccNorm= sqrt(AccX*AccX + AccY*AccY + AccZ*AccZ);

	return AccNorm;
}

