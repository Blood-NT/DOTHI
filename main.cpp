#include <iostream>
#include <winbgim.h>
#include <cstring>
#include "stack.h"
#include "queue.h"
#include "lib.h"
//#include<graphics.h>
#include <fstream>
using namespace std;

const int TIME = 1000;
const int infinite = 9999;
const int MAX_NUT = 15;
// ---------------------------------------------------------------------------//
//                          DOTHI dt										  //
// ---------------------------------------------------------------------------//
struct Node
{
	int x, y;
	char name[2];  
};
struct DoThi
{
	Node *node;
	int TongSo_Dinh;
	int **MT;
	void delete_Data()
	{
		delete[] node;
		for (int i = 0; i <= TongSo_Dinh; i++)
			delete[] MT[i];
		delete[] MT;
	}
	void init_Node(int t, int ind)
	{
		if (t == 1)
		{ // TANG NODE LEN 1
			Node *node2 = new Node[TongSo_Dinh + 1];
			for (int i = 0; i <= TongSo_Dinh - 1; i++)
				node2[i] = node[i];
			delete[] node;
			node = node2;
		}
		if (t == -1)
		{ //GIAM NODE XUONG 1
			Node *node2 = new Node[TongSo_Dinh + 1];
			for (int i = 0; i <= ind - 1; i++)
				node2[i] = node[i];
			for (int i = ind; i <= TongSo_Dinh; i++)
				node2[i] = node[i + 1];
			delete[] node;
			node = node2;
		}
		if (t == 0)
		{ // CAP NHAT NODE KHI OPEN FILE
			Node *node2 = new Node[TongSo_Dinh + 1];
			delete[] node;
			node = node2;
		}
	}
	void reset_MT(int **MT1)
	{
		for (int i = 0; i <= TongSo_Dinh; i++)
			for (int j = 0; j <= TongSo_Dinh; j++)
				MT1[i][j] = 0;
	}

	void init_MT(int t, int ind)
	{
		if (t == 1)
		{ //TANG MT LEN 1
			int **MT2 = new int *[TongSo_Dinh + 1];
			for (int i = 0; i <= TongSo_Dinh; i++)
				MT2[i] = new int[TongSo_Dinh + 1];
			reset_MT(MT2);
			for (int i = 0; i <= TongSo_Dinh - 1; i++)
				for (int j = 0; j <= TongSo_Dinh - 1; j++)
					MT2[i][j] = MT[i][j];

			for (int i = 0; i <= TongSo_Dinh - 1; i++)
				delete[] MT[i];
			delete[] MT;
			MT = MT2;
		}
		if (t == -1)
		{ //GIAM MT DI 1
			int **MT2 = new int *[TongSo_Dinh + 1];
			for (int i = 0; i <= TongSo_Dinh; i++)
				MT2[i] = new int[TongSo_Dinh + 1]; //sao chep cac dinh cua MT sang MT2

			for (int j = 0; j <= TongSo_Dinh + 1; j++)
			{ //Xoa Dinh ind
				MT[ind][j] = 0;
				MT[j][ind] = 0;
			}

			for (int x1 = 0; x1 <= TongSo_Dinh + 1; x1++)
				for (int y1 = 0; y1 <= TongSo_Dinh + 1; y1++)
					if (MT[x1][y1] != 0)
					{ //ton tai dinh
						int t1, t2;
						if (x1 > ind)
							t1 = x1 - 1;
						else
							t1 = x1;

						if (y1 > ind)
							t2 = y1 - 1;
						else
							t2 = y1;

						if (x1 > ind || y1 > ind)
						{
							MT[t1][t2] = MT[x1][y1];
							MT[x1][y1] = 0;
						}
					}

			reset_MT(MT2);
			for (int i = 0; i <= TongSo_Dinh; i++)
				for (int j = 0; j <= TongSo_Dinh; j++)
					MT2[i][j] = MT[i][j];

			for (int i = 0; i <= TongSo_Dinh + 1; i++)
				delete[] MT[i];
			delete[] MT;
			MT = MT2;
		}
		if (t == 0)
		{ //KHOI TAO LAI MT KHI OPEN FILE
			for (int i = 0; i <= ind; i++)
				delete[] MT[i];

			delete[] MT;
			int **MT2 = new int *[TongSo_Dinh + 1];
			for (int i = 0; i <= TongSo_Dinh; i++)
				MT2[i] = new int[TongSo_Dinh + 1];
			reset_MT(MT2);
			MT = MT2;
		}
	}
	~DoThi()
	{
		delete[] node;
		for (int i = 0; i <= TongSo_Dinh; i++)
			delete[] MT[i];
		delete[] MT;
	}
} dt;

int **init_tmp()
{
	int **tmp = new int *[dt.TongSo_Dinh + 1];
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		tmp[i] = new int[dt.TongSo_Dinh + 1];
	return tmp;
}

void Copy_MT(int **tmp)
{
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
		{
			dt.MT[i][j] > 0 ? tmp[i][j] = 1 : tmp[i][j] = 0;
		}
	}
}

void Co_Lap_Dinh(int **tmp, int pos)
{
	Copy_MT(tmp);
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		tmp[pos][i] = 0;
		tmp[i][pos] = 0;
	}
}
//xoa canh
void Xoa_Canh(int **tmp, int u, int v)
{
	Copy_MT(tmp);
	tmp[u][v] = 0;
	tmp[v][u] = 0;
} //////////////////////////////////////////
void Hoan_Canh(int **tmp, int u, int v)
{
	Copy_MT(tmp);
	tmp[u][v] = 1;
	tmp[v][u] = 1;
} //////////////////////////////////////////
// ---------------------------------------------------------------------------
//                          Variables
// ---------------------------------------------------------------------------
int x, y, tam, dem = 0, tam1 = -1, tam2 = -1;
bool working = false;
char tenfile[50];
bool kt_Del = false, kt_Add = false, kt_Move = false, kt_Connect = false, kt_Rename = false, kt_New = false, kt_Save = false, kt_Open = false;
bool kt_DFS = false, kt_BFS = false, kt_DThat = false, kt_XY = false, kt_TPLT = false, kt_Ham = false, kt_Dtru = false, kt_Eurle = false, kt_CCau = false;

// ---------------------------------------------------------------------------
//                          Algorithm
// ---------------------------------------------------------------------------
void DFS(int);
void BFS(int);
void Dijkstra(); //xy

// ---------------------------------------------------------------------------
//                          Input/putput
// ---------------------------------------------------------------------------
void Openfile();
void Save();

void Set_True(bool &a)
{
	kt_Del = false;
	kt_Add = false;
	kt_Move = false;
	kt_Connect = false;
	kt_Rename = false;
	kt_DFS = false;
	kt_BFS = false;
	kt_DThat = false;
	kt_XY = false;
	kt_TPLT = false;
	kt_Ham = false;
	kt_Dtru = false;
	kt_Eurle = false;
	kt_CCau = false;
	a = true;
}

void Set_False()
{
	kt_Del = false;
	kt_Add = false;
	kt_Move = false;
	kt_Connect = false;
	kt_Rename = false;
	kt_DFS = false;
	kt_BFS = false;
	kt_DThat = false;
	kt_XY = false;
	kt_TPLT = false;
	kt_Ham = false;
	kt_Dtru = false;
	kt_Eurle = false;
}

void clearMouse()
{
	clearmouseclick(WM_LBUTTONDOWN);
}

void Link_File(char linkFile[], char fileName[])
{
	strcpy(linkFile, "./saves/"); //sao ch�p chuoi k� tu tu . / s a v e s / toi chuoi linkFile.
	strcat(linkFile, fileName);	  //noi chuoi fileName phia sau linkFile
	strcat(linkFile, ".graph");	  //noi chuoi .graph phia sau linkFile
}

void updateChar(int i, char s[])
{
	for (; i < 50; i++)
		s[i] = '\0';
}

void Del_Node(int i)
{
	setcolor(15);
	setfillstyle(1, 15);
	pieslice(dt.node[i].x, dt.node[i].y, 0, 360, 21);
}
void Draw_Node(int i, int color)
{
	setcolor(color);
	setbkcolor(color);
	setfillstyle(1, color);
	pieslice(dt.node[i].x, dt.node[i].y, 0, 360, 21);
	setcolor(0);
	settextstyle(8, HORIZ_DIR, 1);
	outtextxy(dt.node[i].x - 14, dt.node[i].y - 10, dt.node[i].name);
	circle(dt.node[i].x, dt.node[i].y, 21);
}

///////////////////////////////////////////////////OPEN///////////////////////////////////////////////

void Load_Nodes()
{
	Work_screen();
	settextstyle(8, HORIZ_DIR, 2);
	setbkcolor(15);

	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
		{
			if (dt.MT[i][j] != 0 and dt.MT[j][i] == 0)
				toadomoi(dt.node[i].x, dt.node[i].y, dt.node[j].x, dt.node[j].y, dt.MT[i][j], true, 4);
			if (dt.MT[i][j] != 0 and dt.MT[j][i] != 0 and i <= j)
				toadomoi(dt.node[i].x, dt.node[i].y, dt.node[j].x, dt.node[j].y, dt.MT[i][j], true, 4);
			if (dt.MT[i][j] != 0 and dt.MT[j][i] != 0 and i > j)
				toadomoi(dt.node[i].x, dt.node[i].y, dt.node[j].x, dt.node[j].y, dt.MT[i][j], false, 4);
		}
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		tao_node(dt.node[i].x, dt.node[i].y, 1);
		outtextxy(dt.node[i].x - 14, dt.node[i].y - 10, dt.node[i].name); ///////////////////////
	}
}

