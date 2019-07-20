#pragma once
#include <vector>
//com
#define MAXPORT		256
#define OTHERWISE	5
//IMUZ��1�p�P�b�g�̃T�C�Y
#define PACKET_SIZE 18

using namespace std;

/// <summary>
/// ���[���B�ʐM�m�[�h�Ƃ��Ă̖����B
/// </summary>
/// <remarks>
/// IMU-Z�́A�����䐔�Ŏg�p����ꍇ�ɁA�l�X�Ȑڑ��`�Ԃ��Ƃ邱�Ƃ��ł���B
/// ���ꂼ���IMU-Z�́A
/// Bluetooth���g�p���邩�ACAN�ʐM���܂Ƃ߂�Bluetooth�ɓ]�����邩�ACAN���g�p���邩�A
/// ��3��̂����ꂩ�ƂȂ�B
/// </remarks>
typedef enum tag_Role
{
	/// <summary>
	/// �V���O��(Bluetooth�ڑ�)
	/// </summary>
	SingleBT = 0,
	/// <summary>
	/// CAN�}�X�^(Bluetooth�ڑ�����)
	/// </summary>
	CanMasterBT,
	/// <summary>
	/// CAN�X���[�u
	/// </summary>
	CanSlave,
} Role;


typedef enum tag_AccelerometerRange
{
	/// <summary>
	/// �����W�}2[g]�B(Imuz,Imuz2)
	/// </summary>
	Range2g = 0,
	/// <summary>
	/// �����W�}4[g]�B(Imuz,Imuz2)
	/// </summary>
	Range4g,
	/// <summary>
	/// �����W�}8[g]�B(Imuz,Imuz2)
	/// </summary>
	Range8g,
	/// <summary>
	/// �����W�}16[g]�B(Imuz2)
	/// </summary>
	Range16g,

} AccelerometerRange;
	
/// <summary>
/// �W���C���Z���T�̃����W�ݒ�
/// </summary>
/// <remarks>
/// IMU-Z�ɓ��ڂ̃W���C���Z���T�͑���̃����W�Ɗ��x��2�i�K�ɐ؂�ւ�����B
/// </remarks>
typedef enum tag_GyroscopeRange
{
	/// <summary>
	/// �����W�}500[deg/sec]�B(Imuz,Imuz2)
	/// </summary>
	Range500dps = 0,
	/// <summary>
	/// �����W�}2000[deg/sec]�B(Imuz,Imuz2)
	/// </summary>
	Range2000dps,
	/// <summary>
	/// �����W�}250[deg/sec]�B(Imuz2)
	/// </summary>
	Range250dps,

} GyroscopeRange;

/// <summary>
/// �n���C�Z���T�̃����W�ݒ�
/// </summary>
/// <remarks>
/// IMU-Z�ɓ��ڂ̒n���C�Z���T�͑���̃����W�Ɗ��x��7�i�K�ɐ؂�ւ�����B
/// </remarks>
typedef enum tag_CompassRange
{
	/// <summary>
	/// �����W�}0.88[gauss] (Imuz2)
	/// </summary>
	Range0_88Ga = 0,
	/// <summary>
	/// �����W�}1.3[gauss] (Imuz2)
	/// </summary>
	Range1_3Ga,
	/// <summary>
	/// �����W�}1.9[gauss] (Imuz2)
	/// </summary>
	Range1_9Ga,
	/// <summary>
	/// �����W�}2.5[gauss] (Imuz2)
	/// </summary>
	Range2_5Ga,
	/// <summary>
	/// �����W�}4.0[gauss] (Imuz2)
	/// </summary>
	Range4_0Ga,
	/// <summary>
	/// �����W�}4.7[gauss] (Imuz2)
	/// </summary>
	Range4_7Ga,
	/// <summary>
	/// �����W�}5.6[gauss] (Imuz2)
	/// </summary>
	Range5_6Ga,
	/// <summary>
	/// �����W�}8.1[gauss] (Imuz2)
	/// </summary>
	Range8_1Ga,

	/// <summary>
	/// �����W�}0.5[gauss] (Imuz)
	/// </summary>
	Range0_7Ga = 0,
	/// <summary>
	/// �����W�}1.0[gauss] (Imuz)
	/// </summary>
	Range1_0Ga,
	/// <summary>
	/// �����W�}1.5[gauss] (Imuz)
	/// </summary>
	Range1_5Ga,
	/// <summary>
	/// �����W�}2.0[gauss] (Imuz)
	/// </summary>
	Range2_0Ga,
	/// <summary>
	/// �����W�}3.2[gauss] (Imuz)
	/// </summary>
	Range3_2Ga,
	/// <summary>
	/// �����W�}3.8[gauss] (Imuz)
	/// </summary>
	Range3_8Ga,
	/// <summary>
	/// �����W�}4.5[gauss] (Imuz)
	/// </summary>
	Range4_5Ga,

} CompassRange;



