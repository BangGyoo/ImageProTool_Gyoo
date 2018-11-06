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
	ON_COMMAND(ID_CHAP_SOBEL, &CImageProToolView::OnSobel)
	ON_COMMAND(ID_CHAP_NEARESTSCALING, &CImageProToolView::OnChapNearestscaling)
	ON_COMMAND(ID_CHAP_ROTATION, &CImageProToolView::OnRotation)
	ON_COMMAND(ID_CHAP_BINARYDILATION, &CImageProToolView::OnBinaryDilation)
	ON_COMMAND(ID_CHAP_TRANSLATION, &CImageProToolView::OnTranslation)
	ON_COMMAND(ID_CHAP_BINARYEROSION, &CImageProToolView::OnBinaryErosion)
	ON_COMMAND(ID_CHAP_CONNECTEDLABELING, &CImageProToolView::OnConnectedLabeling)
	ON_COMMAND(ID_CHAP_INVARIANTMOMENT, &CImageProToolView::OnInvariantMoment)
	ON_COMMAND(ID_CHAP_KMEANCLUSTERING, &CImageProToolView::OnKmeanClustering)
	ON_COMMAND(ID_HOMEWORK_SEARCHSHARK, &CImageProToolView::OnSearchshark)
	ON_COMMAND(ID_HOMEWORK_NAIVEBAYES, &CImageProToolView::OnNaivebayes)
	ON_COMMAND(ID_HOMEWORK_PARSEBMPTOTXT, &CImageProToolView::OnParseBMPtoTXT)
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
	float percentage = 5;
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
			image[i][j] = Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);
		}
	int max, min, Nmax, Nmin;
	Nmax = 255, Nmin = 0;
	int histogram[256] = { 0 };

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			histogram[image[i][j]]++;


	int count = 0;
	float total = (float)(width * height);
	float per = (percentage * total / 100.0);

	for (i = 0; i < 256; i++) {
		count += histogram[i];
		if (per <= count) {
			min = i;
			break;
		}
	}
	count = 0;
	for (i = 255; i >= 0; i--) {
		count += histogram[i];
		if (per <= count) {
			max = i;
			break;
		}
	}


	for (i = 0; i < height; i++)
		for (j = 0; j <width; j++) {
			image[i][j] = (int)((float)(image[i][j] - min) * ((float)(Nmax - Nmin) / (float)(max - min)) + Nmin);
			image[i][j] = Saturation(image[i][j]);
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


void CImageProToolView::OnSobel()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i, j, m, n;
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
	int g1, g2, gradient;
	g1 = g2 = gradient = 0;

	int gx[3][3] = { -1 , 0 , 1 ,
					-2 , 0 , 2,
					-1 , 0 , 1 };
	int gy[3][3] = { 1, 2, 1,
					0, 0, 0,
					-1, -2, -1 };
	for(i=1;i<height -1 ;i++)
		for (j = 1; j < width - 1; j++) {
			for (m = -1; m <= 1; m++)
				for (n = -1; n <= 1; n++) {
					g1 += (int)image[i + m][j + n] * gx[m + 1][n + 1];
					g2 += (int)image[i + m][j + n] * gy[m + 1][n + 1];
				}
			gradient = Saturation(abs(g1) + abs(g2));
			out_image[i][j] = (unsigned char)gradient;
			g1 = g2 = 0;
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


void CImageProToolView::OnChapNearestscaling()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int x,y,i, j;
	int **image = new int*[height];
	int **out_image = new int*[height];


	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];

	for (i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
	}

	Seperate_RGB(pData, ptr1);

	double scale = 0.5;
	int new_height = (int)(height *scale);
	int new_width = (int)(width*scale);
	RGBptr** ptr2;
	if (scale > 1) {

		ptr2 = new RGBptr*[new_height];
		for (i = 0; i < new_height; i++) ptr2[i] = new RGBptr[new_width];

		for (i = 0; i < new_height; i++) {
			for (j = 0; j < new_width; j++) {
				x = (int)width * j / new_width;
				y = (int)height * i / new_height;

				ptr2[i][j] = ptr1[y][x];

			}
		}
		
	}
	else {

		ptr2 = new RGBptr*[height];
		for (i = 0; i < height; i++) ptr2[i] = new RGBptr[width];

		for (i = 0; i < height; i++) 
			for (j = 0; j < width; j++) {
				ptr2[i][j].r = 0;
				ptr2[i][j].g = 0;
				ptr2[i][j].b = 0;
			}
		for(i=0; i <new_height;i++)
			for (j = 0; j < new_width; j++) {
				x = (int)width * j / new_width;
				y = (int)height * i / new_height;

				ptr2[i][j] = ptr1[y][x];
			}

	}
	//SetRGBptr(pData, ptr2, new_width, new_height);
		// 윈도우 크기 변경함수를 못찾겠다.
	SetRGBptr(pData, ptr2, width, height);

	for (i = 0; i < height; i++) delete[] ptr1[i];
	for (i = 0; i < new_height; i++) delete[] ptr2[i];

	delete[] ptr1;
	delete[] ptr2;
	
	Invalidate();
}