void Show_Link(int i, int j)
{ //i->j
	if (dt.MT[i][j] != 0 and dt.MT[j][i] == 0)
		toadomoi(dt.node[i].x, dt.node[i].y, dt.node[j].x, dt.node[j].y, dt.MT[i][j], true, 1);
	else if (dt.MT[i][j] != 0 and dt.MT[j][i] != 0 and i <= j)
		toadomoi(dt.node[i].x, dt.node[i].y, dt.node[j].x, dt.node[j].y, dt.MT[i][j], true, 1);
	else if (dt.MT[i][j] != 0 and dt.MT[j][i] != 0 and i > j)
		toadomoi(dt.node[i].x, dt.node[i].y, dt.node[j].x, dt.node[j].y, dt.MT[i][j], false, 1);
	Draw_Node(j, 15);
}

void load_matrix()
{ //////////////////////////																									lOAD MA TRAN
	Matrix_screen();
	if (dt.TongSo_Dinh >= 0)
	{
		setcolor(4);
		for (int i = 0; i <= dt.TongSo_Dinh; i++)
			for (int j = 0; j <= dt.TongSo_Dinh; j++)
				if (dt.MT[i][j] != 0)
				{
					char a[10];
					itoa(dt.MT[i][j], a, 10);
					settextstyle(0, 0, 1);
					outtextxy(100 + 20 * j - 3, 112 + 20 * i + 3, a);
				}
				else
				{
					settextstyle(0, 0, 1);
					outtextxy(100 + 20 * j, 112 + 20 * i + 3, "-");
				}
		settextstyle(0, 0, 1);
		for (int i = 0; i <= dt.TongSo_Dinh; i++)
		{
			setcolor(0);
			outtextxy(94 + 20 * i + 3, 97, dt.node[i].name); //stt dinh theo chieu ngang
		}
		for (int i = 0; i <= dt.TongSo_Dinh + 2; i++)
		{
			setcolor(0);
			line(75 + 20 * i, 90, 75 + 20 * i, 90 + 20 * (dt.TongSo_Dinh + 2)); //cot ngang
		}
		for (int i = 0; i <= dt.TongSo_Dinh; i++)
		{
			setcolor(0);
			outtextxy(78, 112 + 20 * i + 3, dt.node[i].name); //stt dinh theo chieu doc
		}
		for (int i = 0; i <= dt.TongSo_Dinh + 2; i++)
		{
			setcolor(0);
			line(75, 90 + 20 * i, 75 + 20 * (dt.TongSo_Dinh + 2), 90 + 20 * i); //cot doc
		}
	}
}

void Open_DoThi(char fileName[])
{
	Notify_screen();
	char linkFile[100];
	Link_File(linkFile, fileName);
	ifstream Openfile;
	Openfile.open(linkFile, ios::in);
	if (!Openfile.good())
	{
		settextstyle(8, HORIZ_DIR, 2);
		setcolor(4);
		outtextxy(468, 570, "File khong ton tai");
		delay(2 * TIME);
		kt_Save = false;
		home();
		return;
	}
	int t = dt.TongSo_Dinh;
	Openfile >> dt.TongSo_Dinh;
	dt.init_Node(0, 0);
	dt.init_MT(0, t);
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
			dt.MT[i][j] = 0;

	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		Openfile >> dt.node[i].name >> dt.node[i].x >> dt.node[i].y;

	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
			Openfile >> dt.MT[i][j];

	Openfile.close();
	load_matrix();
	Load_Nodes();
}
void Openfile()
{
	Notify_screen();
	settextstyle(8, HORIZ_DIR, 2);
	for (int i = 0; i <= 50; i++)
		tenfile[i] = '\0';
	setbkcolor(15);
	setcolor(0);
	outtextxy(468, 570, "Nhap ten file (Do dai ten file <=50)            [Nhan phim ESC de thoat!]");
	settextstyle(8, HORIZ_DIR, 2);
	char s[50];
	s[50] = '\0';
	for (int i = 0; i <= 50; i++)
		s[i] = '\0';
	int d = 0;
	while (true)
	{
		s[d] = getch();
		if (!(('0' <= s[d] and s[d] <= '9') or ('a' <= s[d] and s[d] <= 'z') or ('A' <= s[d] and s[d] <= 'Z') or s[d] == '_' or s[d] == 13 or s[d] == 8 or s[d] == 27))
			continue;
		if (d == 50)
		{
			if (s[d] != 13 && s[d] != 8)
			{
				s[49] = s[50];
				d--;
				setcolor(4);
				updateChar(d + 1, s);
				outtextxy(468, 590, s);
			}
		}
		if (s[d] == 13)
		{ //enter

			for (int i = 0; i <= 50; i++)
				tenfile[i] = '\0';
			for (int i = 0; i <= d - 1; i++)
				tenfile[i] = s[i];
			break;
		}
		else if (s[d] == 8)
		{ //delete
			d--;
			Notify_screen();
			settextstyle(8, HORIZ_DIR, 2);
			setcolor(0);
			outtextxy(468, 570, "Nhap ten file (Do dai ten file <=50)            [Nhan phim ESC de thoat!]");
			setcolor(4);
			updateChar(d, s);
			outtextxy(468, 590, s);
			if (d == -1)
				d = 0;
			continue;
		}
		else if (s[d] == 27)
		{
			setcolor(4);
			outtextxy(468, 620, "Thoat mo file !!");
			delay(TIME);
			break;
		}
		else
		{
			setcolor(2);
			updateChar(d + 1, s);
			outtextxy(468, 590, s);
		}
		d++;
	}
	Open_DoThi(tenfile);
	kt_Save = true;
}
///////////////////////////////////////////////////Save///////////////////////////////////////////////
void Save_DoThi(char fileName[])
{
	char linkFile[100];
	Link_File(linkFile, fileName);
	ofstream saveFile;
	saveFile.open(linkFile, ios::out | ios::trunc);
	saveFile << dt.TongSo_Dinh << "\n";

	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		saveFile << dt.node[i].name << " " << dt.node[i].x << " " << dt.node[i].y;
		saveFile << "\n";
	}

	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
		{
			saveFile << dt.MT[i][j] << " ";
		}
		saveFile << "\n";
	}
	saveFile.close();
}

void Save() //Khi chua mo file co san
{
	Notify_screen();
	setcolor(0);
	settextstyle(8, HORIZ_DIR, 2);
	outtextxy(468, 570, "Nhap ten file (Do dai ten file <=50): ");
	char s[50];
	s[50] = '\0';
	int d = 0;
	while (true)
	{
		s[d] = getch();
		if (!(('0' <= s[d] and s[d] <= '9') or ('a' <= s[d] and s[d] <= 'z') or ('A' <= s[d] and s[d] <= 'Z') or s[d] == '_' or s[d] == 13 or s[d] == 8))
			continue;
		//neu nhap phim ko hop le (0-9,a-z,A-Z,s[d] == 13, s[d] == 8 '_') se thoat vong lap va cho phep nhap phim moi

		if (d == 50 && s[d] != 13 && s[d] != 8)
		{
			s[49] = s[50]; //neu chuoi dai hon 30 thi gan ki tu s[30] vua nhap vao cho vi tri s[29] cho phep cuoi cung
			d--;
		}

		if (s[d] == 13)
		{
			if (d == 0)
				continue;

			for (int i = 0; i <= 50; i++)
				tenfile[i] = '\0';
			for (int i = 0; i <= d - 1; i++)
			{
				tenfile[i] = s[i];
				tenfile[d] = '\0';
			}
			Notify_screen();
			setcolor(4);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(468, 570, "Da luu file");
			delay(TIME);
			break;
		}
		else if (s[d] == 8)
		{ //delete
			d--;
			Notify_screen();
			setcolor(0);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(468, 570, "Nhap ten file (Do dai ten file <=50): ");
			updateChar(d, s);
			setcolor(4);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(468, 600, s);
			if (d == -1)
				d = 0;
			continue;
		}
		else
		{
			setcolor(2);
			updateChar(d + 1, s);
			outtextxy(468, 600, s);
		}
		d++;
	}
	kt_Save = true;
	Save_DoThi(tenfile);
}

void Save2() //Khi da mo file co san
{
	Save_DoThi(tenfile);
	Notify_screen();
	setcolor(4);
	settextstyle(8, HORIZ_DIR, 2);
	outtextxy(468, 570, "Da luu file");
	delay(TIME);
}

void start()
{
	dt.TongSo_Dinh = -1;
	Notify_screen();
	Algorithm_screen();
	Matrix_screen();
	Work_screen();
	kt_Save = false;
	Load_Nodes();
	load_matrix();
}