typedef struct tag_MeasurementData
{
	/// <summary>
	/// �m�[�h�ԍ�
	/// </summary>
	UINT node_no;
	/// <summary>
	/// �^�C���X�^���v�B�P�ʂ�[msec]�BIMU-Z�̋N��������J�E���g�A�b�v����A
	/// �܂��́AResetTimestamp���b�Z�[�W�ɂ���ă��Z�b�g����J�E���g�A�b�v����l�B
	/// �ő�l��32bit�̍ő�l(��50��)�B
	/// </summary>
	UINT time;
	/// <summary>
	/// �����x�Z���T�̒l3�����B�����x�A�P�ʂ�[g]�B
	/// �z��͏��ɁAx,y,z���̒l�������B
	/// </summary>
	double acc[3];
	/// <summary>
	/// �W���C���Z���T�̒l3�����B�p���x�A�P�ʂ�[deg/sec]�B
	/// �z��͏��ɁAx,y,z���̒l�������B
	/// </summary>
	double gyro[3];
	/// <summary>
	/// �n���C�Z���T�̒l3�����B�������x�A�P�ʂ�[gauss]�B
	/// �z��͏��ɁAx,y,z���̒l�������B
	/// </summary>
	double comp[3];

} MeasurementData;


/// <summary>
/// IMU-Z�X�e�[�^�X�f�[�^
/// </summary>
/// <remarks>
/// IMU-Z�̌��݂̏�ԂƁA�����ŕۑ����Ă���ݒ�l��ʒm���邽�߂̃f�[�^
/// </remarks>
typedef struct tag_StatusData
{
	/// <summary>
	/// �m�[�h�ԍ�
	/// </summary>
	UINT node_no;
	/// <summary>
	/// ���[���B�m�[�h�̖��������߂��B
	/// ���[���ɂ���ĒʐM�@�\���I�������B
	/// </summary>
	Role role;
	/// <summary>
	/// �v���X�V�Ԋu�B
	/// IMU-Z�͂���Ŏw�肳�ꂽ���ԊԊu�ŁA���ׂẴZ���T���X�L�������Ēl���擾���Ēl�𑗐M����B
	/// </summary>
	UINT period;
	/// <summary>
	/// �����x�Z���T�̃����W�̐ݒ�B3�i�K�B
	/// </summary>
	AccelerometerRange range_acc;
	/// <summary>
	/// �W���C���Z���T�̃����W�̐ݒ�B2�i�K�B
	/// </summary>
	GyroscopeRange range_gyro;
	/// <summary>
	/// �n���C�Z���T�̃����W�̐ݒ�B7�i�K�B
	/// </summary>
	CompassRange range_comp;
	/// <summary>
	/// ���݂̃o�b�e���[���x���B
	/// </summary>
	double batt;
	/// <summary>
	/// ���݂̃��b�Z�[�W�t�H�[�}�b�g�B
	/// </summary>
	UINT msg_format;
	
} StatusData;


/// <summary>
/// IMU-Z�X�e�[�^�X�f�[�^
/// </summary>
/// <remarks>
/// IMU-Z�̌��݂̏�ԂƁA�����ŕۑ����Ă���ݒ�l��ʒm���邽�߂̃f�[�^
/// </remarks>
typedef struct tag_Status2Data
{
	/// <summary>
	/// �m�[�h�ԍ�
	/// </summary>
	UINT node_no;
	/// <summary>
	/// �����x�Z���T�v���X�V�Ԋu�B
	/// </summary>
	UINT period_1;
	/// <summary>
	/// �W���C���Z���T�v���X�V�Ԋu�B
	/// </summary>
	UINT period_2;
	/// <summary>
	/// �n���C�Z���T�v���X�V�Ԋu�B
	/// </summary>
	UINT period_3;
	/// <summary>
	/// �f�[�^�^�C�v�t���O
	/// </summary>
	BYTE data_flag;
	
} Status2Data;


/// <summary>
/// �f�o�C�X�v���t�B�[���f�[�^
/// </summary>
/// <remarks>
/// IMU-Z�ŗL�̏���ʒm���邽�߂̃f�[�^�B
/// </remarks>
typedef struct tag_ProfileData
{
	/// <summary>
	/// �m�[�h�ԍ�
	/// </summary>
	UINT node_no;
	/// <summary>
	/// �n�[�h�E�F�A���r�W����
	/// </summary>
	UINT hardware;
	/// <summary>
	/// �t�@�[���E�F�A�o�[�W����
	/// </summary>
	UINT firmware;
	//public byte[] bd_addr = new byte[6];
	/// <summary>
	/// BD�A�h���X�BBluetooth�Ŏg�p����ŗL�̃A�h���X�l�B
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
	//�����W�ɂ���ČW�����Ⴂ�܂�
	// �S�Z���T���̌W����ێ�
	float s_accRatio[29];//static�@������imu�g�����Ǝv���ƕK�v��������Ȃ�
	float s_gyroRatio[29];//static�@������imu�g�����Ǝv���ƕK�v��������Ȃ�
	float s_compRatio[29];//static�@������imu�g�����Ǝv���ƕK�v��������Ȃ�
	int s_periodScale[29];//static�@������imu�g�����Ǝv���ƕK�v��������Ȃ�
	float s_battRatio[29];//static�@������imu�g�����Ǝv���ƕK�v��������Ȃ�
	int s_timestamp_scale[29];//static�@������imu�g�����Ǝv���ƕK�v��������Ȃ�

	WORD s_cnt;  //static�@������imu�g�����Ǝv���ƕK�v��������Ȃ�
	BOOL s_bRecover; //static�@������imu�g�����Ǝv���ƕK�v��������Ȃ�
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

