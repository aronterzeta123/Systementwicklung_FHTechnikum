#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget *label;
static void
on_response (GtkDialog *dialog, gint response_id, gpointer user_data)
{

        /* If the button clicked gives response OK (response_id being -5) */
        if (response_id == GTK_RESPONSE_OK)
                gtk_label_set_text (GTK_LABEL (label), "OK clicked!");
        /* If the message dialog is destroyed (for example by pressing escape) */
        else if (response_id == GTK_RESPONSE_DELETE_EVENT)
                gtk_label_set_text (GTK_LABEL (label), "Dialog closed!");

        /* Destroy the dialog after one of the above actions have taken place */
        gtk_widget_destroy (GTK_WIDGET (dialog));
}




static void
message_cb (GtkWidget *widget, gpointer user_data)
{
	GtkWidget *message_dialog;
	message_dialog = gtk_message_dialog_new (GTK_WINDOW (window),
						 GTK_DIALOG_MODAL,
						 GTK_MESSAGE_WARNING,
						 GTK_BUTTONS_OK_CANCEL,
						 "This game is programmed by Aron Terzeta. It is a simple sudoku-game.");
	/* display the message dialog */
	gtk_widget_show_all (message_dialog);
	/* connect the response signal */
	g_signal_connect (GTK_DIALOG (message_dialog), "response",
			  G_CALLBACK (on_response), NULL);
}









int main(int argc, char *argv[]) {

  //GtkWidget *window;
  GtkWidget *vbox;

  GtkWidget *menubar;
  GtkWidget *fileMenu;
  GtkWidget *imprMenu;
  GtkWidget *sep;
  GtkWidget *fileMi;
  GtkWidget *imprMi;
  GtkWidget *feedMi;
  GtkWidget *bookMi;
  GtkWidget *mailMi;
  GtkWidget *quitMi;
  GtkWidget *helpMi;
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_window_set_title(GTK_WINDOW(window), "Submenu");

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  menubar = gtk_menu_bar_new();
  
  fileMenu = gtk_menu_new();
  fileMi = gtk_menu_item_new_with_label("Menu");
  
  imprMenu = gtk_menu_new();
  imprMi = gtk_menu_item_new_with_label("Game");
  feedMi = gtk_menu_item_new_with_label("Restart");
  bookMi = gtk_menu_item_new_with_label("Difficulty");
  mailMi = gtk_menu_item_new_with_label("High Score");
  quitMi = gtk_menu_item_new_with_label("Quit");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(imprMi), imprMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(imprMenu), feedMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(imprMenu), bookMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(imprMenu), mailMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(imprMenu), quitMi);
  sep = gtk_separator_menu_item_new();  
  helpMi = gtk_menu_item_new_with_label("Help");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), imprMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), helpMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), sep);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

  g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(quitMi), "activate",
        G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(helpMi), "activate",
        G_CALLBACK(message_cb), NULL);
  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
