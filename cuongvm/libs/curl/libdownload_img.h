#ifndef MYSQL_SOCKET_H_
#define MYSQL_SOCKET_H_
//Create lib
//gcc -H -c libdownload_img.c -o libdownload_img.o
//ar rcs libdownload_img.a libdownload_img.o

//-----------------Download image-------------------------------
/**
	return: void
	params: 
		char* url; Download image from this url
		char* filename; Store in filename path
**/
void download_image(char* url, char* filename);

#endif
