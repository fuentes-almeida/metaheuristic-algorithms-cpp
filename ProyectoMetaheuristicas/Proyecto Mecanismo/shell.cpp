#ifndef _SHELL_CPP_
#define _SHELL_CPP_

#include "shell.hpp"

using namespace std;

#ifndef vector
#define vector(X,Y) mvector(X,Y)
#endif

window *GetLastWindow(window *ptr)
{
  static window *W=NULL;
  if (ptr)
    W=ptr;

 return W;
}
window *GetFirstWindow(window *ptr)
{
  static window *W=NULL;
  if (ptr)
    W=ptr;

 return W;
}

CShell *GetApplicationPointer(CShell *ptr)
{
  static CShell *App=NULL;
  if (ptr)
    App=ptr;

 return App;

}

char *GetWindowNamw(const char *_name)
{
  static char name[512]={""};
  if (_name!=NULL)
    strcpy(name,_name);
    return name;
}

/************* Funcion para reservar una tabla de datos **********************/
void** Tabla(unsigned long rows,unsigned long cols,unsigned long sizeElem)
  {
  void** p;
  if((p=(void **) calloc(rows,sizeof(void *)))==NULL) return NULL;
  for(unsigned i=0;i<rows;i++)
	 if((p[i]=calloc(cols,sizeElem))==NULL) return NULL;
  return p;
  }
/************* Funcion para liberar una tabla de datos ***********************/
void freetabla(void **tabla,unsigned long rows)
  {  while(rows--) free(tabla[rows]); free(tabla);  }

/******************************************************************************/
int random(int k)
{
return (rand() % k) ;
}
//***********************************************************
void randomize()
{
    time_t k ;
    time(&k) ;
    srand((int)k*13451 % 77777) ;
}
//****************************************************************
float randfloat()
{
      return (float)(rand()/(float)RAND_MAX) ;
}
/******************************************************************************/
unsigned char  *FillBptr(unsigned char **R,unsigned char **B,unsigned char **G,int nr,int nc)
{
int i,j;
int nrp,ncp ;
unsigned char *bptr ;
if (nr > nc) nrp = ncp = nr ;
else nrp = ncp = nc ;
if (nrp % 4)
  nrp = ncp = nrp + 4 - (nrp%4)  ;
bptr = (unsigned char  *)malloc(nrp*ncp*3) ;
		for (i = 0 ; i < nrp ; i++)
				for (j = 0 ; j < ncp ; j++)
						{
						if (i < nr && j < nc)
						  {
						  bptr[3*i*ncp+3*j]=G[i][j];
						  bptr[3*i*ncp+3*j+1]=B[i][j];
						  bptr[3*i*ncp+3*j+2]=R[i][j];
						  }
						else
						  {
						  bptr[3*i*ncp+3*j]=255;
						  bptr[3*i*ncp+3*j+1]=255;
						  bptr[3*i*ncp+3*j+2]=255;
						  }
						}
return bptr ;
}
/******************************************************************************/
unsigned char  *FillBptr(COLORREF **col,int nr,int nc)
{
int i,j;
int nrp,ncp ;
unsigned char *bptr ;
if (nr > nc) nrp = ncp = nr ;
else nrp = ncp = nc ;
if (nrp % 4)
  nrp = ncp = nrp + 4 - (nrp%4)  ;
bptr = (unsigned char  *)malloc(nrp*ncp*3) ;
		for (i = 0 ; i < nrp ; i++)
				for (j = 0 ; j < ncp ; j++)
						{
						if (i < nr && j < nc)
						  {
						  bptr[3*i*ncp+3*j]=GetGValue(col[i][j]);
						  bptr[3*i*ncp+3*j+1]=GetBValue(col[i][j]);
						  bptr[3*i*ncp+3*j+2]=GetRValue(col[i][j]);
						  }
						else
						  {
						  bptr[3*i*ncp+3*j]=255;
						  bptr[3*i*ncp+3*j+1]=255;
						  bptr[3*i*ncp+3*j+2]=255;
						  }
						}
return bptr ;
}



///-----------------------------------------------------------------------------------------------
///Declara la tabla de eventos
wxBEGIN_EVENT_TABLE(ShellWindow, wxFrame)
     //  EVT_TIMER(TIMER_ID,ShellWindow::OnTimerTimeout)
wxEND_EVENT_TABLE()

///Constructor
ShellWindow::ShellWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    CreateStatusBar();
    SetStatusText( "Main Window" );
    SetMinSize(GetSize());
    SetMaxSize(GetSize());
   // Timer= new wxTimer(this,TIMER_ID );
   // Timer->Start(100);
    drawPane = new wxCShellPanel(this);
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

///-----------------------------------------------------------------------------------------------
///Destructor
ShellWindow::~ShellWindow()
{

}


/** ************* METODOS DE LA CLASE wxCShellPanel******************** **/
///Declaracion de la tabla de eventos
BEGIN_EVENT_TABLE(wxCShellPanel, wxPanel)
///Evento de pintado llamado por el sistema
EVT_PAINT(wxCShellPanel::paintEvent)
EVT_RIGHT_UP(wxCShellPanel::MouseRightUp)
EVT_LEFT_UP(wxCShellPanel::MouseLeftUp)
EVT_RIGHT_DOWN(wxCShellPanel::MouseRightDown)
EVT_LEFT_DOWN(wxCShellPanel::MouseLeftDown)
END_EVENT_TABLE()

wxCShellPanel::wxCShellPanel(wxFrame* parent) : wxPanel(parent)
{
  wxSize sz=parent->GetClientSize();
  Parent=parent;
  nr=sz.GetHeight ();
  nc=sz.GetWidth ();
  InitData=data= (unsigned char*)malloc(3*nr*nc*sizeof(unsigned char));
  for (int i=0; i<nr; i++){
    for (int j=0; j<nc; j++){
        data[3*i*nc+3*j]=255;
	data[3*i*nc+3*j+1]=255;
        data[3*i*nc+3*j+2]=255;
    }
  }
  image=new wxImage(sz,data,true);
  printPos=16;
}
wxCShellPanel::~wxCShellPanel()
{
   free(InitData);
delete(image);
}


///---------------------------------------------------------------------
/// Evento de pintado llamado por el sistema
void wxCShellPanel::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}


