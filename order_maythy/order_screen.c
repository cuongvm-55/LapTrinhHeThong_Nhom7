#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

struct Customer {
	const char *name;
	const char *email;
	const char *address;
	const char *phone;
	gboolean is_male;
};

struct Product {
	const char *name;
	const char *color;
	const char *size;
	const int price;
	const int sale_off;
};

struct Customer customer;

GtkBuilder *builder;

/**
Declare all global widgets
*/
GtkWidget *window;
GtkWidget *button_step_1;
GtkWidget *button_step_2;
GtkWidget *button_step_3;
GtkWidget *label_title;
GtkWidget *button_next_step;

/**
Declare all widgets on screen step 1
*/
GtkWidget *box_content_step_1;
GtkWidget *entry_name;
GtkWidget *entry_phone;
GtkWidget *label_lack_name;
GtkWidget *label_lack_phone;

/**
Declare all widgets on screen step 2
*/
GtkWidget *box_content_step_2;
GtkWidget *entry_name_full;
GtkWidget *checkbutton_gender;
GtkWidget *entry_email;
GtkWidget *entry_address;
GtkWidget *entry_phone_full;
GtkWidget *entry_message;

/**
Declare all widget on screen step 3
*/
GtkWidget *box_content_step_3;
GtkWidget *line_name;
GtkWidget *line_address;
GtkWidget *line_phone;
GtkWidget *line_email;
GtkWidget *value_total_amount;
GtkWidget *value_discount;
GtkWidget *value_final_money;
GtkWidget *label_products;

int current_step = 1;
int button_next_click_count = 0;

const gchar *text_name;
const gchar *text_phone;

const gchar *text_name_full;
const gchar *text_email;
const gchar *text_address;
const gchar *text_phone_full;
const gchar *text_message;
gboolean status_gender;

enum {
  COL_NAME = 0,
  COL_PRICE,
  COL_SIZE,
  NUM_COLS
};

int findWidgets(void);
void changeStep(int);
void getInfos(int);
void setInfos(int);
void to_next_step(void);
void printCustomer(struct Customer customer);
void addProductLine(char *data);
char * printProduct(struct Product product);

void closeApp(GtkWidget *window, gpointer data) {
	printf("Quit graphic mode.\n");
	gtk_main_quit();
}

/* Callback allows the application to cancel a close/destroy event (Return TRUE to cancel) */
gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
	//printf("In delete event\n");
	return FALSE;
}

void on_step_1_clicked(GtkWidget *button, gpointer data) {
	current_step = 1;
	changeStep(1);
	printf("Step %d\n", current_step);
}

void on_step_2_clicked(GtkWidget *button, gpointer data) {
	if (current_step >= 2) {
		current_step = 2;
		changeStep(2);
	}
	printf("Step %d\n", current_step);
}

void on_step_3_clicked(GtkWidget *button, gpointer data) {
	if (current_step >= 3) {
		current_step = 3;
		changeStep(3);
	}
	printf("Step %d\n", current_step);
}

/**
Callback for on next step button clicked
*/
void on_next_step_clicked(GtkWidget *button, gpointer data) {
	if (current_step == 1) {
		getInfos(1);

		if (strcmp(text_name, "") == 0) {
			gtk_widget_set_visible(label_lack_name, TRUE);
			gtk_widget_grab_focus(entry_name);
			return;
		}

		if (strcmp(text_phone, "") == 0) {
			gtk_widget_set_visible(label_lack_phone, TRUE);
			gtk_widget_grab_focus(entry_phone);
			return;
		}
	} else if (current_step == 2) {
		getInfos(2);

		if (strcmp(text_name_full, "") == 0) {
			gtk_widget_grab_focus(entry_name_full);
			return;
		}

		if (strcmp(text_phone_full, "") == 0) {
			gtk_widget_grab_focus(entry_phone_full);
			return;
		}
	}

	to_next_step();
	printf("Step %d\n", current_step);
}

void to_next_step() {
	button_next_click_count++;
	if (current_step < 3) {
		current_step++;
	}

	changeStep(current_step);
}

void on_entry_name_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_phone);
}

void on_entry_phone_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(button_next_step);
}

void on_entry_name_full_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_email);
}

void on_entry_email_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_address);
}

void on_entry_address_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_phone_full);
}

void on_entry_phone_full_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_message);
}

void on_entry_message_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(button_next_step);
}

void on_entry_name_changed(GtkWidget *entry, gpointer data) {
	gtk_widget_set_visible(label_lack_name, FALSE);
}

