#ifndef MYSQL_SOCKET_H_
#define MYSQL_SOCKET_H_

//cac ham thao tac Jansson---------------------------------------------------------------
const char *get_value(json_t *root, char* key);
void get_data(char *type, json_t *data, int sockfd, void (*callback)(char*));

//Ham thao tac khi nhan duoc sdu lieu tra ve tu server------------------------------------

//Lay tat ca cac san pham trong csdl
void get_all_product(int sockfd, char* current_page);
void get_product_list_listener(int sig);

//Lay tat ca cac san pham co typeId = "typeId", page = page_num: trang thu page_num
void get_all_product_by_type(int sockfd, char* typeId, char* current_page);

//Lay tat ca cac type san pham trong csdl
void get_product_type_by_gender(int sockfd, char* gender);
void get_product_type_list_listener(int sig);

void customer_info_listener(char* response);

int get_sockfd(); //tra ve gia tri id ket noi socket

#endif

