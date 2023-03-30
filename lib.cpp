//#include<graphics.h>
#include<cmath>
#include<cstring>
#include<iostream>
#include <winbgim.h> 


void nut(int x1,int y1, int x2, int y2,int color,int bkcolor, int thickness)
{ 
	setfillstyle(1,bkcolor);
	setcolor(color);
	setbkcolor(bkcolor); 
	bar(x1,y1,x2,y2);
	for (int i=0; i<=thickness; i++){
		rectangle(x1+i,y1+i,x2+i,y2+i);
	}
}

void tongang(int x1,int y,int x2, int color, int thickness)
{
	setcolor(color);
	for (int i=0; i<=thickness; i++){
		line(x1,y+i,x2,y+i);
		
	}
}
void todoc(int x1,int y1, int y2,int color, int thickness)
{
	setcolor(color);
	for (int i=0; i<=thickness; i++)
	{
		line(x1+i,y1,x1+i,y2);
	}
}

void hcn_day(int x1,int y1, int x2, int y2,int color, int thickness)
{
	settextstyle(0,0,1);
	setcolor(color);
	for (int i=0; i<=thickness; i++)
	{
		rectangle(x1+i,y1+i,x2+i,y2+i);
	}
}
int kc(int x1,int y1,int x2,int y2){//kiemtra chuot
	return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

bool kt_rect(int x1,int y1,int x2,int y2,int x,int y)
{
	if(x1<x&&x<x2&&y1<y&&y<y2) return true; else return false;
}
	
bool kt_circle(int x,int y,int x1,int y1){
	if((x1-x)*(x1-x)+(y1-y)*(y1-y)<=20*20) return true;else return false;
}

int kiemtragoc(int x, int y, int xt, int yt, int goc)
{
	if (x > xt)
	{
		if (y > yt)
			return 360 - goc;
		else
			return goc;
	}
	else
	{
		if (y > yt)
			return 180 + goc;
		else
			return 180 - goc;
	}
}
/////ve cung
void tamgiac(int x1, int y1, int x2, int y2, int color){
	setfillstyle(1, color);
	int a[8];
	a[0] = x1 + y2 - y1;
	a[1] = y1 + x1 - x2;
	a[2] = x1 - y2 + y1;
	a[3] = y1 - x1 + x2;
	a[4] = x2; a[5] = y2;
	a[6] = a[0]; a[7] = a[1];
	setcolor(color);
	fillpoly(4,a);
}
void cungtron(int x1, int y1, int x2, int y2, int trongso, int color)
{
	int bk = 22;
	int start, end;
	// tam 
	float xO = (x1 + x2) / 2 + (y1 - y2) / M_SQRT2; // or +2 can 2
	float yO = (y1 + y2) / 2 + (x2 - x1) / M_SQRT2;

	int er = sqrt((x1 - xO) * (x1 - xO) + (y1 - yO) * (y1 - yO));
	float mid = atan2((y1 + y2) / 2 - yO, (x1 + x2) / 2 - xO);
	float xT = xO + er * cos(mid);
	float yT = yO + er * sin(mid);
	float t = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2))/100.0;
	float xT1 = xO + (er+t*15) * cos(mid);
	float yT1 = yO + (er+t*15) * sin(mid);
	///
	float corner = atan(float(abs(yT1 - y2)) / abs(xT1 - x2));
	float Ry = bk * sin(corner);
	float Rx = bk * cos(corner);
	// lay lai toa do diem cuoi de tao tam giac
	int x22 = x2 - Rx; //
	int y22 = y2 - Ry;
	if (xT1 > x2)
	{
		x22 = x2 + Rx;
	}
	if (yT1 > y2)
	{ 
		y22 = y2 + Ry;
	}
	//kiem tra goc
	start = atan(float(abs(y1 - yO)) / abs(x1 - xO)) * 180 / M_PI;
	end = atan(float(abs(y22 - yO)) / abs(x22 - xO)) * 180 / M_PI;
	start = kiemtragoc(x1, y1, xO, yO, start);
	end = kiemtragoc(x2, y2, xO, yO, end);
// in
	char xau[10];
	setcolor(color);
	itoa(trongso, xau, 10);
	outtextxy(xT, yT, xau);
	setcolor(color);
	tamgiac(2 * x22 - (x2 + x22) / 2, 2 * y22 -(y22 + y2) / 2, x22, y22, color);
	arc(xO, yO, end, start, er);
}

void toadomoi(int x1, int y1, int x2, int y2, int trongso, bool test, int color)
{
	int xtd = (x1 + x2) / 2;
	int ytd = (y1 + y2) / 2;
	int r = 20;
	float corner = atan(float(abs(y1 - y2)) / abs(x1 - x2));
	float Ry = r * sin(corner);
	float Rx = r * cos(corner);
	int x11 = x1 + Rx; // gia su : x1 nam ben trai va phia tren x2
	int y11 = y1 + Ry; //
	int x22 = x2 - Rx; //
	int y22 = y2 - Ry;
	if (x1 > x2)
	{
		x11 = x1 - Rx; // neu x1 nam ben phai x2
		x22 = x2 + Rx;
	}
	if (y1 > y2)
	{ // neu x1 nam duoi x2
		y11 = y1 - Ry;
		y22 = y2 + Ry;
	}

	if (test == true)
	{
		char xau[10];
		setcolor(color);
		itoa(trongso, xau, 10);
		outtextxy(xtd, ytd, xau);
		tamgiac(2 * x22 - (x2 + x22) / 2, 2 * y22 -(y22 + y2) / 2, x22, y22, color);
		line(x11, y11, x22, y22);
	}
	else
		cungtron(x1, y1, x2, y2, trongso, color);
}

