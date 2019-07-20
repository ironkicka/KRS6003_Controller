#pragma once
#include <vector>
//com
#define MAXPORT		256
#define OTHERWISE	5
//IMUZの1パケットのサイズ
#define PACKET_SIZE 18

using namespace std;

/// <summary>
/// ロール。通信ノードとしての役割。
/// </summary>
/// <remarks>
/// IMU-Zは、複数台数で使用する場合に、様々な接続形態をとることができる。
/// それぞれのIMU-Zは、
/// Bluetoothを使用するか、CAN通信をまとめてBluetoothに転送するか、CANを使用するか、
/// の3種のいずれかとなる。
/// </remarks>
typedef enum tag_Role
{
	/// <summary>
	/// シングル(Bluetooth接続)
	/// </summary>
	SingleBT = 0,
	/// <summary>
	/// CANマスタ(Bluetooth接続あり)
	/// </summary>
	CanMasterBT,
	/// <summary>
	/// CANスレーブ
	/// </summary>
	CanSlave,
} Role;


typedef enum tag_AccelerometerRange
{
	/// <summary>
	/// レンジ±2[g]。(Imuz,Imuz2)
	/// </summary>
	Range2g = 0,
	/// <summary>
	/// レンジ±4[g]。(Imuz,Imuz2)
	/// </summary>
	Range4g,
	/// <summary>
	/// レンジ±8[g]。(Imuz,Imuz2)
	/// </summary>
	Range8g,
	/// <summary>
	/// レンジ±16[g]。(Imuz2)
	/// </summary>
	Range16g,

} AccelerometerRange;
	
/// <summary>
/// ジャイロセンサのレンジ設定
/// </summary>
/// <remarks>
/// IMU-Zに搭載のジャイロセンサは測定のレンジと感度を2段階に切り替えられる。
/// </remarks>
typedef enum tag_GyroscopeRange
{
	/// <summary>
	/// レンジ±500[deg/sec]。(Imuz,Imuz2)
	/// </summary>
	Range500dps = 0,
	/// <summary>
	/// レンジ±2000[deg/sec]。(Imuz,Imuz2)
	/// </summary>
	Range2000dps,
	/// <summary>
	/// レンジ±250[deg/sec]。(Imuz2)
	/// </summary>
	Range250dps,

} GyroscopeRange;

/// <summary>
/// 地磁気センサのレンジ設定
/// </summary>
/// <remarks>
/// IMU-Zに搭載の地磁気センサは測定のレンジと感度を7段階に切り替えられる。
/// </remarks>
typedef enum tag_CompassRange
{
	/// <summary>
	/// レンジ±0.88[gauss] (Imuz2)
	/// </summary>
	Range0_88Ga = 0,
	/// <summary>
	/// レンジ±1.3[gauss] (Imuz2)
	/// </summary>
	Range1_3Ga,
	/// <summary>
	/// レンジ±1.9[gauss] (Imuz2)
	/// </summary>
	Range1_9Ga,
	/// <summary>
	/// レンジ±2.5[gauss] (Imuz2)
	/// </summary>
	Range2_5Ga,
	/// <summary>
	/// レンジ±4.0[gauss] (Imuz2)
	/// </summary>
	Range4_0Ga,
	/// <summary>
	/// レンジ±4.7[gauss] (Imuz2)
	/// </summary>
	Range4_7Ga,
	/// <summary>
	/// レンジ±5.6[gauss] (Imuz2)
	/// </summary>
	Range5_6Ga,
	/// <summary>
	/// レンジ±8.1[gauss] (Imuz2)
	/// </summary>
	Range8_1Ga,

	/// <summary>
	/// レンジ±0.5[gauss] (Imuz)
	/// </summary>
	Range0_7Ga = 0,
	/// <summary>
	/// レンジ±1.0[gauss] (Imuz)
	/// </summary>
	Range1_0Ga,
	/// <summary>
	/// レンジ±1.5[gauss] (Imuz)
	/// </summary>
	Range1_5Ga,
	/// <summary>
	/// レンジ±2.0[gauss] (Imuz)
	/// </summary>
	Range2_0Ga,
	/// <summary>
	/// レンジ±3.2[gauss] (Imuz)
	/// </summary>
	Range3_2Ga,
	/// <summary>
	/// レンジ±3.8[gauss] (Imuz)
	/// </summary>
	Range3_8Ga,
	/// <summary>
	/// レンジ±4.5[gauss] (Imuz)
	/// </summary>
	Range4_5Ga,

} CompassRange;



typedef struct tag_MeasurementData
{
	/// <summary>
	/// ノード番号
	/// </summary>
	UINT node_no;
	/// <summary>
	/// タイムスタンプ。単位は[msec]。IMU-Zの起動時からカウントアップする、
	/// または、ResetTimestampメッセージによってリセットされカウントアップする値。
	/// 最大値は32bitの最大値(約50日)。
	/// </summary>
	UINT time;
	/// <summary>
	/// 加速度センサの値3軸分。加速度、単位は[g]。
	/// 配列は順に、x,y,z軸の値を示す。
	/// </summary>
	double acc[3];
	/// <summary>
	/// ジャイロセンサの値3軸分。角速度、単位は[deg/sec]。
	/// 配列は順に、x,y,z軸の値を示す。
	/// </summary>
	double gyro[3];
	/// <summary>
	/// 地磁気センサの値3軸分。磁束密度、単位は[gauss]。
	/// 配列は順に、x,y,z軸の値を示す。
	/// </summary>
	double comp[3];

} MeasurementData;


