#include <gtk/gtk.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include "libs/mysql_sockets/libmysql_socket.h"
#include "libs/curl/libdownload_img.h"

//COMPILE
//gcc -g main.c -o main `pkg-config gtk+-3.0 --cflags --libs` -L libs/mysql_sockets -lmysql_socket -L libs/curl -ldownload_img -ljansson -lpthread `pkg-config libcurl --cflags --libs`

//-------------------------------------------Cac du lieu dung chung-------------------------------------------------------
int sockfd;

char current_page[5]; //trang hien tai
char total_page[5]; //tong so trang

int grid_size; //tong so san pham da duoc render trong grid hien tai

//-------------------------------------------Du lieu danh sach------------------------------------------------------------
struct product product_list[30]; //luu mang product tra ve tu server
int product_list_size; //do lon mang product_list
char* image_list[30]; //mang luu duong dan cua imgs

//------------------------------Cac ham xu ly su kien------------------------------------------------------
//Xu li su kien khi nguoi dung chon cac category
void  on_changed(GtkWidget *widget, gpointer statusbar);
void on_row_activated( GtkTreeView *treeview,
                       GtkTreePath *path,
                       GtkTreeViewColumn *column,
                       gpointer *data );

//Xu li su kien tat window
static void destroy(GtkWidget*, gpointer);
static gboolean delete_event(GtkWidget*, GdkEvent*, gpointer);

//Xu li su kien thay doi hinh con tro chuot
static gboolean enter_func( GtkWidget *widget, gpointer   data );
static gboolean leave_func( GtkWidget *widget, gpointer   data );

//Ham xu li su kien click vao anh san pham de xem thong tin chi tiet
static gboolean button_press_callback (GtkWidget *event_box, GdkEventButton *event, gpointer  data);

//Ham xu li click nut "login"
void on_login_button_click( GtkWidget *widget, gpointer   data )
{

}

//Ham xu li nut "gio hang"
void on_viewcard_button_click( GtkWidget *widget, gpointer   data )
{
  
}

//Ham xu li nut "them sp vao gio hang"
void on_addproduct_button_click( GtkWidget *widget, gpointer   data )
{
  
}

//Ham xu li nut prev-phan trang
void on_prev_paging_click( GtkWidget *widget, gpointer data );

//Ham xu li nut next-phan trang
void on_next_paging_click( GtkWidget *widget, gpointer data );


//-----------------------------Tao categories box ben trai man hinh-----------------------------------------
//Enum du lieu thanh Category
enum
{
  COLUMN = 0,
  NUM_COLS
} ;

//tao model
static GtkTreeModel *create_and_fill_model (void);

//tao tree view
static GtkWidget *create_view_and_model (void);

//them treeview vao box
static void fill_category_box(GtkWidget *vbox);


//---------------------------Cac ham render du lieu ra man hinh--------------------------------------------
//Download image va tra lai mang la cac img paths-duong dan den file anh
void get_image();

//Render san pham vao grid view
void render_grid_view(GtkWidget *grid);

//Xu li trang xem thong tin chi tiet san pham
void manipulate_detail_window(gpointer data);

