#ifndef SHELL_H
#define SHELL_H


/***********LIBRERIAS GENERALES*************************************/
#ifndef WX_PRECOMP
  #include <wx/wx.h>
  #include <wx/msgdlg.h>
  #include <wx/utils.h>
  #include <wx/stc/stc.h>
  #include <wx/clipbrd.h>
  #include <wx/dcbuffer.h>
#endif
#include <stdarg.h>
#include <fstream>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef vector
#define vector(X,Y) mvector(X,Y)
#endif

/********************************************************************/
// #define main() _global_var_for_nothing;wxIMPLEMENT_APP(CShell); bool CShell::OnInit()
#ifndef _SHELL_CPP_
#define main(...) ShellMain(int argc, char **argv)
#endif

#define TIMER_ID 1001

int ShellMain(int argc, char **argv);
// int ShellMain(int argc, char *argv[]);

#if defined (__linux__) || defined(__APPLE__)

/// Esta estructura es para almacenar un color RGB en ella, como esta definida en C++ tiene dos constructores,
/// uno por default en negro y otro que se le mandan los valores. Solo se define para linux/mac, porque windows tiene una igual.
struct RGB{
  unsigned char R,G,B;
  RGB() : R(0), G(0), B(0){}
  RGB(unsigned char _R, unsigned char _G, unsigned char _B) : R(_R), G(_G), B(_B){}
};
///Para mantener la compatibilidad con windows se definen en linux y mac el RGB y COLORREF, para linux y mac son exactamente lo mismo.
typedef RGB COLORREF;


///Funciones de compatibilidad de codigo, en windows ya existen, en linux y mac se definen aqui
unsigned char GetRValue(COLORREF col);

unsigned char GetGValue(COLORREF col);

unsigned char GetBValue(COLORREF col);


#endif
#if defined (__linux__) || defined(__APPLE__)
#define millisleep(X) usleep(1000*X)

#endif


typedef enum{SBMP, SJPG, STIFF,SPNG} ShellImgType;
typedef double (*FUNA2D)(double, double);
typedef double (*FUNB2D)(double*);
typedef double (*FUNC2D)(double[]);




#if defined (__MINGW32__) || (__MINGW64__)

#define millisleep(X) Sleep(X)
#endif

/** Definiciones que estaban en el shell original de JLM ****************************/
#define BLACK   RGB(0,0,0)
#define RED     RGB(255,0,0)
#define GREEN   RGB(0,255,0)
#define BLUE    RGB(0,0,255)
#define CYAN    RGB(0,255,255)
#define MAGENTA RGB(255,0,255)
#define YELLOW  RGB(255,255,0)
#define GRAY1   RGB(255,255,0)
#define GRAY2   RGB(255,255,0)
#define WHITE   RGB(255,255,255)
#define BLUE2   RGB(102,102,255)

#define MByte    Matrix<unsigned char>
#define MFloat   Matrix<float>
#define MComplex Matrix<fcomplex>
#define VByte    Vector<unsigned char>
#define VFloat   Vector<float>
#define VComplex Vector<fcomplex>


#define OBJECTTYPE char
#define ARC 'a'
#define CIRCLE 'b'
#define ELLIPSE 'c'
#define ELLIPTICARC 'd'
#define LINE 'e'
#define POINT 'f'
#define POLYGON 'g'
#define RECTANGLE 'h'
#define ROTATEDTEXT 'i'
#define ROUNDEDRECTANGLE 'j'
#define SPLINE 'k'
//#define TEXT 'l'



/**  Significado de las variables de acuerdo al tipo de objeto ****
 * ARC
 * xStart=xoordX[0], yStart=ycoord[0]
 * xEnd=coordx[1], yEnd=ycoord[1]
 * xc=coordx[2], yc=ycoord[2]
 * CIRCLE
 * xc=coordx[0], yc=ycoord[0]
 * radius=r
 * RECTANGLE xc,yc =top left corner
 * xc=coordx[0], yc=ycoord[0]
 * width=width, height=height
 *
**/

