// GLPictureCtrl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "GLPictureCtrl.h"

//	OpenGLのヘッダをインクルード
#include <GL/gl.h>
#include <GL/glu.h>


#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "opengl32.lib")
// CGLPictureCtrl

IMPLEMENT_DYNAMIC(CGLPictureCtrl, CStatic)

CGLPictureCtrl::CGLPictureCtrl()
{
	
}

CGLPictureCtrl::~CGLPictureCtrl()
{
}


BEGIN_MESSAGE_MAP(CGLPictureCtrl, CStatic)
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CGLPictureCtrl::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,  0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int pixelformat;
	if (0 == (pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd))) {
		return FALSE;
	}

	if (FALSE == ::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd)) {
		return FALSE;
	}

	return TRUE;
}


BOOL CGLPictureCtrl::InitGLContext()
{
	m_pDC = new CClientDC(this);

	if (NULL == m_pDC) {
		return FALSE;
	}
	if (!SetupPixelFormat()) return FALSE;
	if (0 == (m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc()))) {
		return FALSE;
	}
	if (FALSE == ::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC)) {
		return FALSE;
	}

	return TRUE;
}

// CGLPictureCtrl メッセージ ハンドラー




void CGLPictureCtrl::PreSubclassWindow()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	LONG  style = GetWindowLong(this->m_hWnd, GWL_STYLE );
	style |=  WS_CLIPSIBLINGS | WS_CLIPCHILDREN ;
  	SetWindowLong( this->m_hWnd, GWL_STYLE, style );


	InitGLContext();
	CStatic::PreSubclassWindow();
}


void CGLPictureCtrl::OnDestroy()
{
	CStatic::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	if(FALSE == ::wglMakeCurrent(NULL, NULL)) {
		//	必要に応じてエラーハンドリング
	}

	if(FALSE == ::wglDeleteContext(m_hRC)) {
		//	必要に応じてエラーハンドリング
	}

	if(m_pDC) delete m_pDC;
}


void CGLPictureCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting


	// TODO: ここにメッセージ ハンドラー コードを追加します。
	// 描画メッセージで CStatic::OnPaint() を呼び出さないでください。
	
}


void CGLPictureCtrl::DrawEOGMAP(double size, double x,double y, bool IsDraw,int color_num)
{
	//このレンタリングコンテキストをカレントへ変更
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	//背景を黒へ
	glClearColor(0.0, 0.0, 0.0, 1);
	//size,x,yは±1の範囲で
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//	とりあえず、四角形を描画してみる。
	//座標はたぶん±1がピクチャーコントロールのサイズ
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );
	//色指定
	switch(color_num%3){
		case 0:
			glColor3f(1.0, 0.0, 0.0); // 描画物体に色を設定
			break;
		case 1:
			glColor3f(0.0, 1.0, 0.0); // 描画物体に色を設定
			break;
		case 2:
			glColor3f(0.0, 0.0, 1.0); // 描画物体に色を設定
			break;
		default:
			glColor3f(1.0, 1.0, 1.0); // 描画物体に白色を設定
			break;
	}
	//円を描画
	if(IsDraw){
		glPushMatrix();
		glTranslatef(GLfloat(x), GLfloat(y), 0);
		Circle(size);
		glPopMatrix();
	}


	::glPopMatrix();

	::glFinish();
	if( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc())){}


}


int CGLPictureCtrl::Circle(double size)
{
	int part=100;//分割数
	//x,yのアスペクト比
	//配置したピクチャーコントロールの大きさを縦横1として扱っている模様
	float aspect;//大きくするとx方向が短くなる
	int i;
	double rate;
	
	double M_PI=3.1415926535897;
	double x,y,r;
	
	aspect=1.7;//大きくするとx方向が短くなる

	r=size;
	glBegin(GL_POLYGON); // ポリゴンの描画
	// 円を描画
	for (i = 0; i < part; i++) {
		rate = (double)i / part;
		x = r * cos(2.0 * M_PI * rate)/aspect;
		y = r * sin(2.0 * M_PI * rate);
		glVertex3f(GLfloat(x), GLfloat(y), 0.0); // 頂点座標を指定
	}
	glEnd(); // ポリゴンの描画終了

	return 0;
}

void CGLPictureCtrl::DrawCircle(double size, double aspect,int type=0)
{
	int part=100;//分割数
	//x,yのアスペクト比
	//配置したピクチャーコントロールの大きさを縦横1として扱っている模様
	int i;
	double rate;
	
	double M_PI=3.1415926535897;
	double x,y,r;
	
	r=size;

	if(type==0){glBegin(GL_LINE_LOOP);} // ポリゴンの描画
	if(type==1){glBegin(GL_POLYGON);}
	// 円を描画
	for (i = 0; i < part; i++) {
		rate = (double)i / part;
		x = r * cos(2.0 * M_PI * rate)/aspect;
		y = r * sin(2.0 * M_PI * rate);
		glVertex3f(GLfloat(x), GLfloat(y), 0.0); // 頂点座標を指定
	}
	glEnd(); // ポリゴンの描画終了

}

