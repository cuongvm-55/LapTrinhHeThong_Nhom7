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
void get_data(char *type, json_t *data, int sockfd, void (*callback)(char*));

void customer_info_listener(char* response);

char* response_data;

int file_pipes[2];

void ding(int sig) {
	char *buf = (char *) malloc(500*sizeof(char));
	printf("Ding : %d\n", getpid());
	close(file_pipes[1]);
	int len;
	while ((len = read(file_pipes[0], buf, 500)) > 0) {
		printf("%s\n", buf);
	}
	printf("len: %d\n", len);
	close(file_pipes[0]);
}

void err_quit(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv[]) {
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

	json_t *data = json_object();
	json_object_set(data, "phone", json_string("01672642420"));
	json_object_set(data, "name", json_string("May Thy"));

	response_data = (char *) malloc(500*sizeof(char));
	get_data("gcustomerinfo", data, sockfd, &customer_info_listener);
	(void) signal(SIGALRM, ding);
	pause();

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