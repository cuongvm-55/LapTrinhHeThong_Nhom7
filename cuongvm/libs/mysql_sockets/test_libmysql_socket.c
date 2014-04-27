#include "libmysql_socket.h"
//compile
//gcc -g test_lib.c -o test_lib -L. -lmysql_socket -lpthread -ljansson

int main()
{
	int sockfd = get_sockfd();
	struct productType product_type_list[30]; //luu mang product tra ve tu server
	struct product product_list[30]; //luu mang product type tra ve tu server
	int product_list_size; //do lon mang product_list
	int product_type_list_size; //do lon mang product_type_list


	//get_all_product(sockfd,"0", product_list, &product_list_size);
	get_all_product_by_type(sockfd,"2","0",product_list,&product_list_size);
	print_product_list(product_list, product_list_size);
	//get_product_type_by_gender(sockfd, "2", product_type_list, &product_type_list_size);
	//print_product_type_list(product_type_list, product_type_list_size);

	close(sockfd);
	return 0;
}