void CGLPictureCtrl::DrawFeatureVector(vector<double> featureVector,vector<vector<double>> teachingFV,vector<int>IsDrawTeachingFVnum,int numofemgch)
{

	int i,j;
	double rate;
	double aspect=1.0;
	double M_PI=3.1415926535897;
	double x,y;
	//このレンタリングコンテキストをカレントへ変更
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	//背景を白へ
	glClearColor(1.0, 1.0, 1.0, 1);
	
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//座標はたぶん±1がピクチャーコントロールのサイズ
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );

	//円を描画
	glColor3f(0.0, 0.0, 0.0); // 描画物体に黒色を設定
	DrawCircle(1.0,aspect);
	
	//レーダーチャート
	glEnable(GL_LINE_STIPPLE);//破線を有効
	glLineStipple(1,0xF0F0);//破線のパターン設定
	glColor3f(GLfloat(0.6), GLfloat(0.6), GLfloat(0.6)); // 描画物体に灰色を設定
	DrawRadarChart(numofemgch,aspect);
	glDisable(GL_LINE_STIPPLE);//破線を無効

	// fetureVectorを描画
	glColor3f(0.0, 0.0, 0.0); // 描画物体に黒色を設定
	glBegin(GL_LINE_LOOP); // 線描画
	for (i = 0; i < featureVector.size(); i++) {
		rate = (double)i / featureVector.size();
		x = featureVector[i] * cos(M_PI/2.0-2.0 * M_PI * rate)/aspect;
		y =  featureVector[i] * sin(M_PI/2.0-2.0 * M_PI * rate);
		glVertex3f(GLfloat(x), GLfloat(y), 0.0); // 頂点座標を指定
	}
	glEnd(); // 描画終了

	//教示したFeatureVectorを描画
	
	for(j=0; j<teachingFV.size(); j++){
		if(IsDrawTeachingFVnum[j]==-1){}
		else{
			//色変更
			SwitchColor(IsDrawTeachingFVnum[j]);
			glBegin(GL_LINE_LOOP); // 線描画
			for (i = 0; i < teachingFV[j].size(); i++) {
				rate = (double)i / teachingFV[j].size();
				x = teachingFV[j][i] * cos(M_PI/2.0-2.0 * M_PI * rate)/aspect;
				y =  teachingFV[j][i] * sin(M_PI/2.0-2.0 * M_PI * rate);
				glVertex3f(GLfloat(x), GLfloat(y), 0.0); // 頂点座標を指定
			}
			glEnd(); // 描画終了
		}
	}
	
	::glPopMatrix();

	::glFinish();
	if( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc())){}

}


void CGLPictureCtrl::DrawSpeed(double speed, double max, double min,int motion)
{
	double hight;
	//このレンタリングコンテキストをカレントへ変更
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	//背景を黒へ
	glClearColor(0.0, 0.0, 0.0, 1);
	
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//	とりあえず、四角形を描画してみる。
	//座標はたぶん±1がピクチャーコントロールのサイズ
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );
	//色指定
	SwitchColor(motion);
	
	hight=((0.95-0.05)*speed+0.05*max-0.95*min)/(max-min);
	glPushMatrix();
		glTranslatef(0.5, 0, 0);
		bar(2.0*hight,1.0);
		glPopMatrix();
	

	::glPopMatrix();

	::glFinish();
	if( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc())){}

}


void CGLPictureCtrl::DrawFFT(vector<double> spectrum,double maxvalue)
{
	double size;
	size=maxvalue;
	//このレンタリングコンテキストをカレントへ変更
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	int i;
	//背景を白へ
	glClearColor(1.0, 1.0, 1.0, 1);
	
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//	とりあえず、四角形を描画してみる。
	//座標はたぶん±1がピクチャーコントロールのサイズ
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );
	//色指定
	glColor3f(0.0, 0.0, 0.0); // 描画物体に黒色を設定
	
	//棒グラフ±1.0の空間
	for(i=0; i<spectrum.size(); i++){
		glPushMatrix();
		glTranslatef(i*2.0*0.5/double(spectrum.size()), 0, 0);//棒
		glTranslatef((i+1)*2.0*0.5/double(spectrum.size()+1), 0, 0);//余白
		bar(spectrum[i]/size,2.0*0.5/double(spectrum.size()));
		glPopMatrix();
	}
	
	

	::glPopMatrix();

	::glFinish();
	if( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc())){}

}


