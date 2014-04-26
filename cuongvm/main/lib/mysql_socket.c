#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <jansson.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#define BUFSZ PIPE_BUF
//Compile
//gcc -o client client.c `pkg-config jansson --cflags --libs`



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

/**
	Sending data to server
	Parameters:
		char *type: type of data
		json_t *data: data object
		int sockfd: connection
		void (*callback)(char*): callback function
*/

char* response_data;

int file_pipes[2];
struct productType *product_type_list; //luu mang product tra ve tu server
struct product *product_list; //luu mang product type tra ve tu server
int product_list_size; //do lon mang product_list
int product_type_list_size; //do lon mang product_type_list

void err_quit(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

struct productType *get_product_type_list(){
	
	return product_type_list;
}

struct product *get_product_list(){
	return product_list;
}

void print_product_list(){
	int i =0;
	printf("Product List: \n");
	for (i=0;i<product_list_size;i++){
		printf("%d\t%s\t%d\n", product_list[i].productId, product_list[i].productName, product_list[i].typeId);
	}

}

void print_product_type_list(){
	int i =0;
	printf("Product Type List: \n");
	for (i=0;i<	product_type_list_size;i++){
		printf("%d\t%s\n", product_type_list[i].typeId, product_type_list[i].typeName);
	}

}

int get_sockfd() {
	char buf[500];
	printf("main : %d\n", getpid());
	// Socket variables
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;

	/** Create the pipe */
	if ((pipe(file_pipes)) < 0) {
		printf("%d\n", 2);
		err_quit("pipe");	
	}

	// Json parser error variable
    json_error_t error;

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

/*
	json_t *data = json_object();
	json_object_set(data, "phone", json_string("01672642420"));
	json_object_set(data, "name", json_string("May Thy"));

	response_data = (char *) malloc(500*sizeof(char));
	get_data("gcustomerinfo", data, sockfd, &customer_info_listener);
*/
	
	//getTypeByGender(sockfd, "2");
	//get_data("gproduct", data, sockfd, &customer_info_listener);

	//json_t *productlist = json_array();
	
	//get_data("gallproduct", json_object(), sockfd, &customer_info_listener);
	
	//json_t *data = json_object();
	//json_object_set(data, "gender", json_string("2"));

	//response_data = (char *) malloc(500*sizeof(char));
	//get_data("ggendertype", data, sockfd, &customer_info_listener);
	//get_data("gallproduct", json_object(), sockfd, NULL);
	//(void) signal(SIGALRM, get_all_product_listener);
	//get_all_product(sockfd,"0");
	//get_product_type_by_gender(sockfd, "2");
	//get_all_product_by_type(sockfd, "1", "0");
	//pause();
	//close(sockfd);

	return sockfd;
}

/**
	Sending data to server
	Parameters:
		char *type: type of data
		json_t *data: data object
		int sockfd: connection
		void (*callback)(char*): callback function
*/
void get_data(char *type, json_t *data, int sockfd, void (*callback)(char*)) {
	char buf[500];
	pid_t child_pid;

	json_t *root = json_object();
	json_t *j_type = json_string(type);

	json_object_set(root, "type", j_type);
	json_object_set(root, "data", data);

	// Convert JSON data to string
	char *decode = json_dumps(root, 1);

	// Create a proccess for reading response data
	if (0 == (child_pid = fork())) {
		// Send a request to server
		write(sockfd, decode, 500);

		printf("Send data:\n%s\n", decode);

		char *result_string = (char *) malloc(500*sizeof(char));

		read(sockfd, result_string, 500);

		// Run callback
		// callback(result_string);

		close(file_pipes[0]);
		write(file_pipes[1], result_string, 500);
		close(file_pipes[1]);

		printf("Send signal: %d\n", getpid());
		kill(getppid(), SIGALRM);

		exit(0);
	}

	json_decref(root);
	json_decref(j_type);
}

void customer_info_listener(char* response) {
	printf("My process ID : %d\n", getpid());
	printf("Data received:\n");
	strcpy(response_data, response);
	printf("%s\n", response_data);
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
void get_product_list_listener(int sig) {
	char *buf = (char *) malloc(500*sizeof(char));
	printf("Ding : %d\n", getpid());
	close(file_pipes[1]);
	int len;
	int i =0;
	while ((len = read(file_pipes[0], buf, 500)) > 0) {
	}
	strcpy(response_data,buf);
	close(file_pipes[0]);
	free(buf);

	json_error_t error;
	size_t n=0;
	json_t *root = json_loads(response_data, 0, &error);
	json_t *object;
	json_t *element;

	if (!root){
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		//return NULL;
	}

	object = json_object_get(root, "data"); //lay mang object tu root voi key="data"
	n = json_array_size(object);

	printf("Size: n = %d\n", n);

	free(product_list);
	product_list_size=0;
	product_list = (struct product*)malloc(n*sizeof(struct product));
	printf("Type Array? %d\n", json_is_array(object));
	for (i=0;i<n;i++){
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
	product_list_size = n;
	print_product_list();
	
}

void get_all_product_by_type(int sockfd, char* typeId, char* page_num){
	json_t *data = json_object();
	json_object_set(data, "typeId", json_string(typeId));
	json_object_set(data, "current_page", json_string(page_num));

	response_data = (char *) malloc(500*sizeof(char));
	get_data("gtypeid", data, sockfd, NULL);
	
	(void) signal(SIGALRM, get_product_list_listener);
}


void get_all_product(int sockfd, char* page_num){
	response_data = (char *) malloc(500*sizeof(char));
	json_t *data = json_object();
	json_object_set(data, "current_page", json_string(page_num));

	get_data("gallproduct", data, sockfd, NULL);
	
	(void) signal(SIGALRM, get_product_list_listener);
}

//-----------------------------------------Get Product Type List----------------------------------------
void get_product_type_list_listener(int sig) {
	char *buf = (char *) malloc(500*sizeof(char));
	printf("Ding : %d\n", getpid());
	close(file_pipes[1]);
	int len;
	int i =0;

	//lay du lieu tu server
	while ((len = read(file_pipes[0], buf, 500)) > 0) {
	}
	strcpy(response_data,buf);
	close(file_pipes[0]);
	free(buf);

	//lay du lieu tu file json
	json_error_t error;
	size_t n=0;
	json_t *root = json_loads(response_data, 0, &error);
	json_t *object;
	json_t *element;

	if (!root){
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		//return NULL;
	}

	object = json_object_get(root, "data"); //lay mang object tu root voi key="data"
	n = json_array_size(object);

	printf("Size: n = %d\n", n);

	free(product_type_list);
	product_type_list_size = 0;
	product_type_list = (struct productType*)malloc(n*sizeof(struct productType));
	printf("Type Array? %d\n", json_is_array(object));
	for (i=0;i<n;i++){
		element = json_array_get(object, i);

		product_type_list[i].typeId = atoi(json_string_value(json_object_get(element, "typeId")));
		strcpy( product_type_list[i].typeName, json_string_value(json_object_get(element, "typeName")) );
		product_type_list[i].gender = atoi( json_string_value(json_object_get(element, "gender")) );
		product_type_list[i].kind = atoi(json_string_value(json_object_get(element, "kind")));
	
	}
	product_type_list_size = n;
	print_product_type_list();
	
}

void get_product_type_by_gender(int sockfd, char* gender){
	json_t *data = json_object();
	json_object_set(data, "gender", json_string(gender));

	response_data = (char *) malloc(500*sizeof(char));
	get_data("ggendertype", data, sockfd, NULL);
	
	(void) signal(SIGALRM, get_product_type_list_listener);
}
