#define CURL_STATICLIB
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

//Check co mang hay khong
int is_web_server_connected(char* url){
    CURL *curl = curl_easy_init();
    CURLcode res;
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, url);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    if(res != CURLE_OK){
        return 0;
    }
    else{
        return 1;
    }
}

//doawnload image tren mang
void download_image(char* url, char* filename){
	CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();

    if (curl) {
        fp = fopen(filename,"wb");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, NULL);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}