void CImageProToolView::OnRotation()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();

	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i, j;
	int **image = new int*[height];
	
	for (i = 0; i < height; i++) {
		image[i] = new int[width];
	}

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	RGBptr** ptr2 = new RGBptr*[height];
	
	for (i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		ptr2[i] = new RGBptr[width];
	}

	Seperate_RGB(pData, ptr1);

	int center_x, center_y;
	center_x = width / 2;
	center_y = height / 2;

#ifndef M_PI
#define M_PI 3.14159265
#endif
	double angle = M_PI / 180 * 90;

	double x1, y1, x2, y2, a, b;
	int x, y;

	for(i=0;i<height;i++)
		for (j = 0; j < width; j++) {
			x1 = (j - center_x)*cos(angle);
			y1 = -1 * ((i - center_y)*sin(angle));
			x = (int)(x1 + y1 + center_x);

			x2 = ((j - center_x)*sin(angle));
			y2 = (i - center_y)*cos(angle);
			y = (int)(x2 + y2 + center_y);

			if (y < 0 || y >= (height - 1) || x < 0 || x >= (width - 1))
				ptr2[i][j].r = ptr2[i][j].g = ptr2[i][j].b = 0;
			else {
				ptr2[i][j].r = (double)ptr1[y][x].r;
				ptr2[i][j].g = (double)ptr1[y][x].g;
				ptr2[i][j].b = (double)ptr1[y][x].b;
			}
		}
	SetRGBptr(pData, ptr2, width, height);

	for (i = 0; i < height; i++) delete[] ptr1[i];
	for (i = 0; i < height; i++) delete[] ptr2[i];

	delete[] ptr1;
	delete[] ptr2;

	Invalidate();
}




void CImageProToolView::OnBinaryDilation()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();

	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i, j;
	int **image = new int*[height];
	int **tmp_image = new int*[height];

	for (i = 0; i < height; i++) {
		image[i] = new int[width];
		tmp_image[i] = new int[width];
	}

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];

	for (i = 0; i < height; i++)
		ptr1[i] = new RGBptr[width];


	Seperate_RGB(pData, ptr1);

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			image[i][j] = (int)Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);

	for(i=0;i<height;i++)
		for (j = 0; j < width; j++) {
			/*if (image[i][j] > 150)
				tmp_image[i][j] = 255;*/
			if (ptr1[i][j].r > 190) tmp_image[i][j] = 255;
			else
				tmp_image[i][j] = 0;
		}
	for (i = 1; i < height - 1; i++) {
		for (j = 1; j < width - 1; j++) {
			if (tmp_image[i - 1][j - 1] == 0 && tmp_image[i - 1][j] == 0 &&
				tmp_image[i - 1][j + 1] == 0 && tmp_image[i][j - 1] == 0 &&
				tmp_image[i][j + 1] == 0 && tmp_image[i + 1][j - 1] == 0 &&
				tmp_image[i + 1][j] == 0 && tmp_image[i + 1][j + 1] == 0)
				image[i][j] = 0;
			else image[i][j] = 255;

		}

	}
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++) {
			ptr1[i][j].r = image[i][j];
			ptr1[i][j].g = image[i][j];
			ptr1[i][j].b = image[i][j];
		}

	SetRGBptr(pData, ptr1, width, height);

	for (i = 0; i < height; i++) {
		delete[] ptr1[i];
		delete[] image[i];
		delete[] tmp_image[i];
	}


	delete[] ptr1;
	delete[] image;
	delete[] tmp_image;

	Invalidate();
}


void CImageProToolView::OnTranslation() {
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();

	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i, j,x,y;
	int **image = new int*[height];

	for (i = 0; i < height; i++) {
		image[i] = new int[width];
	}

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	RGBptr** ptr2 = new RGBptr*[height];

	for (i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		ptr2[i] = new RGBptr[width];
	}

	Seperate_RGB(pData, ptr1);

	int b1, b2;
	b1 = b2 = 10;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			y = i + b1;
			x = j + b2;

			if (y < height && x < width) ptr2[i][j] = ptr1[y][x];
		}
		

	}
	SetRGBptr(pData, ptr2, width, height);
	

	for (i = 0; i < height; i++) delete[] ptr1[i];
	for (i = 0; i < height; i++) delete[] ptr2[i];

	delete[] ptr1;
	delete[] ptr2;

	Invalidate();
}



void CImageProToolView::OnBinaryErosion() {
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();

	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i, j;
	int **image = new int*[height];
	int **tmp_image = new int*[height];

	for (i = 0; i < height; i++) {
		image[i] = new int[width];
		tmp_image[i] = new int[width];
	}

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];

	for (i = 0; i < height; i++)
		ptr1[i] = new RGBptr[width];


	Seperate_RGB(pData, ptr1);

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			image[i][j] = (int)Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);

	for (i = 0; i<height; i++)
		for (j = 0; j < width; j++) {
			/*if (image[i][j] > 150)
			tmp_image[i][j] = 255;*/
			if (ptr1[i][j].r > 190) tmp_image[i][j] = 255;
			else
				tmp_image[i][j] = 0;
		}
	for (i = 1; i < height - 1; i++) {
		for (j = 1; j < width - 1; j++) {
			if (tmp_image[i - 1][j - 1] == 255 && tmp_image[i - 1][j] == 255 &&
				tmp_image[i - 1][j + 1] == 255 && tmp_image[i][j - 1] == 255 &&
				tmp_image[i][j + 1] == 255 && tmp_image[i + 1][j - 1] == 255 &&
				tmp_image[i + 1][j] == 255 && tmp_image[i + 1][j + 1] == 255)
				image[i][j] = 255;
			else image[i][j] = 0;

		}

	}
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++) {
			ptr1[i][j].r = image[i][j];
			ptr1[i][j].g = image[i][j];
			ptr1[i][j].b = image[i][j];
		}

	SetRGBptr(pData, ptr1, width, height);

	for (i = 0; i < height; i++) {
		delete[] ptr1[i];
		delete[] image[i];
		delete[] tmp_image[i];
	}


	delete[] ptr1;
	delete[] image;
	delete[] tmp_image;

	Invalidate();
}


