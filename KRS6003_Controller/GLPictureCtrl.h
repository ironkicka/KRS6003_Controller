#pragma once
#include <vector>
using namespace std;
// CGLPictureCtrl
//グローバルに定義
	struct Position{
		double x;
		double y;
	};
	struct TargetPosition{
		double r;
		double sita;
		double size;
	};
class CGLPictureCtrl : public CStatic
{
	DECLARE_DYNAMIC(CGLPictureCtrl)

public:
	CGLPictureCtrl();
	virtual ~CGLPictureCtrl();
protected:
	DECLARE_MESSAGE_MAP()
protected:
	HGLRC m_hRC;
	CDC* m_pDC;

	BOOL SetupPixelFormat();
	BOOL InitGLContext();
	virtual void PreSubclassWindow();
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	void DrawEOGMAP(double size, double x,double y, bool IsDraw,int color_num);
	int Circle(double size);
	void DrawFFT(vector<double> spectrum,double maxvalue);
	void bar(double hight, double width);
	void DrawFeatureVector(vector<double> featureVector,vector<vector<double>> teachingFV,vector<int>IsDrawTeachingFVnum,int numofemgch);
	void DrawSpeed(double speed, double max, double min,int motion);
	void DrawCircle(double size, double aspect,int type);

	int SwitchColor(int num);
	void DrawRadarChart(int NumOfElement,double aspect);
	void DrawHandSpeed(vector<double> speed, vector<int> color,double TargetSpeed);

	void DrawVRE(Position cursor,TargetPosition target,bool IsCollision,bool IsCompletion);
	void Cursor(double size);
	void guide(void);
	int DrawSignal(vector<double> sig,int framesize, double posiylim , double negaylim);
};


