#include <stdio.h>

struct product{
	int productId; //id cua san pham
	char productCode[30]; //ma quan li cua san pham
	char productName[255]; //ten san pham
	int typeId; //id cua productType tuong ung
	char brand[30]; //nhan hieu san pham
	unsigned int price; //gia
	int quantityInStock; //so luong co trong kho hang
	char description[255]; //mo ta san pham
	char image[255]; //link anh san pham
	int saleOff; //phan tram giam gia, e.x. 20 <-> 20%, 30 <-> 30%
};

struct productType{
	int typeId; //id cua loai san pham
	char typeName[30]; //ten loai san pham
	int gender; //1- trai, 0- gai
	int kind; //e.x. 1- ao, 2- quan, ...
};

struct order{
	int orderId; //id don dat hang
	char orderDate; //ngay dat hang, strig theo format yyyy-mm-dd
	int customerId; //id khach hang la chu cua hoa don
	char status[30]; //tinh trang giao hang; dang cho/da chuyen/da huy
};

struct orderDetail{
	int orderDetailId; //id cua don dat hang chi tiet
	int orderId; //id cua don dat tong
	int productId; //id cua san pham
	int quantity; //so luong san pham dat mua
};

struct customer{
	int customerId; //id cua khach hang
	char name[64]; //full name
	char phone[30]; //so dien thoai
	char address[255]; //dia chi
	char email[64]; //email
	int gender; //gioi tinh: 1- nam, 0- nu
};
int main(int argc, char *argv[])
{

	return 1;
}