#include <mysql.h>
#include <stdlib.h>
#include <stdio.h>

// Variables for mysql connection
MYSQL my_connection;
int result;

void getCustomer(MYSQL *my_connection, char *phone);

int main(int argc, char *argv[]) {
	mysql_init(&my_connection);
	if (mysql_real_connect(&my_connection, "localhost", "root", "6789", "shop", 0, NULL, 0)) {
		printf("Connection is successfully\n");
		result = mysql_query(&my_connection, "SET NAMES utf8");

		getCustomer(&my_connection, "01672642420");

		mysql_close(&my_connection);

	} else {
		fprintf(stderr, "Connection failed\n");
		if (mysql_errno(&my_connection)) {
			fprintf(stderr, "Connection error %d: %s\n", mysql_errno(&my_connection), mysql_error(&my_connection));
		}
	}

	mysql_library_end();
	return EXIT_SUCCESS;
}

void getCustomer(MYSQL *my_connection, char *phone) {
	MYSQL_RES *res_ptr;
	MYSQL_ROW sqlrow;
	char query[100];
		
	sprintf(query, "SELECT * FROM customer WHERE phone=%s", phone);

	result = mysql_query(my_connection, query);
	if (!result) {
		res_ptr = mysql_store_result(my_connection);
		if (res_ptr) {
			printf("Retrieved %lu rows\n", (unsigned long) mysql_num_rows(res_ptr));
			printf("List of results\n");
			while ((sqlrow = mysql_fetch_row(res_ptr))) {
				printf("Customer ID: %s, Customer name: %s\n", sqlrow[0], sqlrow[1]);
			}

			if (mysql_errno(my_connection)) {
				fprintf(stderr, "Retrive error: %s\n", mysql_error(my_connection));
			}

			mysql_free_result(res_ptr);
		}
	} else {
		printf("SELECT error: %s\n", mysql_error(my_connection));
	}
}