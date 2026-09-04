#include"drawCard.h"
#include<iostream>
#include<Windows.h>

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

using namespace std;
HANDLE hConsole2=GetStdHandle(STD_OUTPUT_HANDLE);

void drawDot(int picture,int n){
	SetConsoleTextAttribute(hConsole2,7);
	switch(picture){
	case 1:
		{
			SetConsoleTextAttribute(hConsole2,12);
			if(n==0) cout<<"         ";
			if(n==1) cout<<"    m    ";
			if(n==2) cout<<"  m   m  ";
			SetConsoleTextAttribute(hConsole2,7);
			break;
		}
	case 2:
		{
			SetConsoleTextAttribute(hConsole2,12);
			if(n==0) cout<<"         ";
			if(n==1) cout<<"    D    ";
			if(n==2) cout<<"  D   D  ";
			SetConsoleTextAttribute(hConsole2,7);
			break;
		}
	case 3:
		{
			if(n==0) cout<<"         ";
			if(n==1) cout<<"    w    ";
			if(n==2) cout<<"  w   w  ";
			break;
		}
	case 4:
		{
			if(n==0) cout<<"         ";
			if(n==1) cout<<"    %    ";
			if(n==2) cout<<"  %   %  ";
			break;
		}
	default:
		break;
	}
}
//有数字情况
void drawDot2(int picture,int n){
	SetConsoleTextAttribute(hConsole2,7);
	switch(picture){
	case 1:
		{
			SetConsoleTextAttribute(hConsole2,12);
			if(n==0) cout<<"        ";
			if(n==1) cout<<"   m    ";
			if(n==2) cout<<" m   m  ";
			SetConsoleTextAttribute(hConsole2,7);
			break;
		}
	case 2:
		{
			SetConsoleTextAttribute(hConsole2,12);
			if(n==0) cout<<"        ";
			if(n==1) cout<<"   D    ";
			if(n==2) cout<<" D   D  ";
			SetConsoleTextAttribute(hConsole2,7);
			break;
		}
	case 3:
		{
			if(n==0) cout<<"        ";
			if(n==1) cout<<"   w    ";
			if(n==2) cout<<" w   w  ";
			break;
		}
	case 4:
		{
			if(n==0) cout<<"        ";
			if(n==1) cout<<"   %    ";
			if(n==2) cout<<" %   %  ";
			break;
		}
	default:
		break;
	}
}

