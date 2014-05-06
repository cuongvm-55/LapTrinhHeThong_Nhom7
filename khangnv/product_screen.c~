#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

//compile: gcc -o product_screen product_screen.c `pkg-config gtk+-3.0  --cflags --libs`

struct Admin {
	const char *username;
	const char *password;
};

struct Product {
	const int id;
	const char *name;
	const int price;
	const int number;
	const int saleoff;
	const char *description;
};

struct Admin admin;
struct Product product;
GtkBuilder *builder;

//Widget window_login
GtkWidget *window_login;
GtkWidget *label_login;
GtkWidget *button_login;
GtkWidget *button_cancel;
GtkWidget *entry_username;
GtkWidget *entry_password;
GtkWidget *label_lack_username;
GtkWidget *label_lack_password;

//Widget window_management
GtkWidget *window_management;
GtkWidget *label_tieude;
GtkWidget *entry_id;
GtkWidget *entry_name;
GtkWidget *entry_price;
GtkWidget *entry_number;
GtkWidget *entry_saleoff;
GtkWidget *entry_description;
GtkWidget *button_add;

//Widget window_edit
GtkWidget *window_edit;
GtkWidget *entry_id_edit;
GtkWidget *entry_name_edit;
GtkWidget *entry_price_edit;
GtkWidget *entry_number_edit;
GtkWidget *entry_saleoff_edit;
GtkWidget *entry_description_edit;
GtkWidget *button_ok;

GtkWidget *box_product_list;

//
const gchar *username;
const gchar *password;

const gint *id;
const gchar *name;
const gint *price;
const gint *number;
const gint *saleoff;
const gchar *description;

//close program
void closeApp(GtkWidget *window, gpointer data) {
	printf("Quit graphic mode.\n");
	gtk_main_quit();
}

gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
	//printf("In delete event\n");
	return FALSE;
}

//
void on_entry_username_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_password);
}

void on_entry_password_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(button_login);
}

void on_entry_username_changed(GtkWidget *entry, gpointer data) {
	gtk_widget_set_visible(label_lack_username, FALSE);
}

void on_entry_password_changed(GtkWidget *entry, gpointer data) {
	gtk_widget_set_visible(label_lack_password, FALSE);
}
//
void on_entry_id_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_name);
}

void on_entry_name_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_price);
}

void on_entry_price_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_number);
}

void on_entry_number_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_saleoff);
}

void on_entry_saleoff_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_description);
}

void on_entry_description_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(button_add);
}
//
void on_entry_id_edit_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_name_edit);
}

void on_entry_name_edit_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_price_edit);
}

void on_entry_price_edit_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_number_edit);
}

void on_entry_number_edit_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_saleoff_edit);
}

void on_entry_saleoff_edit_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(entry_description_edit);
}

void on_entry_description_edit_activate(GtkWidget *entry, gpointer data) {
	gtk_widget_grab_focus(button_ok);
}

//
void on_login_clicked(GtkWidget *button, gpointer data)
{
	username = gtk_entry_get_text(GTK_ENTRY(entry_username));
	password = gtk_entry_get_text(GTK_ENTRY(entry_password));

	admin.username = username;
	admin.password = password;
	if (strcmp(username, "") == 0) {
			gtk_widget_set_visible(label_lack_username, TRUE);
			gtk_widget_grab_focus(entry_username);
			return;
	}

	if (strcmp(password, "") == 0) {
			gtk_widget_set_visible(label_lack_password, TRUE);
			gtk_widget_grab_focus(entry_password);
			return;
	}

	gtk_widget_hide(GTK_WIDGET(window_login));
	gtk_widget_show_all(GTK_WIDGET(window_management));

}

