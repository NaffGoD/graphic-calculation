// MFCApplication1.cpp: определяет поведение классов для приложения.
//

#define _AFXDLL
#include "resource.h"
#include <afxwin.h>
#include <windows.h>
#include "afxcmn.h"
#include <afxext.h>
#include <stdio.h>
#include "glut.h"

CStdioFile myFile;
CString str1, str2, str3, str4, str5;

CString strTextRes;
CFileException fileException;

double result = 0;
double a, b, c, d, x, arr[8] = { 5, 3.2, 4, 7, 9.3, 5.8, 7.8, 9 }, arrMul = 1;

char resultStr[256];
int charCounter = 0;

//const char name[20] = "RESULT.txt"

void MyInit();
void Display();
void Rotate();
void Reshape(int w, int h);
void DividePyramid(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d, GLfloat* s, int level);
void DrawPyramid(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d, GLfloat* s);
void DrawTriangle(GLfloat* a, GLfloat* b, GLfloat* c);
void DrawQuoad(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d);
void DrawLineTriangle(GLfloat* a, GLfloat* b, GLfloat* c);
void DrawLineQuoad(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d);
void Keyboard(unsigned char key, int x, int y);

// угол для поворота
GLfloat angle = 45;
// количество уровней
int level = 3;
// закрашиваем или рисуем только ребра: false-закрашиваем, true- только ребра(клавиша P)
bool state = false;
// задаем пирамиду
GLfloat pyramid[5][3] = {
	{ -1.0, -1.0,  1.0 }, // A
	{  1.0, -1.0,  1.0 }, // B
	{  1.0, -1.0, -1.0 }, // C
	{ -1.0, -1.0, -1.0 }, // D
	{  0.0,  1.0,  0.0 }  // S
};

void MyInit()
{
	// делаем черный фон
	glClearColor(0.0, 0.0, 0.0, 1.0);
	// разрешить тест глубины
	glEnable(GL_DEPTH_TEST);
	// Улучшение в вычислении перспективы
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Выбор матрицы вида модели
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// сдвинулись вглубь экрана на 4
	glTranslatef(0.0, 0.0, -4.0);
	// вращать будем по оси Y
	glRotatef(angle, 0, 1, 0);
	// делим пирамиду если level > 0 и отрисовываем
	DividePyramid(pyramid[0], pyramid[1], pyramid[2], pyramid[3], pyramid[4], level);
	//Делаем сигнал на перерисовку, ибо постоянно меняем угол поворота
	glutPostRedisplay();
	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	// загружаем матрицу проекции
	glMatrixMode(GL_PROJECTION);
	//  Сброс матрицы проекции
	glLoadIdentity();
	// Вычисление соотношения геометрических размеров для окна
	gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 0.1, 200.0);
	// Выбор матрицы вида модели
	glMatrixMode(GL_MODELVIEW);
	// Сброс матрицы вида модели
	glLoadIdentity();
}

void DividePyramid(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d, GLfloat* s, int level)
{
	// координаты для середин ребер
	GLfloat ab[3], bc[3], ac[3], ad[3], cd[3], bs[3], as[3], cs[3], ds[3];
	if (level == 0)
	{
		DrawPyramid(a, b, c, d, s);
	}
	else
	{
		for (int j = 0; j < 3; j++)
		{
			ab[j] = (a[j] + b[j]) / 2.0; // середина A и B
			bc[j] = (b[j] + c[j]) / 2.0; // середина B и C
			ac[j] = (a[j] + c[j]) / 2.0; // середина A и C (центр квадрата-основания)
			ad[j] = (a[j] + d[j]) / 2.0; // середина A и D
			cd[j] = (c[j] + d[j]) / 2.0; // середина C и D
			bs[j] = (b[j] + s[j]) / 2.0; // середина B и S
			as[j] = (a[j] + s[j]) / 2.0; // середина A и S
			cs[j] = (c[j] + s[j]) / 2.0; // середина C и S
			ds[j] = (d[j] + s[j]) / 2.0; // середина D и S
		}

		// рекурсивно вызываем для 5 меньших пирамидок
		DividePyramid(a, ab, ac, ad, as, level - 1);
		DividePyramid(ab, b, bc, ac, bs, level - 1);
		DividePyramid(ac, bc, c, cd, cs, level - 1);
		DividePyramid(ad, ac, cd, d, ds, level - 1);
		DividePyramid(as, bs, cs, ds, s, level - 1);
	}
}

void DrawPyramid(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d, GLfloat* s)
{
	// рисуем закрашенную
	if (!state)
	{
		glBegin(GL_TRIANGLES);
	
		glColor3f(0.52, 0.44, 1.0);
		DrawTriangle(a, b, s);
	
		glColor3f(0.2, 0.6, 0.3);
		DrawTriangle(b, c, s);
		
		glColor3f(0.3, 0.5, 0.5);
		DrawTriangle(c, d, s);
		
		glColor3f(0.4, 0.4, 1.0);
		DrawTriangle(a, d, s);
		glEnd();

		glBegin(GL_QUADS);
		
		glColor3f(0.48, 0.42, 1.0);
		DrawQuoad(a, b, c, d);
		glEnd();
	}
	// или отрисовываем только ребра
	else
	{
		glBegin(GL_LINES);
		// красим в синий цвет
		glColor3f(0.0, 0.0, 1.0);
		DrawLineTriangle(a, b, s);
		DrawLineTriangle(c, d, s);
		DrawLineQuoad(a, b, c, d);
		glEnd();
	}
}

