// Histogram.cpp: 구현 파일
//

#include "stdafx.h"
#include "ImageProTool.h"
#include "Histogram.h"
#include "afxdialogex.h"


// Histogram 대화 상자

IMPLEMENT_DYNAMIC(Histogram, CDialogEx)

Histogram::Histogram(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HISTOGRAM, pParent)
{
	ROK = GOK = BOK = YOK = FALSE;
}

Histogram::~Histogram()
{
}

void Histogram::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Histogram, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_RED, &Histogram::OnBnClickedRadioRed)
	ON_BN_CLICKED(IDC_RADIO_GREEN, &Histogram::OnBnClickedRadioGreen)
	ON_BN_CLICKED(IDC_RADIO_BLUE, &Histogram::OnBnClickedRadioBlue)
	ON_BN_CLICKED(IDC_RADIO_GRAY, &Histogram::OnBnClickedRadioGray)
	ON_BN_CLICKED(IDC_Histo, &Histogram::OnBnClickedHisto)
END_MESSAGE_MAP()


void Histogram::SetImage(float histogray[256], float histoR[256], float histoG[256], float histoB[256])
{
	// m_Histogram 배열의 최대값을 구한다.
	register int i;
	float max_valuegray = histogray[0];
	float max_valueR = histoR[0];
	float max_valueG = histoG[0];
	float max_valueB = histoB[0];
	for (i = 1; i < 256; i++)
	{
		if (histogray[i] > max_valuegray)
			max_valuegray = histogray[i];
		if (histoR[i] > max_valueR)
			max_valueR = histoR[i];
		if (histoG[i] > max_valueG)
			max_valueG = histoG[i];
		if (histoB[i] > max_valueB)
			max_valueB = histoB[i];
	}

	// m_Histogram 배열의 최대값이 100이 되도록 전체 배열의 값을 조절한다.
	for (i = 0; i < 256; i++)
	{
		m_Histogram_gray[i] =
			(int)(histogray[i] * 100 / max_valuegray);
		m_HistogramR[i] =
			(int)(histoR[i] * 100 / max_valueR);
		m_HistogramG[i] =
			(int)(histoG[i] * 100 / max_valueG);
		m_HistogramB[i] =
			(int)(histoB[i] * 100 / max_valueB);
	}
}

void Histogram::OnBnClickedRadioRed()
{
	if (ROK == FALSE) { ROK = TRUE; GOK = BOK = YOK = FALSE; }
	else ROK = FALSE;
}


void Histogram::OnBnClickedRadioGreen()
{
	if (GOK == FALSE) { GOK = TRUE; ROK = BOK = YOK = FALSE; }
	else GOK = FALSE;
}


void Histogram::OnBnClickedRadioBlue()
{
	if (BOK == FALSE) { ROK = GOK = YOK = FALSE; BOK = TRUE; }
	else BOK = FALSE;
}


void Histogram::OnBnClickedRadioGray()
{
	if (YOK == FALSE) { YOK = TRUE; ROK = GOK = BOK = FALSE; }
	else YOK = FALSE;
}


void Histogram::OnBnClickedHisto()
{
	const int pen_bold = 2;
	const int pen_pos_x = 100;
	const int pen_pos_y = 300;
	if (ROK == TRUE)
	{
		CClientDC dc(this);
		CPen pen;
		int pen_bold = 2;
		CPen* old_pen;
		pen.CreatePen(PS_SOLID, pen_bold, RGB(255, 0, 0));
		old_pen = (CPen*)dc.SelectObject(&pen);

		dc.MoveTo(pen_pos_x, pen_pos_y);
		for (int i = 0; i<256; i++)
		{
			dc.LineTo(pen_pos_x + pen_bold*i, pen_pos_y - 2*m_HistogramR[i]);
			dc.MoveTo(pen_pos_x + pen_bold*i + pen_bold, pen_pos_y);
		}

		dc.SelectObject(old_pen);
		pen.DeleteObject();
		ReleaseDC(&dc);
	}

	if (GOK == TRUE)
	{
		CClientDC dc(this);
		CPen pen;
		CPen* old_pen;
		pen.CreatePen(PS_SOLID, pen_bold, RGB(0, 255, 0));
		old_pen = (CPen*)dc.SelectObject(&pen);

		dc.MoveTo(pen_pos_x, pen_pos_y);
		for (int i = 0; i<256; i++)
		{
			dc.LineTo(pen_pos_x + pen_bold * i , pen_pos_y - 2 * m_HistogramG[i]);
			dc.MoveTo(pen_pos_x + pen_bold * i + pen_bold, pen_pos_y);
		}
		dc.SelectObject(old_pen);
		pen.DeleteObject();
		ReleaseDC(&dc);
	}

	if (BOK == TRUE)
	{
		CClientDC dc(this);
		CPen pen;
		CPen* old_pen;
		pen.CreatePen(PS_SOLID, pen_bold, RGB(0, 0, 255));
		old_pen = (CPen*)dc.SelectObject(&pen);

		dc.MoveTo(pen_pos_x, pen_pos_y);
		for (int i = 0; i<256; i++)
		{
			dc.LineTo(pen_pos_x + pen_bold * i , pen_pos_y - 2 * m_HistogramB[i]);
			dc.MoveTo(pen_pos_x + pen_bold * i + pen_bold, pen_pos_y);
		}
		dc.SelectObject(old_pen);
		pen.DeleteObject();
		ReleaseDC(&dc);
	}

	if (YOK == TRUE)
	{
		CClientDC dc(this);
		CPen pen;
		CPen* old_pen;
		pen.CreatePen(PS_SOLID, pen_bold, RGB(10, 10, 10));
		old_pen = (CPen*)dc.SelectObject(&pen);

		dc.MoveTo(pen_pos_x, pen_pos_y);
		for (int i = 0; i<256 ; i++)
		{
			dc.LineTo(pen_pos_x + pen_bold * i, pen_pos_y - 2 * m_Histogram_gray[i]);
			dc.MoveTo(pen_pos_x + pen_bold * i + pen_bold, pen_pos_y);
		}
		dc.SelectObject(old_pen);
		pen.DeleteObject();
		ReleaseDC(&dc);
	}
}


INT_PTR Histogram::DoModal()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::DoModal();
}