void CImageProToolView::OnConnectedLabeling()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();

	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i, j;
	int **image = new int*[height];

	for (i = 0; i < height; i++) {
		image[i] = new int[width];
	}

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];

	for (i = 0; i < height; i++)
		ptr1[i] = new RGBptr[width];


	Seperate_RGB(pData, ptr1);

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			image[i][j] = (int)Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);

	int minRegionCount = 10;
	int **Label = new int*[height];
	for (i = 0; i < height; i++) {
		Label[i] = new int[width];
		memset(Label[i], 0, sizeof(int)*width);
	}
	int num, left, top, k;
	int *r, *area;
	r = new int[height * width];
	area = new int[width*height];
	memset(r, 0, sizeof(int)*height*width);
	memset(area, 0, sizeof(int)*height*width);

	for (i = 0; i<height; i++)
		for (j = 0; j<width; j++) {
			if (image[i][j] < 200) { Label[i][j] = 0; }// 레이블링 잘해야 나온다.
			else { Label[i][j] = -1; }
		}

	for (j = 0; j < width; j++) {
		Label[0][j] = -1;
		Label[height - 1][j] = -1;
	}
	for (i = 0; i < height; i++) {
		Label[i][0] = -1;
		Label[i][width - 1] = -1;

	}

	num = 0;
	for (i = 1; i < height; i++)
		for (j = 1; j < width; j++) {
			if (Label[i][j] >= 0) {
				left = Label[i][j - 1];
				top = Label[i - 1][j];
				if (left == -1 && top != -1) {
					Label[i][j] = r[top];
				}
				else if (left != -1 && top == -1) {
					Label[i][j] = r[left];
				}
				else if (left == -1 && top == -1) {
					num++;
					r[num] = num;
					Label[i][j] = r[num];
				}
				else if (left != -1 && top != -1) {
					if (r[left] == r[top])
						Label[i][j] = r[left];
					else if (r[left] > r[top]) {
						Label[i][j] = r[top];
						r[left] = r[top];
					}
					else {
						Label[i][j] = r[left];
						r[top] = r[left];
					}
				}

			}
		}
	for (k = 0; k < num; k++) {
		if (k != r[k]) r[k] = r[r[k]];
		area[k] = 0;
	}

	for (i = 0; i<height; i++)
		for (j = 0; j < width; j++) {
			if (Label[i][j] > -1) {
				Label[i][j] = r[Label[i][j]];

				area[Label[i][j]]++;
			}
		}
	int cnt = 0;
	for (k = 0; k < num; k++) {
		if (area[k] >= minRegionCount) {
			r[k] = cnt++;
		}
		else r[k] = -1;
	}
	for (i = 0; i<height; i++)
		for (j = 0; j < width; j++) {
			if (Label[i][j] >= 0)
				Label[i][j] = r[Label[i][j]];
		}

	for (i = 0; i < height; i++) {
		delete[] ptr1[i];
		delete[] image[i];

	}

	delete[] ptr1;
	delete[] image;

	CString str;
	str.Format(_T("레이블 개수 = %d"), cnt);


	AfxMessageBox(str);

	Invalidate();
}


	void CImageProToolView::OnInvariantMoment()
	{
		CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
		ASSERT_VALID(pDoc);

		int width = pDoc->m_Width;
		int height = pDoc->m_Height;

		BYTE* pData = pDoc->m_pDib->GetBitsAddress();

		RGBptr** ptr1 = new RGBptr*[height];
		BYTE** image = new BYTE*[height];

		for (int i = 0; i < height; i++)
		{
			ptr1[i] = new RGBptr[width];
			image[i] = new BYTE[width];
		}

		Seperate_RGB(pData, ptr1);

		int i, j;

		for (i = 0; i < height; i++){
		 for (j = 0; j < width; j++) {
		  image[i][j] =
		   (BYTE)Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);
		 }
			 }

			 //임의의 임계값으로 이진화 (전역 임계값을 적용해도 됨)
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				if (image[i][j] < 50) { image[i][j] = 0; }
				else { image[i][j] = 255; }
			}
		}

		double mu00, mu11, mu20, mu02, mu30, mu03, mu21, mu12;
		double eta20, eta02, eta11, eta30, eta03, eta21, eta12;
		double *phi = new double[7];
		double count = 0., CenX = 0., CenY = 0.;

		for (i = 0; i < height; i++){
		 for (j = 0; j < width; j++) {
		  if (image[i][j] == 0)
		  {
		   count++;
		   CenX += j;
		   CenY += i;
		  }
		 }
			 }
			 //영역의 무게 중심 구하기
		CenX /= count;
			CenY /= count;

		//중심 모멘트 구하기
		mu00 = mu11 = mu20 = mu02 = mu30 = mu03 = mu21 = mu12 = 0;
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				if (image[i][j] == 0) //0보다 큰 영역에서만 적용 (레이블된 경우라면?)
				{
					mu00 += pow((double)(j - CenX), 0)*pow((double)(i - CenY), 0.);
					mu20 += pow((double)(j - CenX), 2.)*pow((double)(i - CenY), 0.);
					mu02 += pow((double)(j - CenX), 0.)*pow((double)(i - CenY), 2.);
					mu11 += pow((double)(j - CenX), 1.)*pow((double)(i - CenY), 1.);
					mu30 += pow((double)(j - CenX), 3.)*pow((double)(i - CenY), 0.);
					mu03 += pow((double)(j - CenX), 0.)*pow((double)(i - CenY), 3.);
					mu21 += pow((double)(j - CenX), 2.)*pow((double)(i - CenY), 1.);
					mu12 += pow((double)(j - CenX), 1.)*pow((double)(i - CenY), 2.);
				}
			}
		}

		//중심 모멘트 정규화 하기
		eta20 = mu20 / pow(mu00, (2. + 0.) / 2. + 1.);
		eta02 = mu02 / pow(mu00, (0. + 2.) / 2. + 1.);
		eta11 = mu11 / pow(mu00, (1. + 1.) / 2. + 1.);
		eta30 = mu30 / pow(mu00, (3. + 0.) / 2. + 1.);
		eta03 = mu03 / pow(mu00, (0. + 3.) / 2. + 1.);
		eta21 = mu21 / pow(mu00, (2. + 1.) / 2. + 1.);
		eta12 = mu12 / pow(mu00, (1. + 2.) / 2. + 1.);

		//불변 모멘트 구하기
		phi[0] = eta20 + eta02;
		phi[1] = (eta20 - eta02)*(eta20 - eta02) + 4 * eta11*eta11;
		phi[2] = (eta30 - 3 * eta12)*(eta30 - 3 * eta12) + (3 * eta21 - eta03)*(3 * eta21 - eta03);
		phi[3] = (eta30 + eta12)*(eta30 + eta12) + (eta21 + eta03)*(eta21 + eta03);
		phi[4] = (eta30 - 3 * eta12)*(eta30 + eta12)*((eta30 + eta12)*(eta30 + eta12) - 3 * (eta21 + eta03)*(eta21 + eta03))
			+ (3 * eta21 - eta03)*(eta21 + eta03)*(3 * (eta30 + eta12)*(eta30 + eta12) - (eta21 + eta03)*(eta21 + eta03));
		phi[5] = (eta20 - eta02)*((eta30 + eta12)*(eta30 + eta12) - (eta21 + eta03)*(eta21 + eta03))
			+ 4 * eta11*(eta30 + eta12)*(eta21 + eta03);
		phi[6] = (3 * eta21 - eta03)*(eta30 + eta12)*((eta30 + eta12)*(eta30 + eta12) - 3 * (eta21 + eta03)*(eta21 + eta03))
			+ (3 * eta12 - eta30)*(eta21 + eta03)*(3 * (eta30 + eta12)*(eta30 + eta12) - (eta21 + eta03)*(eta21 + eta03));

		CString str = _T("Invariant moments:n\n");
		for (int i = 0; i < 7; i++)
		{
			str.AppendFormat(_T("m[%d] = %10.10lf\n"), i, phi[i] * 1000);
		}

		AfxMessageBox(str);

		for (i = 0; i < height; i++){
		 delete[] ptr1[i];
		 delete[] image[i];
			}
		delete[] ptr1;
			delete[] image;
			delete[] phi;
			Invalidate();
}