void DrawTriangle(GLfloat* a, GLfloat* b, GLfloat* c)
{
	glVertex3fv(a);
	glVertex3fv(b);
	glVertex3fv(c);
}

void DrawQuoad(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d)
{
	glVertex3fv(a);
	glVertex3fv(b);
	glVertex3fv(c);
	glVertex3fv(d);
}

void DrawLineTriangle(GLfloat* a, GLfloat* b, GLfloat* c)
{
	glVertex3fv(a);
	glVertex3fv(b);
	glVertex3fv(b);
	glVertex3fv(c);
	glVertex3fv(a);
	glVertex3fv(c);
}

void DrawLineQuoad(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d)
{
	glVertex3fv(a);
	glVertex3fv(b);
	glVertex3fv(b);
	glVertex3fv(c);
	glVertex3fv(c);
	glVertex3fv(d);
	glVertex3fv(a);
	glVertex3fv(d);

}

void Rotate()
{
	angle += 0.05;
	if (angle > 360) angle = 0;
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case '+':
		if (level < 10) level += 1;
		Display();
		break;
	case '-':
		if (level > 0) level -= 1;
		Display();
		break;
	case 'p':
		state = !state;
		Display();
		break;
	}
}

class CMyDialog :public CDialog
{
public:
	CMyDialog(CWnd* pParent = NULL);
	enum { IDD = IDD_DIALOG1 };
protected:
	CListBox pList1, pList2, pList3, pList4, pList5;

	BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg void OnOK();
	afx_msg void OnCancel();
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	
END_MESSAGE_MAP()

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_LIST1, pList1);
	DDX_Control(pDX, IDC_LIST2, pList2);
	DDX_Control(pDX, IDC_LIST3, pList3);
	DDX_Control(pDX, IDC_LIST4, pList4);
	DDX_Control(pDX, IDC_LIST5, pList5);
	CDialog::DoDataExchange(pDX);

}

CMyDialog::CMyDialog(CWnd* pParent)
	:CDialog(CMyDialog::IDD, pParent)
{
}

void CMyDialog::OnCancel()
{
	CDialog::OnCancel();
}

BOOL CMyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 1; i < 10; i++)
	{
		str1.Format(L"%d", i);
		pList1.AddString((LPCTSTR)str1);

		str2.Format(L"%d", i);
		pList2.AddString((LPCTSTR)str2);

		str3.Format(L"%d", i);
		pList3.AddString((LPCTSTR)str3);

		str4.Format(L"%d", i);
		pList4.AddString((LPCTSTR)str4);

		str5.Format(L"%d", i);
		pList5.AddString((LPCTSTR)str5);
	}

	return TRUE;
}

void CMyDialog::OnOK()
{
	//CString buffer;
	//int a, b, c, d, x;

	a = pList1.GetCurSel() + 1.0;
	str1.Format(L"%f", a);
	MessageBox((LPCTSTR)str1);

	b = pList2.GetCurSel() + 1.0;
	str2.Format(L"%f", b);
	MessageBox((LPCTSTR)str2);

	c = pList3.GetCurSel() + 1.0;
	str3.Format(L"%f", c);
	MessageBox((LPCTSTR)str3);

	d = pList4.GetCurSel() + 1.0;
	str4.Format(L"%f", d);
	MessageBox((LPCTSTR)str4);

	x = pList5.GetCurSel() + 1.0;
	str5.Format(L"%f", x);
	MessageBox((LPCTSTR)str5);

	::RedrawWindow(GetOwner()->m_hWnd, NULL, NULL, RDW_ERASE |
		RDW_INVALIDATE);
	CDialog::OnOK();
}

class CMyFrameWnd :public CFrameWnd {
public:

	CMyFrameWnd();

	CToolBar m_wndToolBar;

	afx_msg void OnInput();

	afx_msg void OnRun();
	afx_msg void OnResult();
	afx_msg void OnExit();

	afx_msg void OnInfo();

	afx_msg void OnOpen();
	afx_msg void OnSave();
	afx_msg void OnRename();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	// afx_msg void On();
DECLARE_MESSAGE_MAP();
};

BEGIN_MESSAGE_MAP(CMyFrameWnd, CFrameWnd)

	//ON_WM_CREATE()

	ON_COMMAND(ID_INPUT, OnInput)
	
	ON_COMMAND(ID_RUN, OnRun)
	ON_COMMAND(ID_RESULT, OnResult)
	ON_COMMAND(ID_EXIT, OnExit)

	ON_COMMAND(ID_INFO, OnInfo)
	ON_COMMAND(ID_OPEN, OnOpen)
	ON_COMMAND(ID_SAVE, OnSave)
	ON_COMMAND(ID_RENAME, OnRename)

	//ON_COMMAND(ID_OPENGLRESULT, OnMainGraphicFunction)

	ON_WM_CREATE()