// ------------------------------------------------------------------------------------------------------------------
//                       					   THUAT TOAN
// ------------------------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////DFS///////////////////////////////////////////////////////////
void DFS(int root) //O(max(n, m)) n so dinh, m so canh
{
	Queue ds_ke[dt.TongSo_Dinh + 1]; //tao danh sach ke cho moi dinh
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
			if (dt.MT[i][j] > 0)
				ds_ke[i].push(j);

	bool visited[dt.TongSo_Dinh + 1];
	Stack stack; //LIFO: Last In First Out
	//khoi tao gia tri cho check
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		visited[i] = false;

	stack.push(root);
	visited[root] = true;
	int dem = 1;
	Notify_screen();
	settextstyle(0, 0, 2);
	setcolor(2);
	outtextxy(468, 570, "DFS:");
	settextstyle(8, HORIZ_DIR, 2);
	setcolor(4);
	outtextxy(468, 610, dt.node[root].name);
	Del_Node(root);
	Draw_Node(root, 10);
	delay(TIME / 2);
	while (!stack.empty()) //while 1
	{
		int root_tmp; //lan 1: root_tmp = root.,lan 2: root_tmp = i.,
		stack.pop(root_tmp);
		while (!ds_ke[root_tmp].empty()) //while 2
		{
			int i;
			ds_ke[root_tmp].pop(i); //xoa dinh ke i dang xet ra khoi hang doi ds_ke[root_tmp]
			if (visited[i] == false)
			{
				setbkcolor(15);
				stack.push(root_tmp);
				stack.push(i); //stack = [i,root]
				visited[i] = true;

				Show_Link(root_tmp, i);
				delay(TIME / 2);

				Del_Node(i);
				setcolor(2);
				settextstyle(8, HORIZ_DIR, 1);
				outtextxy(468 + 30 * dem, 610, "->");
				dem++;
				setcolor(4);
				settextstyle(8, HORIZ_DIR, 2);
				outtextxy(468 + 30 * dem, 610, dt.node[i].name);
				Draw_Node(i, 10);
				delay(TIME / 2);
				dem++;
				break; //ket thuc vong lap while 2
			}
		}
	}
	settextstyle(8, HORIZ_DIR, 2);
	setcolor(2);
	setbkcolor(15);
	outtextxy(468, 640, "DA DUYET XONG !");
}
///////////////////////////////////////////////////BFS///////////////////////////////////////////////
void BFS(int root)
{
	Queue ds_ke[dt.TongSo_Dinh + 1]; //tao danh sach ke cho moi dinh ,. FIFO: First In First Out
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
			if (dt.MT[i][j] > 0)
				ds_ke[i].push(j);

	Queue queue;
	int *back = new int[MAX_NUT];
	bool visited[dt.TongSo_Dinh + 1];
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		visited[i] = false;
	queue.push(root); //queue = [root]
	visited[root] = true;
	Notify_screen();
	setcolor(2);
	settextstyle(0, 0, 2);
	outtextxy(468, 570, "BFS :");
	setcolor(4);
	settextstyle(8, HORIZ_DIR, 2);
	outtextxy(468, 600, dt.node[root].name);
	Del_Node(root);
	Draw_Node(root, 10);
	// duyet bfs theo phuong phap bieu dien danh sach ke
	while (!queue.empty())
	{
		int root_tmp;
		queue.pop(root_tmp);
		settextstyle(8, HORIZ_DIR, 2);
		if (root_tmp == root)
		{
			Del_Node(root);
			Draw_Node(root, 14);
			delay(TIME / 2);
		}
		else
		{
			setbkcolor(15);
			Show_Link(back[root_tmp], root_tmp);
			delay(TIME / 2);
			Del_Node(root_tmp);
			Draw_Node(root_tmp, 14);
			setbkcolor(15);
			setcolor(2);
			settextstyle(8, HORIZ_DIR, 1);
			outtextxy(500 + 30 * dem, 600, "->");
			dem++;
			setcolor(4);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(500 + 30 * dem, 600, dt.node[root_tmp].name);
			delay(TIME / 2);
			dem++;
		}
		while (!ds_ke[root_tmp].empty())
		{
			int i;
			ds_ke[root_tmp].pop(i); //xoa dinh ke dau tien ra khoi hang doi ds_ke[root_tmp]
			if (visited[i] == false)
			{
				queue.push(i);
				visited[i] = true;
				back[i] = root_tmp;
			}
		}
	}
	settextstyle(8, HORIZ_DIR, 2);
	setcolor(2);
	outtextxy(468, 640, "DA DUYET XONG !");
}
///////////////////////////////////////////////////DUONG DI NGAN NHAT TU X->Y////////////////////////////////////////////////////
void Dijkstra(int start, int finish)
{
	Queue ds_ke[dt.TongSo_Dinh + 1]; //tao danh sach ke cho tung dinh ,. FIFO: First In First Out
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
			if (dt.MT[i][j] > 0)
				ds_ke[i].push(j);

	int TrongSo[dt.TongSo_Dinh + 1][dt.TongSo_Dinh + 1];
	int distance[dt.TongSo_Dinh + 1];
	int back[dt.TongSo_Dinh + 1]; //diem truoc CONNECT voi diem dang xet
	bool visited[dt.TongSo_Dinh + 1];

	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		visited[i] = false;

	visited[start] = true;
	dem = 1; // so luong nut i visited[i]=true
	distance[start] = 0;
	//khoi tao TrongSo dua vao ma tran trong so
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
		{
			if ((dt.MT[i][j] > 0))
				TrongSo[i][j] = dt.MT[i][j]; //neu 2 dinh i,j ke nhau
			else
				TrongSo[i][j] = infinite; //9999
			//khoi tao distance, back
		}

	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		if (visited[i] == false)
		{
			distance[i] = TrongSo[start][i];
			back[i] = start;
		}

	while (dem <= dt.TongSo_Dinh + 1)
	{ //neu chua duyet TRUE het tat ca dinh
		//tim NUT co TRONG SO den nut start NHO NHAT so voi cac nut con lai
		int min_nut = 0, min_trongso = infinite;
		for (int i = 0; i <= dt.TongSo_Dinh; i++)
			if (visited[i] == false)
			{
				if (distance[i] < min_trongso)
				{
					min_trongso = distance[i];
					min_nut = i;
				}
			}

		if (min_nut == finish)
			break;

		visited[min_nut] = true;
		dem++;

		while (!ds_ke[min_nut].empty())
		{
			int i;				   //cac dinh ke cua min_nut
			ds_ke[min_nut].pop(i); //xoa dinh ke i dang xet ra khoi hang doi ds_ke[min_nut]

			if (visited[i] == false)
			{
				if (distance[min_nut] + TrongSo[min_nut][i] < distance[i])
				{
					distance[i] = distance[min_nut] + TrongSo[min_nut][i]; //cap nhat lai distance[i]
					back[i] = min_nut;									   //cap nhat lai back[i]
				}
			}
		}
	}

	if (distance[finish] < infinite)
	{ //sau khi break;
		Notify_screen();
		setcolor(0);
		setbkcolor(15);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(468, 630, "Tong trong so :");
		char ss[10];
		itoa(distance[finish], ss, 10); //char *itoa ( int value, char * str, int co so );
		setcolor(2);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(668, 630, ss);
	}
	else
	{
		Notify_screen();
		setcolor(0);
		setbkcolor(15);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(468, 570, "Khong co duong di tu    ->");
		setcolor(2);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(725, 570, dt.node[start].name);
		outtextxy(800, 570, dt.node[finish].name);
		return;
	}
	Stack st;
	int f = finish;
	st.push(finish);
	while (f != start)
	{
		f = back[f];
		st.push(f);
	}
	int previous;
	int d = 0; // dem,
	while (!st.empty())
	{
		setcolor(2);
		setbkcolor(15);
		settextstyle(0, 0, 2);
		outtextxy(468, 570, "DIJKSTRA :");
		int tmp;
		st.pop(tmp); //LIFO
		if (d == 0)
		{
			previous = tmp;
			setcolor(4);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(468 + 50 * d, 600, dt.node[tmp].name);
			Del_Node(tmp);
			Draw_Node(tmp, 14);
		}
		else
		{
			setcolor(4);
			Show_Link(previous, tmp);
			delay(TIME / 2);

			Del_Node(tmp);
			if (st.empty()) //dinh cuoi
				Draw_Node(tmp, 7);
			else //chua phai dinh cuoi
				Draw_Node(tmp, 10);
			setbkcolor(15);
			setcolor(4);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(468 + 50 * d, 600, dt.node[tmp].name);
			setcolor(2);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(468 + 50 * d - 25, 600, "->");
			previous = tmp; //cap nhat da ve link thanh dinh previous
			delay(TIME / 2);
		}
		d++;
	}
	setbkcolor(15);
	setcolor(2);
	settextstyle(8, HORIZ_DIR, 2);
	outtextxy(468, 660, "DA DUYET XONG !");
}

////////////////////////////////////////////////HAMILTON////////////////////////////////////////////////////////
bool *visited;
int *chu_trinh;
bool kt_ham;
void run()
{
	kt_ham = true;
	Notify_screen();
	Load_Nodes();
	setbkcolor(15);
	setcolor(0);
	settextstyle(8, HORIZ_DIR, 2);
	outtextxy(468, 570, "Chu trinh hamilton:");
	setcolor(4);
	settextstyle(8, HORIZ_DIR, 2);
	outtextxy(700, 570, dt.node[chu_trinh[0]].name); //nut start
	Del_Node(chu_trinh[0]);
	Draw_Node(chu_trinh[0], 10);
	delay(TIME);
	//
	for (int i = 1; i <= dt.TongSo_Dinh + 1; i++)
	{
		setbkcolor(15);
		setcolor(2);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(700 + i * 50 - 25, 570, "->");
		setcolor(4);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(700 + i * 50, 570, dt.node[chu_trinh[i]].name);
		int back = chu_trinh[i - 1], tam = chu_trinh[i];
		Show_Link(back, tam);
		delay(TIME / 2 / 2);
		if (i != dt.TongSo_Dinh + 1)
		{
			Del_Node(chu_trinh[i]);
			Draw_Node(chu_trinh[i], 14);
			delay(TIME);
		}
		else
			Draw_Node(chu_trinh[i], 10);
	}
}
void xuly(int i)
{
	for (int j = 0; j <= dt.TongSo_Dinh; j++)
		if (dt.MT[chu_trinh[i - 1]][j] > 0 && !kt_ham)
			if (i == dt.TongSo_Dinh + 1 && j == chu_trinh[0])
			{
				chu_trinh[i] = j;
				run();
				delay(TIME);
			}
			else if (visited[j] == false)
			{
				chu_trinh[i] = j;
				visited[j] = true;
				xuly(i + 1);
				visited[j] = false;
			}
}

void Hamilton(int start)
{
	visited = new bool[dt.TongSo_Dinh + 1]; //
	chu_trinh = new int[dt.TongSo_Dinh + 2];
	kt_ham = false;
	for (int j = 0; j <= dt.TongSo_Dinh; j++)
		visited[j] = false;

	chu_trinh[0] = start;
	visited[start] = true;
	xuly(1);
	if (!kt_ham)
	{
		Notify_screen();
		setbkcolor(15);
		setcolor(4);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(468, 570, "Khong co chu trinh Hamilton");
	}
	delete[] visited;
	delete[] chu_trinh;
}

////////////////////////////////////////////////SUPPORT FOR TPLT/////////////////////////////////////////////