void OutPut(int x,int y,char a){
	if(a=='1') outtextxy(x,y,"1");
	if(a=='2') outtextxy(x,y,"2");
	if(a=='3') outtextxy(x,y,"3");
	if(a=='4') outtextxy(x,y,"4");
	if(a=='5') outtextxy(x,y,"5");
	if(a=='6') outtextxy(x,y,"6");
	if(a=='7') outtextxy(x,y,"7");
	if(a=='8') outtextxy(x,y,"8");
	if(a=='9') outtextxy(x,y,"9");
	if(a=='0') outtextxy(x,y,"0");
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Nnew(bool test)
{
	if (test == true)
		nut(10,8,100,40,0,10,1);
	else
		nut(10,8,100,40, 1,3,1);
		setcolor(0);
		outtextxy(25,17,"New");
}


void Nopen(bool test)
{
	if ( test == true)
        nut(120,8,220,40,0,10,1);
    else
	    nut(120,8,220,40,1,3,1);
		setcolor(0);
		outtextxy(135,17,"Open");
}

void Nsave(bool test)
{
    if (test == true )
        nut(240,6,350,40,0,10,1);
    else
	    nut(240,6,350,40,1,3,1);
		setcolor(0);
		outtextxy(265,17,"Save");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Add(bool test)
{
    if (test == true )
        nut(400,8,500,40,0,10,1);
    else
        nut(400,8,500,40,1,3,1);
		setcolor(0);
		outtextxy(420,17,"Add");
		setlinestyle(0,1,1);//(line style, color, thickness)
}


void Delete(bool test)
{
    if (test == true )
        nut(520,8,625,40,0,10,1);
    else
    	nut(520,8,625,40, 1,3,1);
		setcolor(0);	
		outtextxy(526,17,"Delete");
		setlinestyle(0,1,1);
}
void Move(bool test)
{
    if (test == true)
        nut(640,8,740,40,0,10,1);
    else
	    nut(640,8,740,40, 1,3,1);
		setcolor(0);
		outtextxy(659,17,"Move");
		setlinestyle(0,1,1);
}


void Connect(bool test)
{
    if (test == true)
   	 nut(760,8,880,40,0,10,1);
    else
		nut(760,8,880,40, 1,3,1);
		setcolor(0);
		outtextxy(765,18,"Connect");
		setlinestyle(0,1,1);
}


void Rename(bool test)
{
    if (test == true)
    	nut(900,8,1015,40,0,10,1);
    else
    	nut(900,8,1015,40, 1,3,1);
		setcolor(0);
		outtextxy(910,17,"Rename");
		setlinestyle(0,1,1);
}
void Info(bool test)
{
    if (test == true)
    	nut(1150,8,1265,40, 0,10,1);
    else
    	nut(1150,8,1265,40, 1,3,1);
		setcolor(0);
		outtextxy(1173,17,"Info");
		setlinestyle(0,1,1);
}
void Close(bool test)
{
    if (test == true)
    	nut(1285,8,1400,40,0,8,1);
    else
    	nut(1285,8,1400,40,1,12,1);
		setcolor(0);
		outtextxy(1300,17,"Close");
		setlinestyle(0,1,1);
}

void DFS(bool test)
{ 
    if (test == true)
	    nut(25,475,140,525,0,10,1);
    else
	    nut(25,475,140,525, 1,3,1);
		setcolor(0);
		outtextxy(60,492,"DFS");
}


void BFS(bool test)
{
    if (test == true)
	    nut(170,475,290,525,0,10,1);
    else
	    nut(170,475,290,525, 1,3,1);
		setcolor(0);
		outtextxy(205,492,"BFS");
}

void Top(bool test)
{
    if (test == true)
	    nut(320,475,430,525,0,10,1);
    else
	    nut(320,475,430,525, 1,3,1);
		setcolor(0);
		outtextxy(345,492,"TOP");
}


void Euler(bool test)
{
    if (test == true)
	    nut(25,540,140,590,0,10,1);
    else
	    nut(25,540,140,590, 1,3,1);
		setcolor(0);
		outtextxy(47,557,"Euler");
}


void Hamil(bool test)
{
    if (test == true)
	    nut(170,540,290,590,0,10,1);
    else
	    nut(170,540,290,590, 1,3,1);
		setcolor(0);
		outtextxy(192,557,"Hamil");
}


void Xy(bool test)
{
    if (test == true)
	    nut(320,540,430,590,0,10,1);
    else
	    nut(320,540,430,590, 1,3,1);
		setcolor(0);
		outtextxy(345,557,"X->Y");
}

void Lienthong(bool test)
{
    if (test == true)
	    nut(25,620,215,690,0,10,1);
    else
	    nut(25,620,215,690, 1,3,1);
		setcolor(0);
		outtextxy(45,645,"Lien Thong");	
}


void Dinhtru(bool test)
{
    if (test == true)
	    nut(240,620,430,690,0,10,1);
    else
	    nut(240,620,430,690, 1,3,1);
		setcolor(0);
		outtextxy(277,645,"Dinh Tru");
}


void Dinhthat(bool test)
{
    if (test == true)
	    nut(25,705,215,775,0,10,1);
    else
	    nut(25,705,215,775, 1,3,1);
		setcolor(0);
		outtextxy(55,730,"Dinh That");
}

void Canhcau(bool test)
{
    if (test == true)
        nut(240,705,430,775,0,10,1);
    else
    	nut(240,705,430,775, 1,3,1);
		setcolor(0);
		outtextxy(270,730,"Canh Cau");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void tao_node(int x, int y, int color)
{
	setlinestyle (0,0,1);
	setcolor(color);
	setfillstyle(1,color);
	for (int i=0; i<=1;i++)
	{
		circle(x,y,21-i);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void toolbar(){
	settextstyle(0,0,2);
	Nnew(false);
	Nopen(false);
	Nsave(false);
	Add(false);
	Delete(false);
	Move(false);
	Connect(false);
	Rename(false);
	Close(false);
	Info(false);
}

void Algorithm_screen(){
	hcn_day(11,430,450,785,1,1);//thuat toan
	settextstyle(1,0,2);  
	setcolor(2);
	setbkcolor(15);
	outtextxy(20,436,"Algorithm"); 
	settextstyle(0,0,2); 
	DFS(false);
	BFS(false);
	Top(false);
	Euler(false);
	Hamil(false);
	Xy(false);
	Lienthong(false);
	Dinhtru(false);
	Dinhthat(false);
	Canhcau(false);
}

void Algorithm_tool_screen(){
	Algorithm_screen();
	toolbar();
}

void Work_screen(){//hinh chu nhat lon nhat
	setfillstyle(1,15);
	bar(458,60,getmaxx()-10,550);  
	setcolor(1);
	for (int i=0;i<=1;i++)
	{
		rectangle(458+i,59+i,1395-i,550-i);
	}
}

void Matrix_screen(){
	setfillstyle(1,15);
	bar(13,61,452,422);
	hcn_day(11,59,450,420,1,1);
	setcolor(2);	setbkcolor(15); 
	settextstyle(1,0,2);	outtextxy(20,65,"Matrix");
} 

void Notify_screen()
{
	setfillstyle(1,15); //to phia trong bar mau trong suot
	bar(458,560,1394,785); 
	setcolor(2);
	hcn_day(458,560,1394,785,1,1);
}

void home(){
	Algorithm_screen();
	toolbar();
	Matrix_screen();
	Work_screen();
	Notify_screen();	
}
 
void Info_screen(){
	home();
	setfillstyle(1,15);
	bar(458,60,getmaxx()-10,550);
	setcolor(1);
	for (int i=0;i<=1;i++)
	{
		rectangle(458+i,59+i,getmaxx()-17-i,550-i);
	}
	setfillstyle(1,3);
	bar(460,61,getmaxx()-18,130);
	
	setcolor(0);	setbkcolor(3); 
	settextstyle(4,0,4);	outtextxy(720,77,"Demo De Tai Do Thi");
	
	setfillstyle(1,11);
	bar(460,130,getmaxx()-18,550);
	
	setcolor(0);setbkcolor(11);
	settextstyle(4,0,2);
	outtextxy(765,160,"Thuc Hien Boi Nhom 11: ");
	line(765,182,1120,182);
	
	setcolor(4);
	settextstyle(4,0,3);
	outtextxy(590,300,"1. VO QUANG TUONG          N19DCCN185 ");
	outtextxy(590,340,"2. NGUYEN TRONG SON     N19DCCN161 ");
	setcolor(12);
	outtextxy(800,510,"Xin Cam On! ");
	
} 

void Start_screen()
{
	int color=1,bkcolor=3,thinkness=1;
	initwindow(1420,815);
	setbkcolor(15);
	cleardevice();	 
	home();
	
	setcolor(12);	setbkcolor(15);
	settextstyle(1,0,2);	outtextxy(472,575,"Guide:")	; 
	hcn_day(458,560,1394,785,color,thinkness);  	
	setcolor(0);
	settextstyle(8, HORIZ_DIR, 2);
	outtextxy(472,620,"(New): Tao do thi moi                         (Open): Mo do thi da co");
	outtextxy(472,650,"(Save): Luu do thi                            (Add):  Them dinh");
	outtextxy(472,680,"(Delete): Xoa dinh                            (Move): Di chuyen dinh");
	outtextxy(472,710,"(Connect): Tao cung va chinh sua              (Rename): Doi ten dinh ");
	outtextxy(472,740,"(Close): Thoat chuong trinh                   (Info): Thong tin do an");
}