void K_Mean_Clustering(int **image, int *ME, int K, int T, int width, int height) {
	float *C_mean = new float[K];			// cluster 당 1개의 평균
	float *distance = new float[K];			// 평균과의 차를 구하기 위한 distance
	int *Count = new int[K];
	int *Cluster = new int[K];
	int i, j, k, s;
	int **label = new int*[height];

	for (i = 0; i < height; i++) {				// 레이블 0으로 초기화
		label[i] = new int[width];
		memset(label[i], 0, sizeof(int)*width);
	}
	float min, temp_mean, Sum_mean;
	for (k = 0; k < K; k++) {
		C_mean[k] = distance[k] = 0.0f;
		Count[k] = Cluster[k] = 0;
	}
	Sum_mean = min = temp_mean = 0.0f;
	for (k = 0; k < K; k++) C_mean[k] = (float)ME[k];

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			for (k = 0; k < K; k++) distance[k] = (float)fabs(C_mean[k] - (float)image[i][j]);
			min = distance[0];
			s = 0;					//s는 최소 distance의 군집 넘버를 세기위한 값

			for (k = 1; k < K; k++) {
				if (distance[k] <= min) {
					min = distance[k];
					s = k;
				}
				Cluster[s] += image[i][j];
				label[i][j] = s;
				Count[s]++;
			}
		}
	}
	for (k = 0; k < K; k++) {
		temp_mean = C_mean[k];
		if (Count[k] == 0) C_mean[k] = 0.0;
		else {
			C_mean[k] = (float)(Cluster[k] / Count[k]);
		}
		Sum_mean += (float)fabs(C_mean[k] - temp_mean);
		temp_mean = 0.0;

	}

	if (Sum_mean <= T) {
		for (k = 0; k < K; k++) {
			for (i = 0; i < height; i++) {
				for (j = 0; j < width; j++) {
					if (label[i][j] == k) image[i][j] = (int)C_mean[k];
				}
			}
		}
		return;
	}
	else {
		Sum_mean = 0.0;
		for (k = 0; k < K; k++) ME[k] = (int)C_mean[k];
		K_Mean_Clustering(image, ME, K, T, width, height);
	}
	for (i = 0; i < height; i++) {
		delete[] label[i];
	}
	delete[] label;
	delete[] C_mean;
	delete[] distance;
	delete[] Count;
	delete[] Cluster;
}

