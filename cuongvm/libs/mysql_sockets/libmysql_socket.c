#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <jansson.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#define BUFSZ PIPE_BUF
//Compile
//gcc -o client_pthread client_pthread.c `pkg-config jansson --cflags --libs` -lpthread

//-----------------Cau truc du lieu-------------------------------------------------------------------
struct product{
	int productId; //id cua san pham
	char productCode[30]; //ma quan li cua san pham
	char productName[255]; //ten san pham
	int typeId; //id cua productType tuong ung
	char brand[30]; //nhan hieu san pham
	double price; //gia
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

int get_sockfd() {
	// Socket variables
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	address.sin_family = PF_INET;
	// To bind socket with localhost
	// address.sin_addr.s_addr = inet_addr("127.0.0.1");
	// To bind socket with all interface
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(9733);
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);

	if (result == -1) {
		perror("oops: client");
		exit(1);
	}
	return sockfd;		
}

void err_quit(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

void print_product_list(struct product product_list[], int product_list_size){
	int i =0;
	printf("Product List: \n");
	for (i=0;i<product_list_size;i++){
		printf("%d\t%s\t%d\n", product_list[i].productId, product_list[i].productName, product_list[i].typeId);
	}

}

void print_product_type_list(struct productType product_type_list[],
							int product_type_list_size){
	int i =0;
	printf("Product Type List: \n");
	for (i=0;i<	product_type_list_size;i++){
		printf("%d\t%s\n", product_type_list[i].typeId, product_type_list[i].typeName);
	}

}

//Doan ma chuong trinh cho thread, xu li yeu cau tu client
void *thread_function(void *arg){
	printf("Getting data from server...\n");
	int* sockfd=(int*)arg;
	char* result_string = (char *) malloc(500*sizeof(char));
	while(read(*sockfd, result_string, 500) <= 0){
	}
	pthread_exit(result_string);
}
/**
	Sending data to server
	Parameters:
		char *type: type of data
		json_t *data: data object
		int sockfd: connection
		void (*callback)(char*): callback function
*/
char* get_data(char *type, json_t *data, int* sockfd) {
	char buf[500];
	pthread_t a_thread;
	void* thread_result;

	int res;

	json_t *root = json_object();
	json_t *j_type = json_string(type);

	json_object_set(root, "type", j_type);
	json_object_set(root, "data", data);

	// Convert JSON data to string
	char *decode = json_dumps(root, 1);
	json_decref(root);
	json_decref(j_type);

	//ghi du lieu qua socket
	write(*sockfd, decode, 500);
	//printf("Send data:\n%s\n", decode);

	// Create a proccess for reading response data
	int n = pthread_create(&a_thread, NULL, thread_function, (void*)sockfd);
	if (n != 0){
		perror("Error when create pthread to read data from server!\n");
		return;
	}
	printf("Wait for the thread to finish...\n");
	res = pthread_join(a_thread, &thread_result);
	if (res != 0){
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}

	return (char*)thread_result;
}

/**
	Get POST value
	Parameters:
		json_t *root: root data
		char *key: key of value
*/
const char *get_value(json_t *root, char* key) {
	json_t *j_data = json_object_get(root, "data");
	json_t *j_object = json_object_get(j_data, key);

	if (!j_object) {
		return 0;
	} else {
		if (json_string_value(j_object)) return json_string_value(j_object);
		else return 0;
	}
}

//----------------------------------------------Get Product List------------------------------------------------
void get_product_list_listener(char* response_data, 
							 	struct product product_list[], 
								int *product_list_size, char* total_page) {
	int i =0;
	json_error_t error;
	size_t n=0;
	json_t *root = json_loads(response_data, 0, &error);
	json_t *object;
	json_t *element;

	if (!root){
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return;
	}

	object = json_object_get(root, "data"); //lay mang object tu root voi key="data"
	n = json_array_size(object);

	printf("Size: n = %d\n", n);

	for (i=0;i<n-1;i++){
		element = json_array_get(object, i);

		product_list[i].productId = atoi(json_string_value(json_object_get(element, "productId")));
		strcpy( product_list[i].productCode, json_string_value(json_object_get(element, "productCode")) );
		strcpy( product_list[i].productName, json_string_value(json_object_get(element, "productName")) );
		product_list[i].typeId = atoi(json_string_value(json_object_get(element, "typeId")));
		strcpy( product_list[i].brand, json_string_value(json_object_get(element, "brand")) );
		product_list[i].price = atof( json_string_value(json_object_get(element, "price")) );
		product_list[i].quantityInStock = atoi(json_string_value(json_object_get(element, "quantityInStock")));
		strcpy( product_list[i].description, json_string_value(json_object_get(element, "description")) );
		strcpy( product_list[i].image, json_string_value(json_object_get(element, "image")) );
		product_list[i].saleOff = atoi( json_string_value(json_object_get(element, "saleOff")) );
		
	}

	//Lay ra total_page
	if (n > 0){
		element = json_array_get(object, n-1);
		strcpy(total_page, json_string_value(json_object_get(element, "total_page")));
	}
	else{
		strcpy(total_page, "0");
	}

	*product_list_size = n-1;
}

void get_all_product_by_type(int sockfd,
							 char* typeId,
							 char* page_num,
							 struct product product_list[],
					 		 int *product_list_size,
					 		 char* total_page){
	json_t *data = json_object();
	json_object_set(data, "typeId", json_string(typeId));
	json_object_set(data, "current_page", json_string(page_num));

	char* response_data = get_data("gtypeid", data, &sockfd);
	
	get_product_list_listener(response_data, product_list, product_list_size, total_page);
}


void get_all_product(int sockfd,
					 char* page_num,
					 struct product product_list[],
					 int *product_list_size,
					 char* total_page){
	json_t *data = json_object();
	json_object_set(data, "current_page", json_string(page_num));

	char* response_data = get_data("gallproduct", data, &sockfd);
	//printf("data from server: \n%s\n", response_data);
	get_product_list_listener(response_data, product_list, product_list_size, total_page);
}

//-----------------------------------------Get Product Type List----------------------------------------
void get_product_type_list_listener(char* response_data,
									struct productType product_type_list[], 
									int *product_type_list_size) {
	int i =0;

	//lay du lieu tu file json
	json_error_t error;
	size_t n=0;
	json_t *root = json_loads(response_data, 0, &error);
	json_t *object;
	json_t *element;

	if (!root){
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return;
	}

	object = json_object_get(root, "data"); //lay mang object tu root voi key="data"
	n = json_array_size(object);

	printf("Size: n = %d\n", n);

	for (i=0;i<n;i++){
		element = json_array_get(object, i);

		product_type_list[i].typeId = atoi(json_string_value(json_object_get(element, "typeId")));
		strcpy( product_type_list[i].typeName, json_string_value(json_object_get(element, "typeName")) );
		product_type_list[i].gender = atoi( json_string_value(json_object_get(element, "gender")) );
		product_type_list[i].kind = atoi(json_string_value(json_object_get(element, "kind")));
	
	}
	*product_type_list_size = n;
}

void get_product_type_by_gender(int sockfd,
								char* gender,
								struct productType product_type_list[],
					 			int *product_type_list_size){
	json_t *data = json_object();
	json_object_set(data, "gender", json_string(gender));

	char* response_data = get_data("ggendertype", data, &sockfd);
	//printf("data from server: \n%s\n", response_data);
	get_product_type_list_listener(response_data, product_type_list, product_type_list_size);
}