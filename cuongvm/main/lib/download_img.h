#ifndef MYSQL_SOCKET_H_
#define MYSQL_SOCKET_H_

//ghi img ra file
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

//doawnload image tren mang
void download_image(char* url, char* filename);

#endif