int *TPLT_Num, *TPLT_Low, cnt, TPLT_Count;
bool *TPLT_Connect;
Queue TPLT_a[MAX_NUT ];
Stack st;
void TPLT_dfs(int u)
{
	TPLT_Low[u] = TPLT_Num[u] = cnt++; //gan thoi gian, dinh danh
	st.push(u);						   //
	TPLT_Connect[u] = true;
	while (!TPLT_a[u].empty())
	{
		int v;
		TPLT_a[u].pop(v);
		if (TPLT_Num[v] == -1)
			TPLT_dfs(v); // duyet dfs
		if (TPLT_Connect[v])
			TPLT_Low[u] = min(TPLT_Low[u], TPLT_Low[v]); // neu v con trong ngan xep -> cap nhap lai dinh danh TPLT_Low[u]
	}
	if (TPLT_Num[u] == TPLT_Low[u])
	{ // u la goc cua mot thanh phan lien thong manh chua u
		TPLT_Count++;
		// lay cac dinh la thanh phan lien thong manh trong ngan xep ra
		while (!st.empty())
		{
			int v;
			st.pop(v);
			TPLT_Connect[v] = false;
			if (u == v)
				break;
		}
	}
}
int TPLT(int **tmp, bool f = true)
{
	////kouuu
	cnt = 0; // thoi gian
	TPLT_Count = 0;
	TPLT_Num = new int[MAX_NUT ];	  // TPLT_Num[u] danh dau thoi gian diem ma u duoc tham -> Dinh Danh
	TPLT_Low = new int[MAX_NUT ];	  // TPLT_Low[u] dinh danh cua w bat ki. co duong di tu u -> w, w thuoc TPLT
	TPLT_Connect = new bool[MAX_NUT ]; //TPLT_Connect[u] kiem tra dinh u con trong do thi hay khong
	bool check[MAX_NUT ];
	// tao danh sach ke
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
			if (tmp[i][j] > 0)
				TPLT_a[i].push(j);

	// reset gia tri
	for (int i = 0; i <= MAX_NUT - 1; i++)
	{
		TPLT_Num[i] = -1;
		TPLT_Low[i] = 0;
		TPLT_Connect[i] = false;
		check[i] = true;
	}
	///// run
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		if (TPLT_Num[i] == -1)
			TPLT_dfs(i);
	/////// In ra man hinh
	int d = -1;
	if (f)
	{
		int Color[] = {3, 7, 9, 10, 11, 14, 13, 1, 2, 5, 6, 8, 12, 0};
		int sizeColor = sizeof(Color) / sizeof(Color[0]);
		int Color_name = 0;
		for (int i = 0; i <= TPLT_Count - 1; i++)
		{
			d++;
			setcolor(0);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(478 + (30 * d) - 15, 590, "(");
			d--;
			for (int j = 0; j <= dt.TongSo_Dinh; j++)
				if (check[j])
				{
					for (int k = j; k <= dt.TongSo_Dinh; k++)
						if (TPLT_Low[k] == TPLT_Low[j])
						{
							check[k] = false;
							if (TPLT_Count <= sizeColor && Color_name < sizeColor)
							{
								Del_Node(k);
								Draw_Node(k, Color[Color_name]);
							}
							setcolor(2);
							d++;
							outtextxy(478 + (30 * d), 590, dt.node[k].name);
						}
					break;
				}
			Color_name++;
			d++;
			setcolor(0);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(478 + 30 * d, 590, ")");
		}
		Notify_screen();
		setbkcolor(15);
		setcolor(0);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(468, 570, "So thanh phan lien thong la :");
		char xau[10];
		itoa(TPLT_Count, xau, 10);
		setcolor(2);
		outtextxy(820, 570, xau);
	}
	/////// clear
	delete[] TPLT_Num;
	delete[] TPLT_Low;
	delete[] TPLT_Connect;
	////
	return TPLT_Count;
}
//////////////
bool check_duongdi_LT(int u, int v, int **tmp, bool f = true)
{ // f=true => check duong di; f=false => check Lien Thong do thi vo huong
	Queue queue;
	bool check[dt.TongSo_Dinh + 1];
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		check[i] = false;
	queue.push(u);
	check[u] = true;
	while (!queue.empty())
	{
		int s;
		queue.pop(s);
		////////
		for (int i = 0; i <= dt.TongSo_Dinh; i++)
			if (!check[i] && tmp[s][i] > 0)
			{
				queue.push(i);
				check[i] = true;

				if (v == i && f)
					return true;
			}
	}
	if (!f)
	{
		for (int i = 0; i <= dt.TongSo_Dinh; i++)
			if (!check[i])
				return false;
		return true;
	}
	return false;
}

///////////////////////////////////////////////////DINH THAT///////////////////////////////////////////////
void Dinh_That(int u, int v)
{	/// u dinh dau, v la dinh cuoi
	///init
	int **tmp = init_tmp();
	int count = 0;
	int t = 0;
	//////
	Copy_MT(tmp); //copy MT
	if (!check_duongdi_LT(u, v, tmp))
	{
		////////////delete
		Notify_screen();
		setcolor(0);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(468, 570, "Khong co dinh that giua 2 dinh ");
		outtextxy(890, 570, "->");
		settextstyle(8, HORIZ_DIR, 1);
		outtextxy(865, 570, dt.node[u].name);
		outtextxy(925, 570, dt.node[v].name);

		for (int i = 0; i <= dt.TongSo_Dinh; i++)
			delete[] tmp[i];
		delete[] tmp;
		return;
	}
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		if (i != u && i != v)
		{
			Co_Lap_Dinh(tmp, i);
			if (!check_duongdi_LT(u, v, tmp))
			{
				count++;
				/// draw node
				Del_Node(i);
				Draw_Node(i, 14); ////////////////////
				setcolor(2);
				setbkcolor(15);
				settextstyle(8, HORIZ_DIR, 1);
				outtextxy(900 + 50 * t, 570, dt.node[i].name);
				t++;
			}
		}

	if (count == 0)
	{
		Notify_screen();
		setcolor(0);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(468, 570, "Khong co dinh that giua 2 dinh ");
		outtextxy(890, 570, "->");
		setcolor(2);
		settextstyle(8, HORIZ_DIR, 1);
		outtextxy(865, 570, dt.node[u].name);
		outtextxy(925, 570, dt.node[v].name);
	}
	////////////delete
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		delete[] tmp[i];

	delete[] tmp;
}
//////////////////////////////////////////////////////DINH TRU////////////////////////////////////////////////////////////////////////
void dinh_tru()
{
	//init
	int **tmp = init_tmp();
	Copy_MT(tmp);
	int Min = TPLT(tmp, false), count = 0;
	Notify_screen();
	int d = 0;
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		Co_Lap_Dinh(tmp, i);
		if (TPLT(tmp, false) - 1 > Min)
		{
			d++;
			Del_Node(i);
			Draw_Node(i, 14);
			setcolor(4);
			setbkcolor(15);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(650 + d * 50, 570, dt.node[i].name);
		}
	}

	if (d == 0)
	{
		Notify_screen();
		setcolor(4);
		setbkcolor(15);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(468, 570, "Khong co dinh tru");
	}
	else
	{
		setcolor(0);
		outtextxy(468, 570, "Co ");
		char xau[10];
		itoa(d, xau, 10);
		setcolor(12);
		outtextxy(495, 570, xau);
		setcolor(0);
		outtextxy(520, 570, "dinh tru :");
	}
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		delete[] tmp[i];
	delete[] tmp;
}

