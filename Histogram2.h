#pragma once


// Histogram2 대화 상자

class Histogram2 : public CDialogEx
{
	DECLARE_DYNAMIC(Histogram2)

public:
	Histogram2(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Histogram2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = Histogram2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	void 
	DECLARE_MESSAGE_MAP()
public:
//	virtual INT_PTR DoModal();
	virtual INT_PTR DoModal();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnClickedButton1();
};
