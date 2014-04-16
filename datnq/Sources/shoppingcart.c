#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>

struct product {
	int productId; //id cua san pham
	char *productCode; //ma quan li cua san pham
	char *productName; //ten san pham
	int typeId; //id cua productType tuong ung
	char *brand; //nhan hieu san pham
	unsigned int price; //gia
	int quantityInStock; //so luong co trong kho hang
	char *description; //mo ta san pham
	char *image; //link anh san pham
	int saleOff; //phan tram giam gia, e.x. 20 <-> 20%, 30 <-> 30%
};

struct product products[5];
int productSize = 5;
int nowProductListSize = 5; // So luong san pham trong gio hang hien tai

/**
Declare all global widgets
*/
GtkWidget *window;
GtkBuilder *builder;

/**
Declare title widgets
*/
GtkWidget *lbQuantity;
GtkWidget *lbSum;
GtkWidget *btnBuy;

/**
Declare all widget in product 1
*/
struct widget
{
	int productId;
	GtkWidget *box;
	GtkWidget *img;
	GtkWidget *lbDetail;
	GtkWidget *btnRemove;
};

struct widget widgets[5];
int widgetSize = 5;

// Prototype
int findWidgets(void);
unsigned int getSum();
int getQuantity();
void displaySumAndQuantity();
void removeProductInArray(int index);

/* Callback allows the application to cancel a close/destroy event (Return TRUE to cancel) */
gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
	//printf("In delete event\n");
	return FALSE;
}

void closeApp(GtkWidget *window, gpointer data) {
	printf("Quit graphic mode.\n");
	gtk_main_quit();
}

// On event clicked

void removeProduct(GtkWidget *widget, GdkEvent *event, gpointer data){
	int i = 0;
	for (i = 0; i < widgetSize; i++)
	{
		if (widgets[i].btnRemove == widget)
		{
			gtk_widget_set_visible(widgets[i].box, FALSE);
			gtk_widget_set_visible(widgets[i].img, FALSE);
			gtk_widget_set_visible(widgets[i].lbDetail, FALSE);
			gtk_widget_set_visible(widgets[i].btnRemove, FALSE);
			break;
		}
	}

	removeProductInArray(i);
}
// Next to order step
void nextToOrderStep(GtkWidget *widget, GdkEvent *event, gpointer data){
	printf("Next to order step\n");
}

void initProduct() {
	//struct product *products = (product *) malloc(sizeof(product) * 10);

	products[0].productId = 1;
	products[0].productName = "Ao Thun nam";
	products[0].brand = "May 11";
	products[0].price = 300000;
	products[0].saleOff = 20;
	products[0].image = "aothunnam1.png";

	products[1].productId = 2;
	products[1].productName = "Ao Thun nam 2";
	products[1].brand = "May 11";
	products[1].price = 400000;
	products[1].saleOff = 30;
	products[1].image = "aothunnam1.png";

	products[2].productId = 3;
	products[2].productName = "Ao Thun nam 3";
	products[2].brand = "May 11";
	products[2].price = 500000;
	products[2].saleOff = 40;
	products[2].image = "aothunnam1.png";

	products[3].productId = 4;
	products[3].productName = "Ao Thun nam 4";
	products[3].brand = "May 11";
	products[3].price = 600000;
	products[3].saleOff = 50;
	products[3].image = "aothunnam1.png";

	products[4].productId = 5;
	products[4].productName = "Ao Thun nam 5";
	products[4].brand = "May 11";
	products[4].price = 600000;
	products[4].saleOff = 50;
	products[4].image = "aothunnam1.png";

	// Init widget 
	int i=0;
	for (i = 0; i < productSize; i++)
	{
		char *content = (char *) malloc(sizeof(char) * 200);

		sprintf(content, "Ten: %s\n Gia: %d \tGiam gia: %d %%",
		products[i].productName, products[i].price, products[i].saleOff);
		gtk_label_set_text(GTK_LABEL(widgets[i].lbDetail), content);
		gtk_image_set_from_file(GTK_IMAGE(widgets[i].img),products[i].image);

		widgets[i].productId = products[i].productId;
	}

	// Remove extra wigets
	i = productSize;
	for (i = productSize; i < widgetSize; i++)
	{
		gtk_widget_set_visible(widgets[i].box, FALSE);
		gtk_widget_set_visible(widgets[i].img, FALSE);
		gtk_widget_set_visible(widgets[i].lbDetail, FALSE);
		gtk_widget_set_visible(widgets[i].btnRemove, FALSE);
	}

	// gtk_image_set_from_resource(GTK_IMAGE(widgets[0].img), products[0].image); 
	
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	int build_status = findWidgets();
	if (build_status == 1) return 1;

	initProduct();

	displaySumAndQuantity();

	// Event handler
	g_signal_connect(window, "destroy", G_CALLBACK(closeApp), NULL);
	g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);

	int i=0;
	for (i = 0; i < widgetSize; i++){
			g_signal_connect(widgets[i].btnRemove, "clicked", G_CALLBACK(removeProduct), NULL);
	}	
	// Next to order step
	g_signal_connect(btnBuy, "clicked", G_CALLBACK(nextToOrderStep), NULL);

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_show(window);
	/* Start main loop */ 
	gtk_main();	
	return 0;
}