void on_entry_phone_changed(GtkWidget *entry, gpointer data) {
	gtk_widget_set_visible(label_lack_phone, FALSE);
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	int build_status = findWidgets();
	if (build_status == 1) return 1;

	gtk_widget_grab_focus(entry_name);

	// Event handlers
	g_signal_connect(window, "destroy", G_CALLBACK(closeApp), NULL);
	g_signal_connect(window, "delete_event", G_CALLBACK(delete_event), NULL);

	g_signal_connect(button_step_1, "clicked", G_CALLBACK(on_step_1_clicked), NULL);
	g_signal_connect(button_step_2, "clicked", G_CALLBACK(on_step_2_clicked), NULL);
	g_signal_connect(button_step_3, "clicked", G_CALLBACK(on_step_3_clicked), NULL);
	g_signal_connect(button_next_step, "clicked", G_CALLBACK(on_next_step_clicked), NULL);

	g_signal_connect(entry_name, "activate", G_CALLBACK(on_entry_name_activate), NULL);
	g_signal_connect(entry_phone, "activate", G_CALLBACK(on_entry_phone_activate), NULL);
	g_signal_connect(entry_name_full, "activate", G_CALLBACK(on_entry_name_full_activate), NULL);
	g_signal_connect(entry_email, "activate", G_CALLBACK(on_entry_email_activate), NULL);
	g_signal_connect(entry_address, "activate", G_CALLBACK(on_entry_address_activate), NULL);
	g_signal_connect(entry_phone_full, "activate", G_CALLBACK(on_entry_phone_full_activate), NULL);
	g_signal_connect(entry_message, "activate", G_CALLBACK(on_entry_message_activate), NULL);

	g_signal_connect(entry_name, "changed", G_CALLBACK(on_entry_name_changed), NULL);
	g_signal_connect(entry_phone, "changed", G_CALLBACK(on_entry_phone_changed), NULL);

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_show(window);
   
    /* Start main loop */ 
	gtk_main();

	return 0;
}

/**
Function to print data of customer struct
*/
void printCustomer(struct Customer customer) {
	printf("About customer:\n");
	printf("name: %s\n", customer.name);
	printf("email: %s\n", customer.email);
	printf("adress: %s\n", customer.address);
	printf("phone: %s\n", customer.phone);
	printf("is_male: %d\n", customer.is_male);
}

/**
Get infos from form data
*/
void getInfos(int step) {
	if (step == 1) {
		text_name = gtk_entry_get_text(GTK_ENTRY(entry_name));
		text_phone = gtk_entry_get_text(GTK_ENTRY(entry_phone));

		customer.name = text_name;
		customer.phone = text_phone;

	} else if (step == 2) {
		text_name = gtk_entry_get_text(GTK_ENTRY(entry_name_full));
		text_phone = gtk_entry_get_text(GTK_ENTRY(entry_phone_full));
		text_name_full = gtk_entry_get_text(GTK_ENTRY(entry_name_full));
		text_email = gtk_entry_get_text(GTK_ENTRY(entry_email));
		text_address = gtk_entry_get_text(GTK_ENTRY(entry_address));
		text_phone_full = gtk_entry_get_text(GTK_ENTRY(entry_phone_full));
		status_gender = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_gender));
		text_message = gtk_entry_get_text(GTK_ENTRY(entry_message));

		customer.name = text_name_full;
		customer.email = text_email;
		customer.address = text_address;
		customer.phone = text_phone_full;
		if (status_gender) customer.is_male = TRUE;
		else customer.is_male = FALSE;
	}

	printCustomer(customer);
	printf("Message: %s\n", text_message);
}

/**
Fill infos
*/
void setInfos(int step) {
	if (step == 2) {
		gtk_entry_set_text(GTK_ENTRY(entry_name_full), text_name);
		gtk_entry_set_text(GTK_ENTRY(entry_phone_full), text_phone);

	} else if (step == 3) {
		char temp[100];
		char *str_name = (char*) text_name_full;
		
		if (status_gender) {
			strcpy(temp, "Anh ");
		} else {
			strcpy(temp, "Chị ");
		}
		strcat(temp, str_name);

		gtk_label_set_text(GTK_LABEL(line_name), temp);
		gtk_label_set_text(GTK_LABEL(line_address), text_address);
		gtk_label_set_text(GTK_LABEL(line_phone), text_phone_full);
		gtk_label_set_text(GTK_LABEL(line_email), text_email);
	}
}