///////////////////////////////////////////////////CANH CAU///////////////////////////////////////////////
void Canh_Cau()
{
	//init
	Queue canh;
	Queue link;
	int **tmp = init_tmp();
	Copy_MT(tmp);
	int Min = TPLT(tmp, false);
	Notify_screen();

	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
		{
			Xoa_Canh(tmp, i, j);
			if (TPLT(tmp, false) != Min)
			{
				canh.push(i);
				link.push(i);
				Hoan_Canh(tmp, i, j);
			}
		}
	}
	int previous;
	int dem = 0;
	while (!link.empty())
	{
		int tam;
		link.pop(tam);
		setbkcolor(15);
		if (dem == 0)
		{
			previous = tam;
			dem = 1;
		}
		else if (dem == 1)
		{
			Show_Link(previous, tam);
			delay(TIME / 2);
			dem = 2;
		}
		else
		{
			previous = tam;
			dem = 1;
		}
	}
	int d = 0;
	while (!canh.empty())
	{
		int tam;
		canh.pop(tam);
		setcolor(4);
		setbkcolor(15);
		outtextxy(475 + 50 * d, 600, dt.node[tam].name);
		Del_Node(tam);
		Draw_Node(tam, 10);
		setbkcolor(15);
		if (d % 2 == 0)
			outtextxy(465 + 50 * (d), 600, "(");
		else
			outtextxy(500 + 50 * (d), 600, ")");
		d++;
	}
	for (int i = 0; i < d / 2; i++)
	{
		setcolor(4);
		outtextxy(510 + 100 * i, 600, ",");
	}
	if (d == 0)
	{
		Notify_screen();
		setcolor(4);
		setbkcolor(15);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(468, 570, "Khong co canh cau");
	}
	else
	{
		setbkcolor(15);
		setcolor(0);
		outtextxy(468, 570, "Co  canh cau");
		settextstyle(8, HORIZ_DIR, 2);
		char xau[10];
		itoa(d / 2, xau, 10);
		setcolor(4);
		outtextxy(495, 570, xau);
	}
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		delete[] tmp[i];
	delete[] tmp;
}
///////////////////////////////////////////////////EULER///////////////////////////////////////////////
bool kiemtra_LT(int **tmp)
{
	Copy_MT(tmp);
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
			if (tmp[i][j] > 0)
			{
				tmp[i][j] = 1;
				tmp[j][i] = 1;
			}
	if (check_duongdi_LT(0, -1, tmp, false))
		return true;
	return false;
}
bool kiemtra_deg(int **tmp)
{
	Copy_MT(tmp);
	int deg1 = 0, deg2 = 0;
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
	{
		deg1 = 0;
		deg2 = 0;
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
		{
			deg1 += tmp[i][j];
			deg2 += tmp[j][i];
		}
		if (deg1 != deg2)
			return false;
	}
	return true;
}
void EulerCycle(int **tmp, int u)
{
	//init
	// tao danh sach ke
	Queue ke[dt.TongSo_Dinh + 1];
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		for (int j = 0; j <= dt.TongSo_Dinh; j++)
			if (tmp[i][j] > 0)
				ke[i].push(j);

	///
	Stack stack, CE;
	stack.push(u);
	while (!stack.empty())
	{
		int s = stack.get();
		if (!ke[s].empty())
		{
			int t;		  
			ke[s].pop(t); 
			stack.push(t);
		}
		else
		{				  
			stack.pop(s); 
			CE.push(s);	  
		}
	}
	int stt = 0;  // danh dau diem dau tien mau vang
	int back = u; // ve net cung dau tien
	while (!CE.empty())
	{
		int t;
		CE.pop(t);
		if (stt == 0)
		{
			//draw
			Del_Node(t);
			Draw_Node(t, 10);
			//
			Notify_screen();
			setbkcolor(15);
			setcolor(2);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(468, 570, "Chu trinh EULER :");
			setcolor(4);
			settextstyle(8, HORIZ_DIR, 1);
			outtextxy(468, 600, dt.node[t].name);
		}
		else
		{
			///// draw
			setbkcolor(15);
			Show_Link(back, t);
			setcolor(4);
			settextstyle(8, HORIZ_DIR, 1);
			outtextxy(468 + 50 * stt, 600, dt.node[t].name);
			setcolor(2);
			settextstyle(8, HORIZ_DIR, 2);
			outtextxy(468 + 50 * stt - 25, 600, "->");
			Del_Node(t);
			Draw_Node(t, 14);
			delay(TIME);
		}
		stt++;
		back = t;
	}
}
void Euler(int u)
{
	////init
	int **tmp = init_tmp();
	////kiem tra
	if (kiemtra_LT(tmp) && kiemtra_deg(tmp) && dt.TongSo_Dinh > 0)
	{
		Load_Nodes();
		Copy_MT(tmp);
		EulerCycle(tmp, u);
	}
	else
	{
		Notify_screen();
		setcolor(0);
		settextstyle(8, HORIZ_DIR, 2);
		outtextxy(468, 570, "Khong co chu trinh Euler");
	}
	/////delete tmp
	for (int i = 0; i <= dt.TongSo_Dinh; i++)
		delete[] tmp[i];
	delete[] tmp;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	working = false;
	Start_screen();
	dt.TongSo_Dinh = -1;
	while (1)
	{
		int x = mousex();
		int y = mousey();
		if (ismouseclick(WM_LBUTTONDOWN))
		{
			getmouseclick(WM_LBUTTONDOWN, x, y);
			////////////////////////////////////////////////NEW//////////////////////////////////////////////////////////////////////
			if (kt_rect(10, 8, 100, 40, x, y))
			{
				setfillstyle(1, 15);
				bar(0, 0, 1420, 815);
				home();
				start();
				settextstyle(0, 0, 2);
				Nnew(true);
				Set_False();
				working = true; //1
			}
			////////////////////////////////////////////////ADD//////////////////////////////////////////////////////////////////////
			if (kt_rect(400, 8, 500, 40, x, y))
			{
				Load_Nodes();
				Algorithm_tool_screen();
				Add(true);
				if (dt.TongSo_Dinh > MAX_NUT - 2)
				{
					Notify_screen();
					setcolor(0);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "DA DU       DINH!!");
					char chuoi[10];			  //
					itoa(MAX_NUT, chuoi, 10); //char *itoa ( int value, char * str, int cos so );
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(550, 570, chuoi);
					Set_False();
					continue;
				}
				Notify_screen();
				if (working) ////chi ADD duoc khi da NEW do thi hoac OPEN do thi
				{
					Notify_screen();
					Set_True(kt_Add);
					settextstyle(8, HORIZ_DIR, 2);
					setcolor(0);
					setbkcolor(15);
					outtextxy(468, 570, "Click chuot trai vao khoang trong de tao dinh moi!");
					outtextxy(468, 590, "So dinh <= 15");
				}
				else
				{
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc!");
				}
			}

			if (kt_Add && kt_rect(485, 80, 1375, 525, x, y)) //add++
			{
				if (dt.TongSo_Dinh > MAX_NUT - 2) //
				{
					Notify_screen();
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Da du dinh, nen khong the them!");
					char chuoi[10];			  //
					itoa(MAX_NUT, chuoi, 10); //char *itoa ( int value, char * str, int cos so );
					outtextxy(550, 570, chuoi);
					continue;
				}

				bool kiemtra = false;
				for (int i = 0; i <= dt.TongSo_Dinh; i++) // kiem tra khoang cach den cac diem con lai
					if (kc(x, y, dt.node[i].x, dt.node[i].y) <= 45)
					{
						kiemtra = true;
						break;
					}
				if (kiemtra == true)
					continue;
				tao_node(x, y, 4);
				Notify_screen();
				setcolor(0);
				settextstyle(8, HORIZ_DIR, 2);
				outtextxy(468, 570, "Nhap ten dinh :");
				outtextxy(468, 590, "(Ten dinh theo dinh dang: 00->99 va khac ten da ton tai)");
				outtextxy(468, 610, "So dinh <=15");
				setcolor(4);
				char s[2];
				for (int i = 0; i <= 2; i++)
					s[i] = '\0'; //ngat chuoi o ki tu thu 2
				int d = 0;
				char xx;
				while (true) //vd nhap 36
				{
					xx = getch(); //xx1=3		,xx2=9  , xx3=6
					if ((('0' <= xx && xx <= '9') || xx == 13 || xx == 8))
					{
						if (d == 0 && '0' <= xx && xx <= '9')
						{
							s[0] = xx;
							d = 1; //s[0]=3
						}
						else if (d == 1)
						{
							if ('0' <= xx && xx <= '9')
							{
								s[1] = xx; //s[1]==xx2=9
								d = 2;
							}
							else if (xx == 8)
							{ //sau khi nhan phim backspace thi ngat chuoi
								for (int i = 0; i <= 2; i++)
								{
									s[i] = '\0';
								} //s[0]='\0' =>>xoa luon phan tu dau
								d = 0;
							}
						}
						else if (d == 2)
						{
							if (xx == 8)
							{ //sau khi nhan phim backspace thi ngat chuoi
								for (int i = 1; i <= 2; i++)
								{
									s[i] = '\0'; //s[0]=3, s[1]='\0'
								}
								d = 1;
							}
							else if ('0' <= xx && xx <= '9')
							{
								s[1] = xx; //s[1]=6 update s[1] cho moi lan nhan getch() moi
								d = 2;
							}
							else if (xx == 13)
							{
								bool kt2 = true;
								for (int i = 0; i <= dt.TongSo_Dinh; i++)
									if (dt.node[i].name[0] == s[0] && dt.node[i].name[1] == s[1])
									{
										kt2 = false;
										setcolor(4);
										outtextxy(468, 630, "Ten dinh da ton tai, moi nhap ten khac !!");
										delay(TIME);
									}
								if (kt2)
								{
									dt.TongSo_Dinh++;
									dt.init_Node(1, 1);
									dt.init_MT(1, 1);
									dt.node[dt.TongSo_Dinh].x = x; //dua vi tri x=x  cho node [TongSo_Dinh].x
									dt.node[dt.TongSo_Dinh].y = y; //dua vi tri y=y  cho node [TongSo_Dinh].y
									dt.node[dt.TongSo_Dinh].name[0] = s[0];
									dt.node[dt.TongSo_Dinh].name[1] = s[1];
									dt.node[dt.TongSo_Dinh].name[2] = '\0';
									Load_Nodes();
									load_matrix();
									Notify_screen();
									break;
								}
							}
						}
						Notify_screen();
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(0);
						outtextxy(468, 570, "Nhap ten dinh :");
						outtextxy(468, 590, "(Ten dinh theo dinh dang: 00->99 va khac ten da ton tai)");
						outtextxy(468, 610, "So dinh <=15");
						setcolor(12);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(768, 570, s);
					}
				}
			}
			////////////////////////////////////////////////////////open//////////////////////////////////////////////////////////////////////
			if (kt_rect(120, 8, 220, 40, x, y))
			{
				setfillstyle(1, 15);
				bar(0, 0, 1420, 815);
				home();
				settextstyle(0, 0, 2);
				Nopen(true);
				Set_False();
				working = true;
				Openfile();
			}
			////////////////////////////////////////////////////////Save//////////////////////////////////////////////////////////////////////
			if (kt_rect(240, 6, 350, 40, x, y))
			{
				Algorithm_tool_screen();
				Nsave(true);
				Load_Nodes();
				Set_False();
				if (working == false || dt.TongSo_Dinh == -1)
				{ //chi Save duoc khi da NEW do thi hoac OPEN do thi
					Notify_screen();
					setbkcolor(15);
					setcolor(4);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Chua co do thi");
					delay(TIME);
					Notify_screen();
				}
				else
				{
					if (kt_Save == false)
						Save(); //opened=>Save file moi
					Save2();	//Save chong len file cu
				}
				Algorithm_tool_screen();
			}
			////////////////////////////////////////////////topo//////////////////////////////////////////////////////////////////////
			if (kt_rect(320, 475, 430, 525, x, y))
			{
				Algorithm_tool_screen();
				Work_screen();
				Notify_screen();
				settextstyle(0, 0, 2);
				Top(true);
			}
			////////////////////////////////////////////////delete//////////////////////////////////////////////////////////////////////
			if (kt_rect(520, 8, 620, 40, x, y))
			{
				Notify_screen();
				Load_Nodes();
				Algorithm_tool_screen();
				Delete(true);
				settextstyle(8, HORIZ_DIR, 2);
				if (working) //chi DELETE duoc khi da NEW do thi hoac OPEN do thi
				{
					Notify_screen();
					Set_True(kt_Del);
					if (dt.TongSo_Dinh == -1)
					{
						Notify_screen();
						setcolor(4);
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Do thi khong co dinh => khong the xoa!");
						outtextxy(468, 590, "Ban phai mo do thi hoac tao do thi moi truoc!");
						Set_False();
					}
					else
					{
						setcolor(4);
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Click chuot trai vao dinh can xoa!");
					}
				}
				else
				{
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Ban can mo hoac tao do thi moi truoc!");
				}
			}

			if (kt_Del && kt_rect(485, 80, 1375, 525, x, y)) //delete++
			{
				if (dt.TongSo_Dinh == -1)
				{
					Notify_screen();
					setcolor(4);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Do thi khong co dinh => khong the xoa");
					continue;
				}
				for (int i = 0; i <= dt.TongSo_Dinh; i++)
					if (kc(x, y, dt.node[i].x, dt.node[i].y) <= 20)
					{
						dt.init_Node(-1, i);
						dt.TongSo_Dinh--;
						dt.init_MT(-1, i);
						break;
					}
				Load_Nodes();
				load_matrix();
			}
			////////////////////////////////////////////////move//////////////////////////////////////////////////////////////////////
			if (kt_rect(640, 8, 740, 40, x, y))
			{
				Notify_screen();
				Load_Nodes();
				Algorithm_tool_screen();
				Notify_screen();
				settextstyle(0, 0, 2);
				Move(true);
				if (working) //chi MOVE duoc khi da NEW haoc OPEN
				{
					if (dt.TongSo_Dinh == -1)
					{
						Notify_screen();
						setcolor(4);
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Do thi khong co dinh");
						Set_False();
					}
					else
					{
						Notify_screen();
						Set_True(kt_Move);
						setcolor(0);
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Click chuot trai lan 1 vao dinh de chon dinh!");
						outtextxy(468, 600, "Click chuot trai lan 2 vao khoang trong de di chuyen dinh toi toa do moi!");
					}
				}
				else
				{
					settextstyle(8, HORIZ_DIR, 2);
					setcolor(4);
					setbkcolor(15);
					outtextxy(468, 570, "Ban can mo do thi hoac tao do thi moi truoc!");
				}
			}

			if (kt_Move && kt_rect(485, 80, 1375, 525, x, y))
			{
				if (dem == 0)
				{
					for (int i = 0; i <= dt.TongSo_Dinh; i++)
						if (kc(x, y, dt.node[i].x, dt.node[i].y) <= 20)
						{ //vitri mouse so vs vitri nut [i] can di chuyen khong qua 20pixel
							dem = 1;
							tam = i;
							tao_node(dt.node[tam].x, dt.node[tam].y, 12);
							Draw_Node(i, 14);
							break;
						}
				}
				else if (dem == 1)
				{ //da chon duoc nut can MOVE
					bool kiemtra = true;
					for (int i = 0; i <= dt.TongSo_Dinh; i++)
						if (kc(x, y, dt.node[i].x, dt.node[i].y) <= 20 and dt.node[i].x <= 1420 and dt.node[i].y <= 550)
							kiemtra = false; //neu khoang cach vi tri chuot( vitri nut moi) so vs nut cu >=45pixel =>cho tao nut co vitri moi
					if (kiemtra == true)
					{
						dt.node[tam].x = x; //gan vitri nut cu (x ) cho nut moi
						dt.node[tam].y = y; //gan vitri nut cu (y ) cho nut moi
						Load_Nodes();
						dem = 0;
					}
				}
			}
			///////////////////////////////////////////////////connect///////////////////////////////////////////////////////////////////
			if (kt_rect(760, 8, 880, 40, x, y))
			{
				Notify_screen();
				Load_Nodes();
				Algorithm_tool_screen();
				Connect(true);
				settextstyle(8, HORIZ_DIR, 2);
				if (working)
				{
					dem = 0;
					if (dt.TongSo_Dinh < 1)
					{
						Notify_screen();
						settextstyle(8, HORIZ_DIR, 2);
						setcolor(4);
						setbkcolor(15);
						outtextxy(468, 570, "Ban can mo do thi hoac tao do thi moi truoc!");
						Set_False();
					}
					else
					{
						Notify_screen();
						Set_True(kt_Connect);
						setcolor(0);
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Click chuot trai lan luot vao A va B de tao cung!");
						outtextxy(468, 600, "Neu cung AB da ton tai, click lan 2 de xoa hoac chinh sua!");
					}
				}
				else
				{
					setcolor(4);
					setbkcolor(15);
					outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc!");
				}
			}
			if (kt_Connect && dt.TongSo_Dinh >= 1 && kt_rect(485, 80, 1375, 525, x, y)) //co it nhat 2 nut
			{
				settextstyle(8, HORIZ_DIR, 2);
				if (dem == 0)
				{
					for (int i = 0; i <= dt.TongSo_Dinh; i++)
						if (kc(x, y, dt.node[i].x, dt.node[i].y) <= 20)
						{
							dem = 1;
							tam1 = i;
							tao_node(dt.node[tam1].x, dt.node[tam1].y, 12);
							Draw_Node(i, 14);
							break;
						}
				}
				else if (dem == 1)
				{
					bool kt1 = false, kt2 = false, kt3 = false; //kt1: kiemtra xem diem B co trung voi diem A khong, KHONG=>kt1=true;

					for (int i = 0; i <= dt.TongSo_Dinh; i++)
						if (kc(x, y, dt.node[i].x, dt.node[i].y) <= 20 && i != tam1)
						{
							kt1 = true;
							tam2 = i;
							tao_node(dt.node[tam2].x, dt.node[tam2].y, 12);
							Draw_Node(i, 14);
							break;
						}

					if (kt1 == false)
						continue;

					for (int i = 0; i <= dt.TongSo_Dinh; i++)
						for (int j = 0; j <= dt.TongSo_Dinh; j++)
							if (dt.MT[tam1][tam2] != 0)
								kt2 = true; //kt2: kiem tra co cung tu A->B khong

					for (int i = 0; i <= dt.TongSo_Dinh; i++)
						for (int j = 0; j <= dt.TongSo_Dinh; j++)
							if (dt.MT[tam2][tam1] != 0)
								kt3 = true; //kt2: kiem tra co cung tu B->A khong

					if (kt1 == true && kt2 == false && kt3 == true)
					{ //A khong trung B, khong co A->B, co B->A
						Notify_screen();
						setbkcolor(15);
						setcolor(0);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Nhap trong so cung: ");
						outtextxy(468, 590, "0 < Trong so < 1000");

						setcolor(2);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(800, 570, dt.node[tam1].name);
						outtextxy(880, 570, dt.node[tam2].name);

						settextstyle(8, HORIZ_DIR, 1);
						outtextxy(840, 570, "->");
						outtextxy(920, 570, ":");

						char s[20];
						int d = 0;
						setcolor(4);
						while (true)
						{
							s[d] = getch();
							if ((('0' <= s[d] && s[d] <= '9') || s[d] == 13 || s[d] == 8))
							{
								if (d == 3 && s[d] != 13 && s[d] != 8)
								{
									continue;
								} //d<=2

								else if (s[d] == 13)
								{
									if (d == 0)
										continue;
									else
									{ //d=1 or 2
										settextstyle(8, HORIZ_DIR, 2);
										char xau[d];
										for (int i = 0; i <= d - 1; i++)
										{
											xau[i] = s[i];
											xau[d] = '\0';
										}
										int t = atoi(xau); //atoi: transform str to int
										dt.MT[tam1][tam2] = t;
										Notify_screen();
										Load_Nodes();
										load_matrix();
										break;
									}
								}
								else if (s[d] == 8)
								{
									setfillstyle(1, 15);
									setbkcolor(15);
									bar(950 + 10 * (d - 1), 570, 950 + 10 * (d), 590);
									d--;
									if (d == -1)
										d = 0;
									continue;
								}
								else
								{
									setcolor(12);
									OutPut(950 + 10 * d, 570, s[d]);
									d++;
								}
							}
						}
					}
					if (kt1 == true && kt2 == false && kt3 == false) //A khong trung B, khong co A->B, khong co B->A
					{
						dem = 0;
						Notify_screen();
						setcolor(0);
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Nhap trong so cung: ");
						outtextxy(468, 590, "0 < Trong so < 1000");

						setcolor(2);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(800, 570, dt.node[tam1].name);
						outtextxy(880, 570, dt.node[tam2].name);

						settextstyle(8, HORIZ_DIR, 1);
						outtextxy(840, 570, "->");
						outtextxy(920, 570, ":");

						setcolor(4);
						char s[20];
						int d = 0;
						fflush(stdin); //xoa bo dem, vd bo ky tu ENTER
						while (true)
						{
							s[d] = getch();
							if ((('0' <= s[d] && s[d] <= '9') || s[d] == 13 || s[d] == 8))
							{
								if (d == 3 && s[d] != 13 && s[d] != 8)
								{
									continue;
									d--;
								} //d<=2
								if (s[d] == 13)
								{
									if (d == 0)
										continue;
									else
									{ //d=1 or 2
										char xau[d];
										for (int i = 0; i <= d - 1; i++)
										{
											xau[i] = s[i];
											xau[d] = '\0'; //ket thuc chuoi
										}
										int t = atoi(xau); //atoi: transform str to int
										dt.MT[tam1][tam2] = t;
										Notify_screen();
										Load_Nodes();
										load_matrix();
										break;
									}
								}
								else if (s[d] == 8)
								{
									setfillstyle(1, 15);
									bar(950 + 10 * (d - 1), 570, 950 + 10 * (d), 590);
									d--;
									if (d == -1)
										d = 0;
									continue;
								}
								else
								{
									setcolor(12);
									OutPut(950 + 10 * d, 570, s[d]);
									d++;
								}
							}
						}
					}
					bool kt_Change = false, kt_Remove = false, kt_Back = false;
					dem = 0;
					if (kt1 == true && kt2 == true) //A khong trung B, khong co B->A
					{
						Notify_screen();
						setcolor(2);
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "0  Quay lai");
						outtextxy(468, 590, "1  Xoa cung");
						outtextxy(468, 610, "2  Chinh sua trong so");
						outtextxy(468, 630, "nhap lua chon :");
						setcolor(4);
						int s[2];
						fflush(stdin);
						int count = 0;
						while (true)
						{
							if (count = 0)
							{
								s[0] = getch();
							}
							if ((s[0] == '0' || s[0] == '1' || s[0] == '2'))
							{
								setcolor(2);
								OutPut(654, 630, s[0]);
							}
							if (count = 1)
							{
								s[1] = getch();
							}
							if ((s[1] == '0' || s[1] == '1' || s[1] == '2' || s[1] == 13))
							{
								if (s[1] == 13)
								{
									if (s[0] == '0')
										kt_Back = true; //quay lai
									else if (s[0] == '1')
										kt_Remove = true; //xoa cung
									else if (s[0] == '2')
										kt_Change = true; //chinh trong so
									break;
								}
								else
								{
									s[0] = s[1];
									count = 0;
								}
							}
						}
						s[0] = '\0';
					}
					if (kt_Back == true)
					{
						Load_Nodes();
						load_matrix();
						Notify_screen();
					}
					if (kt_Remove == true)
					{
						dt.MT[tam1][tam2] = 0; //xoa cung dang xet
						Load_Nodes();
						load_matrix();
						Notify_screen();
					}
					if (kt_Change)
					{
						tao_node(dt.node[tam1].x, dt.node[tam1].y, 4);
						Draw_Node(tam1, 14);
						tao_node(dt.node[tam2].x, dt.node[tam2].y, 4);
						Draw_Node(tam2, 14);
						Notify_screen();

						setcolor(0);
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Nhap trong so cung: ");
						outtextxy(468, 590, "0 < Trong so < 1000");

						setcolor(2);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(800, 570, dt.node[tam1].name);
						outtextxy(880, 570, dt.node[tam2].name);

						settextstyle(8, HORIZ_DIR, 1);
						outtextxy(840, 570, "->");
						outtextxy(920, 570, ":");

						setcolor(4);
						char s[15];
						int d = 0;
						fflush(stdin);
						while (true)
						{
							s[d] = getch();
							if ((('0' <= s[d] && s[d] <= '9') || s[d] == 13 || s[d] == 8))
							{
								if (d == 3 && s[d] != 13 && s[d] != 8)
								{
									s[2] = s[3];
									d--;
								}
								else if (s[d] == 13)
								{
									if (d == 0)
										continue;
									else
									{ //d=1 or 2
										char xau[d];
										for (int i = 0; i <= d - 1; i++)
										{				   //d-1=0
											xau[i] = s[i]; //xau[0] = 9
											xau[d] = '\0'; //xau[1]=\0
										}
										int t = atoi(xau); //atoi: transform str to int
										Notify_screen();
										dt.MT[tam1][tam2] = t;
										Load_Nodes();
										load_matrix();
										break;
									}
								}
								else if (s[d] == 8)
								{
									setfillstyle(1, 15);
									bar(950 + 10 * (d - 1), 570, 950 + 10 * (d), 590);
									d--;
									if (d == -1)
										d = 0;
									else
										continue;
								}
								else
								{
									setcolor(12);
									OutPut(950 + 10 * d, 570, s[d]);
									d++;
								}
							}
						}
					}
				}
				Notify_screen();
				Set_True(kt_Connect);
				settextstyle(8, HORIZ_DIR, 2);
				setbkcolor(15);
				setcolor(0);
				outtextxy(468, 570, "Click chuot trai lan luot vao A va B de tao cung!");
				outtextxy(468, 600, "Neu cung AB da ton tai, click lan 2 de xoa hoac chinh sua!");
			}
			///////////////////////////////////////////////////////Rename///////////////////////////////////////////////////////////////
			if (kt_rect(900, 8, 1015, 40, x, y))
			{
				Notify_screen();
				Load_Nodes();
				Algorithm_tool_screen();
				setbkcolor(15);
				Rename(true);
				if (working) //chi Rename duoc khi da NEW do thi hoac OPEN do thi
				{
					if (dt.TongSo_Dinh == -1)
					{
						Notify_screen();
						setcolor(4);
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Do thi khong co dinh => khong the doi ten!");
						Set_False();
						continue;
					}
					Notify_screen();
					Set_True(kt_Rename);
					setcolor(0);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Click chuot trai vao dinh de doi ten!");
				}
				else
				{
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Ban can mo hoac tao do thi moi truoc!");
				}
			}

			if (kt_Rename && kt_rect(485, 80, 1375, 525, x, y))
			{
				for (int i = 0; i <= dt.TongSo_Dinh; i++)
					if (kc(x, y, dt.node[i].x, dt.node[i].y) <= 20)
					{
						Notify_screen();
						tao_node(dt.node[i].x, dt.node[i].y, 4);
						Draw_Node(i, 14);

						setcolor(0);
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Nhap ten dinh :");
						outtextxy(468, 590, "(Ten dinh theo dinh dang: 00->99 va khac ten da ton tai)");
						outtextxy(468, 610, "So dinh <=15");
						setcolor(4);
						char s[2];
						for (int j = 0; j <= 2; j++)
							s[j] = '\0'; //ngat chuoi o ki tu thu 2
						int d = 0;
						char tmp;
						while (true)
						{
							tmp = getch();
							if ((('0' <= tmp && tmp <= '9') || tmp == 13 || tmp == 8) == false)
								continue;
							if (d == 0)
							{
								if ('0' <= tmp && tmp <= '9')
								{
									s[0] = tmp;
									d = 1;
								}
							}
							else if (d == 1)
							{
								if ('0' <= tmp && tmp <= '9')
								{
									s[1] = tmp;
									d = 2;
								}
								else if (tmp == 8)
								{
									for (int i = 0; i <= 2; i++)
										s[i] = '\0';
									d = 0;
								}
							}
							if (d == 2)
							{
								if ('0' <= tmp && tmp <= '9')
								{
									s[1] = tmp; //s[1]=6 update s[1] cho moi lan nhan getch() moi
									d = 2;
								}
								else if (tmp == 8)
								{
									for (int i = 1; i <= 2; i++)
										s[i] = '\0';
									d = 1;
								}
								else if (tmp == 13)
								{ //d=1 or 2
									settextstyle(8, HORIZ_DIR, 2);
									bool check = true;
									for (int i = 0; i <= dt.TongSo_Dinh; i++)
										if (dt.node[i].name[0] == s[0] && dt.node[i].name[1] == s[1])
										{
											check = false;
											setcolor(4);
											outtextxy(468, 630, "Ten dinh da ton tai, moi nhap ten khac !!");
											delay(TIME);
										}
									if (check)
									{
										dt.node[i].name[0] = s[0];
										dt.node[i].name[1] = s[1];
										dt.node[i].name[2] = '\0';
										Load_Nodes();
										Notify_screen();
										load_matrix();
										break;
									}
								}
							}
							Notify_screen();
							setcolor(0);
							settextstyle(8, HORIZ_DIR, 2);
							outtextxy(468, 570, "Nhap ten dinh :");
							outtextxy(468, 590, "(Ten dinh theo dinh dang: 00->99 va khac ten da ton tai)");
							outtextxy(468, 610, "So dinh <=15");
							setcolor(12);
							outtextxy(768, 570, s);
						}
					}
			}

			////////////////////////////////////////////////////////////////////////////////DFS//////////////////////////////////////////////////////////////////////
			if (kt_rect(25, 475, 140, 525, x, y))
			{
				Algorithm_tool_screen();
				settextstyle(0, 0, 2);
				DFS(true);
				Load_Nodes();
				if (working == false)
				{
					Notify_screen();
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc!");
					continue;
				}
				if (dt.TongSo_Dinh < 0)
				{
					Notify_screen();
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Do thi chua co dinh!");
					Set_False();
					continue;
				}
				Set_True(kt_DFS);
				Notify_screen();
				setcolor(0);
				setbkcolor(15);
				settextstyle(8, HORIZ_DIR, 2);
				outtextxy(468, 570, "Click vao dinh bat ki de duyet DFS");
			}
			if (kt_DFS == true && kt_rect(485, 80, 1375, 525, x, y))
			{
				int temp = -1;
				for (int i = 0; i <= dt.TongSo_Dinh; i++)
				{
					if (kt_circle(x, y, dt.node[i].x, dt.node[i].y))
						temp = i;
				}
				if (temp != -1)
				{
					Load_Nodes();
					DFS(temp);
					kt_DFS = false;
				}
			}

			////////////////////////////////////////////////////////////////////////////////BFS//////////////////////////////////////////////////////////////////////
			if (kt_rect(170, 475, 290, 525, x, y))
			{
				Algorithm_tool_screen();
				settextstyle(0, 0, 2);
				BFS(true);
				Load_Nodes();
				if (working == false)
				{
					Notify_screen();
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc!");
					continue;
				}
				if (dt.TongSo_Dinh < 0)
				{
					Notify_screen();
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Do thi chua co dinh!");
					Set_False();
					continue;
				}
				Set_True(kt_BFS);
				Notify_screen();
				setcolor(0);
				setbkcolor(15);
				settextstyle(8, HORIZ_DIR, 2);
				outtextxy(468, 570, "Click vao dinh bat ki de duyet DFS");
			}

			if (kt_BFS == true && kt_rect(485, 80, 1375, 525, x, y))
			{
				int temp = -1;
				for (int i = 0; i <= dt.TongSo_Dinh; i++)
				{
					if (kt_circle(x, y, dt.node[i].x, dt.node[i].y))
						temp = i;
				}
				if (temp != -1)
				{
					Load_Nodes();
					BFS(temp);
					kt_BFS = false;
				}
			}
			///////////////////////////////////////////////////////////////////XY///////////////////////////////////////////////////////////////////
			if (kt_rect(320, 540, 430, 590, x, y))
			{
				Algorithm_tool_screen();
				Xy(true);
				Load_Nodes();
				if (working == false)
				{
					Notify_screen();
					setbkcolor(15);
					setcolor(4);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc!");
					continue;
				}
				if (dt.TongSo_Dinh < 1)
				{
					Notify_screen();
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Do thi phai co it nhat 2 dinh!");
					Set_False();
					continue;
				}
				dem = 0;
				Notify_screen();
				setcolor(0);
				settextstyle(8, HORIZ_DIR, 2);
				outtextxy(468, 570, "Click vao dinh bat ki de chon dinh bat dau");
				Set_True(kt_XY);
			}

			if (kt_XY == true && kt_rect(485, 80, 1375, 525, x, y))
			{
				int start;
				int finish;
				if (dem == 0)
				{
					for (int i = 0; i <= dt.TongSo_Dinh; i++)
					{
						if (kt_circle(x, y, dt.node[i].x, dt.node[i].y))
						{
							start = i;
							dem++;
						}
					}
					tao_node(dt.node[start].x, dt.node[start].y, 4);
					Draw_Node(start, 14);
					Notify_screen();
					setcolor(0);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Click vao dinh bat ki de chon dinh ket thuc");
				}
				else
				{
					for (int i = 0; i <= dt.TongSo_Dinh; i++)
						if (kt_circle(x, y, dt.node[i].x, dt.node[i].y) and i != start)
						{
							tao_node(dt.node[i].x, dt.node[i].y, 4);
							Draw_Node(i, 14);
							dem = 0; //da co du 2 dinh start va finish
							finish = i;
						}
					if (dem == 0)
					{ //da co du 2 dinh start va finish
						Notify_screen();
						setcolor(4);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Duong di ngan nhat tu ");
						outtextxy(748, 570, dt.node[start].name);
						outtextxy(800, 570, dt.node[finish].name);

						settextstyle(8, HORIZ_DIR, 1);
						outtextxy(775, 570, "->");
						outtextxy(830, 570, ":");
						Dijkstra(start, finish);
						kt_XY = false;
					}
				}
			}
			////////////////////////////////////////////////////////hamilton////////////////////////////////////////////////////////////////
			if (kt_rect(170, 540, 290, 590, x, y))
			{
				Algorithm_tool_screen();
				Load_Nodes();
				settextstyle(0, 0, 2);
				Hamil(true);
				if (working == false)
				{
					Notify_screen();
					settextstyle(8, HORIZ_DIR, 2);
					setbkcolor(15);
					setcolor(4);
					outtextxy(468, 570, "Ban can phai mo do thi hoac tao do thi moi truoc!");
					continue;
				}
				if (dt.TongSo_Dinh < 0)
				{
					Notify_screen();
					settextstyle(8, HORIZ_DIR, 2);
					setbkcolor(15);
					setcolor(4);
					outtextxy(468, 570, "Do thi chua co dinh!");
					Set_False();
					continue;
				}
				Notify_screen();
				Set_True(kt_Ham);
				Notify_screen();
				settextstyle(8, HORIZ_DIR, 2);
				setbkcolor(15);
				setcolor(0);
				outtextxy(468, 570, "Click vao dinh muon bat dau!");
			}
			if (kt_Ham && kt_rect(485, 80, 1375, 525, x, y))
			{
				for (int i = 0; i <= dt.TongSo_Dinh; i++)
					if (kt_circle(x, y, dt.node[i].x, dt.node[i].y))
					{
						Load_Nodes();
						Hamilton(i);
						break;
					}
			}

			//////////////////////////////////////////////THANH PHAN LIEN THONG/////////////////////////////////////
			if (kt_rect(25, 620, 215, 690, x, y))
			{
				Algorithm_tool_screen();
				Work_screen();
				Notify_screen();
				settextstyle(0, 0, 2);
				Lienthong(true);
				Load_Nodes();
				if (working == false)
				{
					Notify_screen();
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc!");
					continue;
				}
				if (dt.TongSo_Dinh < 0)
				{
					Notify_screen();
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Do thi chua co dinh!");
					Set_False();
					continue;
				}
				Notify_screen();
				TPLT(dt.MT);
				Set_False();
			}
			/////////////////////////////////////////////DINH TRU/////////////////////////////////////
			if (kt_rect(240, 620, 430, 690, x, y))
			{
				Algorithm_tool_screen();
				Work_screen();
				Load_Nodes();
				settextstyle(0, 0, 2);
				Dinhtru(true);
				if (working == false)
				{
					Notify_screen();
					settextstyle(8, HORIZ_DIR, 2);
					setcolor(4);
					setbkcolor(15);
					outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc!");
					continue;
				}
				if (dt.TongSo_Dinh < 0)
				{
					Notify_screen();
					settextstyle(8, HORIZ_DIR, 2);
					setcolor(4);
					setbkcolor(15);
					outtextxy(468, 570, "Do thi chua co dinh!");
					Set_False();
					continue;
				}
				dinh_tru();
				Set_False();
			}
			/////////////////////////////////////////////CANH CAU/////////////////////////////////////
			if (kt_rect(240, 705, 430, 775, x, y))
			{
				Algorithm_tool_screen();
				Work_screen();
				Load_Nodes();
				settextstyle(0, 0, 2);
				Canhcau(true);
				if (working == false)
				{
					Notify_screen();
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc!");
					continue;
				}
				if (dt.TongSo_Dinh < 0)
				{
					Notify_screen();
					settextstyle(8, HORIZ_DIR, 2);
					setcolor(4);
					setbkcolor(15);
					outtextxy(468, 570, "Do thi phai co it nhat 2 dinh de tao thanh canh! ");
					Set_False();
					continue;
				}
				Notify_screen();
				Canh_Cau();
				Set_False();
			}
			/////////////////////////////////////////////////dinhthat/////////////////////////////////////////////////////////
			if (kt_rect(25, 705, 215, 775, x, y))
			{
				Algorithm_tool_screen();
				Notify_screen();
				Load_Nodes();
				settextstyle(0, 0, 2);
				Dinhthat(true);
				if (working == false)
				{
					Notify_screen();
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc!");
					continue;
				}
				if (dt.TongSo_Dinh < 1)
				{
					Notify_screen();
					setcolor(4);
					setbkcolor(15);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Do thi phai co it nhat 2 dinh!");
					Set_False();
					continue;
				}

				dem = 0;
				Notify_screen();
				setcolor(0);
				setbkcolor(15);
				settextstyle(8, HORIZ_DIR, 2);
				outtextxy(468, 570, "Click vao dinh bat ki de chon dinh bat dau");
				Set_True(kt_DThat);
			}
			if (kt_DThat && kt_rect(485, 80, 1375, 525, x, y))
			{
				if (dem == 0)
				{
					for (int i = 0; i <= dt.TongSo_Dinh; i++)
						if (kc(x, y, dt.node[i].x, dt.node[i].y) <= 20)
						{
							tam = i;
							dem = 1;
						}
					if (dem == 1)
					{
						tao_node(dt.node[tam].x, dt.node[tam].y, 2); ///
						Notify_screen();
						setcolor(0);
						setbkcolor(15);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(468, 570, "Click vao dinh khac de chon dinh ket thuc");
					}
				}
				else
				{
					int t;
					for (int i = 0; i <= dt.TongSo_Dinh; i++)
						if (kc(x, y, dt.node[i].x, dt.node[i].y) <= 20 && i != tam)
						{
							tao_node(dt.node[i].x, dt.node[i].y, 2);
							dem = 0;
							t = i;
						}
					if (dem == 0)
					{
						Notify_screen();
						setcolor(0);
						settextstyle(8, HORIZ_DIR, 1);
						outtextxy(468, 570, "Dinh that giua 2 dinh ");
						outtextxy(748, 570, "-->");
						outtextxy(840, 570, ":");

						setcolor(12);
						settextstyle(8, HORIZ_DIR, 2);
						outtextxy(715, 570, dt.node[tam].name);
						outtextxy(795, 570, dt.node[t].name);
						Dinh_That(tam, t);
						kt_DThat = false;
					}
				}
			}
			//////////////////////////////////////////////////////euler//////////////////////////////////////////////////////////////////
			if (kt_rect(25, 540, 140, 590, x, y))
			{
				Algorithm_tool_screen();
				// Work_screen();
				Notify_screen();
				settextstyle(0, 0, 2);
				Euler(true);
				setbkcolor(15);
				setcolor(4);
				if (working == false)
				{
					Notify_screen();
					setcolor(4);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Ban phai mo do thi hoac tao do thi moi truoc!");
					continue;
				}
				if (dt.TongSo_Dinh < 0)
				{
					Notify_screen();
					setcolor(4);
					settextstyle(8, HORIZ_DIR, 2);
					outtextxy(468, 570, "Do thi chua co dinh!");
					Set_False();
					continue;
				}

				Notify_screen();
				Set_True(kt_Eurle);
				Notify_screen();
				setcolor(0);
				settextstyle(8, HORIZ_DIR, 2);
				outtextxy(468, 570, "Click vao dinh de chon dinh bat dau");
			}
			if (kt_Eurle && kt_rect(485, 80, 1375, 525, x, y))
			{
				for (int i = 0; i <= dt.TongSo_Dinh; i++)
					if (kt_circle(x, y, dt.node[i].x, dt.node[i].y))
					{
						Load_Nodes();
						Euler(i);
						break;
					}
			}
			//////////////////////////////////////////////////////INFO//////////////////////////////////////////////////////////////////
			if (kt_rect(1150, 8, 1265, 40, x, y))
			{
				settextstyle(0, 0, 2);
				Info(true);
				Info_screen();
			}
			//////////////////////////////////////////////////////CLOSE//////////////////////////////////////////////////////////////////
			if (kt_rect(1285, 8, 1400, 40, x, y))
			{
				settextstyle(0, 0, 2);
				Close(true);
				goto dongtep;
			}
		}
		clearMouse();
		while (kbhit())
		{
			char s = getch();
			if (s == 27)
			{
				dt.delete_Data();
				return 0;
			}
		}
		delay(0.0001);
	}
dongtep:
	int fclose(FILE * tenfile);
	closegraph();
	dt.delete_Data();
	return 0;
}