int main(int argc, char *argv[])
{
  GtkBuilder *builder;
	GtkWidget *window, *box;
  GtkWidget *box_banner;
  GtkWidget *grid_content;
  GtkWidget *button_view_card;
  GtkWidget *button_login;
  GtkWidget *box_paging;
  GtkWidget *button_prev;
  GtkWidget *button_next;
  GtkWidget *label_paging;

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
  box_banner = GTK_WIDGET(gtk_builder_get_object(builder,"box_banner"));
	box = GTK_WIDGET(gtk_builder_get_object(builder,"box8"));
  grid_content = GTK_WIDGET(gtk_builder_get_object(builder, "grid1"));
  button_login = GTK_WIDGET(gtk_builder_get_object(builder, "button_login"));
  button_view_card = GTK_WIDGET(gtk_builder_get_object(builder, "button_view_card"));
  box_paging = GTK_WIDGET(gtk_builder_get_object(builder, "box_paging"));
  button_prev = GTK_WIDGET(gtk_builder_get_object(builder, "button_prev"));
  button_next = GTK_WIDGET(gtk_builder_get_object(builder, "button_next"));
  label_paging = GTK_WIDGET(gtk_builder_get_object(builder, "label_current_p"));
  

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref (G_OBJECT(builder));

  //-------------Chinh cac thuoc tinh cho cac thanh phan-------------------------------------------------------------------
	gtk_window_set_title(GTK_WINDOW(window), "Trang chủ");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	//gtk_container_set_border_width(GTK_CONTAINER (window), -1); //Tricks
	gtk_widget_set_size_request (window, 1000, 700);
  gtk_widget_set_size_request (box_banner, 1000, 100);
  gtk_widget_set_size_request (box, 140, 550);
  gtk_widget_set_size_request (grid_content, 850, 500);

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

  g_signal_connect (G_OBJECT (button_prev), "button_press_event",
                        G_CALLBACK (on_prev_paging_click), (gpointer) grid_content);
  g_signal_connect( G_OBJECT( button_prev ), "enter-notify-event",
                 G_CALLBACK( enter_func ), NULL );
  g_signal_connect( G_OBJECT( button_prev ), "leave-notify-event",
                 G_CALLBACK( leave_func ), NULL );

  g_signal_connect (G_OBJECT (button_next), "button_press_event",
                        G_CALLBACK (on_next_paging_click), (gpointer) grid_content);
  g_signal_connect( G_OBJECT( button_next ), "enter-notify-event",
                 G_CALLBACK( enter_func ), NULL );
  g_signal_connect( G_OBJECT( button_next ), "leave-notify-event",
                 G_CALLBACK( leave_func ), NULL );

  //---------------Tao banner-------------------------------------------------------------------------------
  //Load image
  GdkPixbuf *pixbuf_banner;
  GtkWidget *image_banner;
  pixbuf_banner = gdk_pixbuf_new_from_file_at_scale("banner.jpg", 1000, 105, FALSE,&error);
  
  if (!pixbuf_banner){
    g_print ("Error: %s\n", error->message);
    g_error_free (error);
    // Handle error here
  }

  image_banner = gtk_image_new_from_pixbuf(pixbuf_banner);
  g_object_unref (pixbuf_banner);
  gtk_container_add(GTK_CONTAINER(box_banner), image_banner);

  //---------------Tao ket noi socket den server-------------------------------------------------------------------------
  sockfd = get_sockfd();
  //---------------Tao cot Category ben trai man hinh---------------------------------------------------------------------
	fill_category_box(box);
  
  //---------------Render danh sach san pham------------------------------------------------------------------------------
  
  //Lay danh sach san pham tu server
  strcpy(current_page, "0");
  get_all_product(sockfd, current_page, product_list, &product_list_size, total_page);

  //Download images tu web
  get_image();

  //Render grid view
  grid_size = product_list_size;
  render_grid_view(grid_content);
  
  
  //-----------------------------Hien thi phan trang--------------------------------------------------------
  //GtkWidget *label_paging;
  
  char labelpaging[15];
  strcpy(labelpaging, "");
  if (atoi(total_page) > 0){
    int cur_page_int = atoi(current_page) + 1;
    sprintf(labelpaging, "Trang %d của %s", cur_page_int, total_page);
  }
  else{
    sprintf(labelpaging, "Trang %s của %s", current_page, total_page);
  }
  gtk_label_set_text(GTK_LABEL(label_paging), labelpaging);

  //-----------------------------Hien thi tat ca cac widget cua window--------------------------------------
	gtk_widget_show_all(window);

	//Hand control over the main loop.
	gtk_main();

	return 0;
}

