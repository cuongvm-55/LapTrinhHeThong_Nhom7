#ifndef ORDER_H_
#define ORDER_H_


struct Customer {
	char *id;
	const char *name;
	const char *email;
	const char *address;
	const char *phone;
	gboolean is_male;
};

// struct Product {
// 	const char *name;
// 	const char *brand;
// 	int price;
// 	int sale_off;
// };

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
	int current_quantity;
};

int findWidgets(void);
void changeStep(int);
void getInfos(int);
void setInfos(int);
void to_next_step(void);
void printCustomer(struct Customer customer);
void addProductLine(char *data);
char *printProduct(struct product product);
void fillAllProducts();

void closeApp(GtkWidget *window, gpointer data);
// char* get_data(char *type, json_t *data);
// const char *get_value(json_t *root, char* key);
// int is_customer_info_changes(json_t* root, const gchar* name, const gchar* email, const gchar* address, gboolean gender);
gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

void on_step_1_clicked(GtkWidget *button, gpointer data);
void on_step_2_clicked(GtkWidget *button, gpointer data);
void on_step_3_clicked(GtkWidget *button, gpointer data);
void on_next_step_clicked(GtkWidget *button, gpointer data);
void to_next_step();
void on_entry_name_activate(GtkWidget *entry, gpointer data);
void on_entry_phone_activate(GtkWidget *entry, gpointer data);
void on_entry_name_full_activate(GtkWidget *entry, gpointer data);
void on_entry_email_activate(GtkWidget *entry, gpointer data);
void on_entry_address_activate(GtkWidget *entry, gpointer data);
void on_entry_phone_full_activate(GtkWidget *entry, gpointer data);
void on_entry_message_activate(GtkWidget *entry, gpointer data);
void on_entry_name_changed(GtkWidget *entry, gpointer data);
void on_entry_phone_changed(GtkWidget *entry, gpointer data);

void testfun();

#endif