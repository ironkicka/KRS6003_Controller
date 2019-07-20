// GLPictureCtrl.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "GLPictureCtrl.h"

//	OpenGL�̃w�b�_���C���N���[�h
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

// CGLPictureCtrl ���b�Z�[�W �n���h���[




void CGLPictureCtrl::PreSubclassWindow()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	LONG  style = GetWindowLong(this->m_hWnd, GWL_STYLE );
	style |=  WS_CLIPSIBLINGS | WS_CLIPCHILDREN ;
  	SetWindowLong( this->m_hWnd, GWL_STYLE, style );


	InitGLContext();
	CStatic::PreSubclassWindow();
}


void CGLPictureCtrl::OnDestroy()
{
	CStatic::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	if(FALSE == ::wglMakeCurrent(NULL, NULL)) {
		//	�K�v�ɉ����ăG���[�n���h�����O
	}

	if(FALSE == ::wglDeleteContext(m_hRC)) {
		//	�K�v�ɉ����ăG���[�n���h�����O
	}

	if(m_pDC) delete m_pDC;
}


void CGLPictureCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting


	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� CStatic::OnPaint() ���Ăяo���Ȃ��ł��������B
	
}


void CGLPictureCtrl::DrawEOGMAP(double size, double x,double y, bool IsDraw,int color_num)
{
	//���̃����^�����O�R���e�L�X�g���J�����g�֕ύX
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	//�w�i������
	glClearColor(0.0, 0.0, 0.0, 1);
	//size,x,y�́}1�͈̔͂�
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//	�Ƃ肠�����A�l�p�`��`�悵�Ă݂�B
	//���W�͂��Ԃ�}1���s�N�`���[�R���g���[���̃T�C�Y
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );
	//�F�w��
	switch(color_num%3){
		case 0:
			glColor3f(1.0, 0.0, 0.0); // �`�敨�̂ɐF��ݒ�
			break;
		case 1:
			glColor3f(0.0, 1.0, 0.0); // �`�敨�̂ɐF��ݒ�
			break;
		case 2:
			glColor3f(0.0, 0.0, 1.0); // �`�敨�̂ɐF��ݒ�
			break;
		default:
			glColor3f(1.0, 1.0, 1.0); // �`�敨�̂ɔ��F��ݒ�
			break;
	}
	//�~��`��
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
	int part=100;//������
	//x,y�̃A�X�y�N�g��
	//�z�u�����s�N�`���[�R���g���[���̑傫�����c��1�Ƃ��Ĉ����Ă���͗l
	float aspect;//�傫�������x�������Z���Ȃ�
	int i;
	double rate;
	
	double M_PI=3.1415926535897;
	double x,y,r;
	
	aspect=1.7;//�傫�������x�������Z���Ȃ�

	r=size;
	glBegin(GL_POLYGON); // �|���S���̕`��
	// �~��`��
	for (i = 0; i < part; i++) {
		rate = (double)i / part;
		x = r * cos(2.0 * M_PI * rate)/aspect;
		y = r * sin(2.0 * M_PI * rate);
		glVertex3f(GLfloat(x), GLfloat(y), 0.0); // ���_���W���w��
	}
	glEnd(); // �|���S���̕`��I��

	return 0;
}

void CGLPictureCtrl::DrawCircle(double size, double aspect,int type=0)
{
	int part=100;//������
	//x,y�̃A�X�y�N�g��
	//�z�u�����s�N�`���[�R���g���[���̑傫�����c��1�Ƃ��Ĉ����Ă���͗l
	int i;
	double rate;
	
	double M_PI=3.1415926535897;
	double x,y,r;
	
	r=size;

	if(type==0){glBegin(GL_LINE_LOOP);} // �|���S���̕`��
	if(type==1){glBegin(GL_POLYGON);}
	// �~��`��
	for (i = 0; i < part; i++) {
		rate = (double)i / part;
		x = r * cos(2.0 * M_PI * rate)/aspect;
		y = r * sin(2.0 * M_PI * rate);
		glVertex3f(GLfloat(x), GLfloat(y), 0.0); // ���_���W���w��
	}
	glEnd(); // �|���S���̕`��I��

}