//Stop the GTK+ main loop function when the window is destroyed.
static void destroy(GtkWidget *window, gpointer data)
{
  close(sockfd);
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

//Download image va tra lai mang la cac img paths-duong dan den file anh
void get_image(){
  int i = 0;
  char* url;
  //Tao duong dan luu cac anh tu dong
  for (i=0;i<product_list_size;i++){

    if (image_list[i] == NULL){
      image_list[i] = (char*)malloc(30* sizeof(char));
    }

    url = (char*)malloc(255* sizeof(char));
    strcpy(image_list[i], "");
    sprintf(image_list[i], "temp/img_%d.jpg",i);
    strcpy(url,product_list[i].image);
    download_image(url,image_list[i]);
    free(url);
  }
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

  gtk_window_set_title(GTK_WINDOW(window), "Thông tin chi tiết");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  gtk_widget_set_size_request (window, 750, 400);

  gtk_widget_set_size_request (box_img, 350, 350);
  gtk_widget_set_size_request (frame_price, 350, 50);

  gtk_widget_set_size_request (frame_product_name, 400, 50);
  gtk_widget_set_size_request (frame_description, 400, 300);
  gtk_widget_set_size_request (button_add_to_card, 400, 100);

  gtk_frame_set_label(GTK_FRAME(frame_price), "Giá");
  gtk_frame_set_label(GTK_FRAME(frame_product_name), "Tên sản phẩm");
  gtk_frame_set_label(GTK_FRAME(frame_description), "Mô tả");

  //----------------------------Lay thong tin san pham ma nguoi dung click-----------------------------------
  int index = (int)data;
  char product_price[30];
  strcpy(product_price, "");
  sprintf(product_price, "Giá %lf vnđ", product_list[index].price);
  

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
  
  //gtk_widget_set_tooltip_text(button_add_to_card, "Add product to your card");
  g_signal_connect( G_OBJECT( button_add_to_card ), "enter-notify-event",
                 G_CALLBACK( enter_func ), NULL );
  g_signal_connect( G_OBJECT( button_add_to_card ), "leave-notify-event",
                 G_CALLBACK( leave_func ), NULL );

  gtk_widget_show_all(window);
}


//-----------------------------Tao categories box ben trai man hinh-----------------------------------------
//tao model
static GtkTreeModel *create_and_fill_model (void)
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
                     COLUMN, "Hàng mới",
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
                     COLUMN, "Nữ",
                     -1);

  //lay tat ca cac loai quan ao cua "Nu" nhan tu server
  get_product_type_by_gender(sockfd, "2", product_type_list, &product_type_list_size);
  for (i=0;i<product_type_list_size;i++){
    gtk_tree_store_append(treestore, &child, &toplevel);
    gtk_tree_store_set(treestore, &child, COLUMN, product_type_list[i].typeName);
  }

  return GTK_TREE_MODEL(treestore);
}


//tao tree view
static GtkWidget *create_view_and_model (void)
{
  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkWidget *view;
  GtkTreeModel *model;

  view = gtk_tree_view_new();
  col = gtk_tree_view_column_new();

  gtk_tree_view_column_set_title(col, "Danh mục");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
  gtk_widget_set_tooltip_text(view, "Danh mục sản phẩm");
  //gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view), FALSE);

  g_signal_connect( G_OBJECT( view ), "enter-notify-event",
                 G_CALLBACK( enter_func ), NULL );
  g_signal_connect( G_OBJECT( view ), "leave-notify-event",
                 G_CALLBACK( enter_func ), NULL );
  g_signal_connect(G_OBJECT( view ), "row-activated",
                 G_CALLBACK( on_row_activated ), NULL);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, 
      "text", COLUMN);

  model = create_and_fill_model();
  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
  g_object_unref(model); 

  return view;
}

//them treeview vao box
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

//Xu li su kien khi nguoi dung chon cac category
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

