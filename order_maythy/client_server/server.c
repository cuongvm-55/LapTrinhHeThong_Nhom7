#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <mysql.h>

MYSQL my_connection;

/**
	Get POST value
	Parameters:
		json_t *root: root data
		char *key: key of value
*/
const char *get_value(json_t *root, char* key);

/**
	Retrieve result from sql database
	Parameters:
		char *query: query
*/
MYSQL_RES* get_data_from_database(char *query);

/**
	Send response data to client.
	Parameters:
		int success: Status about data
		json_t *data: Data for response
		int client_sockfd: connection
*/
void response_data(int success, json_t *data, int client_sockfd);

/**
	GET SPECIFIC DATA FUNCTIONS
*/
json_t* get_customer(const char *phone);
json_t* new_customer(const char* name, const char* email, const char* address, const char* phone, const char* gender, const char* message);
json_t* update_customer(const char* id, const char* name, const char* email, const char* address, const char* gender);
json_t* new_order(const char* customer_id, const char* message);
json_t* new_order_detail(const char* order_id, const char* product_id, const char* quantity);

int main(int argc, char*argv[]) {

	char* test = (char *) malloc(500*sizeof(char));

    json_error_t error;
	json_t *root;
	json_t *data;
	json_t *j_type;
	json_t *j_data;
	json_t *phone;

	const char *type;
	const char *customer_phone;

	int server_sockfd, client_sockfd;
	int server_len, client_len;

	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	server_sockfd = socket(PF_INET, SOCK_STREAM, 0);
	server_address.sin_family = PF_INET;
	// server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(9733);
	server_len = sizeof(server_address);

	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

	listen(server_sockfd, 5);

	while (1) {
		test = (char *) malloc(500*sizeof(char));
		printf("server waitting\n");
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
		
		read(client_sockfd, test, 500);

		root = json_loads(test, 0, &error);

		free(test);

		if(!root) {
	        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
	        // Response error to client
			response_data(0, json_object(), client_sockfd);

    	} else {
	    	j_type = json_object_get(root, "type");
	    	j_data = json_object_get(root, "data");

	    	if (!j_type || !j_data) {
				response_data(0, json_object(), client_sockfd);

	    	} else {
	    		type = json_string_value(j_type);
		    	printf("Type: %s\n", type);
		    	if (strcmp(type, "gcustomerinfo") == 0) {
		    		const char * phone = get_value(root, "phone");
		    		json_t *data = get_customer(phone);

					response_data(1, data, client_sockfd);
		    	
		    	} else if (strcmp(type, "pcustomerinfo") == 0) {
		    		const char * name = get_value(root, "name");
		    		const char * email = get_value(root, "email");
		    		const char * address = get_value(root, "address");
		    		const char * phone = get_value(root, "phone");
		    		const char * gender = get_value(root, "gender");

		    		json_t *data = new_customer(name, email, address, phone, gender, "test");

		    		response_data(1, data, client_sockfd);

		    	} else if (strcmp(type, "ucustomerinfo") == 0) {
		    		const char * id = get_value(root, "id");
		    		const char * name = get_value(root, "name");
		    		const char * email = get_value(root, "email");
		    		const char * address = get_value(root, "address");
		    		const char * gender = get_value(root, "gender");

		    		json_t *data = update_customer(id, name, email, address, gender);

		    		response_data(1, data, client_sockfd);

		    	} else if (strcmp(type, "porder") == 0) {
		    		const char * customer_id = get_value(root, "id");
		    		const char * message = get_value(root, "message");

		    		json_t *data = new_order(customer_id, message);

		    		response_data(1, data, client_sockfd);

		    	} else if (strcmp(type, "porderdetail") == 0) {
		    		const char * order_id = get_value(root, "oid");
		    		const char * product_id = get_value(root, "pid");
		    		const char * quantity = get_value(root, "quantity");

		    		json_t *data = new_order_detail(order_id, product_id, quantity);

		    		response_data(1, data, client_sockfd);

		    	// Default
		    	} else {
		    		response_data(0, json_object(), client_sockfd);
		    	}
	    	}
    	}

		close(client_sockfd);
	}
}