typedef struct {
  char type;
  int ncoord;
  int coordX[10], coordY[10];
  int radius;
  int width;
  int height;
  int lwd;
  COLORREF FillCol;
  COLORREF BorderCol;
}DOBJ;

/** Estructura de malla           ****
 * Significado de las variables
 * nnod= numero de nodos en la malla
 * nele= numero de elementos en la malla
 * npel= nodos por elemento
 * nfix0= nodos con restriccion (nodos fijos en esfuerzo plano)
 * nfix1= nodos con valor de la propiedad fijo (cargas)
 * fillcolor= Colores de los elementos
 * bordercolor= color del borde de los elementos
 * cproperty=  valor de la propiedad representada en el falso color
 * xcoord= coordenadas de los nodos en X
 * ycoord= Coordenadas de los nodos en Y
 * conect= conectividades de los nodos
 */
class DMESH
{
protected:
 int nnod, nele, npel, nfix0, nfix1;
 int *fixnode0, *fixnode1;
 COLORREF *fillcolor;
 COLORREF bordercolor;
 double *cproperty, *xcoord,*ycoord, *fix0X, *fix0Y, *fix1X,*fix1Y;
 int **con,*ind;
 char name[256];
 double maxX, maxY,minX,minY, maxP,minP;
public:
 DMESH();
 ~DMESH();
 /**
  * file= nombre del archivo de entrada
  * lcoordini= linea donde comienzan las coordenadas
  * skip= numero de lineas (a saltar) que hay entre las coordenadas y conectividades
  * lname= Linea donde esta el nombre del problema
  * lele= linea donde se lee el numero de elementos
  * lnodes= linea donde se lee el numero de nodos
  * lfix0= linea donde se lee el numero de restricicones (nodos fijos)
  * lfix1= linea donde se lee el numero de nodos con propiedad (carga)
  * */
 DMESH(char *file, int lcoordini=8,int _npel=4, int skipC=1,int skipF0=1, int skipF1=1, int lname=1, int lele=3,int lnodes=4, int lfix0=5, int lfix1=6);
 DMESH(int _nnod, int _nele, int _nfix0=0, int _nfix1=0, int _npel=4);

 void setNNOD(int val);
 void setNELE(int val);
 void setNFIX0(int val);
 void setNFIX1(int val);
 void setNPEL(int val);
 void setXCOORD(int i, double val);
 void setYCOORD(int i, double val);
 void setCONNECT(int i, int conec[]);
 void setFIXNODE0(int i, int val);
 void setFIXNODE1(int i, int val);
 void setXFIX0(int i, double val);
 void setYFIX0(int i, double val);
 void setXFIX1(int i, double val);
 void setYFIX1(int i, double val);
 void setFILLCOLOR(int i, COLORREF val){fillcolor[i]=val;}
 void setBORDERCOLOR(COLORREF val){bordercolor=val;};
 void setCPROPERTY(int i, double val);
 void setINDICATOR(int i, int val){ind[i]=val;};

 int getNNOD(){return nnod;};
 int getNELE(){return nele;};
 int getNFIX0() {return nfix0;};
 int getNFIX1() {return nfix1;};
 int getNPEL() {return npel;};
 double getXCOORD(int i) {return xcoord[i];};
 double getYCOORD(int i){return ycoord[i];};
 double getCPROPERTY(int i){return cproperty[i];}
 void getCONNECT(int i, int conec[]) {for (int j=0; j<getNPEL(); j++) conec[j]=con[i][j];};
 int getFIXNODE0(int i){return fixnode0[i];};
 int getFIXNODE1(int i){return fixnode1[i];};
 double getXFIX0(int i){return fix0X[i];};
 double getYFIY0(int i){return fix0Y[i];};
 double getXFIX1(int i){return fix1X[i];};
 double getYFIY1(int i){return fix1Y[i];};
 double getMAXX(){return maxX;};
 double getMAXY(){return maxY;};
 double getMINX(){return minX;};
 double getMINY(){return minY;};
 double getMAXP(){return maxP;};
 double getMINP(){return minP;};
// COLORREF getFILLCOLOR(int i){fillcolor[i];}
 COLORREF getBORDERCOLOR(){return bordercolor;};
 int getINDICATOR(int i){return ind[i];};

};




