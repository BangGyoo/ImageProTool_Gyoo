#pragma once


// Histogram 대화 상자

class Histogram : public CDialogEx
{
	DECLARE_DYNAMIC(Histogram)

public:
	Histogram(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Histogram();

	int m_Histogram_gray[256];
	int m_HistogramR[256];
	int m_HistogramG[256];
	int m_HistogramB[256];
	BOOL ROK, GOK, BOK, YOK;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioRed();
	afx_msg void OnBnClickedRadioGreen();
	afx_msg void OnBnClickedRadioBlue();
	afx_msg void OnBnClickedRadioGray();
	afx_msg void OnBnClickedHisto();
	virtual INT_PTR DoModal();
	void SetImage(float[], float[], float[], float[]);
};