void CGLPictureCtrl::bar(double hight, double width)
{
	glBegin(GL_POLYGON); // ポリゴンの描画
		glVertex3f(-1.0,         -1.0        ,  0.0); // 頂点座標を指定
		glVertex3f((-1.0+width), -1.0        ,  0.0); // 頂点座標を指定
		glVertex3f((-1.0+width), (-1.0+hight),  0.0); // 頂点座標を指定
		glVertex3f(-1.0,         (-1.0+hight),  0.0); // 頂点座標を指定
	glEnd(); // ポリゴンの描画終了


}



int CGLPictureCtrl::SwitchColor(int num)
{
	switch(num){
		case 0  : glColor3f(1.000f, 0.000, 0.000); break;
		case 1  : glColor3f(0.000f, 1.000, 0.000); break;
		case 2  : glColor3f(0.000f, 0.000, 1.000); break;
		case 3  : glColor3f(1.000f, 1.000, 0.000); break;
		case 4  : glColor3f(1.000f, 0.000, 1.000); break;
		case 5  : glColor3f(0.000f, 1.000, 1.000); break;
		case 6  : glColor3f(0.741f, 0.718, 0.420); break;
		case 7  : glColor3f(0.804f, 0.361, 0.361); break;
		case 8  : glColor3f(0.729f, 0.333, 0.827); break;
		case 9  : glColor3f(0.000f, 1.000, 0.498); break;
		case 10 : glColor3f(1.000f, 0.498, 0.141) ; break;
		default : glColor3f(0.0, 0.0, 0.0); break;//黒

	}


	return 0;
}


void CGLPictureCtrl::DrawRadarChart(int NumOfElement,double aspect)
{
	int i;
	double rate,angle;;
	double x,y;
	double M_PI=3.1415926535;
	
	rate = 1.0 / (double)NumOfElement;
	angle=0.0;

	// ポリゴンの描画
	glBegin(GL_LINE_LOOP); 
	for(i=0; i<NumOfElement; i++){
		glVertex3f(0.0, 0.0, 0.0); 
		x = 1.0 * cos(M_PI/2.0-2.0 * M_PI * angle)/aspect;
		y = 1.0 * sin(M_PI/2.0-2.0 * M_PI * angle);
		glVertex3f(x, y, 0.0);
		angle=angle+rate;
		x = 1.0 * cos(M_PI/2.0-2.0 * M_PI * angle)/aspect;
		y = 1.0 * sin(M_PI/2.0-2.0 * M_PI * angle);
		glVertex3f(x, y, 0.0);
	}
		
	glEnd(); // ポリゴンの描画終了

}

void CGLPictureCtrl::DrawHandSpeed(vector<double> speed, vector<int> color,double TargetSpeed){
	
	//このレンタリングコンテキストをカレントへ変更
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	int i;
	double x,y;
	double dx;
	//背景を白へ
	glClearColor(1.0, 1.0, 1.0, 1);
	
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//座標はたぶん±1がピクチャーコントロールのサイズ
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );
	//色指定
	glColor3f(0.0, 0.0, 0.0); // 描画物体に黒色を設定

	//速度描画
	x=-1.0;
	dx=2.0/double(speed.size());

	glLineWidth(2.0); 
	glBegin(GL_LINE_STRIP); 
	for(i=0; i<speed.size(); i++){
		SwitchColor(color[i]);
		x+=dx;
		y=1.8*speed[i]-0.9;
		if(y>0.9){y=0.9;}
		else if(y<-0.9){y=-0.9;}
		glVertex3f(x, y, 0.0);
	}
	glEnd(); // ポリゴンの描画終了

	//目標値描画
	glLineWidth(3.0); 
	glBegin(GL_LINE_STRIP); 
	glColor3f(0.0, 0.0, 0.0); // 描画物体に黒色を設定

	glVertex3f(-1, 1.8*TargetSpeed-0.9, 0.0);
	glVertex3f(1, 1.8*TargetSpeed-0.9, 0.0);

	glEnd(); // ポリゴンの描画終了



	::glPopMatrix();

	::glFinish();
	if( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc())){}


}