void **Tabla(unsigned long, unsigned long, unsigned long);
void freetabla(void **tabla,unsigned long rows);
int random(int k) ;
void randomize() ;
float randfloat() ;
unsigned char  *FillBptr(unsigned char **R,unsigned char **B,unsigned char **G,int nr,int nc);
unsigned char  *FillBptr(COLORREF **col,int nr,int nc) ;

/*****************************************************************************/
template<class T> class Vector{
	 T *Vpt;
	 int size;
  public:
	 Vector(int S) {size=S; Vpt=(T *)calloc(size,sizeof(T));}
	 ~Vector(){free(Vpt);}
	 T & operator () (int Elem) { return Vpt[Elem]; }
	 T *Ptr(){return Vpt;}
	 int Size(){return size;}
};
/******************************************************************************/
template<class T> class Matrix{
	 int rows,cols;
	 T **Mpt;
  public:
	 Matrix(int R,int C){rows=R;cols=C;Mpt=(T **) Tabla(rows,cols,sizeof(T));}
	 ~Matrix(){freetabla((void **) Mpt,rows);}
	 int Rows() { return rows;}
	 int Cols() { return cols;}
	 T & operator () (int R,int C) { return Mpt[R][C]; }
	 T *Row(int R) {return Mpt[R];}
	 T **Ptr() {return Mpt;}
};
/** ***************************************************************************/


/** ***************DEFINICIONES DE CLASES***************************************************/

/** ****************Boton para la pausa****************************************/
    class ButtonFrame: public wxFrame // MainFrame is the class for our window,
    {
    public:
      ButtonFrame( const wxString &title, const wxPoint &pos, const wxSize &size );
      wxPanel *Panel;
      wxButton *ButtonOk;
      void OnExit( wxCommandEvent& event );

      DECLARE_EVENT_TABLE()
    };

    enum
    {
      BUTTON_Ok = wxID_HIGHEST + 1
    };
/** ***************************************************************************/


///Clase Panel, basicamente son los dispositivos utilizados para pintar ya sea una imagenes o una grafica
class wxCShellPanel : public wxPanel
{
   ///Se utiliza para pintar, el Image tiene asociados datos RGB, antes de pintar el Image se convierte a Bitmap, y luego se pinta.
   wxBitmap bmp;
public:
    wxCShellPanel(wxFrame* parent);   ///Constructor de panel para mostrar imagenes.
    ~wxCShellPanel();
    wxFrame *Parent;
    int nr,nc; ///Numero de renglones y columnas de la zona de pintado.
    unsigned char *data,*InitData;
    wxImage *image; ///Imagen para mostrar
    COLORREF tmpColor;
    int StartRX, StartRY, EndRX, EndRY;
    int StartLX, StartLY, EndLX, EndLY;
    int printPos;
    ///Evento de pintado llamado por el sistema
    void paintEvent(wxPaintEvent & evt);
    ///Esta funcion es la que realmente dibuja y redibuja, ya sea imagen o grafica
    void render(wxDC& dc);
    void MouseLeftUp(wxMouseEvent& event);
    void MouseRightUp(wxMouseEvent& event);
    void MouseLeftDown(wxMouseEvent& event);
    void MouseRightDown(wxMouseEvent& event);
    void Snapshot(const char*FileName, ShellImgType _T, int x0, int y0, int x1, int y1, int silent=1);
    void PutPixel(int x,int y,int Size, COLORREF Color);
    void printf(int x, int y, wxString str);
    void printf(wxString str);
    void DrawObjects(int nobj,DOBJ *objs);
    friend class window;
    ///Declara la tabla de eventos
    DECLARE_EVENT_TABLE()
};