/*Xu li trang sua thong tin san pham
void manipulate_edit_window(gpointer data){
  	GtkBuilder *builder = gtk_builder_new();
  	gtk_builder_add_from_file(builder, "product_screen.glade", NULL);
   	
	//lấy các thành phần của window_edit
  	window_edit = GTK_WIDGET(gtk_builder_get_object(builder,"window_edit"));
	entry_id_edit = GTK_WIDGET(gtk_builder_get_object(builder, "entry_id_edit"));
	entry_name_edit = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name_edit"));
	entry_price_edit = GTK_WIDGET(gtk_builder_get_object(builder, "entry_price_edit"));
	entry_number_edit = GTK_WIDGET(gtk_builder_get_object(builder, "entry_number_edit"));
	entry_saleoff_edit = GTK_WIDGET(gtk_builder_get_object(builder, "entry_saleoff_edit"));
	entry_description_edit = GTK_WIDGET(gtk_builder_get_object(builder, "entry_description_edit"));

	button_ok = GTK_WIDGET(gtk_builder_get_object(builder, "button_ok"));
	
	//kết nối sự kiện
	g_signal_connect(window_edit, "destroy", G_CALLBACK(closeApp), NULL);
	g_signal_connect(window_edit, "delete_event", G_CALLBACK(delete_event), NULL);

	g_signal_connect(entry_id_edit, "activate", G_CALLBACK(on_entry_id_edit_activate), NULL);
	g_signal_connect(entry_name_edit, "activate", G_CALLBACK(on_entry_name_edit_activate), NULL);
	g_signal_connect(entry_price_edit, "activate", G_CALLBACK(on_entry_price_edit_activate), NULL);
	g_signal_connect(entry_number_edit, "activate", G_CALLBACK(on_entry_number_edit_activate), NULL);
	g_signal_connect(entry_saleoff_edit, "activate", G_CALLBACK(on_entry_saleoff_edit_activate), NULL);
	g_signal_connect(entry_description_edit, "activate", G_CALLBACK(on_entry_description_edit_activate), NULL);

	//g_signal_connect(button_ok, "clicked", G_CALLBACK(on_ok_clicked), NULL);
	gtk_widget_show_all(window_edit);
	
}*/
//
int findWidgets()
{
	// Gerror
	GError *error = NULL;

	builder = gtk_builder_new();

	/* Load UI from file. If error occurs, report it and quit application.  
	 * Replace "tut.glade" with your saved project. */
	if(!gtk_builder_add_from_file(builder, "product_screen.glade", &error)) {
		g_warning("%s", error->message);
		g_free(error);
		return(1);
	}
	
	//lấy các thành phần window_login
	window_login = GTK_WIDGET(gtk_builder_get_object(builder, "window_login"));
	label_login = GTK_WIDGET(gtk_builder_get_object(builder, "label_login"));
	button_login = GTK_WIDGET(gtk_builder_get_object(builder, "button_login"));
	button_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "button_cancel"));
	
	entry_username = GTK_WIDGET(gtk_builder_get_object(builder, "entry_username"));
	entry_password = GTK_WIDGET(gtk_builder_get_object(builder, "entry_password"));
	label_lack_username = GTK_WIDGET(gtk_builder_get_object(builder, "label_lack_username"));
	label_lack_password = GTK_WIDGET(gtk_builder_get_object(builder, "label_lack_password"));

	//lấy các thành phần window_management
	window_management = GTK_WIDGET(gtk_builder_get_object(builder, "window_management"));
	label_tieude = GTK_WIDGET(gtk_builder_get_object(builder,"label_tieude"));
	entry_id = GTK_WIDGET(gtk_builder_get_object(builder, "entry_id"));
	entry_name = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name"));
	entry_price = GTK_WIDGET(gtk_builder_get_object(builder, "entry_price"));
	entry_number = GTK_WIDGET(gtk_builder_get_object(builder, "entry_number"));
	entry_saleoff = GTK_WIDGET(gtk_builder_get_object(builder, "entry_saleoff"));
	entry_description = GTK_WIDGET(gtk_builder_get_object(builder, "entry_description"));

	button_add = GTK_WIDGET(gtk_builder_get_object(builder, "button_add"));
	
	box_product_list = GTK_WIDGET(gtk_builder_get_object(builder, "box_product_list"));

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));
	return 0;
}
//Sublime
//------------------------Tao bang product---------------------------------------------------------
enum{
	COL_STT = 0,
	COL_NAME,
	COL_DESCRIPTION,
	NUM_COLS
};


static GtkTreeModel *create_and_fill_model(void)
{
	GtkListStore *store;
	GtkTreeIter	iter;
	/* Create a new button */
    button = gtk_button_new ();

	store = gtk_list_store_new(NUM_COLS, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	
	int i =0, n=5; //
	for (i=1; i<=n;i++){
		gtk_container_add (GTK_CONTAINER (button), box1);
		//them 1 dong va them du lieu
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter,
						COL_STT, i,
						COL_NAME, "product name",
						COL_DESCRIPTION, "product description",
						-1);
	}
	return GTK_TREE_MODEL(store);

}