json_t* get_customer(const char *phone) {
	MYSQL_RES *res_ptr;
	MYSQL_ROW sqlrow;

	char query[100];
	sprintf(query, "SELECT * FROM customer WHERE phone=%s", phone);
	res_ptr = get_data_from_database(query);

	if (res_ptr) {
		json_t *data = json_object();

		while ((sqlrow = mysql_fetch_row(res_ptr))) {

			json_object_set(data, "id", json_string(sqlrow[0]));
			json_object_set(data, "name", json_string(sqlrow[1]));
			json_object_set(data, "phone", json_string(sqlrow[2]));
			json_object_set(data, "address", json_string(sqlrow[3]));
			json_object_set(data, "email", json_string(sqlrow[4]));
			json_object_set(data, "gender", json_string(sqlrow[5]));
			json_object_set(data, "created_at", json_string(sqlrow[6]));
			json_object_set(data, "updated_at", json_string(sqlrow[7]));
		}

		mysql_free_result(res_ptr);

		return data;
	} else {
		return json_object();
	}	
}

json_t* new_customer(const char* name, const char* email, const char* address,
	const char* phone, const char* gender, const char* message) {
	MYSQL_RES *res_ptr;

	char query[200];
	sprintf(query, "INSERT INTO customer (Name, Phone, Address, Email, Gender, CreatedAt, UpdatedAt) VALUES ('%s', '%s', '%s', '%s', '%s', now(), now())",
		name, phone, address, email, gender);
	printf("%s\n", query);
	res_ptr = get_data_from_database(query);

	printf("%s\n", "get_customer phone");
	return get_customer(phone);
}

json_t* update_customer(const char* id, const char* name, const char* email, const char* address, const char* gender) {
	MYSQL_RES *res_ptr;

	char query[200];
	sprintf(query, "UPDATE customer SET Name='%s', Email='%s', Address='%s', Gender='%s', UpdatedAt=now() WHERE CustomerId='%s'",
		name, email, address, gender, id);
	printf("%s\n", query);
	res_ptr = get_data_from_database(query);

	return json_object();
}

json_t* new_order(const char* customer_id, const char* message) {
	MYSQL_RES *res_ptr;

	char query[200];
	sprintf(query, "INSERT INTO shop.order (OrderDate, Status, Message, CustomerId, CreatedAt) VALUES (now(), 'Pending', '%s', '%s', now())",
		message, customer_id);
	printf("%s\n", query);
	res_ptr = get_data_from_database(query);

	return json_object();
}

json_t* new_order_detail(const char* order_id, const char* product_id, const char* quantity) {
	MYSQL_RES *res_ptr;

	char query[200];
	sprintf(query, "INSERT INTO shop.orderdetail (OrderId, ProductId, Quantity) VALUES ('%s', '%s', '%s')",
		order_id, product_id, quantity);
	printf("%s\n", query);
	res_ptr = get_data_from_database(query);

	return json_object();
}

json_t* get_order_id(const char* customer_id) {
	MYSQL_RES *res_ptr;

	char query[200];
	sprintf(query, "SELECT * FROM shop.order WHERE shop.CustomerId = '%s'", customer_id);
	printf("%s\n", query);
	res_ptr = get_data_from_database(query);

	return json_object();
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

/**
	Send response data to client.
	Parameters:
		int success: Status about data
		json_t *data: Data for response
		int client_sockfd: connection
*/
void response_data(int success, json_t *data, int client_sockfd) {
	json_t *root = json_object();

	if (success == 1) json_object_set(root, "success", json_true());
	else json_object_set(root, "success", json_false());
	json_object_set(root, "data", data);
	
	char *decode = json_dumps(root, 1);

	write(client_sockfd, decode, 500);

	json_decref(root);
}

/**
	Retrieve result from sql database
	Parameters:
		char *query: query
*/
MYSQL_RES* get_data_from_database(char *query) {
	MYSQL_RES *res_ptr;

	int result;
	mysql_init(&my_connection);
	if (mysql_real_connect(&my_connection, "localhost", "root", "6789", "shop", 0, NULL, 0)) {
		printf("Connection is successfully\n");
		result = mysql_query(&my_connection, "SET NAMES utf8");

		result = mysql_query(&my_connection, query);

		if (!result) {
			res_ptr = mysql_store_result(&my_connection);
			if (res_ptr) {
				if (mysql_errno(&my_connection)) {
					fprintf(stderr, "Retrive error: %s\n", mysql_error(&my_connection));
				}
				return res_ptr;
			} else {
				return 0;
			}
		} else {
			return 0;
		}

		mysql_close(&my_connection);

	} else {
		return 0;
	}

	mysql_library_end();
}