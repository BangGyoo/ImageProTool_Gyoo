// ImageProToolView.cpp : implementation of the CImageProToolView class
//

#include "stdafx.h"
#include "ImageProTool.h"
#include "Dib.h"
#include "math.h"
#include "time.h"
#include "ImageProToolDoc.h"
#include "ImageProToolView.h"
#include "Histogram.h"


const double PI = acos(-1.0);
const int N=15;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//========================================
/////////////////////////////////////////////////////////////////////////////
// CImageProToolView

IMPLEMENT_DYNCREATE(CImageProToolView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProToolView, CScrollView)
	//{{AFX_MSG_MAP(CImageProToolView)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)

	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_RGBTOGRAY, &CImageProToolView::OnRgbtogray)
	ON_COMMAND(ID_HISTOGRAM, &CImageProToolView::OnHistogram)
	ON_COMMAND(ID_CHAP_GLOBALTHRESHOLDING, &CImageProToolView::OnGlobalThresholding)
	ON_COMMAND(ID_CHAP_HISTOGRAMSTRETCHING, &CImageProToolView::OnHistogramStretching)
	ON_COMMAND(ID_CHAP_HISTOGRAMEQUAL, &CImageProToolView::OnHistogramEqual)
	ON_COMMAND(ID_CHAP_MEANFILTER, &CImageProToolView::OnMeanfilter)
	ON_COMMAND(ID_CHAP_MEDIANFILTER, &CImageProToolView::OnMedianfilter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProToolView construction/destruction

CImageProToolView::CImageProToolView()
{
  Is_FileOpen = false;
}

CImageProToolView::~CImageProToolView()
{
}

BOOL CImageProToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProToolView drawing

void CImageProToolView::OnDraw(CDC* pDC)
{
	CImageProToolDoc* pDoc = GetDocument();
  	ASSERT_VALID(pDoc);

	if(Is_FileOpen)
	{
  
    CSize sizeTotal;
		
	sizeTotal.cx =pDoc->m_Width;
	sizeTotal.cy =pDoc->m_Height;
	SetScrollSizes(MM_TEXT, sizeTotal);
    
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);
   	
	CPalette* pOldPalette = pDC->SelectPalette(pDoc->m_pDib->m_pPalDib, FALSE);
	pDC->RealizePalette();
	pDoc->m_pDib->Draw(pDC, 0, 0,pDoc->m_Width, pDoc->m_Height);	

	}
}


void CImageProToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
    CImageProToolDoc* pDoc = GetDocument();
	CSize sizeTotal;
	
	if(pDoc->m_Height<=0 || pDoc->m_Width<=0)
	{sizeTotal.cx =100;
	sizeTotal.cy =100;}
	else {
    sizeTotal.cx =pDoc->m_Height;
	sizeTotal.cy =pDoc->m_Width;
	}
	SetScrollSizes(MM_TEXT, sizeTotal);
    
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProToolView printing

BOOL CImageProToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageProToolView diagnostics

#ifdef _DEBUG
void CImageProToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProToolDoc* CImageProToolView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProToolDoc)));
	return (CImageProToolDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProToolView message handlers

void CImageProToolView::OnFileOpen() 
{   

	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	
	pDoc->OnFileOpen();
	Is_FileOpen = true;
	Invalidate();
}


//-----------------------------------------------------------------------------------------------Seperate and Set RGB
void CImageProToolView::Seperate_RGB(BYTE* Data, RGBptr** ptr)
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int width = pDoc->m_Width;    
	int height = pDoc->m_Height; 

	int i,j,y,x;

	for(i=height-1,y=0;i>=0;i--,y++)
		for(j=0,x=0;j<width;j++,x++)
		{  
			ptr[y][x].b=Data[i*width*3+j*3];
			ptr[y][x].g=Data[i*width*3+j*3+1];
			ptr[y][x].r=Data[i*width*3+j*3+2];
		}
}
void CImageProToolView::SetRGBptr(BYTE* pData, RGBptr** ptr1,int width, int height)
{
	int i,j,y,x;
	for(i=height-1,y=0;i>=0;i--,y++)
		for(j=0,x=0;j<width;j++,x++)
		{
			pData[i*width*3+j*3]=(BYTE)(ptr1[y][x].b);
			pData[i*width*3+j*3+1]=(BYTE)(ptr1[y][x].g);
			pData[i*width*3+j*3+2]=(BYTE)(ptr1[y][x].r);

		}	
}