/**
All actions when chaging a step
*/
void changeStep(int step) {
	if (step == 1) {
		gtk_button_set_relief(GTK_BUTTON(button_step_1), GTK_RELIEF_NORMAL);
		gtk_button_set_relief(GTK_BUTTON(button_step_2), GTK_RELIEF_NONE);
		gtk_button_set_relief(GTK_BUTTON(button_step_3), GTK_RELIEF_NONE);

		gtk_label_set_text(GTK_LABEL(label_title), "ĐẶT HÀNG NHANH");

		gtk_widget_set_visible(box_content_step_1, TRUE);
		gtk_widget_set_visible(box_content_step_2, FALSE);
		gtk_widget_set_visible(box_content_step_3, FALSE);

		gtk_button_set_label(GTK_BUTTON(button_next_step), "Tiếp tục");
	} else if (step == 2) {
		struct Product p;
		p.name = "Ao thun nam";
		//printProduct(p);
		setInfos(step);

		gtk_widget_grab_focus(entry_name_full);

		gtk_button_set_relief(GTK_BUTTON(button_step_1), GTK_RELIEF_NONE);
		gtk_button_set_relief(GTK_BUTTON(button_step_2), GTK_RELIEF_NORMAL);
		gtk_button_set_relief(GTK_BUTTON(button_step_3), GTK_RELIEF_NONE);

		gtk_label_set_text(GTK_LABEL(label_title), "CHI TIẾT THÔNG TIN");

		gtk_widget_set_visible(box_content_step_1, FALSE);
		gtk_widget_set_visible(box_content_step_2, TRUE);
		gtk_widget_set_visible(box_content_step_3, FALSE);

		gtk_button_set_label(GTK_BUTTON(button_next_step), "Tiếp tục");
	} else if (step == 3) {
		setInfos(step);

		gtk_button_set_relief(GTK_BUTTON(button_step_1), GTK_RELIEF_NONE);
		gtk_button_set_relief(GTK_BUTTON(button_step_2), GTK_RELIEF_NONE);
		gtk_button_set_relief(GTK_BUTTON(button_step_3), GTK_RELIEF_NORMAL);

		gtk_label_set_text(GTK_LABEL(label_title), "XÁC NHẬN ĐƠN HÀNG");

		gtk_widget_set_visible(box_content_step_1, FALSE);
		gtk_widget_set_visible(box_content_step_2, FALSE);
		gtk_widget_set_visible(box_content_step_3, TRUE);

		gtk_button_set_label(GTK_BUTTON(button_next_step), "Gửi đơn hàng");
	}
}

void addProductLine(char *data) {
	printf("Data: %s\n", data);
	//GtkWidget *label_product = gtk_label_new("ok");
	//gtk_box_pack_start(GTK_BOX(box_products), label_product, TRUE, FALSE, 0);
}

char *printProduct(struct Product product) {
	char* temp;
	temp = strcpy("Name: ", product.name);
	printf("%s\n", temp);
	return temp;
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
	if(!gtk_builder_add_from_file(builder, "order_screen_4.glade", &error)) {
		g_warning("%s", error->message);
		g_free(error);
		return(1);
	}

	/**
	Find all global widgets
	*/
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	button_step_1 = GTK_WIDGET(gtk_builder_get_object(builder, "button_step_1"));
	button_step_2 = GTK_WIDGET(gtk_builder_get_object(builder, "button_step_2"));
	button_step_3 = GTK_WIDGET(gtk_builder_get_object(builder, "button_step_3"));
	label_title = GTK_WIDGET(gtk_builder_get_object(builder, "label_title"));
	button_next_step = GTK_WIDGET(gtk_builder_get_object(builder, "button_next_step"));

	/**
	Find all widgets on screen step 1
	*/
	box_content_step_1 = GTK_WIDGET(gtk_builder_get_object(builder, "box_content_step_1"));
	entry_name = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name"));
	entry_phone = GTK_WIDGET(gtk_builder_get_object(builder, "entry_phone"));
	label_lack_name = GTK_WIDGET(gtk_builder_get_object(builder, "label_lack_name"));
	label_lack_phone = GTK_WIDGET(gtk_builder_get_object(builder, "label_lack_phone"));

	/**
	Find all widgets on screen step 2
	*/
	box_content_step_2 = GTK_WIDGET(gtk_builder_get_object(builder, "box_content_step_2"));
	entry_name_full = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name_full"));
	checkbutton_gender = GTK_WIDGET(gtk_builder_get_object(builder, "checkbutton_gender"));
	entry_email = GTK_WIDGET(gtk_builder_get_object(builder, "entry_email"));
	entry_address = GTK_WIDGET(gtk_builder_get_object(builder, "entry_address"));
	entry_phone_full = GTK_WIDGET(gtk_builder_get_object(builder, "entry_phone_full"));
	entry_message = GTK_WIDGET(gtk_builder_get_object(builder, "entry_message"));

	/**
	Find all widgets on screen step 3
	*/
	box_content_step_3 = GTK_WIDGET(gtk_builder_get_object(builder, "box_content_step_3"));
	line_name = GTK_WIDGET(gtk_builder_get_object(builder, "line_name"));
	line_address = GTK_WIDGET(gtk_builder_get_object(builder, "line_address"));
	line_phone = GTK_WIDGET(gtk_builder_get_object(builder, "line_phone"));
	line_email = GTK_WIDGET(gtk_builder_get_object(builder, "line_email"));
	value_total_amount = GTK_WIDGET(gtk_builder_get_object(builder, "value_total_amount"));
	value_discount = GTK_WIDGET(gtk_builder_get_object(builder, "value_discount"));
	value_final_money = GTK_WIDGET(gtk_builder_get_object(builder, "value_final_money"));
	label_products = GTK_WIDGET(gtk_builder_get_object(builder, "label_products"));

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));

	return 0;
}