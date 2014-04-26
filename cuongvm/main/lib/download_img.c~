#define CURL_STATICLIB
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <string.h>

//Compile:
//gcc -o output input.c `pkg-config libcurl --cflags --libs`

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

//doawnload image tren mang
void download_image(char* url, char* filename){
	CURL *curl;
    FILE *fp;
    CURLcode res;
    //char *url = "https://dl.dropboxusercontent.com/s/y84j8qzpbkd5fyd/ezra-1397-514382-1-product.jpg?dl=1&token_hash=AAH4bnl1a-1to7CH-sV1-tKjAa3c5nxZV39h4ldj-Ipjjw";
    //char outfilename[FILENAME_MAX] = "temp/text.jpg";
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(filename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}