/// <summary>
/// IMU-Zステータスデータ
/// </summary>
/// <remarks>
/// IMU-Zの現在の状態と、内部で保存している設定値を通知するためのデータ
/// </remarks>
typedef struct tag_StatusData
{
	/// <summary>
	/// ノード番号
	/// </summary>
	UINT node_no;
	/// <summary>
	/// ロール。ノードの役割をしめす。
	/// ロールによって通信機能が選択される。
	/// </summary>
	Role role;
	/// <summary>
	/// 計測更新間隔。
	/// IMU-Zはこれで指定された時間間隔で、すべてのセンサをスキャンして値を取得して値を送信する。
	/// </summary>
	UINT period;
	/// <summary>
	/// 加速度センサのレンジの設定。3段階。
	/// </summary>
	AccelerometerRange range_acc;
	/// <summary>
	/// ジャイロセンサのレンジの設定。2段階。
	/// </summary>
	GyroscopeRange range_gyro;
	/// <summary>
	/// 地磁気センサのレンジの設定。7段階。
	/// </summary>
	CompassRange range_comp;
	/// <summary>
	/// 現在のバッテリーレベル。
	/// </summary>
	double batt;
	/// <summary>
	/// 現在のメッセージフォーマット。
	/// </summary>
	UINT msg_format;
	
} StatusData;


/// <summary>
/// IMU-Zステータスデータ
/// </summary>
/// <remarks>
/// IMU-Zの現在の状態と、内部で保存している設定値を通知するためのデータ
/// </remarks>
typedef struct tag_Status2Data
{
	/// <summary>
	/// ノード番号
	/// </summary>
	UINT node_no;
	/// <summary>
	/// 加速度センサ計測更新間隔。
	/// </summary>
	UINT period_1;
	/// <summary>
	/// ジャイロセンサ計測更新間隔。
	/// </summary>
	UINT period_2;
	/// <summary>
	/// 地磁気センサ計測更新間隔。
	/// </summary>
	UINT period_3;
	/// <summary>
	/// データタイプフラグ
	/// </summary>
	BYTE data_flag;
	
} Status2Data;


/// <summary>
/// デバイスプロフィールデータ
/// </summary>
/// <remarks>
/// IMU-Z固有の情報を通知するためのデータ。
/// </remarks>
typedef struct tag_ProfileData
{
	/// <summary>
	/// ノード番号
	/// </summary>
	UINT node_no;
	/// <summary>
	/// ハードウェアリビジョン
	/// </summary>
	UINT hardware;
	/// <summary>
	/// ファームウェアバージョン
	/// </summary>
	UINT firmware;
	//public byte[] bd_addr = new byte[6];
	/// <summary>
	/// BDアドレス。Bluetoothで使用する固有のアドレス値。
	/// </summary>
	TCHAR bd_addr[7];

} ProfileData;
#define BROADCAST_NODE_NO 31


enum HardwareVersion
{
	HARDWARE_IMUZ = 1, 
	HARDWARE_IMUZ_2 = 31, 
};

class CIMUZ
{
public:
	CIMUZ(void);
	~CIMUZ(void);
	bool CommPortOpen(unsigned int NumberOfCommPort, unsigned long baudRate, unsigned char stopBits, unsigned char parity, unsigned char byteSize);

protected:
	LPCTSTR			commPort;
	DCB				dcb;
	HANDLE			commHandle;
	COMMTIMEOUTS	timeouts;
	//vector<LPCTSTR>			availablePortName;
	//vector<vector<_TCHAR>>	availablePortName;
	vector<unsigned long>	availableBaudRate;
	vector<unsigned char>	availableStopBits;
	vector<unsigned char>	availableParity;
	vector<unsigned char>	availableByteSize;
	bool isError;
	

public:
	//レンジによって係数が違います
	// 全センサ分の係数を保持
	float s_accRatio[29];//static　複数のimu使おうと思うと必要かもしれない
	float s_gyroRatio[29];//static　複数のimu使おうと思うと必要かもしれない
	float s_compRatio[29];//static　複数のimu使おうと思うと必要かもしれない
	int s_periodScale[29];//static　複数のimu使おうと思うと必要かもしれない
	float s_battRatio[29];//static　複数のimu使おうと思うと必要かもしれない
	int s_timestamp_scale[29];//static　複数のimu使おうと思うと必要かもしれない

	WORD s_cnt;  //static　複数のimu使おうと思うと必要かもしれない
	BOOL s_bRecover; //static　複数のimu使おうと思うと必要かもしれない
	bool CommPortClose(void);
	bool com_ReadPacket(PBYTE lpBuff, DWORD readMax);
	bool IMUflag;

	int ReceiveDataSize(void);
	MeasurementData IMUZBinaryDataDecode(PBYTE data);
	MeasurementData conv_Measurement(int node_no, int timestamp, int acc[3], int gyro[3], int comp[3]);


	int conv_Init(void);
	int conv_SetupFactors(UINT node_no, UINT hardware, AccelerometerRange range_acc, GyroscopeRange range_gyro, CompassRange range_comp, BOOL extend);
	int conv_Timestamp(int node_no, int raw_time);
	float conv_Accelerometer(int node_no, int raw_acc);
	float conv_Gyroscope(int node_no, int raw_gyro);
	float conv_Compass(int node_no, int raw_comp);
	
	float imuz2_rangeRatioAcc(AccelerometerRange range_acc);
	float imuz2_rangeRatioGyro(GyroscopeRange range_gyro);
	float imuz2_rangeRatioComp(CompassRange range_comp);
	double GyroAngleEstimation(double PreAngle,double GyroX,double gyroth,bool gyroComparison,double dt,double accX,double accY,double accZ,bool IsMovement,double c1,double c2,int mode);
	double AccAngleEsitimation(double accX,double accY,double accZ);
	double NormAcc(double AccX,double AccY,double AccZ);

	
};