/** ******************************************************************/
/** ******CLASE PARA CREAR LOS DIFERENTES TIPOS DE VENTANAS******** **/
///Esta clase define una ventana.
class ShellWindow: public wxFrame
{
	private:

public:
/*	    wxTimer *Timer;
        void OnTimerTimeout(wxTimerEvent& event)
        {
		      wxClientDC DC(this->drawPane);
              this->drawPane->render(DC);
		}*/
  ///Constructor con los parametros de la ventana
   ShellWindow(const wxString& title, const wxPoint& pos, const wxSize& size);
   ~ShellWindow();
   wxCShellPanel *drawPane;   ///Panel de dibujo
   wxPanel *TextPanel;       ///Panel de texto
   wxTextCtrl *MainEditBox;  ///Caja para texto asociada al panel
   unsigned char *data;
   int nr,nc;
private:
   ///Macro para declarar la tabla de eventos
   wxDECLARE_EVENT_TABLE();
};


///Esta clase es una interfaz a ShellWindow, en esta clase se definen las funciones
/// compatibles con el shell original.
/// basicamente es una interfaz a ShellWindow
class window
{
private:

public:
   ///Constructor con los parametros de la ventana, compatible con el Shell anterior
   window(const char *title, const int height=800, const int width=900, const int posX=10, const int posY=10);
   ~window();
   ShellWindow *w;
   ///Datos de la imagen que se pinta en la ventana es el mismo apuntador del ShellWindow y es el mismo del ShellPanel y es el mismo del wxImage en ese panel.
   unsigned char *data;
   int nr,nc;///Numero de renglones y columnas de la matriz de datos utilizada para dibujar
   void PlotBitmap(float **f,int nr,int nc,int x0,int y0,float zoom); /// Dibuja un Bitmap en escala de grises a partir de una matriz
   ///Dibuja un Bitmap de color a partir de una matriz
   void PlotCBitmap(unsigned char **r,unsigned char **g,unsigned char **b,int nr,int nc,int x0,int y0,float zoom) ;
   void PlotCBitmap(COLORREF **col,int _nr,int _nc,int x0,int y0,float zoom);
   void PlotCBitmapF(unsigned char *bptr,int nrp,int ncp,int x0,int y0,float zoom);
   void PlotFEMMesh(DMESH &msh, int x0=0,int y0=0, int x1=-1, int y1=-1) ;
   void PlotContour(FUNA2D fx, double xinf,double xsup,double  yinf,double  ysup) ;
   void PlotBitmap(unsigned char **f,int nr,int nc,int x0,int y0,float zoom) ;
   void Bitmap(MByte  &B,float zoom=1.0,int OffsetX=10,int OffsetY=10);
   void CBitmap(MByte  &R,MByte &G,MByte &B,float zoom=1.0,int OffsetX=10,int OffsetY=10);
   //void Bitmap(Matrix<unsigned char>  &B,float zoom=1.0,int OffsetX=10,int OffsetY=10);
   void Bitmap(MFloat &F,float zoom=1.0,int OffsetX=10,int OffsetY=10);
   void GetFileNameDialog(char *FileName);
   void Cls();
   void PutPixel(int x,int y,int Size=3,COLORREF Color=COLORREF());
   void line(int x0,int y0,int x1,int x2,COLORREF Color=COLORREF(),int Wide=1);
   void Paint();
   void GetPixelColor(int x,int y,COLORREF *col) ;
   void printf(int x, int y, const char *format,...);
   void printf(const char *format,...);

};

/** **************************CLASE PRINCIPAL ESTA ES LA APLICACION EN SI********************************** **/
///Esta clase herada del wxApp, una de estas se debe
///de instanciar cada que se haga una aplicacion de
///wxWidgets, se sobre carga el OnInit para personalizarlo,
class CShell: public wxApp
{
public:
  virtual bool OnInit();
};
/** ************************************************************************************************* **/






