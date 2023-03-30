struct qNode{
	int data;
	qNode *next;
};
struct Queue{
	qNode *VTxoa;//xoá
	qNode *VTthem;//thêm
	Queue(){
		VTxoa=NULL;
		VTthem=NULL;
	}
	bool empty(){
		return (VTxoa==NULL);
	}
	void push(int value){//thêm vào
		qNode *p= new qNode;
		p->data=value; //gán giá tri value duoc nhap vào cho data cua con tri nút p 
		p->next=NULL; // xoá o dau, thêm o cuoi=> cho p tro den NULL
		if (VTxoa==NULL){
			VTxoa = p;//thêm o cuoi
		}else {
			VTthem->next = p;//thêm o vitri bat ky
		}
		VTthem = p;
	}
	int pop(int &tmp){//lay ra và xoá
		if (empty()) return -1;
		qNode *p = VTxoa;
		tmp = VTxoa->data; //loy giá triinit_MT cua VTxoa do gán cho bien tam tmp
		VTxoa = VTxoa->next;//cap nhat lai noi dung cua nút VTxoa bang nút ko sau nó
		delete p; //xoá con tro p
		return 1;
	}
	// giai phong bo nho
	~Queue()
	{
		while (VTxoa != NULL) //khi dãy khác rong
		{
			qNode *tmp;//tao con tro tmp thuoc sNode 
			tmp = VTxoa;//gán tmp bang nút VTxoa
			VTxoa = VTxoa->next;//cap nhat node cu thành node ko sau cua nó
			delete tmp;//xoá tro tmp;
		}
	}
	
};


