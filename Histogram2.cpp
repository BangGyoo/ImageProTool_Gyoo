// Histogram2.cpp: 구현 파일
//

#include "stdafx.h"
#include "ImageProTool.h"
#include "Histogram2.h"
#include "afxdialogex.h"


// Histogram2 대화 상자

IMPLEMENT_DYNAMIC(Histogram2, CDialogEx)

Histogram2::Histogram2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(Histogram2, pParent)
{

}

Histogram2::~Histogram2()
{
}

void Histogram2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Histogram2, CDialogEx)
	ON_COMMAND(IDC_RADIO1, &Histogram2::OnRadio1)
	ON_COMMAND(IDC_RADIO2, &Histogram2::OnRadio2)
	ON_COMMAND(IDC_RADIO3, &Histogram2::OnRadio3)
	ON_COMMAND(IDC_RADIO4, &Histogram2::OnRadio4)
	ON_BN_CLICKED(IDC_BUTTON1, &Histogram2::OnClickedButton1)
END_MESSAGE_MAP()


// Histogram2 메시지 처리기

INT_PTR Histogram2::DoModal()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::DoModal();
}


void Histogram2::OnRadio1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void Histogram2::OnRadio2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void Histogram2::OnRadio3()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void Histogram2::OnRadio4()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void Histogram2::OnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