void CImageProToolView::OnKmeanClustering()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();

	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i, j;
	int **image = new int*[height];

	for (i = 0; i < height; i++) {
		image[i] = new int[width];
	}

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];

	for (i = 0; i < height; i++)
		ptr1[i] = new RGBptr[width];


	Seperate_RGB(pData, ptr1);

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			image[i][j] = (int)Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);

	const int K = 3, T = 1;
	int*ME = new int[K];
	srand((unsigned)time(NULL));
	for (i = 0; i < K; i++) ME[i] = rand() % 255;
	K_Mean_Clustering(image, ME, K, T, width, height);

	for (i = 0; i < height; i++)
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


void CImageProToolView::shapeX(RGBptr ** pixels, int x, int y)	// 해당 label 확인용 출력 코드
{
	pixels[y-1][x-1].r = 255; pixels[y-1][x-1].g = 0; pixels[y-1][x-1].b = 0;
	pixels[y-1][x].r = 255; pixels[y - 1][x].g = 0; pixels[y - 1][x].b = 0;
	pixels[y-1][x+1].r = 255; pixels[y - 1][x + 1].g = 0; pixels[y - 1][x + 1].b = 0;
	pixels[y][x-1].r = 255; pixels[y][x - 1].g = 0; pixels[y][x - 1].b = 0;
	pixels[y][x].r = 255; pixels[y][x].g = 0; pixels[y][x].b = 0;
	pixels[y][x+1].r = 255; pixels[y][x + 1].g = 0; pixels[y][x + 1].b = 0;
	pixels[y+1][x-1].r = 255; pixels[y + 1][x - 1].g = 0; pixels[y + 1][x - 1].b = 0;
	pixels[y+1][x].r = 255; pixels[y + 1][x].g = 0; pixels[y + 1][x].b = 0;
	pixels[y+1][x+1].r = 255; pixels[y + 1][x + 1].g = 0; pixels[y + 1][x + 1].b = 0;

}