void CGLPictureCtrl::DrawFeatureVector(vector<double> featureVector,vector<vector<double>> teachingFV,vector<int>IsDrawTeachingFVnum,int numofemgch)
{

	int i,j;
	double rate;
	double aspect=1.0;
	double M_PI=3.1415926535897;
	double x,y;
	//���̃����^�����O�R���e�L�X�g���J�����g�֕ύX
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	//�w�i�𔒂�
	glClearColor(1.0, 1.0, 1.0, 1);
	
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//���W�͂��Ԃ�}1���s�N�`���[�R���g���[���̃T�C�Y
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );

	//�~��`��
	glColor3f(0.0, 0.0, 0.0); // �`�敨�̂ɍ��F��ݒ�
	DrawCircle(1.0,aspect);
	
	//���[�_�[�`���[�g
	glEnable(GL_LINE_STIPPLE);//�j����L��
	glLineStipple(1,0xF0F0);//�j���̃p�^�[���ݒ�
	glColor3f(GLfloat(0.6), GLfloat(0.6), GLfloat(0.6)); // �`�敨�̂ɊD�F��ݒ�
	DrawRadarChart(numofemgch,aspect);
	glDisable(GL_LINE_STIPPLE);//�j���𖳌�

	// fetureVector��`��
	glColor3f(0.0, 0.0, 0.0); // �`�敨�̂ɍ��F��ݒ�
	glBegin(GL_LINE_LOOP); // ���`��
	for (i = 0; i < featureVector.size(); i++) {
		rate = (double)i / featureVector.size();
		x = featureVector[i] * cos(M_PI/2.0-2.0 * M_PI * rate)/aspect;
		y =  featureVector[i] * sin(M_PI/2.0-2.0 * M_PI * rate);
		glVertex3f(GLfloat(x), GLfloat(y), 0.0); // ���_���W���w��
	}
	glEnd(); // �`��I��

	//��������FeatureVector��`��
	
	for(j=0; j<teachingFV.size(); j++){
		if(IsDrawTeachingFVnum[j]==-1){}
		else{
			//�F�ύX
			SwitchColor(IsDrawTeachingFVnum[j]);
			glBegin(GL_LINE_LOOP); // ���`��
			for (i = 0; i < teachingFV[j].size(); i++) {
				rate = (double)i / teachingFV[j].size();
				x = teachingFV[j][i] * cos(M_PI/2.0-2.0 * M_PI * rate)/aspect;
				y =  teachingFV[j][i] * sin(M_PI/2.0-2.0 * M_PI * rate);
				glVertex3f(GLfloat(x), GLfloat(y), 0.0); // ���_���W���w��
			}
			glEnd(); // �`��I��
		}
	}
	
	::glPopMatrix();

	::glFinish();
	if( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc())){}

}


void CGLPictureCtrl::DrawSpeed(double speed, double max, double min,int motion)
{
	double hight;
	//���̃����^�����O�R���e�L�X�g���J�����g�֕ύX
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	//�w�i������
	glClearColor(0.0, 0.0, 0.0, 1);
	
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//	�Ƃ肠�����A�l�p�`��`�悵�Ă݂�B
	//���W�͂��Ԃ�}1���s�N�`���[�R���g���[���̃T�C�Y
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );
	//�F�w��
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
	//���̃����^�����O�R���e�L�X�g���J�����g�֕ύX
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	int i;
	//�w�i�𔒂�
	glClearColor(1.0, 1.0, 1.0, 1);
	
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//	�Ƃ肠�����A�l�p�`��`�悵�Ă݂�B
	//���W�͂��Ԃ�}1���s�N�`���[�R���g���[���̃T�C�Y
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );
	//�F�w��
	glColor3f(0.0, 0.0, 0.0); // �`�敨�̂ɍ��F��ݒ�
	
	//�_�O���t�}1.0�̋��
	for(i=0; i<spectrum.size(); i++){
		glPushMatrix();
		glTranslatef(i*2.0*0.5/double(spectrum.size()), 0, 0);//�_
		glTranslatef((i+1)*2.0*0.5/double(spectrum.size()+1), 0, 0);//�]��
		bar(spectrum[i]/size,2.0*0.5/double(spectrum.size()));
		glPopMatrix();
	}
	
	

	::glPopMatrix();

	::glFinish();
	if( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc())){}

}