void on_row_activated( GtkTreeView *treeview,
                       GtkTreePath *path,
                       GtkTreeViewColumn *column,
                       gpointer *data )
{
  GtkTreeModel *model;
  GtkTreeIter iter;
  char *category;
  model = gtk_tree_view_get_model(treeview);
  if (gtk_tree_model_get_iter(model, &iter, path)){
    gtk_tree_model_get(model, &iter, COLUMN, &category, -1);
    g_print("Selected category: %s\n", category);
    g_free(category);
  }

}

//Render grid view
void render_grid_view(GtkWidget *grid){
  int i=0, j=0, x=-1 ,y=0, rows=2, columns = 2;
  /*
  for (i=0;i<grid_size;i++){
    //Tinh toan vi tri
    x = (x+1)%4;
    if(i%4 == 0) y++;
    GtkWidget *child = gtk_grid_get_child_at(GTK_GRID(grid), x, y);
    //if (child != NULL) gtk_widget_destroy(child);
    if (child != NULL) g_object_unref(child);
  } 
  */
  GtkContainer *container = GTK_CONTAINER (grid);
  GList *children = gtk_container_get_children (container);
  GList *list;

  for (list = children; list && list->data; list = list->next)
  {
      GtkWidget *child = list->data;
      //if (GLADE_IS_PLACEHOLDER (child))
      if (child != NULL)
        gtk_container_remove (container, child);
  }
  g_list_free (children);

  for (i=0;i<product_list_size;i++){
      //Ly thong tin cua tung product
      char* product_name = (char*)malloc(60*sizeof(char));
      char* product_price = (char*)malloc(30*sizeof(char));
      strcpy(product_name, product_list[i].productName);
      strcpy(product_price,"");
      sprintf(product_price, "Giá %lf", product_list[i].price);
      
      // Create a Frame
      GtkWidget *frame;
      GtkWidget *vbox;
      GtkWidget *product_img;
      GtkWidget *label_name;
      GtkWidget *label_price;

      GError *error = 0;
      //Khoi tao cac Widget
      frame = gtk_frame_new(NULL);
      gtk_widget_set_size_request (frame, 200, 210);

      vbox = gtk_box_new(TRUE, 2);

      GtkWidget *box = gtk_event_box_new();

      //Load image
      //GError *error = NULL;
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
      
      //-----------Add Frame vao Grid----------------------
      //Tinh toan vi tri
      x = (x+1)%4;
      if(i%4 == 0) y++;

      //gtk_grid_attach(GTK_GRID(grid), frame, x, y, 1, 1);

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
      free(product_name);
      free(product_price);

  }
}

//Ham xu li nut prev-phan trang
void on_prev_paging_click( GtkWidget *widget, gpointer   data )
{
  if (atoi(current_page) <= (atoi(total_page)-1) 
                      && atoi(current_page) > 0){
    GtkWidget *grid = data;
    int prev_page = atoi(current_page) - 1;
    strcpy(current_page, "");
    sprintf(current_page, "%d", prev_page);

    grid_size = product_list_size;
    get_all_product(sockfd, current_page, product_list, &product_list_size, total_page);

    //Download images tu web
    get_image();

    //Render grid view
    render_grid_view(grid);
  }
}

//Ham xu li nut next-phan trang
void on_next_paging_click( GtkWidget *widget, gpointer   data )
{
  //if (atoi(current_page) < (atoi(total_page)-1) 
  //                  && atoi(current_page) >= 0){
    GtkWidget *grid = data;
    //int next_page = atoi(current_page) + 1;
    int next_page = 0;
    strcpy(current_page, "");
    sprintf(current_page, "%d", next_page);

    grid_size = product_list_size;
    get_all_product(sockfd, current_page, product_list, &product_list_size, total_page);
    g_print("current_page = %s, total_page = %s, product_list_size=%d\n", current_page, total_page,product_list_size);
    //Download images tu web
    get_image();

    //Render grid view
    render_grid_view(grid);
  //}
}