static GtkWidget *create_view_and_model()
{

	GtkCellRenderer *renderer;
	GtkTreeModel *model;
	GtkWidget *view;

	view = gtk_tree_view_new();
	gtk_tree_view_set_headers_clickable (GTK_TREE_VIEW(view), TRUE);


	//Cot #1
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
												-1,
												"STT",
												renderer,
												"text", 0,
												NULL);
	//Cot #2
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
												-1,
												"DEL",
												renderer,
												"text", 1,
												NULL);
	//Cot #3
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
												-1,
												"EDIT",
												renderer,
												"text", 2,
												NULL);
	//Cot #4
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
												-1,
												"NAME",
												renderer,
												"text", 3,
												NULL);
	//Cot #5
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
												-1,
												"DESCRIPTION",
												renderer,
												"text", 4,
												NULL);
	
	model = create_and_fill_model();

	gtk_tree_view_set_model (GTK_TREE_VIEW(view), model);

	/* The tree view has acquired its own reference to the
   *  model, so we can drop ours. That way the model will
   *  be freed automatically when the tree view is destroyed */

  	g_object_unref (model);

  	return view;
}
//main
int main(int argc, char *argv[])
{
	//khởi động chế độ đồ họa
	gtk_init(&argc, &argv);

	int build_status = findWidgets();
	if (build_status == 1) return 1;
	
	//kết nối sự kiện với window_login
	g_signal_connect(window_login, "destroy", G_CALLBACK(closeApp), NULL);
	g_signal_connect(window_login, "delete_event", G_CALLBACK(delete_event), NULL);
	
	g_signal_connect(entry_username, "activate", G_CALLBACK(on_entry_username_activate), NULL);
	g_signal_connect(entry_password, "activate", G_CALLBACK(on_entry_password_activate), NULL);

	g_signal_connect(entry_username, "changed", G_CALLBACK(on_entry_username_changed), NULL);
	g_signal_connect(entry_password, "changed", G_CALLBACK(on_entry_password_changed), NULL);

	g_signal_connect(button_login, "clicked", G_CALLBACK(on_login_clicked), NULL);
	g_signal_connect(button_cancel, "clicked", G_CALLBACK(closeApp), NULL);

	//kết nối sự kiện với window_management
	g_signal_connect(window_management, "destroy", G_CALLBACK(closeApp), NULL);
	g_signal_connect(window_management, "delete_event", G_CALLBACK(delete_event), NULL);

	g_signal_connect(entry_id, "activate", G_CALLBACK(on_entry_id_activate), NULL);
	g_signal_connect(entry_name, "activate", G_CALLBACK(on_entry_name_activate), NULL);
	g_signal_connect(entry_price, "activate", G_CALLBACK(on_entry_price_activate), NULL);
	g_signal_connect(entry_number, "activate", G_CALLBACK(on_entry_number_activate), NULL);
	g_signal_connect(entry_saleoff, "activate", G_CALLBACK(on_entry_saleoff_activate), NULL);
	g_signal_connect(entry_description, "activate", G_CALLBACK(on_entry_description_activate), NULL);

	//g_signal_connect(button_add, "clicked", G_CALLBACK(add_product), NULL);
	//g_signal_connect(button_delete, "clicked", G_CALLBACK(delete_product), NULL);

	//set các thuộc tính của màn hình
	gtk_window_set_position(GTK_WINDOW(window_login), GTK_WIN_POS_CENTER);
	gtk_widget_show(window_login);

	gtk_window_set_position(GTK_WINDOW(window_management), GTK_WIN_POS_CENTER);
	
	//Them bang static void fill_category_box(GtkWidget *vbox)

  	GtkTreeSelection *selection;
  	GtkWidget *view;

  	view = create_view_and_model();
  	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));

  	GdkColor color;
  	color.red = 27000;
  	color.green = 30325;
  	color.blue = 34181;
  	gtk_widget_modify_bg(view, GTK_STATE_PRELIGHT, &color);
  	gtk_container_add(GTK_CONTAINER(box_product_list), view);
  
	//gtk_widget_show(window_management);

   
    /* Start main loop */ 
	gtk_main();

	return 0;
}