void CGLPictureCtrl::bar(double hight, double width)
{
	glBegin(GL_POLYGON); // �|���S���̕`��
		glVertex3f(-1.0,         -1.0        ,  0.0); // ���_���W���w��
		glVertex3f((-1.0+width), -1.0        ,  0.0); // ���_���W���w��
		glVertex3f((-1.0+width), (-1.0+hight),  0.0); // ���_���W���w��
		glVertex3f(-1.0,         (-1.0+hight),  0.0); // ���_���W���w��
	glEnd(); // �|���S���̕`��I��


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
		default : glColor3f(0.0, 0.0, 0.0); break;//��

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

	// �|���S���̕`��
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
		
	glEnd(); // �|���S���̕`��I��

}

void CGLPictureCtrl::DrawHandSpeed(vector<double> speed, vector<int> color,double TargetSpeed){
	
	//���̃����^�����O�R���e�L�X�g���J�����g�֕ύX
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	int i;
	double x,y;
	double dx;
	//�w�i�𔒂�
	glClearColor(1.0, 1.0, 1.0, 1);
	
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//���W�͂��Ԃ�}1���s�N�`���[�R���g���[���̃T�C�Y
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );
	//�F�w��
	glColor3f(0.0, 0.0, 0.0); // �`�敨�̂ɍ��F��ݒ�

	//���x�`��
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
	glEnd(); // �|���S���̕`��I��

	//�ڕW�l�`��
	glLineWidth(3.0); 
	glBegin(GL_LINE_STRIP); 
	glColor3f(0.0, 0.0, 0.0); // �`�敨�̂ɍ��F��ݒ�

	glVertex3f(-1, 1.8*TargetSpeed-0.9, 0.0);
	glVertex3f(1, 1.8*TargetSpeed-0.9, 0.0);

	glEnd(); // �|���S���̕`��I��



	::glPopMatrix();

	::glFinish();
	if( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc())){}


}

void CGLPictureCtrl::DrawVRE(Position cursor, TargetPosition target,bool IsCollision,bool IsCompletion)
{
	//���̃����^�����O�R���e�L�X�g���J�����g�֕ύX
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	//�w�i�𔒂�
	glClearColor(1.0, 1.0, 1.0, 1);
	//size,x,y�́}1�͈̔͂�
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//	�Ƃ肠�����A�l�p�`��`�悵�Ă݂�B
	//���W�͂��Ԃ�}1���s�N�`���[�R���g���[���̃T�C�Y
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );
	//�K�C�h�̕`��
	glColor3f(0.0, 0.0, 0.0); // �K�C�h�����F�ɐݒ�
	guide();
	//�J�[�\���̕`��
	glColor3f(1.0, 0.0, 0.0); // �J�[�\����ԐF�ɐݒ�
	glPushMatrix();
		glTranslatef(cursor.x, cursor.y, 0);
		Cursor(0.05);
	glPopMatrix();
	
	//�^�[�Q�b�g�̕`��
	if(IsCollision && !IsCompletion){glColor3f(1.0, 0.65, 0.0);}// �����蔻��^�[�Q�b�g���F�ɐݒ� 1�b�ȉ�
	else if(IsCollision && IsCompletion){glColor3f(1.0, 0.65, 0.0);}// �^�X�N�����^�[�Q�b�g��ԐF�ɐݒ� 1�b�ȏ�
	else{glColor3f(0.0, 0.0, 1.0);}// �^�[�Q�b�g��F�ɐݒ�

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
	glBegin(GL_LINE_STRIP); // �|���S���̕`��
		glVertex3f(-size , 0.0 , 0.0); // ���_���W���w��
		glVertex3f( size , 0.0 , 0.0); // ���_���W���w��
	glEnd(); // �|���S���̕`��I��
	glBegin(GL_LINE_STRIP); // �|���S���̕`��
		glVertex3f(0.0 , -size , 0.0); // ���_���W���w��
		glVertex3f(0.0 ,  size , 0.0); // ���_���W���w��
	glEnd(); // �|���S���̕`��I��


}


