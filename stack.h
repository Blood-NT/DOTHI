struct sNode{
	int data;
	sNode *next;
};
struct Stack{
	sNode *first;
	Stack(){
		first=NULL;
	}
	bool empty(){
		return(first==NULL);
	}
	void push(int value){//day vô
		sNode *p = new sNode;//tao mot con tro p co kieu sNode
		p->data=value;//gan gia tri cua tro p = value nhap vao
		p->next=first;//tro phía sau nút p cho nút first
		first=p;//gán p cho bien first
	}
	int pop(int &tmp){ //lay và xoá
		if (empty())	return -1;
		sNode *p = first;//tao con tro tmp thuoc sNode và gán bang nút first
		tmp=first->data; //lay giá tri cua nút first de gán cho bien tmp
		first=first->next;//cap nhat node cu thành node sau cua node cu
		delete p;//xoá tro p;
		return 1;
	}
	int get()
	{
		if (empty()) return INT_MIN; //ooINT_MIN
		return first->data;
	}
	// giai phong bo nho
	~Stack(){
		while(first!=NULL){ //khi dãy khác rong
			sNode *tmp; //tao con trotmp thuoc sNode và gán bang nút first
			tmp = first;
			first=first->next;//cap nhat node cu thành node sau cua node cux
			delete tmp;//xoá tro tmp
		}
	}
};