//-------------------------------------------------------------------------------------------------------------------


void CImageProToolView::OnRgbtogray()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;	
 	
	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1= new RGBptr*[height];
	  
	for( int i = 0 ; i < height ; i++ )
	{
	      ptr1[i]= new RGBptr [width];		 
	 }		
	
   	Seperate_RGB(pData,ptr1); 
	BYTE gray;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++) 	
		{
			gray=(BYTE)Saturation((ptr1[i][j].r+ptr1[i][j].g+ptr1[i][j].b)/3);
			ptr1[i][j].r = ptr1[i][j].g = ptr1[i][j].b = gray;
		}
	}

	//화면 출력을 위한 과정
	
	SetRGBptr(pData,ptr1,width,height);

	for(int i = 0 ; i < height ; i++ )
	{
		delete [] ptr1[i];
	}
	delete [] ptr1;	
		
    Invalidate();
}


void CImageProToolView::OnHistogram()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i, j;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];

	for (i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
	}

	Seperate_RGB(pData, ptr1);

	int tempR[256], tempG[256], tempB[256], tempgray[256];
	float histoR[256] = { 0.f, }, histoG[256] = { 0.f, }, histoB[256] = { 0.f, }, histogray[256] = { 0.f, };
	memset(tempgray, 0, sizeof(int) * 256);
	memset(tempR, 0, sizeof(int) * 256);
	memset(tempG, 0, sizeof(int) * 256);
	memset(tempB, 0, sizeof(int) * 256);
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			int value = Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);
			tempgray[value]++;
			tempR[ptr1[i][j].r]++;
			tempG[ptr1[i][j].g]++;
			tempB[ptr1[i][j].b]++;
		}

	// 히스토그램 정규화(histogram normalization)

	float area = (float)width*height;
	for (i = 0; i < 256; i++)
	{
		histogray[i] = tempgray[i] / (float)area;
		histoR[i] = (float)tempR[i] / (float)area;
		histoG[i] = (float)tempG[i] / (float)area;
		histoB[i] = (float)tempB[i] / (float)area;
	}


	//Histogram* dlg = new Histogram(); //객체 포인터
	//dlg->SetImage(histogray, histoR, histoG, histoB);
	//dlg->DoModal();

	Histogram dlg; // 객체변수
	dlg.SetImage(histogray, histoR, histoG, histoB);
	dlg.DoModal();
}
int CImageProToolView::Thresholding_Update(int height, int width, int* Y, int *B, int T) {
	int i, count1 = 0, count2 = 0, sum1 = 0, sum2 = 0;
	for (i = 0; i < height; i++) {
		//그룹별 픽셀 총합 계산
		if (B[i] == 255) { sum1 += Y[i]; count1++; }
		else { sum2 += Y[i]; count2++; }
	}
	int ave1=0, ave2=0;
	if (count1)ave1 = sum1 / count1;
	if (count2)ave2 = sum2 / count2;
	// 교수님 준 코드에는 예외처리 안되어 있음, 만약 모두 white || black 으로 들어오면 division error

	int T2 = (ave1 + ave2) / 2;
	return T2;
}