/**
Function to find all widgets from builder
*/
int findWidgets() {
	// Gerror
	GError *error = NULL;

	builder = gtk_builder_new();

	/* Load UI from file. If error occurs, report it and quit application.  
	 * Replace "tut.glade" with your saved project. */
	if(!gtk_builder_add_from_file(builder, "ShoppingCartGTK.glade", &error)) {
		g_warning("%s", error->message);
		g_free(error);
		return(1);
	}

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));

	widgets[0].box = GTK_WIDGET(gtk_builder_get_object(builder, "box2"));
	widgets[0].img = GTK_WIDGET(gtk_builder_get_object(builder, "img1"));
	widgets[0].lbDetail = GTK_WIDGET(gtk_builder_get_object(builder, "lbDetail1"));
	widgets[0].btnRemove = GTK_WIDGET(gtk_builder_get_object(builder, "btnRemove1"));

	widgets[1].box = GTK_WIDGET(gtk_builder_get_object(builder, "box3"));
	widgets[1].img = GTK_WIDGET(gtk_builder_get_object(builder, "img2"));
	widgets[1].lbDetail = GTK_WIDGET(gtk_builder_get_object(builder, "lbDetail2"));
	widgets[1].btnRemove = GTK_WIDGET(gtk_builder_get_object(builder, "btnRemove2"));

	widgets[2].box = GTK_WIDGET(gtk_builder_get_object(builder, "box4"));
	widgets[2].img = GTK_WIDGET(gtk_builder_get_object(builder, "img3"));
	widgets[2].lbDetail = GTK_WIDGET(gtk_builder_get_object(builder, "lbDetail3"));
	widgets[2].btnRemove = GTK_WIDGET(gtk_builder_get_object(builder, "btnRemove3"));

	widgets[3].box = GTK_WIDGET(gtk_builder_get_object(builder, "box5"));
	widgets[3].img = GTK_WIDGET(gtk_builder_get_object(builder, "img4"));
	widgets[3].lbDetail = GTK_WIDGET(gtk_builder_get_object(builder, "lbDetail4"));
	widgets[3].btnRemove = GTK_WIDGET(gtk_builder_get_object(builder, "btnRemove4"));

	widgets[4].box = GTK_WIDGET(gtk_builder_get_object(builder, "box6"));
	widgets[4].img = GTK_WIDGET(gtk_builder_get_object(builder, "img5"));
	widgets[4].lbDetail = GTK_WIDGET(gtk_builder_get_object(builder, "lbDetail5"));
	widgets[4].btnRemove = GTK_WIDGET(gtk_builder_get_object(builder, "btnRemove5"));

	lbQuantity = GTK_WIDGET(gtk_builder_get_object(builder, "lbQuantity"));
	lbSum = GTK_WIDGET(gtk_builder_get_object(builder, "lbSum"));
	btnBuy = GTK_WIDGET(gtk_builder_get_object(builder, "btnBuy"));

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));

	return 0;
}

unsigned int getSum(){
	unsigned int sum = 0;
	int i = 0;
	for (i = 0; i < productSize; i++)
	{
		sum += products[i].price * (100 - products[i].saleOff) / 100;
	}
	return sum;
}

int getQuantity(){
	return productSize;
}
 
void displaySumAndQuantity(){
	char *sum = (char *) malloc(sizeof(char) * 200);
	sprintf(sum, "Tong tien: %d ", getSum());
	gtk_label_set_text(GTK_LABEL(lbSum), sum);

	char *quantity = (char *) malloc(sizeof(char) * 200);
	sprintf(quantity, "So luong: %d ", getQuantity());
	gtk_label_set_text(GTK_LABEL(lbQuantity), quantity);	
}


// Remove a product in products array
void removeProductInArray(int indexWidget){
	// Get position product
	int position = 0;
	for (position = 0; position < productSize; position++)
	{
		if (products[position].productId == widgets[indexWidget].productId)
		{
			int i=position;
			for (i = position; i < productSize-1; i++){
				products[i] = products[i+1];
			}		
			productSize--;
		}
	}
	
	displaySumAndQuantity();
}