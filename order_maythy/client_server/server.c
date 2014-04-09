#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

/**
	Send response data to client.
	Parameters:
		int success: Status about data
		json_t *data: Data for response
		int client_sockfd: connection
*/
void send_data(int success, json_t *data, int client_sockfd);

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
			send_data(0, json_object(), client_sockfd);

    	} else {
	    	j_type = json_object_get(root, "type");
	    	j_data = json_object_get(root, "data");

	    	if (!j_type || !j_data) {
				send_data(0, json_object(), client_sockfd);

	    	} else {
	    		type = json_string_value(j_type);
		    	printf("Type: %s\n", type);
		    	if (strcmp(type, "gcustomerinfo") == 0) {
		    		json_t *data = json_object();
					json_object_set(data, "phone", json_string("01672642420"));
					json_object_set(data, "email", json_string("maythywinter@gmail.com"));
					json_object_set(data, "address", json_string("Xuan Dinh, Tu Liem, Hanoi"));
					json_object_set(data, "name", json_string("May Thy"));

					send_data(1, data, client_sockfd);
		    	
		    	// Default
		    	} else {
		    		send_data(0, json_object(), client_sockfd);
		    	}
	    	}
    	}

		close(client_sockfd);
	}
}

/**
	Send response data to client.
	Parameters:
		int success: Status about data
		json_t *data: Data for response
		int client_sockfd: connection
*/
void send_data(int success, json_t *data, int client_sockfd) {
	json_t *root = json_object();

	if (success == 1) json_object_set(root, "success", json_true());
	else json_object_set(root, "success", json_false());
	json_object_set(root, "data", data);
	
	char *decode = json_dumps(root, 1);

	write(client_sockfd, decode, 500);

	json_decref(root);
}