void drawCard(int card[],int m,int n){
	if(m>n) return;
	//第一层
	for(int i=m;i<=n;i++){
		cout<<"┌─────────┐";
	}
	cout<<endl;
	//第二层
	for(int i=m;i<=n;i++){
		if(card[i]%13!=10) cout<<"│";
		int j=card[i]%13;
		int k=card[i]/13;
		if(j==0){
			j=13;
			k--;
		}
		if(card[i]<=26) SetConsoleTextAttribute(hConsole2,12);
		switch(j){
		case 1:
			cout<<"A";
			drawDot2(k+1,0);
			break;
		case 11:
			cout<<"J";
			drawDot2(k+1,0);
			break;
		case 12:
			cout<<"Q";
			drawDot2(k+1,0);
			break;
		case 13:
			cout<<"K";
			drawDot2(k+1,0);
			break;
		default:
			cout<<j;
			drawDot2(k+1,j<=3?1:2);
			break;
		}
		SetConsoleTextAttribute(hConsole2,7);
		cout<<"│";
	}
	cout<<endl;
	//第三层
	for(int i=m;i<=n;i++){
		cout<<"│";
		int j=card[i]%13;
		int k=card[i]/13;
		if(j==0){
			j=13;
			k--;
		}
		SetConsoleTextAttribute(hConsole2,card[i]<=26?12:7);
		switch(j){
		case 11:
			cout<<"  =   =  ";
			break;
		case 12:
			cout<<"  -   -  ";
			break;
		case 13:
			cout<<"┌───────┐";
			break;
		default:
			drawDot(k+1,j==10?1:0);
		}
		SetConsoleTextAttribute(hConsole2,7);
		cout<<"│";
		
	}
	cout<<endl;
	//第四层
	for(int i=m;i<=n;i++){
		cout<<"│";
		int j=card[i]%13;
		int k=card[i]/13;
		if(j==0){
			j=13;
			k--;
		}
		SetConsoleTextAttribute(hConsole2,card[i]<=26?12:7);
		switch(j){
		case 11:
			cout<<"└ ^   ^ ┘";
			break;
		case 12:
			cout<<"└ -   - ┘";
			break;
		case 13:
			cout<<" ē    ē  ";
			break;
		default:
			drawDot(k+1,j<=6?0:2);
			break;
		}
		SetConsoleTextAttribute(hConsole2,7);
		cout<<"│";
	}
	cout<<endl;
	//第五层
	for(int i=m;i<=n;i++){
		cout<<"│";
		int j=card[i]%13;
		int k=card[i]/13;
		if(j==0){
			j=13;
			k--;
		}
		SetConsoleTextAttribute(hConsole2,card[i]<=26?12:7);
		switch(j){
		case 6:
			drawDot(k+1,2);
			break;
		case 7:
			drawDot(k+1,0);
			break;
		case 11:
			cout<<"   -     ";
			break;
		case 12:
			cout<<"  /^ ＼  ";
			break;
		case 13:
			cout<<"   .     ";
			break;
		default:
			drawDot(k+1,j%2);
			break;
		}
		SetConsoleTextAttribute(hConsole2,7);
		cout<<"│";
	}
	cout<<endl;
	//第六层
	for(int i=m;i<=n;i++){
		cout<<"│";
		int j=card[i]%13;
		int k=card[i]/13;
		if(j==0){
			j=13;
			k--;
		}
		SetConsoleTextAttribute(hConsole2,card[i]<=26?12:7);
		switch(j){
		case 11:
			cout<<"   目    ";
			break;
		case 12:
			cout<<"   曰    ";
			break;
		case 13:
			cout<<"   一    ";
			break;
		default:
			drawDot(k+1,j<=6?0:2);
			break;
		}
		SetConsoleTextAttribute(hConsole2,7);
		cout<<"│";
	}
	cout<<endl;
	//第七层
	for(int i=m;i<=n;i++){
		cout<<"│";
		int j=card[i]%13;
		int k=card[i]/13;
		if(j==0){
			j=13;
			k--;
		}
		SetConsoleTextAttribute(hConsole2,card[i]<=26?12:7);
		switch(j){
		case 11:
			drawDot(k+1,0);
			break;
		case 12:
			drawDot(k+1,0);
			break;
		case 13:
			drawDot(k+1,0);
			break;
		default:
			drawDot(k+1,j==10?1:0);
			break;
		}
		SetConsoleTextAttribute(hConsole2,7);
		cout<<"│";
	}
	cout<<endl;
	//第八层
	for(int i=m;i<=n;i++){
		cout<<"│";
		int j=card[i]%13;
		int k=card[i]/13;
		if(j==0){
			j=13;
			k--;
		}
		SetConsoleTextAttribute(hConsole2,card[i]<=26?12:7);
		switch(j){
		case 1:
			drawDot(k+1,0);
			break;
		case 2:
			drawDot(k+1,1);
			break;
		case 3:
			drawDot(k+1,1);
			break;
		case 4:
			drawDot(k+1,2);
			break;
		case 5:
			drawDot(k+1,2);
			break;
		case 6:
			drawDot(k+1,2);
			break;
		case 7:
			drawDot(k+1,1);
			break;
		case 8:
			drawDot(k+1,2);
			break;
		case 9:
			drawDot(k+1,2);
			break;
		case 10:
			drawDot(k+1,2);
			break;
		case 11:
			drawDot(k+1,0);
			break;
		case 12:
			drawDot(k+1,0);
			break;
		case 13:
			drawDot(k+1,0);
			break;
		default:

			break;
		}
		SetConsoleTextAttribute(hConsole2,7);
		cout<<"│";
	}
	cout<<endl;
	//第九层
	for(int i=m;i<=n;i++){
		cout<<"└─────────┘";
	}
	cout<<endl;
}