void CGLPictureCtrl::guide(void)
{
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1 , 0xF0F0);

	glBegin(GL_LINE_STRIP); // �|���S���̕`��
		glVertex3f(-1.0 , 0.0 , 0.0); // ���_���W���w��
		glVertex3f( 1.0 , 0.0 , 0.0); // ���_���W���w��
	glEnd(); // �|���S���̕`��I��
	glBegin(GL_LINE_STRIP); // �|���S���̕`��
		glVertex3f(0.0 , -1.0 , 0.0); // ���_���W���w��
		glVertex3f(0.0 ,  1.0 , 0.0); // ���_���W���w��
	glEnd(); // �|���S���̕`��I��

	glDisable(GL_LINE_STIPPLE);

}


int CGLPictureCtrl::DrawSignal(vector<double> sig,int framesize, double posiylim , double negaylim)
{
	int i;
	double x,y;
	double dx;
	//���̃����^�����O�R���e�L�X�g���J�����g�֕ύX
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	//�w�i�𔒂�
	glClearColor(1.0, 1.0, 1.0, 1);
	//size,x,y�́}1�͈̔͂�
	CPaintDC dc(this);
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//	�Ƃ肠�����A�l�p�`��`�悵�Ă݂�B
	//���W�͂��Ԃ�}1���s�N�`���[�R���g���[���̃T�C�Y
	::glPushMatrix();
	::glColor3f( 0.0f, 1.0f, 0.0f );

	glColor3f(0.5, 0.5, 0.5); // �`�敨�̂ɊD�F��ݒ�

	//���S���̕`��
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1 , 0xF0F0);
	glBegin(GL_LINE_STRIP); // �|���S���̕`��
		glVertex3f(-1.0 , 0.0 , 0.0); // ���_���W���w��
		glVertex3f(1.0 , 0.0 , 0.0); // ���_���W���w��
	glEnd(); // �|���S���̕`��I��
	//�㔼��
	glBegin(GL_LINE_STRIP); // �|���S���̕`��
		glVertex3f(-1.0 , 0.5 , 0.0); // ���_���W���w��
		glVertex3f(1.0 , 0.5 , 0.0); // ���_���W���w��
	glEnd(); // �|���S���̕`��I��
	//������
	glBegin(GL_LINE_STRIP); // �|���S���̕`��
		glVertex3f(-1.0 , -0.5 , 0.0); // ���_���W���w��
		glVertex3f(1.0 , -0.5 , 0.0); // ���_���W���w��
	glEnd(); // �|���S���̕`��I��
	glDisable(GL_LINE_STIPPLE);


	glColor3f(1.0, 0.0, 0.0); // �`�敨�̂ɍ��F��ݒ�
	//�����̕`��
	glBegin(GL_LINE_STRIP); // �|���S���̕`��
	if(posiylim==negaylim){return -1;}
	dx=2/(double)framesize;
	x=-1;
	for(i=0; i<framesize; i++){
		y=(2*sig[i]-posiylim-negaylim)/(posiylim-negaylim);
		if(y>posiylim){y=posiylim;}
		else if(y<negaylim){y=negaylim;}
		glVertex3f(x , y , 0.0); // ���_���W���w��
		x+=dx;
	}
	glEnd(); // �|���S���̕`��I��

	::glPopMatrix();

	::glFinish();
	if( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc())){}

}
