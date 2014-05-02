#ifndef _LIBMYSQL_SOCKET_H
#define _LIBMYSQL_SOCKET_H

//Create lib
//gcc -H -c libmysql_socket.c -o libmysql_socket.o
//ar rcs libmysql_socket.a libmysql_socket.o

struct product{
	int productId; //id cua san pham
	char productCode[30]; //ma quan li cua san pham
	char productName[255]; //ten san pham
	int typeId; //id cua productType tuong ung
	char brand[30]; //nhan hieu san pham
	long price; //gia
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

//-----------------Ham tra ve sockfd cua client khi ket noi voi server-----------------------------
int get_sockfd();

//-----------------Ham thao tac khi nhan duoc sdu lieu tra ve tu server-----------------------------
/**
Lay tat ca cac san pham trong csdl
	Params:
		int sockfd; socket descriptor cua socket ket noi toi server
		char* current_page; trang hien tai, phuc vu viec phan trang
		struct product product_list[]; mang luu tru danh sach san pham tra ve
		int *product_list_size); do lon cua mang product_list
		char* total_page; tong so trang ket qua

**/
void get_all_product(int sockfd,
					 char* current_page,
					 struct product product_list[],
					 int *product_list_size,
					 char* total_page);

/**
Lay tat ca cac san pham trong 1 type
	params:
		...
		char* typeId; id cua productType can lay
		...
**/
void get_all_product_by_type(int sockfd,
							char* typeId,
							char* current_page,
							struct product product_list[],
				 			int *product_list_size,
				 			char* total_page);

/**
Lay tat ca cac san pham trong 1 gender
	params:
		...
		char* gender; gioi tinh, "1"-Nam, "2"-Nu
		...
**/
void get_all_product_by_gender(int sockfd,
							 char* gender,
							 char* page_num,
							 struct product product_list[],
					 		 int *product_list_size,
					 		 char* total_page);
/**
Lay tat ca cac productType dua tren gioi tinh
	params:
		...
		struct productType product_type_list[]; mang luu tru danh sach productType tra ve
		int *product_type_list_size; do lon cua mang product_type_list
**/
void get_product_type_by_gender(int sockfd,
								char* gender,
								struct productType product_type_list[],
					 			int *product_type_list_size);

/**
In cac list - de test
**/


void print_product_list(struct product product_list[], int product_list_size);
void print_product_type_list(struct productType product_type_list[],
							int product_type_list_size);

#endif