void CImageProToolView::OnSearchshark()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image = new BYTE*[height];
	int i, j;

	for (i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image[i] = new BYTE[width];
	}

	Seperate_RGB(pData, ptr1);

	for (i = 0; i<height;i++)
		for (j = 0; j<width; j++)		// 영상은 흑백이지만 이진화 안되 있을수도 있다.
			image[i][j] = (BYTE)Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);

		//minimum number of pixels in region that to be removed 
		int minRegionCount = 10;

		int** Label = new int*[height];
		for (i = 0; i < height; i++)
		{
			Label[i] = new int[width];
				memset(Label[i], 0, sizeof(int)*width);
		}

	int num, left, top, k;
	int *r, *area;
	r = new int[height*width];
	area = new int[width*height];
	memset(r, 0, sizeof(int)*height*width);
	memset(area, 0, sizeof(int)*height*width);

	//threholding to make binary image
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			if (image[i][j] < 50) { Label[i][j] = 1; }		// 레이블링을 위한 영상
			else { Label[i][j] = -1; }

			//do not label of boundary of image
			for (j = 0; j < width; j++) {
				Label[0][j] = -1;
				Label[height - 1][j] = -1;
			}
			for (i = 0; i < height; i++) {
				Label[i][0] = -1;
				Label[i][width - 1] = -1;
			}

			num = 0; //initial lable number
			for (i = 0; i < height; i++) {
				for (j = 0; j < width; j++) {
					if (i > 0 && j > 0) {
						// scan and if the pixel value is over 0
						if (Label[i][j] >= 1) {
							left = Label[i][j - 1]; //Examine Left and 
							top = Label[i - 1][j];  //Examine top lable
							if (left == -1 && top != -1) {  //if only top label
															//lable is assigned as the top lable
								Label[i][j] = r[top];
							}
							//if only left label
							else if (left != -1 && top == -1) {
								//lable is assigned as the left lable
								Label[i][j] = r[left];
							}
							//if no lable left and top
							else if (left == -1 && top == -1) {
								num++;             //increase label number +1
												   //save label number to equivalent table
								r[num] = num;
								//assign the label value to label image       
								Label[i][j] = r[num];
							}
							//In case left and top have values sametime  
							//1) if labels are same, assign as the left value
							//2) otherwise assign as the smaller value
							// 변경
							else if (left != -1 && top != -1) {
								if (r[left] == r[top]) {
									Label[i][j] = r[left];
								}
								else if (r[left] > r[top]) {
									Label[i][j] = r[top];
									// also lable is changed
									r[left] = r[top];
								}
								else {
									Label[i][j] = r[left];
									r[top] = r[left];
								}
							}
						}
					}
				}
			}
			for (k = 1; k <= num; k++) { //label is re-arranged
										 //if k and its label number is not same, new label is assigned 
				if (k != r[k]) r[k] = r[r[k]];
				area[k] = 0;
			}

			for (i = 0; i < height; i++)
				for (j = 0; j < width; j++) {
					if (Label[i][j] > 0) {
						//new label image is assigned as new labels 
						Label[i][j] = r[Label[i][j]];
						// sum of all numbers of each label ==> to remove small label regions
						area[Label[i][j]]++;
					}
				}
			int cnt = 1;
			for (k = 1; k <= num; k++) {
				//if the number of region's label is below threshold
				//remove it and re-assign the label number
				if (area[k] >= minRegionCount)
				{
					r[k] = cnt++;
				}
				else r[k] = -1;
			}
			cnt--;

			for (i = 0; i < height; i++)
				for (j = 0; j < width; j++) {
					if (Label[i][j] > 0)
						//re-assign the label number
						Label[i][j] = r[Label[i][j]];
				}
	CString str;
	double target[7] = { 532.424342, 244.064340, 13.141272, 10.845762, 0.129482, 5.255920, 0.000087,
	};	// 해당 query의 모멘트 정보
	bool *result = new bool[cnt+1];
	for (int i = 0; i < cnt+1; i++) result[i] = false;
	double mu00, mu11, mu20, mu02, mu30, mu03, mu21, mu12;
	double eta20, eta02, eta11, eta30, eta03, eta21, eta12;
	double **phi = new double*[cnt+1];
	for(int i=0; i <cnt+1; i++) phi[i] = new double[7];


	double count = 0., CenX = 0., CenY = 0.;
	
	int loop;
	//FILE* fp;
	//fp = fopen("test.txt", "w");

	for(loop=1; loop<=cnt; loop++){		// 레이블링 갯수만큼 loop 0은 x
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				if (Label[i][j] == loop)
				{
					count++;
					CenX += j;
					CenY += i;
				}
			}
		}
		//영역의 무게 중심 구하기
		CenX /= count;

		CenY /= count;

		//중심 모멘트 구하기
		mu00 = mu11 = mu20 = mu02 = mu30 = mu03 = mu21 = mu12 = 0;
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				if (Label[i][j] == loop) 
				{
					mu00 += pow((double)(j - CenX), 0)*pow((double)(i - CenY), 0.);
					mu20 += pow((double)(j - CenX), 2.)*pow((double)(i - CenY), 0.);
					mu02 += pow((double)(j - CenX), 0.)*pow((double)(i - CenY), 2.);
					mu11 += pow((double)(j - CenX), 1.)*pow((double)(i - CenY), 1.);
					mu30 += pow((double)(j - CenX), 3.)*pow((double)(i - CenY), 0.);
					mu03 += pow((double)(j - CenX), 0.)*pow((double)(i - CenY), 3.);
					mu21 += pow((double)(j - CenX), 2.)*pow((double)(i - CenY), 1.);
					mu12 += pow((double)(j - CenX), 1.)*pow((double)(i - CenY), 2.);
				}
			}
		}

		//중심 모멘트 정규화 하기
		eta20 = mu20 / pow(mu00, (2. + 0.) / 2. + 1.);
		eta02 = mu02 / pow(mu00, (0. + 2.) / 2. + 1.);
		eta11 = mu11 / pow(mu00, (1. + 1.) / 2. + 1.);
		eta30 = mu30 / pow(mu00, (3. + 0.) / 2. + 1.);
		eta03 = mu03 / pow(mu00, (0. + 3.) / 2. + 1.);
		eta21 = mu21 / pow(mu00, (2. + 1.) / 2. + 1.);
		eta12 = mu12 / pow(mu00, (1. + 2.) / 2. + 1.);

		//불변 모멘트 구하기
		phi[loop][0] = eta20 + eta02;
		phi[loop][1] = (eta20 - eta02)*(eta20 - eta02) + 4 * eta11*eta11;
		phi[loop][2] = (eta30 - 3 * eta12)*(eta30 - 3 * eta12) + (3 * eta21 - eta03)*(3 * eta21 - eta03);
		phi[loop][3] = (eta30 + eta12)*(eta30 + eta12) + (eta21 + eta03)*(eta21 + eta03);
		phi[loop][4] = (eta30 - 3 * eta12)*(eta30 + eta12)*((eta30 + eta12)*(eta30 + eta12) - 3 * (eta21 + eta03)*(eta21 + eta03))
			+ (3 * eta21 - eta03)*(eta21 + eta03)*(3 * (eta30 + eta12)*(eta30 + eta12) - (eta21 + eta03)*(eta21 + eta03));
		phi[loop][5] = (eta20 - eta02)*((eta30 + eta12)*(eta30 + eta12) - (eta21 + eta03)*(eta21 + eta03))
			+ 4 * eta11*(eta30 + eta12)*(eta21 + eta03);
		phi[loop][6] = (3 * eta21 - eta03)*(eta30 + eta12)*((eta30 + eta12)*(eta30 + eta12) - 3 * (eta21 + eta03)*(eta21 + eta03))
			+ (3 * eta12 - eta30)*(eta21 + eta03)*(3 * (eta30 + eta12)*(eta30 + eta12) - (eta21 + eta03)*(eta21 + eta03));
	
		
		for (k = 0; k < 7; k++) phi[loop][k] *= 1000;

		
		count = 0;
		CenX = 0;
		CenY = 0;
	}
	
	
	for(loop=1;loop<=cnt;loop++){
		for (i = 0; i < 7; i++) {
			phi[loop][i] -= target[i];
			phi[loop][i] = fabs(phi[loop][i]);
			
		}
	}
	for (loop = 1; loop <= cnt; loop++) {		 // 선형결합, 차후에 정규화를 시켜주거나 오차값을 조정한다면 더욱 깔끔한 샘플링이 가능하다.
		if (phi[loop][0] < 50) 
			if (phi[loop][1] < 50) 
				if (phi[loop][2] < 50) {
					//shapeX(ptr1, CenX, CenY);
					result[loop] = true;
				}
	}

	//fclose(fp);								// 모멘트 값이 필요할 경우 파일입출력으로 받아서 쓰자
	SetRGBptr(pData, ptr1, width, height);
	Invalidate();
	int result_count = 0;
	
	
	for (int i = 1; i <= cnt; i++)
	{
		if (result[i]) {
			result_count++;
			str.AppendFormat(_T("%d\n"),i);
		}
	}
	str.AppendFormat(_T("상어는 총 %d마리 ~ \n"),result_count);
	
	AfxMessageBox(str);

	for (i = 0; i<height; i++) {
		delete[] ptr1[i];
		delete[] image[i];
		
	}
	for(i=0; i<=cnt; i++) delete[] phi[i];
	delete[] ptr1;
	delete[] image;
	delete[] phi;
	delete[] result;
	
}

