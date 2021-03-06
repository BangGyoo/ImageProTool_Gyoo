// Dialog1.cpp: 구현 파일
//

#include "stdafx.h"
#include "ImageProTool.h"
#include "Dialog1.h"
#include "afxdialogex.h"


// Dialog1 대화 상자

IMPLEMENT_DYNAMIC(Dialog1, CDialogEx)

Dialog1::Dialog1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

Dialog1::~Dialog1()
{
}

void Dialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dialog1, CDialogEx)
//	ON_BN_CLICKED(IDOK, &Dialog1::OnBnClickedOk)
ON_BN_CLICKED(IDOK, &Dialog1::OnBnClickedOk)
END_MESSAGE_MAP()


// Dialog1 메시지 처리기


//void Dialog1::OnBnClickedOk()
//{
//	CClientDC dc(this);
//	dc.MoveTo(10, 10);
//	dc.LineTo(20, 20);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDialogEx::OnOK();
//	CPen pen;
//	pen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
//	CPen* oldPen = dc.SelectObject(&pen);
//	dc.MoveTo(20, 20);
//	dc.LineTo(30, 30);         // 빨간색으로 선을 그림
//
//	dc.SelectObject(oldPen);
//	pen.DeleteObject();
//	ReleaseDC(&dc);
//}


void Dialog1::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
	// 펜 굵기 1의 검은색 실선을 그린다.   
	// 펜 굵기 1의 검은색 실선을 그린다.   

	CClientDC dc(this);
	dc.MoveTo(10, 10);
	dc.LineTo(20, 20);          // 흑백으로 선을 그림


								// DC는 처음 생성될 때 펜의 컬러가 검정색으로 설정된다.

								// 그래서 컬러 또는 다른색으로 선을 그리기 위해서는 컬러펜을 생성한 후 dc에서

								// 사용하도록 선택해야 한다.

								// 이 것은 마치 붓에 다른색 물감을 묻히는 것과 유사하다.



								// 펜 굵기 5의 빨간색 실선을 그린다.

	CPen pen;
	pen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));    // 빨간색 펜 생성

	CPen* oldPen = dc.SelectObject(&pen);
	dc.MoveTo(30, 30);
	dc.LineTo(100, 30);         // 빨간색으로 선을 그림

	dc.SelectObject(oldPen);



	// 만약 빨간색으로 그린 후 파란색으로 그려야 한다면, 다시 새로운 펜을 만들고 그려줘야 한다.

	// 펜 굵기가 10인 파란색 실선을 그림다.

	pen.DeleteObject();        // 빨간색 펜을 삭제한다.

	pen.CreatePen(PS_SOLID, 10, RGB(0, 0, 255));  // 파란색 펜 생성

	oldPen = dc.SelectObject(&pen);
	dc.MoveTo(30, 30);
	dc.LineTo(30, 100);
	dc.SelectObject(oldPen);

}
