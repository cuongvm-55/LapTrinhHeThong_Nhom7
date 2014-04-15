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

/**
	Sending data to server
	Parameters:
		char *type: type of data
		json_t *data: data object
		int sockfd: connection
		void (*callback)(char*): callback function
*/
char* get_data(char *type, json_t *data, int sockfd);

char* response_data;

int main(int argc, char **argv[]) {
	// char buf[500];
	printf("main : %d\n", getpid());
	// Socket variables
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;

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

	json_t *data = json_object();
	json_object_set(data, "phone", json_string("01662329830"));
	//json_object_set(data, "name", json_string("May Thy"));

	response_data = (char *) malloc(500*sizeof(char));
	char* rstring = get_data("gcustomerinfo", data, sockfd);
	printf("%s\n", rstring);

	close(sockfd);
	exit(0);
}

/**
	Sending data to server
	Parameters:
		char *type: type of data
		json_t *data: data object
		int sockfd: connection
		void (*callback)(char*): callback function
*/
char* get_data(char *type, json_t *data, int sockfd) {
	pid_t child_pid;

	json_t *root = json_object();
	json_t *j_type = json_string(type);

	json_object_set(root, "type", j_type);
	json_object_set(root, "data", data);

	// Convert JSON data to string
	char *decode = json_dumps(root, 1);

	write(sockfd, decode, 500);

	printf("Send data:\n%s\n", decode);

	char *result_string = (char *) malloc(500*sizeof(char));

	read(sockfd, result_string, 500);

	json_decref(root);
	json_decref(j_type);

	return result_string;
}