double Mean(BYTE* array, int size) {
	double sum = 0.0;

	for (int i = 0; i < size; i++)
		sum += array[i];

	return (sum / (float)size);
}

double StandardDeviation(BYTE* array, int size) {
	if (size < 2) return sqrt(-1.0);		// 한개는 에러

	double sum = 0.0;
	double sd = 0.0;
	double diff;
	double meanValue = Mean(array, size);

	for (int i = 0; i < size; i++) {
		diff = array[i] - meanValue;
		sum += diff * diff;
	}
	sd = sqrt(sum / (float)size);

	return sd;
}

// #define zeroFromOne
double GaussianNomalization(BYTE x,BYTE* array, int size, double K = 3) {	
	double deviation = StandardDeviation(array, size);
	double mean = Mean(array, size);
	
	double result;
	result = (x - mean) / (K * deviation);
	
#ifdef zeroFromOne
	return result = ((result + 1) / 2);		// 0에서 1로 하고 싶을 때 사용
#else
	return result;
#endif
}
double GaussianNomalization(BYTE x,double mean, double deviation,double K=3) {	// 픽셀 하나당 하나씩 검출하자.
	return ((x - mean) / (K * deviation));
}

void CImageProToolView::OnParseBMPtoTXT()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;
	int i,j;
	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	for (i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
	}


	Seperate_RGB(pData, ptr1);
	FILE *text = fopen("non.txt", "a");			// face 5번까지만 했다, 찾는 값은 face 6번이 될것 , non.txt는 3번 까지 구성
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++) {
			fprintf(text, "%d %d %d\n", ptr1[i][j].r, ptr1[i][j].g, ptr1[i][j].b);
		}
	fprintf(text, "#\n");			// # 이 나오면 해당 자료 검색 완료
	fclose(text);


	for (i = 0; i < height; i++) {
		delete[] ptr1[i];
	}
	delete[] ptr1;
}

class Sample {
public:
	static enum sampling{
		MAP,
		ML
	};

public :
	Sample() {};
	Sample(FILE *fp) {
		fscanf(fp, "%f / %f / %f / %f / %f / %f / %f",
			this->e1_mean, this->e1_deviation,
			this->e2_mean, this->e2_deviation,
			this->e3_mean, this->e3_deviation,
			this->probability);
	}
	Sample(double e1_mean, double e1_deviation, double e2_mean,
		double e2_deviation, double e3_mean, double e3_deviation,double probability) {
		this->e1_mean = e1_mean;
		this->e1_deviation = e1_deviation;
		this->e2_mean = e2_mean;
		this->e2_deviation = e2_deviation;
		this->e3_mean = e3_mean;
		this->e3_deviation = e3_deviation;
		this->probability = probability;
	};
	void setSample(FILE *fp) {
		fscanf(fp, "%f / %f / %f / %f / %f / %f / %f",
			this->e1_mean, this->e1_deviation,
			this->e2_mean, this->e2_deviation,
			this->e3_mean, this->e3_deviation,
			this->probability);
	}
	double CalcProbability(BYTE r,BYTE g,BYTE b , sampling S) {
		double Pe1 = (1 - fabs(GaussianNomalization(r, e1_mean, e1_deviation)));		// 전체 P(A|B)의 확률이다.
		double Pe2 = (1 - fabs(GaussianNomalization(g, e2_mean, e2_deviation)));
		double Pe3 = (1 - fabs(GaussianNomalization(b, e3_mean, e3_deviation)));
		if (S == MAP) Pe1 * Pe2 * Pe3 * probability;			// MAP
		else return Pe1 * Pe2 * Pe3;							// ML
	}

public:
	double e1_mean;
	double e1_deviation;
	double e2_mean;
	double e2_deviation;
	double e3_mean;
	double e3_deviation;

