#include <gtk/gtk.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include "lib/mysql_sockets/libmysql_socket.h"
#include "lib/curl/libdownload_img.h"

//COMPILE
/*gcc -g main.c -o main 
      `pkg-config gtk+-3.0 --cflags --libs`
      -L lib/mysql_sockets -lmysql_socket 
      -L lib/curl -ldownload_img
      -ljansson
      -lpthread
      `pkg-config libcurl --cflags --libs`
*/

//-------------------------------------------Cac du lieu dung chung-------------------------------------------------------
int sockfd;


//-------------------------------------------Du lieu danh sach------------------------------------------------------------
struct product product_list[30]; //luu mang product tra ve tu server
int product_list_size; //do lon mang product_list
char* image_list[30]; //mang luu duong dan cua imgs


//Xu li su kien tat window
static void destroy(GtkWidget*, gpointer);
static gboolean delete_event(GtkWidget*, GdkEvent*, gpointer);

//Xu li su kien thay doi hinh con tro chuot
static gboolean enter_func( GtkWidget *widget, gpointer   data );
static gboolean leave_func( GtkWidget *widget, gpointer   data );

//Enum du lieu thanh Category
enum
{
  COLUMN = 0,
  NUM_COLS
} ;

void  on_changed(GtkWidget *widget, gpointer statusbar) 
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  char *value;


  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, COLUMN, &value,  -1);
   
    g_free(value);
  }
}

static GtkTreeModel *
create_and_fill_model (void)
{
  GtkTreeStore *treestore;
  GtkTreeIter toplevel, child;
  int i=0, n=3;

  //cac bien thao tac server
  struct productType product_type_list[30]; //luu mang product type tra ve tu server
  int product_type_list_size; //do lon mang product_type_list

  treestore = gtk_tree_store_new(NUM_COLS,
                  G_TYPE_STRING);
  gtk_tree_store_append(treestore, &toplevel, NULL);
  gtk_tree_store_set(treestore, &toplevel,
                     COLUMN, "Hang moi",
                     -1);

  gtk_tree_store_append(treestore, &toplevel, NULL);
  gtk_tree_store_set(treestore, &toplevel,
                     COLUMN, "Nam",
                     -1);

  //lay tat ca cac loai quan ao cua "Nam" nhan tu server
  get_product_type_by_gender(sockfd, "1", product_type_list, &product_type_list_size);
  
  for (i=0;i<product_type_list_size;i++){
    gtk_tree_store_append(treestore, &child, &toplevel);
    gtk_tree_store_set(treestore, &child, COLUMN, product_type_list[i].typeName);
  }


  gtk_tree_store_append(treestore, &toplevel, NULL);
  gtk_tree_store_set(treestore, &toplevel,
                     COLUMN, "Nu",
                     -1);

  //lay tat ca cac loai quan ao cua "Nu" nhan tu server
  get_product_type_by_gender(sockfd, "2", product_type_list, &product_type_list_size);
  for (i=0;i<product_type_list_size;i++){
    gtk_tree_store_append(treestore, &child, &toplevel);
	  gtk_tree_store_set(treestore, &child, COLUMN, product_type_list[i].typeName);
  }

  return GTK_TREE_MODEL(treestore);
}



static GtkWidget *
create_view_and_model (void)
{
  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkWidget *view;
  GtkTreeModel *model;

  view = gtk_tree_view_new();
  col = gtk_tree_view_column_new();

  gtk_tree_view_column_set_title(col, "Danh Muc");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  gtk_widget_set_tooltip_text(view, "Categories");
  //gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view), FALSE);

  g_signal_connect( G_OBJECT( view ), "enter-notify-event",
                 G_CALLBACK( enter_func ), NULL );
   g_signal_connect( G_OBJECT( view ), "leave-notify-event",
                 G_CALLBACK( enter_func ), NULL );

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, 
      "text", COLUMN);

  model = create_and_fill_model();
  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
  g_object_unref(model); 

  return view;
}

static void fill_category_box(GtkWidget *vbox)
{
  GtkTreeSelection *selection;
  GtkWidget *view;

  view = create_view_and_model();
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));

  GdkColor color;
  color.red = 27000;
  color.green = 30325;
  color.blue = 34181;
  gtk_widget_modify_bg(view, GTK_STATE_PRELIGHT, &color);

  gtk_box_pack_start(GTK_BOX(vbox), view, TRUE, TRUE, 1);
  
}


//Tao ImageView
static GtkWidget*
  create_image (char* filename)
{
  GtkWidget *image;

  image = gtk_image_new_from_file (filename);
  return image;
}