void wxCShellPanel::MouseRightUp(wxMouseEvent& event)
{
    EndRX=event.GetX();
    EndRY=event.GetY();
    int x0=StartRX,y0=StartRY,x1=EndRX,y1=EndRY;
    if (x0>x1){
       int tx=x0;
       x0=x1;   x1=tx;
    }
    if (y0>y1){
       int tx=y0;
       y0=y1;   y1=tx;
    }
    bmp = wxBitmap(*image);
    wxBitmap bmp2=bmp.GetSubBitmap(wxRect(wxPoint(x1,y1), wxPoint (x0,y0)));

        char str[108];
     wxFileDialog saveFileDialog(this, _("Guardar seccion como imagen"), "", "",
                       "BMP files (*.bmp)|*.bmp|PNG files (*.png)|*.png|JPEG files (*.jpg)|*.jpg|TIFF files (*.tiff)|*.tiff",
                        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...
        if (saveFileDialog.GetFilterIndex()==0)
            bmp2.SaveFile(saveFileDialog.GetPath(),wxBITMAP_TYPE_BMP );
        else if (saveFileDialog.GetFilterIndex()==1)
            bmp2.SaveFile(saveFileDialog.GetPath(),wxBITMAP_TYPE_PNG );
        else if (saveFileDialog.GetFilterIndex()==2)
            bmp2.SaveFile(saveFileDialog.GetPath(),wxBITMAP_TYPE_JPEG );
        else if (saveFileDialog.GetFilterIndex()==3)
            bmp2.SaveFile(saveFileDialog.GetPath(),wxBITMAP_TYPE_TIFF );
        sprintf(str,"Seccion seleccionada x0=%d y0=%d x1=%d y1=%d. Se almaceno como imagen.",x0,y0,x1,y1);
        wxMessageBox(_(str));
}

void wxCShellPanel::Snapshot(const char*FileName, ShellImgType _T, int x0, int y0, int x1, int y1, int silent)
{
    if (x0>x1){
       int tx=x0;
       x0=x1;   x1=tx;
    }
    if (y0>y1){
       int tx=y0;
       y0=y1;   y1=tx;
    }
    bmp = wxBitmap(*image);
    wxBitmap bmp2=bmp.GetSubBitmap(wxRect(wxPoint(x1,y1), wxPoint (x0,y0)));

    char str[108];
        if (_T==SBMP)
            bmp2.SaveFile(FileName,wxBITMAP_TYPE_BMP );
        else if (_T==SPNG)
            bmp2.SaveFile(FileName,wxBITMAP_TYPE_PNG );
        else if (_T==SJPG)
            bmp2.SaveFile(FileName,wxBITMAP_TYPE_JPEG );
        else if (_T==STIFF)
            bmp2.SaveFile(FileName,wxBITMAP_TYPE_TIFF );
	if (!silent){
           sprintf(str,"Seccion seleccionada x0=%d y0=%d x1=%d y1=%d. Se almaceno como imagen.",x0,y0,x1,y1);
           wxMessageBox(_(str));
	}
}

void wxCShellPanel::MouseLeftUp(wxMouseEvent& event)
{

    EndLX=event.GetX();
    EndLY=event.GetY();
    int x0=StartLX,y0=StartLY,x1=EndLX,y1=EndLY;
    if (x0>x1){
       int tx=x0;
       x0=x1;   x1=tx;
    }
    if (y0>y1){
       int tx=y0;
       y0=y1;   y1=tx;
    }
    bmp = wxBitmap(*image);
    wxBitmap bmp2=bmp.GetSubBitmap(wxRect(wxPoint(x1,y1), wxPoint (x0,y0)));

    if (wxTheClipboard->Open())
   {
        wxTheClipboard->AddData( new wxBitmapDataObject( bmp2) );
        wxTheClipboard->Flush();
        wxTheClipboard->Close();
        char str[108];
        sprintf(str,"Seccion seleccionada x0=%d y0=%d x1=%d y1=%d. Se copio al portapapeles.",x0,y0,x1,y1);
        wxMessageBox(_(str));
   }
}

void wxCShellPanel::MouseRightDown(wxMouseEvent& event)
{
    StartRX=event.GetX();
    StartRY=event.GetY();
}

void wxCShellPanel::MouseLeftDown(wxMouseEvent& event)
{
    StartLX=event.GetX();
    StartLY=event.GetY();
}

void wxCShellPanel::printf(int x, int y,wxString str)
{
     wxMemoryDC dc;
     bmp = wxBitmap(*image);
     dc.SelectObject( bmp );
     dc.DrawText(str, x, y);
     dc.SelectObject(wxNullBitmap);
     *image=bmp.ConvertToImage();
     data=image->GetData();
}

void wxCShellPanel::printf(wxString str)
{
     wxMemoryDC dc;
     bmp = wxBitmap(*image);
     dc.SelectObject( bmp );
     dc.DrawText(str, 5,printPos);
     dc.SelectObject(wxNullBitmap);
     *image=bmp.ConvertToImage();
     printPos+=16;
     data=image->GetData();
}

void wxCShellPanel::PutPixel(int x,int y,int Size, COLORREF Color)
{
  float r=((float)Size)/2.0;
  float sr=r*r;

  if (x-r>=0 && x+r<nc && y-r>=0 && y+r<nr){
    if (Size>1){
    for (int i=-r; i<=r; i++){
      for (int j=-r; j<=r; j++){
	     if (((i*i)+(j*j))<sr){
	        int ii=3*(i+y)*nc+3*(j+x);
	        data[ii]=GetRValue(Color);
	        data[ii+1]=GetGValue(Color);
	        data[ii+2]=GetBValue(Color);
	     }
      }
    }
   }
   else{
	        int ii=3*(y)*nc+3*(x);
	        data[ii]=GetRValue(Color);
	        data[ii+1]=GetGValue(Color);
	        data[ii+2]=GetBValue(Color);
        }
  }
}


void wxCShellPanel::DrawObjects(int nobj,DOBJ *objs)
{

 bmp = wxBitmap(*image);
  //dc.SelectObject(bmp);
 wxMemoryDC dc(bmp);

 //wxBufferedPaintDC dc(this,bmp);
 //dc.SelectObject(bmp);
 for (int i=0; i<nobj; i++)
 {
    dc.SetPen(wxPen (wxColour(GetRValue(objs[i].BorderCol),GetGValue(objs[i].BorderCol),GetBValue(objs[i].BorderCol)), objs[i].lwd));
    dc.SetBrush(wxBrush(wxColour(GetRValue(objs[i].FillCol),GetGValue(objs[i].FillCol),GetBValue(objs[i].FillCol)) ));
   switch (objs[i].type)
   {
     case ARC:
       dc.DrawArc(objs[i].coordX[0],objs[i].coordY[0],objs[i].coordX[1],objs[i].coordY[1],objs[i].coordX[2],objs[i].coordY[2]);
       break;
     case CIRCLE:
       dc.DrawCircle(objs[i].coordX[0],objs[i].coordY[0],objs[i].radius);
       break;
     case RECTANGLE:
       dc.DrawRectangle(objs[i].coordX[0],objs[i].coordY[0],objs[i].width, objs[i].height);
       break;
     case LINE:
       dc.DrawLine(objs[i].coordX[0],objs[i].coordY[0],objs[i].coordX[1],objs[i].coordY[1]);
       break;
     case POLYGON:
       wxPoint *px;
       px=new wxPoint[objs[i].ncoord];
       for (int ii=0; ii<objs[i].ncoord; ii++)
	        px[ii]=wxPoint(objs[i].coordX[ii],objs[i].coordY[ii]);
        dc.DrawPolygon(objs[i].ncoord,px);
       delete []px;
       break;
     default:
       break;
   }

 }
 //dc.SelectObject(wxNullBitmap);
 *(image)=bmp.ConvertToImage();
  data=image->GetData();
  wxClientDC DC(this);
  render(DC);
}


///---------------------------------------------------------------------
/// Esta funcion es la que en realidad hce todo el trabajo de pintado
void wxCShellPanel::render(wxDC&  dc)
{
    bmp = wxBitmap(*image);
    dc.DrawBitmap( bmp, 0, 0, false );
}


void DimensionesBmp(const char *fname,int *pnr,int *pnc)
{
   wxImage image;
   wxString file(fname);
   image.LoadFile(file, wxBITMAP_TYPE_BMP);
   *pnc=image.GetWidth();
   *pnr=image.GetHeight();
}

void DimensionesImg(const char *fname,int *pnr,int *pnc)
{
   wxImage image;
   wxString file(fname);
   wxInitAllImageHandlers();
   image.LoadFile(file, wxBITMAP_TYPE_ANY);
   *pnc=image.GetWidth();
   *pnr=image.GetHeight();
}

void LeeBmp(float **M,const char *fname)
{
   wxImage image;
   wxString file(fname);
   image.LoadFile(file, wxBITMAP_TYPE_BMP);
   unsigned char *Data=image.GetData();
  if (image.GetWidth()*image.GetHeight()>0){
      int dcount=0;
      for (int i=0; i<image.GetHeight(); i++)
	  for (int j=0; j<image.GetWidth(); j++){
	      float R=(float)Data[dcount++];
	      float G=(float)Data[dcount++];
	      float B=(float)Data[dcount++];
	      M[i][j]=0.299*R+0.587*G+0.114*B;
      }
  }
}

void LeeImg(float **M,const char *fname)
{
   wxImage image;
   wxString file(fname);
   wxInitAllImageHandlers();
   image.LoadFile(file,  wxBITMAP_TYPE_ANY);
   unsigned char *Data=image.GetData();
  if (image.GetWidth()*image.GetHeight()>0){
      int dcount=0;
      for (int i=0; i<image.GetHeight(); i++)
	  for (int j=0; j<image.GetWidth(); j++){
	      float R=(float)Data[dcount++];
	      float G=(float)Data[dcount++];
	      float B=(float)Data[dcount++];
	      M[i][j]=0.299*R+0.587*G+0.114*B;
      }
  }
}

void LeeBmpColor(unsigned char**R,unsigned char**G,unsigned char**B,const char *nombre)
{
  wxImage image;
  wxString file(nombre);
  image.LoadFile(file, wxBITMAP_TYPE_BMP);
  unsigned char *Data=image.GetData();
  if (image.GetWidth()*image.GetHeight()>0){
      int dcount=0;
      for (int i=0; i<image.GetHeight(); i++)
	  for (int j=0; j<image.GetWidth(); j++){
	      R[i][j]=Data[dcount++];
	      G[i][j]=Data[dcount++];
	      B[i][j]=Data[dcount++];
      }
  }
}

void LeeImgColor(unsigned char**R,unsigned char**G,unsigned char**B,const char *nombre)
{
  wxImage image;
  wxString file(nombre);
  wxInitAllImageHandlers();
  image.LoadFile(file,  wxBITMAP_TYPE_ANY); //wxBITMAP_TYPE_ANY
  unsigned char *Data=image.GetData();
  if (image.GetWidth()*image.GetHeight()>0){
      int dcount=0;
      for (int i=0; i<image.GetHeight(); i++)
	  for (int j=0; j<image.GetWidth(); j++){
	      R[i][j]=Data[dcount++];
	      G[i][j]=Data[dcount++];
	      B[i][j]=Data[dcount++];
      }
  }

}

void LeeImgColor(COLORREF **data,const char *nombre)
{
  wxImage image;
  wxString file(nombre);
  wxInitAllImageHandlers();
  image.LoadFile(file,  wxBITMAP_TYPE_ANY); //wxBITMAP_TYPE_ANY
  unsigned char *Data=image.GetData();
  if (image.GetWidth()*image.GetHeight()>0){
      int dcount=0;
      for (int i=0; i<image.GetHeight(); i++)
	  for (int j=0; j<image.GetWidth(); j++){
	      data[i][j]=RGB(Data[dcount],Data[dcount+1],Data[dcount+2]);
	      dcount+=3;
      }
  }

}

///Despliega un mensaje con solo la opcion Ok, es una pausa informativa
void MessageOk(const char *_message, const char* _title)
{

  wxMessageBox(_message,_title,wxOK|wxCENTRE,GetLastWindow()->w);

}
///----------------------------------------------------------------------------------------------------------------------
///Despliega un mensaje con la opcion Ok y cancel, es una pregunta sobre si se continua o no
bool MessageYesNo(const char *_message, const char* _title)
{

  int answer=0;
  if (_title)
     answer=wxMessageBox(_message,_title, wxYES_NO);
  else
    answer=wxMessageBox(_message,"Confirm", wxYES_NO);

   if (wxYES==answer)
      return true;
 return false;
}

char *InDialog(const char *OutText,const char *Title,const  char *DefaultValue)
{

  wxString valueTyped;
  wxTextEntryDialog myDialog(NULL, _(OutText), _(Title), _(DefaultValue));
  if ( myDialog.ShowModal() == wxID_OK )
  {
    valueTyped = myDialog.GetValue();
  }
  char *dr=(char *)calloc((1024),sizeof(char));
  strcpy(dr,valueTyped.mb_str().data());
return dr;
}


void window::GetFileNameDialog(char *FileName)
{
        wxFileDialog selecFileDialog(this->w, _("Seleccionar imagen"), "", "","",wxFD_OPEN);
	if (selecFileDialog.ShowModal() == wxID_CANCEL)
             return;
//         if (selecFileDialog.GetFilterIndex()==0)
// 	char *r=selecFileDialog.GetPath().c_str();
        strcpy(FileName,selecFileDialog.GetPath().c_str());


}


/** ***************Funciones de la clase window**************************/

void window::PlotBitmap(float **f,int _nr,int _nc,int x0,int y0,float zoom)
{
  data=w->drawPane->data;
  int nnr=(int)((zoom*(float)_nr)+0.4999);
  int nnc=(int)((zoom*(float)_nc)+0.4999);
  if ( nnr>0 && (nnr+y0)<(nr) && nnc>0 && (nnc+x0)<(nc)){
    float maxf=f[0][0],minf=f[0][0], scalef=1.0;
    for (int i=0; i<(_nr); i++)
      for (int j=0; j<(_nc); j++){
           if (maxf<f[i][j]) maxf=f[i][j];
	   if (minf>f[i][j]) minf=f[i][j];
      }
    scalef=254.9999/(maxf-minf);
//       printf("%d %d %d %d %d %d\n",nr, nc,nnr, nnc,y0,x0);
    for (int i=0; i<nnr; i++)
      for (int j=0; j<nnc; j++)
      {
	int ii=(int)(((float)i)/zoom);
	int jj=(int)(((float)j)/zoom);
	unsigned char colorf=(unsigned char)(int)(scalef*(f[ii][jj]-minf)+0.49999);
	int iii=3*(i+y0)*nc+3*(j+x0);
        data[iii]=colorf;
	data[iii+1]=colorf;
        data[iii+2]=colorf;

      }
  }else{
    char str[1028];
    sprintf(str,"Dimensiones de la ventana: %dx%d, dimensiones de la imagen: %dx%d. Offset: %d,%d",nr,nc,nnr,nnc,x0,y0);
    MessageOk(str);
  }
  w->drawPane->Refresh();
}


void window::PlotCBitmap(unsigned char **r,unsigned char **g,unsigned char **b,int _nr,int _nc,int x0,int y0,float zoom)
{
  data=w->drawPane->data;
  int nnr=(int)((zoom*(float)_nr)+0.4999);
  int nnc=(int)((zoom*(float)_nc)+0.4999);
/*        printf("%d %d %d %d %d %d\n",nr, nc,nnr, nnc,y0,x0);
	fflush(stdout)*/;
  if ( nnr>=0 && (nnr+y0)<(nr) && nnc>=0 && (nnc+x0)<nc){
    for (int i=0; i<nnr; i++)
      for (int j=0; j<nnc; j++)
      {
	int ii=(int)(((float)i)/zoom);
	int jj=(int)(((float)j)/zoom);
	int iii=3*(i+y0)*nc+3*(j+x0);
        data[iii]=r[ii][jj];
	data[iii+1]=g[ii][jj];
        data[iii+2]=b[ii][jj];

      }
  }else{
    char str[1028];
    sprintf(str,"Dimensiones de la ventana: %dx%d, dimensiones de la imagen: %dx%d. Offset: %d,%d",nr,nc,nnr,nnc,x0,y0);
    MessageOk(str);

  }
  w->drawPane->Refresh();
}

void window::PlotCBitmap(COLORREF **col,int _nr,int _nc,int x0,int y0,float zoom)
{
  data=w->drawPane->data;
  int nnr=(int)((zoom*(float)_nr)+0.4999);
  int nnc=(int)((zoom*(float)_nc)+0.4999);
  if ( nnr>=0 && (nnr+y0)<(nr) && nnc>=0 && (nnc+x0)<nc){
    for (int i=0; i<nnr; i++)
      for (int j=0; j<nnc; j++)
      {
	int ii=(int)(((float)i)/zoom);
	int jj=(int)(((float)j)/zoom);
	int iii=3*(i+y0)*nc+3*(j+x0);
        data[iii]=GetRValue(col[ii][jj]);
	data[iii+1]=GetGValue(col[ii][jj]);
        data[iii+2]=GetBValue(col[ii][jj]);

      }
  }else{
    char str[1028];
    sprintf(str,"Dimensiones de la ventana: %dx%d, dimensiones de la imagen: %dx%d. Offset: %d,%d",nr,nc,nnr,nnc,x0,y0);
    MessageOk(str);

  }
  w->drawPane->Refresh();
}



void window::PlotCBitmapF(unsigned char *bptr,int nrp,int ncp,int x0,int y0,float zoom)
{
  data=w->drawPane->data;
    int nnr=(int)((zoom*(float)nrp)+0.4999);
  int nnc=(int)((zoom*(float)ncp)+0.4999);
  if ( nnr>=0 && (nnr+y0)<(nr) && nnc>=0 && (nnc+x0)<nc){
    for (int i=0; i<nnr; i++)
      for (int j=0; j<nnc; j++)
      {
	int ii=(int)(((float)i)/zoom);
	int jj=(int)(((float)j)/zoom);
	int iii=3*(i+y0)*nc+3*(j+x0);
        data[iii]=bptr[3*ii*ncp+3*jj];
	data[iii+1]=bptr[3*ii*ncp+3*jj+1];
        data[iii+2]=bptr[3*ii*ncp+3*jj+2];

      }
  }else{
    char str[1028];
    sprintf(str,"Dimensiones de la ventana: %dx%d, dimensiones de la imagen: %dx%d. Offset: %d,%d",nr,nc,nnr,nnc,x0,y0);
    MessageOk(str);

  }
  w->drawPane->Refresh();
}

void window::PlotBitmap(unsigned char **f,int _nr,int _nc,int x0,int y0,float zoom)
{
  data=w->drawPane->data;
  int nnr=(int)((zoom*(float)_nr)+0.4999);
  int nnc=(int)((zoom*(float)_nc)+0.4999);
  if ( nnr>0 && (nnr+y0)<(nr) && nnc>0 && (nnc+x0)<(nc)){
     for (int i = 0 ; i < _nr ; i++)
	for (int j = 0 ; j < _nc ; j++)
            if (f[i][j] > 230) f[i][j] = 230 ;
    for (int i=0; i<nnr; i++)
      for (int j=0; j<nnc; j++)
      {
	int ii=(int)(((float)i)/zoom);
	int jj=(int)(((float)j)/zoom);
	unsigned char colorf=f[ii][jj];
	int iii=3*(i+y0)*nc+3*(j+x0);
        data[iii]=colorf;
	data[iii+1]=colorf;
        data[iii+2]=colorf;

      }
  }else{
    char str[1028];
    sprintf(str,"Dimensiones de la ventana: %dx%d, dimensiones de la imagen: %dx%d. Offset: %d,%d",nr,nc,nnr,nnc,x0,y0);
    MessageOk(str);
  }
  w->drawPane->Refresh();
}
void window::Bitmap(MByte  &B,float zoom,int OffsetX,int OffsetY)
{
  data=w->drawPane->data;
  int nnr=(int)((zoom*(float)B.Rows())+0.4999);
  int nnc=(int)((zoom*(float)B.Cols())+0.4999);
  if ( nnr>0 && (nnr+OffsetY)<(nr) && nnc>0 && (nnc+OffsetX)<(nc)){
    for (int i=0; i<nnr; i++)
      for (int j=0; j<nnc; j++)
      {
	int ii=(int)(((float)i)/zoom);
	int jj=(int)(((float)j)/zoom);
	unsigned char colorf=B(ii,jj);
	int iii=3*(i+OffsetY)*nc+3*(j+OffsetX);
        data[iii]=colorf;
	data[iii+1]=colorf;
        data[iii+2]=colorf;

      }
  }else{
    char str[1028];
    sprintf(str,"Dimensiones de la ventana: %dx%d, dimensiones de la imagen: %dx%d. Offset: %d,%d",nr,nc,nnr,nnc,OffsetX,OffsetY);
    MessageOk(str);
  }
  w->drawPane->Refresh();

}
void window::CBitmap(MByte  &R,MByte &G,MByte &B,float zoom,int OffsetX,int OffsetY)
{
  data=w->drawPane->data;
  int nnr=(int)((zoom*(float)B.Rows())+0.4999);
  int nnc=(int)((zoom*(float)B.Cols())+0.4999);
  if ( nnr>0 && (nnr+OffsetY)<(nr) && nnc>0 && (nnc+OffsetX)<(nc)){
    for (int i=0; i<nnr; i++)
      for (int j=0; j<nnc; j++)
      {
	int ii=(int)(((float)i)/zoom);
	int jj=(int)(((float)j)/zoom);
	int iii=3*(i+OffsetY)*nc+3*(j+OffsetX);
        data[iii]=R(ii,jj);
	data[iii+1]=G(ii,jj);
        data[iii+2]=B(ii,jj);

      }
  }else{
    char str[1028];
    sprintf(str,"Dimensiones de la ventana: %dx%d, dimensiones de la imagen: %dx%d. Offset: %d,%d",nr,nc,nnr,nnc,OffsetX,OffsetY);
    MessageOk(str);
  }
  w->drawPane->Refresh();

}

void window::Bitmap(MFloat &F,float zoom,int OffsetX,int OffsetY)
{
  data=w->drawPane->data;
  int nnr=(int)((zoom*(float)F.Rows())+0.4999);
  int nnc=(int)((zoom*(float)F.Cols())+0.4999);
  if ( nnr>0 && (nnr+OffsetY)<(nr) && nnc>0 && (nnc+OffsetX)<(nc)){
    float maxf=F(0,0),minf=F(0,0), scalef=1.0;
    for (int i=0; i<F.Rows(); i++)
      for (int j=0; j<F.Cols(); j++){
           if (maxf<F(i,j)) maxf=F(i,j);
	   if (minf>F(i,j)) minf=F(i,j);
      }
    scalef=254.9999/(maxf-minf);
    for (int i=0; i<nnr; i++)
      for (int j=0; j<nnc; j++)
      {
	int ii=(int)(((float)i)/zoom);
	int jj=(int)(((float)j)/zoom);
	unsigned char colorf=(unsigned char)(int)(scalef*(F(ii,jj)-minf)+0.49999);
	int iii=3*(i+OffsetY)*nc+3*(j+OffsetX);
        data[iii]=colorf;
	data[iii+1]=colorf;
        data[iii+2]=colorf;

      }
  }else{
    char str[1028];
    sprintf(str,"Dimensiones de la ventana: %dx%d, dimensiones de la imagen: %dx%d. Offset: %d,%d",nr,nc,nnr,nnc,OffsetX, OffsetY);
    MessageOk(str);
  }
  w->drawPane->Refresh();

}



void window::Cls()
{
  data=w->drawPane->data;
    for (int i=0; i<nr; i++){
    for (int j=0; j<nc; j++){
        data[3*i*nc+3*j]=255;
	data[3*i*nc+3*j+1]=255;
        data[3*i*nc+3*j+2]=255;
    }
  }
  w->drawPane->printPos=0;
  w->drawPane->Refresh();

}

void window::PutPixel(int x,int y,int Size, COLORREF Color)
{
    w->drawPane->PutPixel(x,y,Size, Color);
}


void window:: line(int x0,int y0,int x1,int y1,COLORREF Color,int Wide)
{
  wxMemoryDC dc;
  w->drawPane->bmp = wxBitmap(*w->drawPane->image);
  dc.SelectObject(w->drawPane->bmp );
  dc.SetPen(wxPen (wxColour(GetRValue(Color),GetGValue(Color),GetBValue(Color)), Wide));
  dc.DrawLine(x0, y0,x1,y1);
  dc.SelectObject(wxNullBitmap);
  *(w->drawPane->image)=w->drawPane->bmp.ConvertToImage();
  w->drawPane->data=w->drawPane->image->GetData();
  data=w->drawPane->image->GetData();
}


void window::PlotFEMMesh(DMESH &msh, int x0,int y0, int x1, int y1)
{
  wxMemoryDC dc;
  w->drawPane->bmp = wxBitmap(*w->drawPane->image);
  dc.SelectObject(w->drawPane->bmp );
  COLORREF Color=msh.getBORDERCOLOR();
  dc.SetPen(wxPen(wxColour(GetRValue(Color),GetGValue(Color),GetBValue(Color)), 2));

   int width=nc, height=nr;

   double scaleX=-10.0,scaleY=-10.0, scaleC;
   int xs,ys;
   if (x1==-1)
   {
      if (x0>=0)
	 xs=width-x0;
      else xs=width;
      if (xs>0)
	scaleX=((double)xs)/(msh.getMAXX()-msh.getMINX());
//        printf("scaleX=%lf %lf %lf\n",scaleX,msh.getMAXX(),msh.getMINX());
   }
   else{
      if (x0>=0 && x1<=width)
	 xs=x1-x0;
      if (xs>0)
	scaleX=((double)xs)/(msh.getMAXX()-msh.getMINX());
   }
   if (y1==-1)
   {
      if (y0>=0)
	 ys=height-y0;
      if (ys>0)
	scaleY=((double)ys)/(msh.getMAXY()-msh.getMINY());
   }
   else{
      if (y0>=0 && y1<=height)
	 ys=y1-y0;
      if (ys>0)
	scaleY=((double)ys)/(msh.getMAXY()-msh.getMINY());
   }
   if (scaleX<0 || scaleY<0)
     MessageOk("The scale parameters are do not fit the painting area!","Error!");

   double maxP=-9e99, minP=9e99;
   for (int i=0; i<msh.getNELE(); i++)
     if (msh.getINDICATOR(i)>0){
        double p=msh.getCPROPERTY(i);
	if (maxP<p) maxP=p;
        if (minP>p) minP=p;
     }
   scaleC=1.0/(maxP-minP);






   wxPoint *px;
   px=new wxPoint[msh.getNPEL()];
   int *conec=new int[msh.getNPEL()];
   for (int i=0; i<msh.getNELE(); i++)
//    for (int i=0; i<10; i++)
   {
       if (msh.getINDICATOR(i)!=-1){
       double col=scaleC*(msh.getCPROPERTY(i)-minP);
//         std::printf("%lf %lf %lf\n",col,minP,maxP);
       if (col>=0 && col<0.25){
	 double R0=30,G0=5,B0=100, R1=0,G1=255,B1=255;
	 double intensity=col/0.25;
         R0=R0*(1.0-intensity)+R1*intensity+0.5;
         G0=G0*(1.0-intensity)+G1*intensity+0.5;
         B0=B0*(1.0-intensity)+B1*intensity+0.5;
	 Color=RGB((int)R0,(int)G0, (int)B0);
       }
       else if (col>=0.25 && col<0.5){
	 double R0=0,G0=255,B0=255, R1=0,G1=255,B1=0;
	 double intensity=(col-0.25)/0.25;
         R0=R0*(1.0-intensity)+R1*intensity+0.5;
         G0=G0*(1.0-intensity)+G1*intensity+0.5;
         B0=B0*(1.0-intensity)+B1*intensity+0.5;
	 Color=RGB((int)R0,(int)G0, (int)B0);
       }
       else if (col>=0.5 && col<0.75){
	 double R0=0,G0=255,B0=0, R1=255,G1=255,B1=0;
	 double intensity=(col-0.5)/0.25;
         R0=R0*(1.0-intensity)+R1*intensity+0.5;
         G0=G0*(1.0-intensity)+G1*intensity+0.5;
         B0=B0*(1.0-intensity)+B1*intensity+0.5;
	 Color=RGB((int)R0,(int)G0, (int)B0);
       }
       else if (col>=0.75 && col<=1){
	 double R0=255,G0=255,B0=0, R1=128,G1=0,B1=0;
	 double intensity=(col-0.75)/0.25;
         R0=R0*(1.0-intensity)+R1*intensity+0.5;
         G0=G0*(1.0-intensity)+G1*intensity+0.5;
         B0=B0*(1.0-intensity)+B1*intensity+0.5;
	 Color=RGB((int)R0,(int)G0, (int)B0);
       }

       msh.setFILLCOLOR(i, Color);

       if (msh.getINDICATOR(i)==0)
	  Color=RGB(255,255,255);
       else if (msh.getINDICATOR(i)==1)
	 Color=msh.getBORDERCOLOR();
       else if (msh.getINDICATOR(i)==3)
            dc.SetPen(wxPen(wxColour(GetRValue(Color),GetGValue(Color),GetBValue(Color)), 1));
       dc.SetBrush(wxBrush(wxColour(GetRValue(Color),GetGValue(Color),GetBValue(Color))));
       msh.getCONNECT(i,conec);
       for (int ii=0; ii<msh.getNPEL(); ii++){
	   int xx= (int)(scaleX*(msh.getXCOORD(conec[ii]-1)-msh.getMINX())+0.5)+x0;
	   int yy= height-(int)(scaleY*(msh.getYCOORD(conec[ii]-1)-msh.getMINY())+0.5)-y0;
	   px[ii]=wxPoint( xx , yy);
//  	   std::printf("%d %d\n",xx,yy);
       }
        dc.DrawPolygon(msh.getNPEL(),px);
       }
   }

//   dc.DrawLine(0, 0,100,100);
  delete[]px;
  delete[]conec;
  dc.SelectObject(wxNullBitmap);
  *(w->drawPane->image)=w->drawPane->bmp.ConvertToImage();
  w->drawPane->data=w->drawPane->image->GetData();
  data=w->drawPane->image->GetData();
  Paint();
  w->drawPane->Refresh();
}



void window::PlotContour(FUNA2D fx,double  xinf,double xsup,double  yinf,double  ysup)
{
//   wxMemoryDC dc;
//   w->drawPane->bmp = wxBitmap(*w->drawPane->image);
//   dc.SelectObject(w->drawPane->bmp );
  COLORREF Color;
  int width=nc, height=nr;
  double dx=(xsup-xinf)/(double)(width-1);
  double dy=(ysup-yinf)/(double)(height-1);
  double minf=fx(xinf,yinf), maxf=minf;

  for (double x=xinf; x<=xsup; x+=dx)
      for (double y=yinf; y<=ysup; y+=dy){
	   double ff=fx(x,y);
	   if (minf>ff)
	       minf=ff;
	   if (maxf<ff)
	       maxf=ff;
       }


   double y=ysup;
  for (int i=0 ; i<nr; i++){
    double x=xinf;
    for (int j=0 ; j<nc; j++ )
    {


       double col=(fx(x,y)-minf)/(maxf-minf);
//        std::cout<< "Col " <<col << " x " << x << " y " <<y <<" dx " << dx << " dy " <<dy <<endl;
       if (col>=0 && col<0.25){
	 double R0=30,G0=5,B0=100, R1=0,G1=255,B1=255;
	 double intensity=col/0.25;
         R0=R0*(1.0-intensity)+R1*intensity+0.5;
         G0=G0*(1.0-intensity)+G1*intensity+0.5;
         B0=B0*(1.0-intensity)+B1*intensity+0.5;
	 Color=RGB((int)R0,(int)G0, (int)B0);
       }
       else if (col>=0.25 && col<0.5){
	 double R0=0,G0=255,B0=255, R1=0,G1=255,B1=0;
	 double intensity=(col-0.25)/0.25;
         R0=R0*(1.0-intensity)+R1*intensity+0.5;
         G0=G0*(1.0-intensity)+G1*intensity+0.5;
         B0=B0*(1.0-intensity)+B1*intensity+0.5;
	 Color=RGB((int)R0,(int)G0, (int)B0);
       }
       else if (col>=0.5 && col<0.75){
	 double R0=0,G0=255,B0=0, R1=255,G1=255,B1=0;
	 double intensity=(col-0.5)/0.25;
         R0=R0*(1.0-intensity)+R1*intensity+0.5;
         G0=G0*(1.0-intensity)+G1*intensity+0.5;
         B0=B0*(1.0-intensity)+B1*intensity+0.5;
	 Color=RGB((int)R0,(int)G0, (int)B0);
       }
       else if (col>=0.75 && col<=1){
	 double R0=255,G0=255,B0=0, R1=128,G1=0,B1=0;
	 double intensity=(col-0.75)/0.25;
         R0=R0*(1.0-intensity)+R1*intensity+0.5;
         G0=G0*(1.0-intensity)+G1*intensity+0.5;
         B0=B0*(1.0-intensity)+B1*intensity+0.5;
	 Color=RGB((int)R0,(int)G0, (int)B0);
       }
       	int iii=3*(i)*nc+3*(j);
        data[iii]=GetRValue(Color);
	data[iii+1]=GetGValue(Color);
        data[iii+2]=GetBValue(Color);
        x+=dx;
   }
    y-=dy;
  }
//   dc.SelectObject(wxNullBitmap);
//   *(w->drawPane->image)=w->drawPane->bmp.ConvertToImage();
//   w->drawPane->data=w->drawPane->image->GetData();
//   data=w->drawPane->image->GetData();
  Paint();
  w->drawPane->Refresh();
}







void window::Paint()
{
   wxPaintDC dc(w->drawPane);
   w->drawPane->render(dc);
   w->drawPane->Refresh();
   w->drawPane->Update();
}

void window::GetPixelColor(int x,int y,COLORREF *col)
{
  data=w->drawPane->data;
  int ii=3*(x)*nc+3*(y);
   *col=RGB(data[ii],data[ii+1],data[ii+2]);
}


window::window(const char *title, const int height, const int width, const int posX, const int posY)
{
  wxInitAllImageHandlers();
  w=new ShellWindow(title, wxPoint(posX,posY), wxSize(width,height));
  w->Show(true);
  data=w->data=w->drawPane->data;
  nr=w->nr=w->drawPane->nr;
  nc=w->nc=w->drawPane->nc;
  GetLastWindow(this);
  if (GetFirstWindow()==NULL )
     GetFirstWindow(this);
}

void window::printf(int x, int y, const char *format,...)
{

    va_list argPtr;
    va_start(argPtr,format);
    wxString str;
    str.PrintfV(format, argPtr);
    w->drawPane->printf(x,y,str);
va_end(argPtr);
}

void window::printf(const char *format,...)
{

    va_list argPtr;
    va_start(argPtr,format);
    wxString str;
    str.PrintfV(format, argPtr);
    w->drawPane->printf(str);
    va_end(argPtr);
}

window::~window()
{


}

/** **********Funciones de la clase Window (con mayuscula) la interfaz general a las ventanas***********/
Window::Window(const char *_title, const int _height, const int _width, const int _posX, const int _posY)
{
   win=NULL;
   strcpy(title,_title);
   height=_height+20;
   width=_width;
   posX=_posX;
   posY=_posY;
   InitWindow();
   win->Cls();
}

Window::~Window()
{
  win=NULL;
}

///Esta funcion es la que realmente hace la instancia de la ventana
void Window::InitWindow()
{
  if (win==NULL)
  {
    win=new window(title,height,width,posX,posY);
    GetApplicationPointer()->SetTopWindow(this->win->w);
/*    win->w->Show(1);
    win->w->Update();
    win->w->Refresh();*/
    MessageOk("Window");
   // wxMessageDialog diag(this->win->w,"init","window creation");
    //diag.Show(1);
    //diag.Show(0);

   // win->w->Timer->Start(100);
    nr=win->nr;
    nc=win->nc;///Numero de renglones y columnas de la matriz de datos utilizada para dibujar

//     std::printf("Creo ventana\n");
  }

}

void Window::GetFileNameDialog(char *FileName)
{

    win->GetFileNameDialog(FileName);

}


 /// Dibuja un Bitmap en escala de grises a partir de una matriz
void Window::PlotBitmap(float **f,int nr,int nc,int x0,int y0,float zoom)
{
    InitWindow();

    win->PlotBitmap(f,nr,nc,x0,y0,zoom);
}
   ///Dibuja un Bitmap de color a partir de una matriz
void Window::PlotCBitmap(unsigned char **r,unsigned char **g,unsigned char **b,int nr,int nc,int x0,int y0,float zoom)
{
    InitWindow();
    win->PlotCBitmap(r,g,b,nr,nc,x0,y0,zoom);
}
void Window::PlotCBitmap(COLORREF **col,int _nr,int _nc,int x0,int y0,float zoom)
{
     InitWindow();
     win->PlotCBitmap(col,_nr,_nc,x0,y0,zoom);
}

void Window::PlotCBitmapF(unsigned char *bptr,int nrp,int ncp,int x0,int y0,float zoom)
{
    InitWindow();
    win->PlotCBitmapF(bptr,nrp,ncp,x0,y0,zoom);
}


void Window::PlotFEMMesh(DMESH &msh, int x0,int y0, int x1, int y1)
{
    InitWindow();
    win->PlotFEMMesh(msh,x0,y0, x1,y1);
//         std::printf("Creo ventana\n");
}
void Window::PlotContour(FUNA2D fx,double  xinf,double xsup,double  yinf,double  ysup)
{
  InitWindow();
  win->PlotContour(fx,xinf,xsup,yinf,ysup);
}

void Window::Snapshot(const char*FileName, ShellImgType _T, int x0, int y0, int x1, int y1, int silent)
{
  InitWindow();
  win->w->drawPane->Snapshot(FileName,_T, x0, y0, x1, y1,silent);
}

void Window::PlotBitmap(unsigned char **f,int nr,int nc,int x0,int y0,float zoom)
{
     InitWindow();
     win->PlotBitmap(f,nr,nc,x0,y0,zoom);
}
void Window::Bitmap(MByte  &B,float zoom,int OffsetX,int OffsetY)
{
     InitWindow();
     win->Bitmap(B,zoom,OffsetX,OffsetY);

}
void Window::CBitmap(MByte  &R,MByte &G,MByte &B,float zoom,int OffsetX,int OffsetY)
{
     InitWindow();
     win->CBitmap(R,G,B,zoom,OffsetX,OffsetY);

}

void Window::Bitmap(MFloat &F,float zoom,int OffsetX,int OffsetY)
{
     InitWindow();
     win->Bitmap(F,zoom,OffsetX,OffsetY);
}

void Window::Cls()
{
    InitWindow();
    win->Cls();
}
void Window::PutPixel(int x,int y,int Size,COLORREF Color)
{
    InitWindow();
    win->PutPixel(x,y,Size,Color);
}
void Window::line(int x0,int y0,int x1,int x2,COLORREF Color,int Wide)
{
    InitWindow();
    win->line(x0, y0, x1, x2,Color, Wide);
}

void Window::DrawObjects(int nobj, DOBJ *objs)
{
   InitWindow();
   win->w->drawPane->DrawObjects(nobj,objs);
//    Paint();
//    win->w->drawPane->Refresh();
//       Paint();

}


void Window::Paint()
{
   InitWindow();
   win->Paint();
}
void Window::GetPixelColor(int x,int y,COLORREF *col)
{
   InitWindow();
   win->GetPixelColor(x,y,col);
}
void Window::printf(int x, int y, const char *format,...)
{
    InitWindow();
    va_list argPtr;
    va_start(argPtr,format);
    wxString str;
    str.PrintfV(format, argPtr);
    win->w->drawPane->printf(x,y,str);
    va_end(argPtr);

}
void Window::printf(const char *format,...)
{
   InitWindow();
    va_list argPtr;
    va_start(argPtr,format);
    wxString str;
    str.PrintfV(format, argPtr);
    win->w->drawPane->printf(str);
    va_end(argPtr);
}


/** Funcion principal, este es el equivalente al main **/

bool CShell::OnInit()
{
  ///Esta es la funcion que programa el usuario, me sirve para tener control
  ///de cosas que se hagan antes de lo que programa el usuario y despues.
  GetApplicationPointer(this);

  char **margv;
  margv=new char*[argc];
  for (int i=0; i<argc; i++){
      wxString str(argv[i]);
      margv[i]=new char[str.Len()+1];
    strcpy(margv[i], argv[i].c_str());
  }

  ShellMain(argc,margv);
  for (int i=0; i<argc; i++)
      delete []margv[i];
  if (!GetLastWindow())
     Exit();
 return true;
}

/** **********************************Funciones del MessageOk************************************/

  BEGIN_EVENT_TABLE ( ButtonFrame, wxFrame )
    EVT_BUTTON ( BUTTON_Ok, ButtonFrame::OnExit ) // Tell the OS to run MainFrame::OnExit when
  END_EVENT_TABLE() // The button is pressed

  ButtonFrame::ButtonFrame(const wxString &title, const wxPoint &pos, const wxSize
    &size): wxFrame((wxFrame*)NULL, wxSTAY_ON_TOP, title, pos, size)
  {
    Panel= new wxPanel(this,500, wxPoint(0,0), wxSize(50,100));
    ButtonOk = new wxButton(this, BUTTON_Ok, _T("Ok"),
      // shows a button on this window
    wxPoint(50,10), wxSize(150,50)); // with the text "hello World"
  }

  void ButtonFrame::OnExit( wxCommandEvent& event )
  {
    Close(TRUE); // Tells the OS to quit running this process
  }
/** ***********************************************************************************************/


/**Funciones de la clase DMESH para leer y graficar una mallad de elemento finito**/


DMESH::DMESH()
{
 nnod=0; nele=0; npel=0; nfix0=0; nfix1=0;
 fixnode0=NULL; fixnode1=NULL;
 fillcolor=NULL;
 cproperty=NULL;
 xcoord=NULL;
 ycoord=NULL;
 fix0X=NULL;
 fix0Y=NULL;
 fix1X=NULL;
 fix1Y=NULL;
 con=NULL;
 ind=NULL;
 minP=9e99;
 maxP=-9e99;
}

 /**
  * file= nombre del archivo de entrada
  * lcoordini= linea donde comienzan las coordenadas
  * skipC= numero de lineas (a saltar) que hay entre las coordenadas y conectividades
  * skipF0=numero de lineas a saltar entre las conectividades y los nodos fijos 0
  * skipF1=numero de lineas a saltar entre los nodos fijos 0 y los nodos fijos 1
  * lname= Linea donde esta el nombre del problema
  * lele= linea donde se lee el numero de elementos
  * lnodes= linea donde se lee el numero de nodos
  * lfix0= linea donde se lee el numero de restricicones (nodos fijos)
  * lfix1= linea donde se lee el numero de nodos con propiedad (carga)
  * */
DMESH::DMESH(char *file, int lcoordini,int _npel, int skipC,int skipF0, int skipF1, int lname, int lele,int lnodes, int lfix0, int lfix1)
{
  FILE *in=fopen(file,"r");
  npel=_npel;
    minP=9e99;
    maxP=-9e99;

  if (in!=NULL)
  {
     char str[512];
     for (int i=0; i<(lname-1); i++)
         fgets(str,512,in);
     fscanf(in, "%s",name);
     fclose(in);
//      printf("name=%s\n",name);
     in=fopen(file,"r");
     for (int i=0; i<(lele-1); i++)
         fgets(str,512,in);
     fscanf(in, "%d",&nele);
     fclose(in);
//      printf("nele=%d\n",nele);
     in=fopen(file,"r");
     for (int i=0; i<(lnodes-1); i++)
         fgets(str,512,in);
     fscanf(in, "%d",&nnod);
     fclose(in);
//      printf("nnod=%d\n",nnod);
     in=fopen(file,"r");
     for (int i=0; i<(lfix0-1); i++)
         fgets(str,512,in);
     fscanf(in, "%d",&nfix0);
     fclose(in);
//      printf("nfix0=%d\n",nfix0);
     in=fopen(file,"r");
     for (int i=0; i<(lfix1-1); i++)
         fgets(str,512,in);
//      fscanf(in, "%d",&nfix1);
     fclose(in);
//      printf("nfix1=%d\n",nfix1);
     in=fopen(file,"r");
     for (int i=0; i<(lcoordini-1); i++)
         fgets(str,512,in);

     if (nnod>0){
       xcoord= new double[nnod];
       ycoord= new double[nnod];
     }

     if (nele>0){
        con= new int*[nele];
	for (int i=0; i<nele; i++)
	  con[i]=new int[npel];
	cproperty= new double[nele];
	fillcolor=new COLORREF[nele];
	ind= new int[nele];
     }
     if (nfix0>0){
       fixnode0= new int[nfix0];
       fix0X= new double[nfix0];
       fix0Y= new double[nfix0];
     }
     if (nfix0>0){
       fixnode1= new int[nfix1];
       fix1X= new double[nfix1];
       fix1Y= new double[nfix1];
     }
     minY=minX=9e99;
     maxY=maxX=-9e99;

     for (int i=0; i<nnod; i++){
          fscanf(in, "%lf %lf",&xcoord[i],&ycoord[i]);
	  if (minX>xcoord[i]) minX=xcoord[i];
	  if (minY>ycoord[i]) minY=ycoord[i];
	  if (maxX<xcoord[i]) maxX=xcoord[i];
	  if (maxY<ycoord[i]) maxY=ycoord[i];
// 	  printf("x=%lf y=%lf\n",xcoord[i],ycoord[i]);
     }
      for (int i=0; i<(skipC-1); i++)
         fgets(str,512,in);
     for (int i=0; i<nele; i++){
       int aux;
          fscanf(in,"%d",&aux );
	  for (int j=0; j<npel; j++){
	      fscanf(in,"%d ",&con[i][j] );
// 	      printf("%d ",con[i][j]);
	  }
// 	  printf("\n");
     }
      for (int i=0; i<(skipF0-1); i++)
         fgets(str,512,in);

     for (int i=0; i<nfix0; i++)
          fscanf(in,"%d %lf %lf",&fixnode0[i], &fix0X[i],&fix0Y[i]);
      for (int i=0; i<(skipF1-1); i++)
         fgets(str,512,in);
     for (int i=0; i<nfix1; i++)
          fscanf(in,"%d %lf %lf",&fixnode1[i], &fix1X[i],&fix1Y[i]);



      fclose(in);

  }else{
    char msh[312];
    sprintf(msh,"I can not open the file %s!",file);
    MessageOk(msh, "Error");

  }

}
DMESH::DMESH(int _nnod, int _nele, int _nfix0, int _nfix1, int _npel)
{
    minP=9e99;
    maxP=-9e99;
    npel=_npel;
     if (nnod>0){
       xcoord= new double[nnod];
       ycoord= new double[nnod];
     }

     if (nele>0){
        con= new int*[nele];
	for (int i=0; i<nele; i++)
	  con[i]=new int[npel];
	cproperty= new double[nele];
	fillcolor=new COLORREF[nele];
	ind= new int[nele];
     }
     if (nfix0>0){
       fixnode0= new int[nfix0];
       fix0X= new double[nfix0];
       fix0Y= new double[nfix0];
     }
     if (nfix1>0){
       fixnode1= new int[nfix1];
       fix1X= new double[nfix1];
       fix1Y= new double[nfix1];
     }
}

DMESH::~DMESH()
{
     if (nnod>0){
       delete []xcoord;
       delete []ycoord;
     }

     if (nele>0){

	for (int i=0; i<nele; i++)
	  delete []con[i];
	delete []con;
	delete []cproperty;
	delete []fillcolor;
	delete []ind;
     }
     if (nfix0>0){
       delete []fixnode0;
       delete []fix0X;
       delete []fix0Y;
     }
     if (nfix1>0){
       delete []fixnode1;
       delete []fix1X;
       delete []fix1Y;
     }
}


void DMESH::setNNOD(int val)
{

     if (nnod>0){
       delete []xcoord;
       delete []ycoord;
     }
     nnod=val;
     minX=9e99;
     maxX=-9e99;
     minY=9e99;
     maxY=-9e99;
     if (nnod>0){
       xcoord= new double[nnod];
       ycoord= new double[nnod];
     }
}
void DMESH::setNELE(int val)
{
     if (nele>0){

	for (int i=0; i<nele; i++)
	  delete []con[i];
	delete []con;
	delete []cproperty;
	delete []fillcolor;
	delete []ind;
     }
     nele=val;
     minP=9e99;
     maxP=-9e99;
     if (nele>0){
        con= new int*[nele];
	for (int i=0; i<nele; i++)
	  con[i]=new int[npel];
	cproperty= new double[nele];
	fillcolor=new COLORREF[nele];
	ind= new int[nele];
     }
}
void DMESH::setNFIX0(int val)
{
     if (nfix0>0){
       delete []fixnode0;
       delete []fix0X;
       delete []fix0Y;
     }
     nfix0=val;
     if (nfix0>0){
       fixnode0= new int[nfix0];
       fix0X= new double[nfix0];
       fix0Y= new double[nfix0];
     }
}
void DMESH::setNFIX1(int val)
{
     if (nfix1>0){
       delete []fixnode1;
       delete []fix1X;
       delete []fix1Y;
     }
     nfix1=val;
     if (nfix1>0){
       fixnode1= new int[nfix1];
       fix1X= new double[nfix1];
       fix1Y= new double[nfix1];
     }

}
void DMESH::setNPEL(int val)
{
     if (nele>0){
	for (int i=0; i<nele; i++)
	  delete []con[i];
	delete []con;
	delete []cproperty;
	delete []fillcolor;
     }
     npel=val;
     if (nele>0){
        con= new int*[nele];
	for (int i=0; i<nele; i++)
	  con[i]=new int[npel];
	cproperty= new double[nele];
	fillcolor=new COLORREF[nele];
     }

}
 void DMESH::setXCOORD(int i, double val){ xcoord[i]=val;
   	  if (minX>xcoord[i]) minX=xcoord[i];
	  if (maxX<xcoord[i]) maxX=xcoord[i];
}
 void DMESH::setYCOORD(int i, double val){ ycoord[i]=val;
	  if (minY>ycoord[i]) minY=ycoord[i];
	  if (maxY<ycoord[i]) maxY=ycoord[i];
}
 void DMESH::setCONNECT(int i, int conec[]){
   for (int j=0; j<npel; j++)
    con[i][j]=conec[j];
}
 void DMESH::setFIXNODE0(int i, int val){ fixnode0[i]=val;}
 void DMESH::setFIXNODE1(int i, int val){ fixnode1[i]=val;}
 void DMESH::setXFIX0(int i, double val){ fix0X[i]=val;}
 void DMESH::setYFIX0(int i, double val){ fix0Y[i]=val;}
 void DMESH::setXFIX1(int i, double val){ fix1X[i]=val;}
 void DMESH::setYFIX1(int i, double val){ fix1Y[i]=val;}

 void DMESH::setCPROPERTY(int i, double val){ cproperty[i]=val;
	  if (minP>cproperty[i]) minP=cproperty[i];
	  if (maxP<cproperty[i]) maxP=cproperty[i];
}



wxIMPLEMENT_APP(CShell);

#if defined (__linux__) || defined(__APPLE__)
unsigned char GetRValue(COLORREF col)
{  return col.R; }
unsigned char GetGValue(COLORREF col)
{  return col.G; }
unsigned char GetBValue(COLORREF col)
{  return col.B; }

#endif


#endif