	double probability;
};

int BMP(BYTE r, BYTE g, BYTE b,Sample *DB,int DBsize) {		// 가중치가 다를때
	int index;
	double argMax = 0;				// 확률분포도 (1 - fabs(gaussian))% 으로 평가예정
	double argTmp;
	for (int i = 0; i < DBsize; i++) {
		argTmp = DB[i].CalcProbability(r, g, b, Sample::MAP);			// Sample::플래그만 바꿔주면 변경 된다.
		if (argMax < argTmp) {
			argMax = argTmp;
			index = i;
		}
	}
	return index;
}
//
//void Solution() {
//	FILE *fp;
//	FILE *np;
//	int faceSampleSize;
//	int nonFaceSampleSize;
//	int faceIndex,nonFaceIndex;
//	char faceFileName[] = "face.txt";
//	char nonFaceFileName[] = "non.txt";
//	
//	fp = fopen(faceFileName, "r");
//	fscanf(fp,"%d", faceSampleSize);
//	Sample *face = new Sample[faceSampleSize];
//	for (int i = 0; i < faceSampleSize; i++) face[i].setSample(fp);
//	fclose(fp);
//
//	np = fopen(nonFaceFileName, "r");
//	fscanf(np, "%d", nonFaceSampleSize);
//	Sample *nonFace = new Sample[nonFaceSampleSize];
//	for (int i = 0; i < nonFaceSampleSize; i++) nonFace[i].setSample(np);			// DB 로드
//	fclose(np);
//
//	for (int i = 0; i < height; i++) {			// 우선 face 형태 확인
//		for (int j = 0; j < width; j++) {
//			faceIndex = BMP(r, g, b, face, faceSampleSize);
//			nonFaceIndex = BMP(r, g, b, nonFace, nonFaceSampleSize);
//			if (face[faceIndex].CalcProbability(r, g, b, Sample::MAP) > nonFace[nonFaceIndex].CalcProbability(r, g, b, Sample::MAP)) {
//				ptr1[i][j].r = 255; ptr1[i][j].g = 255; ptr1[i][j].b = 255;
//			}
//			else {
//				ptr1[i][j].r = 0; ptr1[i][j].g = 0; ptr1[i][j].b = 0;
//			}
//		}
//	}
//
//
//}
//

void CImageProToolView::OnNaivebayes()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];

	int i, j;

	for (i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
	}

	Seperate_RGB(pData, ptr1);

	FILE *fp;
	FILE *np;
	BYTE r, g, b;
	int faceSampleSize;
	int nonFaceSampleSize;
	int faceIndex, nonFaceIndex;
	char faceFileName[] = "face.txt";
	char nonFaceFileName[] = "non.txt";

	fp = fopen(faceFileName, "r");
	fscanf(fp, "%d", faceSampleSize);
	Sample *face = new Sample[faceSampleSize];
	for (int i = 0; i < faceSampleSize; i++) face[i].setSample(fp);
	fclose(fp);

	np = fopen(nonFaceFileName, "r");
	fscanf(np, "%d", nonFaceSampleSize);
	Sample *nonFace = new Sample[nonFaceSampleSize];
	for (int i = 0; i < nonFaceSampleSize; i++) nonFace[i].setSample(np);			// DB 로드
	fclose(np);

	for (int i = 0; i < height; i++) {			// 우선 face 형태 확인
		for (int j = 0; j < width; j++) {
			r = ptr1[i][j].r;
			g = ptr1[i][j].g;
			b = ptr1[i][j].b;

			faceIndex = BMP(r, g, b, face, faceSampleSize);
			nonFaceIndex = BMP(r, g, b, nonFace, nonFaceSampleSize);
			if (face[faceIndex].CalcProbability(r, g, b, Sample::MAP) > nonFace[nonFaceIndex].CalcProbability(r, g, b, Sample::MAP)) {
				ptr1[i][j].r = 255; ptr1[i][j].g = 255; ptr1[i][j].b = 255;
			}
			else {
				ptr1[i][j].r = 0; ptr1[i][j].g = 0; ptr1[i][j].b = 0;
			}
		}
	}

	


	//CString str;
	//str.AppendFormat(_T("c1 = %f, c2 = %f, c3 = %f\n"));
	//AfxMessageBox(str);


	// 1. 가우시안 확률 밀도 함수 구성 이거는 e1=R,e2=G,e3=B 별로, 1차원 확률 밀도 함수를 구성 clear

	// 2. 픽셀값이 피부값인지 판단하기 위해 MAP방법과 ML 방법으로 구성

	// 3. P( Face | e1,e2,e3 ) , P(Non-Face | e1,e2,e3)를 나이브 베이지안으로 계산

	// 4. P(e1,e2,e3 | Face ) , P(e1,e2,e3 | Non-face)를 나이브 베이지안으로 계산

	SetRGBptr(pData, ptr1, width, height);
	Invalidate();

	for (i = 0; i < width; i++) delete[] ptr1;
	delete ptr1;
	delete face;
	delete nonFace;
}