//Xu li trang xem thong tin chi tiet san pham
void manipulate_detail_window(gpointer data){

  //-------------------------------Giao dien---------------------------------------------------------------
  GtkBuilder *builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "home.glade", NULL);
    
  GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder,"window_product_detail"));
  GtkWidget *box_img = GTK_WIDGET(gtk_builder_get_object(builder,"box_img"));
  GtkWidget *frame_product_name = GTK_WIDGET(gtk_builder_get_object(builder,"frame_product_name"));
  GtkWidget *frame_price = GTK_WIDGET(gtk_builder_get_object(builder,"frame_price"));
  GtkWidget *frame_description = GTK_WIDGET(gtk_builder_get_object(builder, "frame_description"));
  GtkWidget *button_add_to_card = GTK_WIDGET(gtk_builder_get_object(builder, "add_to_card"));

  gtk_window_set_title(GTK_WINDOW(window), "Product Detail Infomations");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  gtk_widget_set_size_request (window, 850, 450);
  gtk_widget_set_size_request (box_img, 400, 400);
  gtk_widget_set_size_request (frame_product_name, 450, 50);
  gtk_widget_set_size_request (frame_description, 450, 350);
  gtk_widget_set_size_request (frame_price, 400, 50);

  gtk_frame_set_label(GTK_FRAME(frame_price), "Price");
  gtk_frame_set_label(GTK_FRAME(frame_product_name), "Product Name");
  gtk_frame_set_label(GTK_FRAME(frame_description), "Description");

  //----------------------------Lay thong tin san pham ma nguoi dung click-----------------------------------
  int index = (int)data;
  char product_price[30];
  strcpy(product_price, "");
  sprintf(product_price, "Gia %lf vnd", product_list[index].price);
  

  //----------------------------Phan hien thi anh san pham lon-----------------------------------------------------------------
  GError *error = 0;
  GtkWidget *product_img;
  GdkPixbuf *pixbuf;

  pixbuf = gdk_pixbuf_new_from_file_at_scale(image_list[index], 395, 400, FALSE,&error);
  //pixbuf = gdk_pixbuf_new_from_file_at_size ("temp/shirt_girl1.jpg", 200, 210, &error);
  //gdk_pixbuf_scale_simple(pixbuf, 200, 210, GDK_INTERP_BILINEAR);
  if (!pixbuf)
  {
    g_print ("Error: %s\n", error->message);
    g_error_free (error);
    /* Handle error here */
  }
  
  product_img = gtk_image_new_from_pixbuf(pixbuf);
  g_object_unref (pixbuf);


  GtkWidget *event_box = gtk_event_box_new();
  gtk_container_add(GTK_CONTAINER(event_box), product_img);
  gtk_container_add(GTK_CONTAINER(box_img), event_box);
  gtk_widget_set_tooltip_text(product_img, product_list[index].productName);

  //----------------------------Hien thi ten, mo ta, add to card----------------------------------------------------------------
  //--------------Hien thi ten sp----------------------------------------------------------------------------------
  GtkWidget *label_name = gtk_label_new(product_list[index].productName);
  gtk_label_set_selectable(GTK_LABEL(label_name), TRUE);
  gtk_container_add(GTK_CONTAINER(frame_product_name), label_name);
  

  //---------------Hien thi gia-------------------------------------------------------------------------------------
  GtkWidget *label_price = gtk_label_new(product_price);
  gtk_label_set_selectable(GTK_LABEL(label_price), TRUE);
  gtk_container_add(GTK_CONTAINER(frame_price), label_price);

  //---------------Hien thi thong tin mo ta-------------------------------------------------------------------------
  GtkWidget *label_description = gtk_label_new(product_list[index].description);
  gtk_label_set_selectable(GTK_LABEL(label_description), TRUE);
  gtk_label_set_line_wrap(GTK_LABEL(label_description), TRUE);

  gtk_container_add(GTK_CONTAINER(frame_description), label_description);
  
  gtk_widget_set_tooltip_text(button_add_to_card, "Add product to your card");
  g_signal_connect( G_OBJECT( button_add_to_card ), "enter-notify-event",
                 G_CALLBACK( enter_func ), NULL );
  g_signal_connect( G_OBJECT( button_add_to_card ), "leave-notify-event",
                 G_CALLBACK( leave_func ), NULL );

  gtk_widget_show_all(window);

}

//Ham xu li su kien click vao anh san pham de xem thong tin chi tiet
static gboolean
 button_press_callback (GtkWidget      *event_box,
                        GdkEventButton *event,
                        gpointer        data)
{
  //gint thisClick = (gint) data;

  //g_print("Click: dong %d - cot %d\n", thisClick, thisClick);
  manipulate_detail_window(data);

 /* Returning TRUE means we handled the event, so the signal
  * emission should be stopped (don't call any further
  * callbacks that may be connected). Return FALSE
  * to continue invoking callbacks.
  */
 return TRUE;
}