END_MESSAGE_MAP();

CMyFrameWnd::CMyFrameWnd() {
	Create(NULL, L"Where is the money, Lebowski?", WS_OVERLAPPEDWINDOW, rectDefault, NULL, MAKEINTRESOURCE(IDR_MENU2));
}

int CMyFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CFrameWnd::OnCreate(lpCreateStruct);

	m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD |
		WS_VISIBLE | CBRS_TOP |  CBRS_GRIPPER  | CBRS_TOOLTIPS |
		CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBar.LoadToolBar(IDR_TOOLBAR1);

	return 0;
}

void CMyFrameWnd::OnInput() {
	CMyDialog dlg;
	dlg.DoModal();
};

void CMyFrameWnd::OnRun() {
	HINSTANCE hDll = LoadLibrary(L"Dll3");
	if(hDll == NULL) 
		MessageBox(L"Something has been wrong", L"Error", MB_OK);
	else
	{
	    for (int i = 0; i < 7; i++)
				arrMul *= arr[i];

		typedef double (*FUN)(double, double, double, double, double, double);
		FUN func;
		func = (FUN)GetProcAddress(hDll, "calc");
		result = (*func)(a, b, c, d, x, arrMul);
		level = result;

		FreeLibrary(hDll);
		MessageBox(L"Result recieved", L"Success", MB_OK);
	}
};

void CMyFrameWnd::OnResult() {
	strTextRes.Format(L"%f", result);
	MessageBox(strTextRes, L"Result", MB_OK);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// создаем окно
	glutInitWindowSize(700, 700);
	glutCreateWindow("Pyramides in Triangle...");

	MyInit();
	// регистрация обратных вызовов
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutIdleFunc(Rotate);

	glutMainLoop();
};

void CMyFrameWnd::OnExit() {
	//PostQuitMessage(0);
	SendMessage(WM_CLOSE);
};

void CMyFrameWnd::OnInfo() {
	::MessageBox(NULL, L"Info", L"Made by Maxym Furman", MB_OK);
};

void CMyFrameWnd::OnOpen() {

	//myFile.Open(name, CFile::modeCreate | CFile::modeReadWrite);

	CFileDialog dlgOpen(TRUE, L"txt", L"", OFN_HIDEREADONLY, L"Data (*.txt) |*.txt| All Files(*.*) |*.*||", this);

	if (dlgOpen.DoModal() == IDOK) {
		CFile myFile(dlgOpen.GetPathName(), CFile::modeReadWrite);
		myFile.Seek(0, CFile::begin);
		UINT nActual = myFile.Read(resultStr, sizeof(resultStr));
		MessageBoxA(NULL, (LPCSTR)resultStr, "File Open", MB_OK);
		myFile.Close();
	}
};

void CMyFrameWnd::OnSave() {
	//MessageBox(L"FileSave", L"FileSave", MB_OK);
	sprintf_s(resultStr, "%f", result);

	CFileDialog DialogDlgSave(FALSE, L"txt", L"", OFN_HIDEREADONLY, L"Data (*.txt) |*.txt| All Files(*.*) |*.*||", this);
	if (DialogDlgSave.DoModal() == IDOK)
	{

		CStdioFile file(DialogDlgSave.GetPathName(), CFile::modeCreate | CFile::modeWrite);
		for (int i = 0; resultStr[i] != '\0'; i++)
			charCounter++;
		file.Write(resultStr, charCounter);
		file.Close();
	}

	::MessageBox(NULL, L"File Saved", L"File Save", MB_OK);
};

void CMyFrameWnd::OnRename() {
	CString strTextA, strTextB;
	CFileDialog  DlgOpen(TRUE, L"txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data (*.txt) |*.txt| All Files(*.*) |*.*||");

	if (DlgOpen.DoModal() == IDOK)
	{
		CFileDialog DlgRename(FALSE, L"txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data (*.txt) |*.txt| All Files(*.*) |*.*||");
		DlgRename.m_ofn.lpstrTitle = (LPCWSTR)"Rename file...";
		if (DlgRename.DoModal() == IDOK) {
			strTextA.Format(L"%s", DlgOpen.GetPathName());
			strTextB.Format(L"%s", DlgRename.GetPathName());

			CStringA charstrA(strTextA);
			CStringA charstrB(strTextB);
			rename((const char*)charstrA, (const char*)charstrB);
		}
	}

	::MessageBox(NULL, L"File Renamed", L"File Rename", MB_OK);
};

class CMyApp :public CWinApp {

public:
	virtual BOOL InitInstance();
};

BOOL CMyApp::InitInstance() {

	CMyFrameWnd* pMainWnd = new CMyFrameWnd;
	m_pMainWnd = pMainWnd;
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	return TRUE;
};

CMyApp app;