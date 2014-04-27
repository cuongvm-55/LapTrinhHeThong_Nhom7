#include "libdownload_img.h"

//Compile
//gcc -g test_libdownload_img.c -o test_libdownload_img -L. -ldownload_img `pkg-config libcurl --cflags --libs`

int main()
{
	char* url = "https://dl.dropboxusercontent.com/s/y84j8qzpbkd5fyd/ezra-1397-514382-1-product.jpg?dl=1&token_hash=AAH4bnl1a-1to7CH-sV1-tKjAa3c5nxZV39h4ldj-Ipjjw";
	char* filename = "img.jpg";
	
	download_image(url, filename);

	return 1;
}