//Download image va tra lai mang la cac img paths-duong dan den file anh
void get_image(){
  int i = 0;
  char* url;
  //Tao duong dan luu cac anh tu dong
  for (i=0;i<product_list_size;i++){
    image_list[i] = (char*)malloc(1* sizeof(char*));
    //url = (char*)malloc(1* sizeof(char*));
    strcpy(image_list[i], "");
    sprintf(image_list[i], "temp/img_%d.jpg",i);
    strcpy(url,product_list[i].image);
    download_image(url,image_list[i]);
  }
}

int main(int argc, char *argv[])
{
  GtkBuilder *builder;
	GtkWidget *window, *box;
  GtkWidget *grid_content;
  GtkWidget *button_view_card;
  GtkWidget *button_login;
  GtkWidget *box_paging;
  GtkWidget *button_prev;
  GtkWidget *button_next;

  /*-- CSS ------------------*/
  GtkCssProvider *provider;
  GdkDisplay *display;
  GdkScreen *screen;
  /*---------------------------*/

	//Khoi tao GTK+ va cac thu vien lien quan.
	gtk_init(&argc, &argv);

  /*---------------- CSS ----------------------------------------------------------------------------------------------------*/
  provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  screen = gdk_display_get_default_screen (display);
  gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gsize bytes_written, bytes_read;

  const gchar* home = "css/home.css";  // your path, for instance: /home/zerohour/Documents/programming/cal1.css

  GError *error = 0;

  gtk_css_provider_load_from_path (provider,
                                   g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),
                                   NULL);
  g_object_unref (provider);