/** *FUnciones definidas para que sean compatibles con el shell original********************************/
void DimensionesBmp(const char *fname,int *pnr,int *pnc);
void DimensionesImg(const char *fname,int *pnr,int *pnc);
void LeeBmp(float **M,const char *fname) ;
void LeeImg(float **M,const char *fname) ;
void LeeBmpColor(unsigned char**R,unsigned char**G,unsigned char**B,const char *nombre) ;
void LeeImgColor(unsigned char**R,unsigned char**G,unsigned char**B,const char *nombre) ;
void LeeImgColor(COLORREF **data,const char *nombre) ;

void MessageOk(const char *_message, const char* _title="Mensaje");
bool MessageYesNo(const char *_message, const char* _title=NULL);
char *InDialog(const char *OutText="Input Text Dialog Box", const char *Title="Titulo", const char *DefaultValue="");
/** ************************************************************************************************* **/

/** * Almacena los apuntadores de la ultima y primera ventanas creadas  ********************************/
window *GetLastWindow(window *ptr=NULL);
window *GetFirstWindow(window *ptr=NULL);
CShell *GetApplicationPointer(CShell *ptr=NULL);

/** ************************************************************************************************* **/
/** * Esta funcion provee compatibilidad completa con el procim anterior, incluyendo la posibilidad de declarar
 * ventanas globales. La idea es que una ventana declara como global no se instancia en realidad, la instancia
 * se realiza hasta que esa ventana se utiliza para pintar algo, esa parte es la que hace esta clase, evitar la
 * instancia directa**/
class Window{

  ///Esta funcion es la que realmente hace la instancia de la ventana
    void InitWindow();
    char title[512];
    int height, width, posX, posY;
public:
  Window(const char *_title="Aplicacion", const int _height=800, const int _width=900, const int _posX=10, const int _posY=10);
  ~Window();
  window *win;
   ///Datos de la imagen que se pinta en la ventana es el mismo apuntador del ShellWindow y es el mismo del ShellPanel y es el mismo del wxImage en ese panel.
   unsigned char *data;
   int nr,nc;///Numero de renglones y columnas de la matriz de datos utilizada para dibujar
   void PlotBitmap(float **f,int nr,int nc,int x0,int y0,float zoom); /// Dibuja un Bitmap en escala de grises a partir de una matriz
   ///Dibuja un Bitmap de color a partir de una matriz
   void PlotCBitmap(unsigned char **r,unsigned char **g,unsigned char **b,int nr,int nc,int x0,int y0,float zoom) ;
   void PlotCBitmap(COLORREF **col,int _nr,int _nc,int x0,int y0,float zoom);
   void PlotCBitmapF(unsigned char *bptr,int nrp,int ncp,int x0,int y0,float zoom);
   void PlotFEMMesh(DMESH &msh, int x0=0,int y0=0, int x1=-1, int y1=-1) ;
   void PlotContour(FUNA2D fx,double  xinf,double xsup,double  yinf,double  ysup);
   void PlotBitmap(unsigned char **f,int nr,int nc,int x0,int y0,float zoom) ;
   void Bitmap(MByte  &B,float zoom=1.0,int OffsetX=10,int OffsetY=10);
   void CBitmap(MByte  &R,MByte &G,MByte &B,float zoom=1.0,int OffsetX=10,int OffsetY=10);
   void GetFileNameDialog(char *FileName);
   //void Bitmap(Matrix<unsigned char>  &B,float zoom=1.0,int OffsetX=10,int OffsetY=10);
   void Bitmap(MFloat &F,float zoom=1.0,int OffsetX=10,int OffsetY=10);
   void Snapshot(const char*FileName, ShellImgType _T, int x0, int y0, int x1, int y1, int silent=1);
   void DrawObjects(int nobj, DOBJ *objs);
   void Cls();
   void PutPixel(int x,int y,int Size=3,COLORREF Color=COLORREF());
   void line(int x0,int y0,int x1,int x2,COLORREF Color=COLORREF(),int Wide=1);
   void Paint();
   void GetPixelColor(int x,int y,COLORREF *col) ;
   void printf(int x, int y, const char *format,...);
   void printf(const char *format,...);
};







#endif