void CImageProToolView::OnGlobalThresholding()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i, j, T1=80, T2 = 0, T0 = 1;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];

	for (i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
	}

	Seperate_RGB(pData, ptr1);

	int *Y = new int[height * width];
	int *Bi = new int[height * width];
	memset(Y, 0, height*width);
	memset(Bi, 0, height*width);

	for( i = 0; i<height ; i++)
		for (j = 0; j < width; j++) {
			//컬러 영상을 그레이 영상으로 변환
			Y[i*width + j] = Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b));

			if (Y[i] > T1) Bi[i] = 255;
			else Bi[i] = 0;
		}

	while (1) {
		int T2 = Thresholding_Update(height, width, Y, Bi, T1);
		
		if (abs(T1 - T2) < T0) {
			for(i=0;i<height;i++)
				for (j = 0; j < width; j++) {
					if (Y[i*width + j] > T2) Bi[i * width + j] = 255;
					else Bi[i*width + j] = 0;
				}
			break;
		}
		else {
			//새로운 임계값으로 다시 이진화 작업
			T1 = T2;

			for (i = 0; i < height; i++)
				for (j = 0; j < width; j++) {
					if (Y[i*width + j] > T2) Bi[i * width + j] = 255;
					else Bi[i*width + j] = 0;
				}
		}
	}	// end while(1)
	
	for( i =0; i<height; i++)
		for (j = 0; j < width; j++) {
			ptr1[i][j].r = Bi[i*width + j];
			ptr1[i][j].g = Bi[i*width + j];
			ptr1[i][j].b = Bi[i*width + j];
		}
	SetRGBptr(pData, ptr1, width, height);

	for (i = 0; i < height; i++) {
		delete[] ptr1[i];
	}
	delete[] ptr1;
	delete[]Y;
	delete[]Bi;
	Invalidate();
}


void CImageProToolView::OnHistogramStretching()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i, j;
	int **image = new int*[height];
	for (i = 0; i < height; i++) image[i] = new int[width];

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];

	for (i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
	}

	Seperate_RGB(pData, ptr1);

	// 컬러 영상을 그레이 영상으로 변환
	for (i = 0; i<height; i++)
		for (j = 0; j < width; j++) {
			//컬러 영상을 그레이 영상으로 변환
			image[i][j] = Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b)/3);

		}
	int max, min, Nmax, Nmin;
	Nmax = 255, Nmin = 0;
	int histogram[256] = { 0 };

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			histogram[image[i][j]]++;

	int count = 0;
	for (i = 0; i < 256; i++) {
		count += histogram[i];
		if (count > 1) {
			min = i;
			break;
		}
	}
	count = 0;
	for (i = 255; i >= 0; i--) {
		count += histogram[i];
		if (count > 1) {
			max = i;
			break;
		}
	}

	for (i = 0; i < height; i++)
		for(j=0; j <width; j++){
			image[i][j] = (int)((float)(image[i][j] - min) * ((float)(Nmax - Nmin) / (float)(max - min)) + Nmin);
			ptr1[i][j].r = image[i][j];
			ptr1[i][j].g = image[i][j];
			ptr1[i][j].b = image[i][j];
		}
	SetRGBptr(pData, ptr1,width, height);
	
	for (i = 0; i < height; i++) {
		delete[] ptr1[i];
		delete[] image[i];
	}
	delete[] ptr1;
	delete[] image;

	Invalidate();
}


void CImageProToolView::OnHistogramEqual()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i, j,index;
	int **image = new int*[height];
	for (i = 0; i < height; i++) image[i] = new int[width];

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];

	for (i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
	}

	Seperate_RGB(pData, ptr1);

	for(i=0;i<height;i++)
		for (j = 0; j < width; j++) {
			image[i][j] = Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);
		}
	float cumulation = 0.0;
	int histogram[256] = { 0 };
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			histogram[image[i][j]]++;
	for (index = 0; index < 256; index++) {
		cumulation += (float)histogram[index];

		histogram[index] = (int)((cumulation / (float)(width*height))*255.0 + 0.5);
	}

	for(i=0;i<height;i++)
		for (j = 0; j < width; j++) {
			index = image[i][j];
			image[i][j] = histogram[index];
		}
	for(i=0;i<height;i++)
		for (j = 0; j < width; j++) {
			ptr1[i][j].r = image[i][j];
			ptr1[i][j].g = image[i][j];
			ptr1[i][j].b = image[i][j];
		}

	SetRGBptr(pData, ptr1, width, height);

	for (i = 0; i < height; i++) {
		delete[] ptr1[i];
		delete[] image[i];
	}
	delete[] ptr1;
	delete[] image;

	Invalidate();
}


