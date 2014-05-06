#include "libdownload_img.h"
#include <stdio.h>
//Compile
//gcc -g test_libdownload_img.c -o test_libdownload_img -L. -ldownload_img `pkg-config libcurl --cflags --libs`

int main()
{
	char* url = "google.com";
	char* filename = "img.jpg";
	if (is_web_server_connected(url)){
		printf("Network connected!\n");
	}
	else{
		printf("Network's not available!\n");
	}
	download_image(url, filename);

	return 1;
}