void CGLPictureCtrl::DrawVRE(Position cursor, TargetPosition target,bool IsCollision,bool IsCompletion)
{
	//このレンタリングコンテキストをカレントへ変更
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	//背景を白へ
	glClearColor(1.0, 1.0, 1.0, 1);
	//size,x,yは±1の範囲で
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//	とりあえず、四角形を描画してみる。
	//座標はたぶん±1がピクチャーコントロールのサイズ
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );
	//ガイドの描画
	glColor3f(0.0, 0.0, 0.0); // ガイドを黒色に設定
	guide();
	//カーソルの描画
	glColor3f(1.0, 0.0, 0.0); // カーソルを赤色に設定
	glPushMatrix();
		glTranslatef(cursor.x, cursor.y, 0);
		Cursor(0.05);
	glPopMatrix();
	
	//ターゲットの描画
	if(IsCollision && !IsCompletion){glColor3f(1.0, 0.65, 0.0);}// 当たり判定ターゲットを橙色に設定 1秒以下
	else if(IsCollision && IsCompletion){glColor3f(1.0, 0.65, 0.0);}// タスク完了ターゲットを赤色に設定 1秒以上
	else{glColor3f(0.0, 0.0, 1.0);}// ターゲットを青色に設定

	glPushMatrix();
		glRotatef(target.sita,0.0,0.0,1.0);
		glTranslatef(target.r,0.0, 0);
		if(IsCompletion){DrawCircle(target.size,1.0,1);}
		else{DrawCircle(target.size,1.0);}
	glPopMatrix();
	


	::glPopMatrix();

	::glFinish();
	if( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc())){}


}




void CGLPictureCtrl::Cursor(double size)
{
	glBegin(GL_LINE_STRIP); // ポリゴンの描画
		glVertex3f(-size , 0.0 , 0.0); // 頂点座標を指定
		glVertex3f( size , 0.0 , 0.0); // 頂点座標を指定
	glEnd(); // ポリゴンの描画終了
	glBegin(GL_LINE_STRIP); // ポリゴンの描画
		glVertex3f(0.0 , -size , 0.0); // 頂点座標を指定
		glVertex3f(0.0 ,  size , 0.0); // 頂点座標を指定
	glEnd(); // ポリゴンの描画終了


}


void CGLPictureCtrl::guide(void)
{
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1 , 0xF0F0);

	glBegin(GL_LINE_STRIP); // ポリゴンの描画
		glVertex3f(-1.0 , 0.0 , 0.0); // 頂点座標を指定
		glVertex3f( 1.0 , 0.0 , 0.0); // 頂点座標を指定
	glEnd(); // ポリゴンの描画終了
	glBegin(GL_LINE_STRIP); // ポリゴンの描画
		glVertex3f(0.0 , -1.0 , 0.0); // 頂点座標を指定
		glVertex3f(0.0 ,  1.0 , 0.0); // 頂点座標を指定
	glEnd(); // ポリゴンの描画終了

	glDisable(GL_LINE_STIPPLE);

}


int CGLPictureCtrl::DrawSignal(vector<double> sig,int framesize, double posiylim , double negaylim)
{
	int i;
	double x,y;
	double dx;
	//このレンタリングコンテキストをカレントへ変更
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	//背景を白へ
	glClearColor(1.0, 1.0, 1.0, 1);
	//size,x,yは±1の範囲で
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//	とりあえず、四角形を描画してみる。
	//座標はたぶん±1がピクチャーコントロールのサイズ
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );

	glColor3f(0.5, 0.5, 0.5); // 描画物体に灰色を設定

	//中心線の描画
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1 , 0xF0F0);
	glBegin(GL_LINE_STRIP); // ポリゴンの描画
		glVertex3f(-1.0 , 0.0 , 0.0); // 頂点座標を指定
		glVertex3f(1.0 , 0.0 , 0.0); // 頂点座標を指定
	glEnd(); // ポリゴンの描画終了
	//上半分
	glBegin(GL_LINE_STRIP); // ポリゴンの描画
		glVertex3f(-1.0 , 0.5 , 0.0); // 頂点座標を指定
		glVertex3f(1.0 , 0.5 , 0.0); // 頂点座標を指定
	glEnd(); // ポリゴンの描画終了
	//下半分
	glBegin(GL_LINE_STRIP); // ポリゴンの描画
		glVertex3f(-1.0 , -0.5 , 0.0); // 頂点座標を指定
		glVertex3f(1.0 , -0.5 , 0.0); // 頂点座標を指定
	glEnd(); // ポリゴンの描画終了
	glDisable(GL_LINE_STIPPLE);


	glColor3f(1.0, 0.0, 0.0); // 描画物体に黒色を設定
	//直線の描画
	glBegin(GL_LINE_STRIP); // ポリゴンの描画
	if(posiylim==negaylim){return -1;}
	dx=2/(double)framesize;
	x=-1;
	for(i=0; i<framesize; i++){
		y=(2*sig[i]-posiylim-negaylim)/(posiylim-negaylim);
		if(y>posiylim){y=posiylim;}
		else if(y<negaylim){y=negaylim;}
		glVertex3f(x , y , 0.0); // 頂点座標を指定
		x+=dx;
	}
	glEnd(); // ポリゴンの描画終了

	::glPopMatrix();

	::glFinish();
	if( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc())){}

}