/*-------------------------------------------------------------------------------------------------------------------------*/

  //-------------Lay cac thanh phan cua giao dien--------------------------------------------------------------------------
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "home.glade", NULL);
    
	window = GTK_WIDGET(gtk_builder_get_object(builder,"window_home"));
	box = GTK_WIDGET(gtk_builder_get_object(builder,"box8"));
  grid_content = GTK_WIDGET(gtk_builder_get_object(builder, "grid1"));
  button_login = GTK_WIDGET(gtk_builder_get_object(builder, "button_login"));
  button_view_card = GTK_WIDGET(gtk_builder_get_object(builder, "button_view_card"));
  box_paging = GTK_WIDGET(gtk_builder_get_object(builder, "box_paging"));
  button_prev = GTK_WIDGET(gtk_builder_get_object(builder, "button_prev"));
  button_next = GTK_WIDGET(gtk_builder_get_object(builder, "button_next"));

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref (G_OBJECT(builder));

  //-------------Chinh cac thuoc tinh cho cac thanh phan-------------------------------------------------------------------
	gtk_window_set_title(GTK_WINDOW(window), "Home Page");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  
	//gtk_container_set_border_width(GTK_CONTAINER (window), -1); //Tricks
	gtk_widget_set_size_request (window, 1000, 700);
  gtk_widget_set_size_request (box, 150, 550);
  gtk_widget_set_size_request (grid_content, 850, 450);

	//-------------Xu li su kien--------------------------------------------------------------------------------------------
	g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL);
	g_signal_connect (G_OBJECT(window), "delete-event", G_CALLBACK(delete_event), NULL);

  g_signal_connect( G_OBJECT( button_login ), "enter-notify-event",
                 G_CALLBACK( enter_func ), NULL );
  g_signal_connect( G_OBJECT( button_login ), "leave_notify_event",
                 G_CALLBACK( leave_func ), NULL );

  g_signal_connect( G_OBJECT( button_view_card ), "enter-notify-event",
                 G_CALLBACK( enter_func ), NULL );
  g_signal_connect( G_OBJECT( button_view_card ), "leave-notify-event",
                 G_CALLBACK( leave_func ), NULL );

  g_signal_connect( G_OBJECT( button_prev ), "enter-notify-event",
                 G_CALLBACK( enter_func ), NULL );
  g_signal_connect( G_OBJECT( button_prev ), "leave-notify-event",
                 G_CALLBACK( leave_func ), NULL );

  g_signal_connect( G_OBJECT( button_next ), "enter-notify-event",
                 G_CALLBACK( enter_func ), NULL );
  g_signal_connect( G_OBJECT( button_next ), "leave-notify-event",
                 G_CALLBACK( leave_func ), NULL );

  //---------------Tao ket noi socket den server-------------------------------------------------------------------------
  sockfd = get_sockfd();
  //---------------Tao cot Category ben trai man hinh---------------------------------------------------------------------
	fill_category_box(box);
  
  //---------------Render danh sach san pham------------------------------------------------------------------------------
  
  //Lay danh sach san pham tu server
  get_all_product(sockfd, "0", product_list, &product_list_size);

  //Download images tu web
  get_image();

  int i=0, x=-1 ,y=0, rows=2, columns = 2;
  /*for (i=0;i<product_list_size;i++){
    printf("%s\n", image_list[i]);
  }*/
  for (i=0;i<product_list_size;i++){
      //Ly thong tin cua tung product
      char product_name[60];
      char product_price[30];
      strcpy(product_name, product_list[i].productName);
      strcpy(product_price,"");
      sprintf(product_price, "Gia %lf", product_list[i].price);
      
      // Create a Frame
      GtkWidget *frame;
      GtkWidget *vbox;
      GtkWidget *product_img;
      GtkWidget *label_name;
      GtkWidget *label_price;

      //Khoi tao cac Widget
      frame = gtk_frame_new(NULL);
      gtk_widget_set_size_request (frame, 200, 210);

      vbox = gtk_box_new(TRUE, 2);

      GtkWidget *box = gtk_event_box_new();

      //Load image
      GError *error = NULL;
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_file_at_scale(image_list[i], 190, 210, FALSE,&error);
      
      if (!pixbuf)
      {
        g_print ("Error: %s\n", error->message);
        g_error_free (error);
        // Handle error here
      }
      
      product_img = gtk_image_new_from_pixbuf(pixbuf);
      g_object_unref (pixbuf);
      
      label_name = gtk_label_new(product_name); //20 ki tu max
      gtk_label_set_selectable(GTK_LABEL(label_name), TRUE);

      label_price = gtk_label_new(product_price);
      gtk_label_set_selectable(GTK_LABEL(label_price), TRUE);

      //Set cac thuoc tinh chi Widget
      gtk_widget_set_tooltip_text(product_img, product_name);
      gtk_container_set_border_width(GTK_CONTAINER(frame), 2);
      gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_OUT);

      gtk_container_add(GTK_CONTAINER(box), product_img);

      //Add cac thanh phan vao vertical box
      gtk_box_pack_start(GTK_BOX(vbox), box, FALSE, FALSE, 1);
      gtk_box_pack_start(GTK_BOX(vbox), label_name, FALSE, FALSE, 1);
      gtk_box_pack_start(GTK_BOX(vbox), label_price, FALSE, FALSE, 1);

      //Add vertical box vao Frame
      gtk_container_add(GTK_CONTAINER(frame), vbox);
      
      //Add Frame vao Grid
      x = (x+1)%4;
      if(i%4 == 0) y++;
      gtk_grid_attach(GTK_GRID(grid_content), frame, x, y, 1, 1);

      //Bat su kien Click chuot
      g_signal_connect (G_OBJECT (box),
                       "button_press_event",
                       G_CALLBACK (button_press_callback),
                       GINT_TO_POINTER(i));

      //Bat su kien di chuot vao anh?
      g_signal_connect( G_OBJECT( box ), "enter-notify-event",
                 G_CALLBACK( enter_func ), NULL );
      g_signal_connect( G_OBJECT( box ), "leave_notify_event",
                 G_CALLBACK( leave_func ), NULL );

  }
  
	gtk_widget_show_all(window);

	//Hand control over the main loop.
	gtk_main();

	return 0;
}

//Stop the GTK+ main loop function when the window is destroyed.
static void destroy(GtkWidget *window, gpointer data)
{
	gtk_main_quit();
}

//Return fail to destroy the widget. By returning True, you can cancel a delete-event.
// This can be used to confirm quitting the application.
static gboolean delete_event(GtkWidget *window,
							GdkEvent *event,
							gpointer data)
{
	return FALSE;
}

//Ham xu ly su kien di chuot vao img
static gboolean
enter_func( GtkWidget *widget,
         gpointer   data )
{
   GdkWindow *win = gtk_widget_get_window( GTK_WIDGET( widget ) );
   GdkCursor *cur;

   cur = gdk_cursor_new( GDK_HAND1 );
   gdk_window_set_cursor( win, cur );
   g_object_unref( cur );
   return TRUE;

   //gdk_window_set_cursor( win, NULL );
}

//Ham xu ly su kien di chuot ra khoi img
static gboolean
leave_func( GtkWidget *widget,
         gpointer   data )
{
   GdkWindow *win = gtk_widget_get_window( GTK_WIDGET( widget ) );

   gdk_window_set_cursor( win, NULL );
   return TRUE;
}