void CImageProToolView::OnMeanfilter()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i, j, index,m,n;
	int **image = new int*[height];
	for (i = 0; i < height; i++) image[i] = new int[width];

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];

	for (i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
	}

	Seperate_RGB(pData, ptr1);

	for (i = 0; i<height; i++)
		for (j = 0; j < width; j++) {
			image[i][j] = Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);
		}
	float mean = 0;
	float mean3x3[3][3];
	for (i = 0; i < 3; i++) for (j = 0; j < 3; j++)  mean3x3[i][j] = 1.0f / 9.0f;
// 5x5 filter
//	float mean5x5[5][5] = { 1.0f / 25.0f };
//	for (i = 0; i < 5; i++) for (j = 0; j < 5; j++)  mean5x5[i][j] = 1.0f / 25.0f;

	int margin;

	// 3x3 filter
	margin = sqrt((float)(sizeof(mean3x3) / sizeof(mean3x3[0][0])) ) / 2;
	
	// 5x5 filter
	//margin = sqrt((float)(sizeof(mean5x5) / sizeof(mean5x5[0][0]))) / 2;

	for(i=margin;i<height-margin;i++)
		for (j = margin; j < width - margin; j++) {
			mean = 0.0f;

			for (m = -margin; m <= margin; m++)		// 중심에서 부터
				for (n = -margin; n <= margin; n++)
					mean += (int)image[i + m][j + n] * mean3x3[m + margin][n + margin];
			image[i][j] = (int)Saturation(mean);
		}

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++) { // 굳이 out_image 재할당 할 필요 없으므로 놔둠
			ptr1[i][j].r = image[i][j];
			ptr1[i][j].g = image[i][j];
			ptr1[i][j].b = image[i][j];
		}
	SetRGBptr(pData, ptr1, width, height);

	for (i = 0; i < height; i++) {
		delete[] ptr1[i];
		delete[] image[i];
	}
	delete[] ptr1;
	delete[] image;

	Invalidate();
}



void CImageProToolView::OnMedianfilter()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i, j, index, m, n;
	int **image = new int*[height];
	int **out_image = new int*[height];
	for (i = 0; i < height; i++) {
		image[i] = new int[width];
		out_image[i] = new int[width];
	}

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];

	for (i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
	}

	Seperate_RGB(pData, ptr1);

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++) {
			image[i][j] = Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);
		}
	int median;
	const int NUM = 9;
	int* pixel = new int[NUM];
	int pidx;

	for(i=1;i<height-1;i++)
		for (j = 1; j < width - 1; j++) {
			pidx = 0;
			for (m = -1; m <= 1; m++)
				for (n = -1; n <= 1; n++)
					pixel[pidx++] = (int)image[i + m][j + n];

			int subi, subj, tmp;

			for(subi=0; subi<NUM-1;subi++)
				for(subj = subi +1; subj < NUM; subj++)
					if (pixel[subi] > pixel[subj])
					{
						tmp = pixel[subi];
						pixel[subi] = pixel[subj];
						pixel[subj] = tmp;
					}
			if (NUM % 2 != 0)
				median = pixel[NUM / 2];
			else
				median = (pixel[NUM / 2 - 1] + pixel[NUM / 2]) / 2;

			out_image[i][j] = (int)Saturation(median);
		}
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++) {
			ptr1[i][j].r = out_image[i][j];
			ptr1[i][j].g = out_image[i][j];
			ptr1[i][j].b = out_image[i][j];
		}
	SetRGBptr(pData, ptr1, width, height);

	for (i = 0; i < height; i++) {
		delete[] ptr1[i];
		delete[] image[i];
		delete[] out_image[i];
	}
	delete[] ptr1;
	delete[] image;
	delete[] out_image;

	Invalidate();
}
