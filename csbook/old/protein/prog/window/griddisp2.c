#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "griddisp2.h"


/****************************************************************************
                     Prototypes and Macro functions
 ****************************************************************************/
#define gtk_table_attach_icon(table, icon, x, y) \
   gtk_table_attach_defaults(GTK_TABLE(table), icon, x, x+1, y, y+1)

#define gtk_grid_new( icon ) \
   gtk_pixmap_new( icon .pixmap,  icon .mask )

#define gtk_grid_set( grid, pixmap_with_mask ) \
   gtk_pixmap_set( GTK_PIXMAP(grid),  pixmap_with_mask.pixmap, pixmap_with_mask.mask )

PixmapMask create_pixmap_with_mask(XpmIcon iconxpm, GtkWidget *window);
GtkWidget* create_button_box(ProtWindow *protwin);
GtkWidget* create_grid_table(ProtWindow *protwin);

void button_callback_wrapper( GtkWidget *widget, gpointer data );
void close_application( GtkWidget *widget, GdkEvent *event, gpointer data );



/****************************************************************************
                                Functions
 ****************************************************************************/

/***************************** Signal Handers *******************************/

/*
 * Call User Interface of the Signal Handler
 */
void
button_callback_wrapper( GtkWidget *widget, gpointer data )
{
    ProtWindow *protwin;
    void     (*cur_button_callback)(ProtWindow *protwin);
    int button_nr;
    protwin = (ProtWindow*) data;


    //disable buttons
    gtk_widget_set_sensitive(protwin->quit_button,  FALSE);
    gtk_widget_set_sensitive(protwin->button1, FALSE);
    gtk_widget_set_sensitive(protwin->button2, FALSE);
    gtk_widget_set_sensitive(protwin->button3, FALSE);
    gtk_widget_set_sensitive(protwin->button4, FALSE);

    //which button cliecked?
    if       (widget == protwin->button1){
        cur_button_callback = protwin->button1_callback;
        button_nr = 1;
    }else if (widget == protwin->button2){
        cur_button_callback = protwin->button2_callback;
        button_nr = 2;
    }else if (widget == protwin->button3){
        cur_button_callback = protwin->button3_callback;
        button_nr = 3;
    }else {
        cur_button_callback = protwin->button4_callback;
        button_nr = 4;
        if (widget != protwin->button4){
            fprintf(stderr, "Error: button_callback_wrapper() called"
                    "with undefined button\n");
            return;
        }
    }

    //Call real callback function
    if (cur_button_callback == NULL) {
        fprintf(stderr, "button%d clicked (No action is defined)\n", button_nr);
    }else{
        cur_button_callback(protwin);
    }

    //enable buttons
    gtk_widget_set_sensitive(protwin->quit_button,  TRUE);
    gtk_widget_set_sensitive(protwin->button1, TRUE);
    gtk_widget_set_sensitive(protwin->button2, TRUE);
    gtk_widget_set_sensitive(protwin->button3, TRUE);
    gtk_widget_set_sensitive(protwin->button4, TRUE);
}


/* When close button of window or "Quit" button clicked */
void
close_application( GtkWidget *widget, GdkEvent *event, gpointer data )
{
    gtk_main_quit();
}


/************* Pixmap ���������(���ݤʤΤǥޥ��������)  ******************/
PixmapMask
create_pixmap_with_mask(XpmIcon iconxpm, GtkWidget *window)
{
    PixmapMask pixmap_with_mask;
    GtkStyle *style;
    style = gtk_widget_get_style(window);
    pixmap_with_mask.pixmap
        = gdk_pixmap_create_from_xpm_d(window->window,
                                       &pixmap_with_mask.mask,
                                       &style->bg[GTK_STATE_NORMAL],
                                       (gchar **)iconxpm);
    return pixmap_with_mask;
}

/**************** Interface : ���ꤷ����ɸ�˥���������֤�  *******************/
/* �⤷ GtkTable �� GtkPixmap���֥������� �����äƤʤ����
 * CtkPixmap ���ä���� GtkTable �γ�����ɸ������.
 * ���� GtkPixmap �����äƤ����, �����˳����Ƥ��Ƥ� pixmap ��񴹤���.
 *
 * x��y������ʬ��̵�뤷��(+1����)��ɸ������Ƥ�.
 */
void
protwin_put_icon(ProtWindow *protwin, int icon_id, int x, int y)
{
    int index;
    /* Makesure, x, y is in range of gridtable */
    if (  x >= protwin->width  ||  y >= protwin->height ){
      fprintf(stderr, "Error: in protwin_put_icon(): exceed window size\n");
    }
    index = ARY_TO_TBL_IDX(protwin->width, x+1, y+1);

    if ( protwin->grid_ary[index]  !=  NULL ) {
        //pixmap already created on the grid.
        gtk_grid_set(  (protwin->grid_ary)[index],
                       (protwin->pixmap_icon_ary)[icon_id]  );
    } else {
        /* pixmap not created */
        (protwin->grid_ary)[index] = gtk_grid_new( (protwin->pixmap_icon_ary)[icon_id] );

        gtk_widget_show(  (protwin->grid_ary)[index]  );

        gtk_table_attach_icon( GTK_TABLE(protwin->gridtable),
                               (protwin->grid_ary)[index], x+1, y+1 );
    }

}


/************************ �ƥ��������åȤκ�����ɽ��  ***********************/
/*
 * �ᥤ�󥦥���ɥ��β�¦�˺��ܥ����ΰ���Ѱդ���
 */
GtkWidget *
create_button_box(ProtWindow *protwin)
{
    GtkWidget *button_box;     //button container(��ʿ�ܥå���)
    GtkWidget *centering_hbox; //����˶��������֤���Τ˻���.
    GtkWidget *quit_button, *button1, *button2, *button3, *button4;

    /**** �桼������ܥ��� ****/
    if ( protwin->button1_label != NULL ){
        button1 = gtk_button_new_with_label(protwin->button1_label);
    } else {
        button1 = gtk_button_new_with_label(" Button 1 ");
    }
    if ( protwin->button2_label != NULL ){
        button2 = gtk_button_new_with_label(protwin->button2_label);
    } else {
        button2 = gtk_button_new_with_label(" Button 2 ");
    }
    if ( protwin->button3_label != NULL ){
        button3 = gtk_button_new_with_label(protwin->button3_label);
    } else {
        button3 = gtk_button_new_with_label(" Button 3 ");
    }
    if ( protwin->button4_label != NULL ){
        button4 = gtk_button_new_with_label(protwin->button4_label);
    } else {
        button4 = gtk_button_new_with_label(" Button 4 ");
    }
    gtk_signal_connect(GTK_OBJECT(button1), "clicked",
                        GTK_SIGNAL_FUNC(button_callback_wrapper), (gpointer)protwin);
    gtk_signal_connect(GTK_OBJECT(button2), "clicked",
                        GTK_SIGNAL_FUNC(button_callback_wrapper), (gpointer)protwin);
    gtk_signal_connect(GTK_OBJECT(button3), "clicked",
                        GTK_SIGNAL_FUNC(button_callback_wrapper), (gpointer)protwin);
    gtk_signal_connect(GTK_OBJECT(button4), "clicked",
                        GTK_SIGNAL_FUNC(button_callback_wrapper), (gpointer)protwin);
    gtk_widget_show(button1);
    gtk_widget_show(button2);
    gtk_widget_show(button3);
    gtk_widget_show(button4);

    /**** Quit�ܥ��� ****/
    quit_button     = gtk_button_new_with_label(" Quit ");
    gtk_widget_show(quit_button);
    gtk_signal_connect (GTK_OBJECT(quit_button), "clicked",
                        GTK_SIGNAL_FUNC(close_application), (gpointer)NULL);

    /**** hbox���Ѱդ���, ��ä��ܥ���������. ****/
    button_box     = gtk_hbox_new(TRUE   /*homogeneous?*/,  5 /*spacing*/);
    centering_hbox = gtk_hbox_new(FALSE  /*homogeneous?*/,  5 /*spacing*/);
    //�ޤ�centering_hbox������...
    gtk_box_pack_start( GTK_BOX(centering_hbox),
                        button1,       TRUE /*expand*/, FALSE /*fill*/ , 5);
    gtk_box_pack_start( GTK_BOX(centering_hbox),
                        button2,       TRUE /*expand*/, FALSE /*fill*/ , 5);
    gtk_box_pack_start( GTK_BOX(centering_hbox),
                        button3,       TRUE /*expand*/, FALSE /*fill*/ , 5);
    gtk_box_pack_start( GTK_BOX(centering_hbox),
                        button4,       TRUE /*expand*/, FALSE /*fill*/ , 5);
    gtk_box_pack_start( GTK_BOX(centering_hbox),
                        quit_button,        TRUE /*expand*/, FALSE /*fill*/ , 30);
    //����� button_box �������.
    gtk_box_pack_start( GTK_BOX(button_box),
                        centering_hbox,     TRUE /*expand*/, FALSE /*fill*/ , 5);
    gtk_widget_show(centering_hbox);

    protwin->quit_button  = quit_button;
    protwin->button1 = button1;
    protwin->button2 = button2;
    protwin->button3 = button3;
    protwin->button4 = button4;
    return button_box;
}



GtkWidget*
create_grid_table(ProtWindow *protwin)
{
    GtkWidget *gridtable, *labeltmp;
    GtkStyle *new_style;
    int i, x, y, square_size;
    char string[6];  //from 000001 upto 999999

    if (protwin->width > 999999 || protwin->height > 999999) {
        fprintf(stderr, "Error: in create_grid_table(): width or height too large\n");
        abort();
    }

    /**** ������ɽ������1�ޥ��ΰ٤�, width+1, height+1 ���� ****/
    square_size =  (protwin->width +1) * (protwin->height +1); //����
    //��table ��������֤����ġ��� grid(�ºݤ� Pixmap ���֥�������)�ؤΥݥ��󥿡�
    //���������ݤ���. �Ȥ����ϰ켡�������ޥ��� ARY_TO_TBL���Ѵ����Ƥ� */
    protwin->grid_ary = malloc( (sizeof (GtkWidget*)) * square_size  );

    for (i=0; i < square_size; i++) {
        (protwin->grid_ary)[i] = NULL;   /* NULL�ǽ���� */
    }

    //gridtable widget�κ���
    gridtable = gtk_table_new(protwin->width+1 , protwin->height+1, TRUE);

    /**** �������� ****/
    labeltmp = gtk_label_new("");
    gtk_widget_ensure_style(labeltmp);
    new_style = gtk_style_copy(labeltmp->style);
    gdk_font_unref(new_style->font);
    new_style->font = gdk_font_load(LABEL_FONT);
    for (x=1; x <= protwin->height ; x++) {
        if(x%2 == 1)
	  sprintf(string, "%d", x/2+1);
	else
	  sprintf(string, " ");
        labeltmp = gtk_label_new(string);
        gtk_widget_set_style(labeltmp, new_style);
        gtk_widget_show(labeltmp);
        gtk_table_attach_defaults(GTK_TABLE(gridtable), labeltmp, x, x+1, 0, 1);
    }
    for (y=1; y <= protwin->width; y++) {
        if(y%2 == 1)
	  sprintf(string, "%d", y/2+1);
	else
	  sprintf(string, " ");
        labeltmp = gtk_label_new(string);
        gtk_widget_set_style(labeltmp, new_style);
        gtk_widget_show(labeltmp);
        gtk_table_attach_defaults(GTK_TABLE(gridtable), labeltmp, 0, 1, y, y+1);
    }

    return gridtable;
}



void
protwin_init(int argc, char **argv, ProtWindow *protwin, int width, int height)
{
    int i;
    GtkWidget *vbox, *hbox, *button_box, *separator, *frame;

    gtk_init(&argc, &argv);

    protwin->width = width;
    protwin->height = height;


    /**** �ᥤ�󥦥���ɥ������ ****/
    protwin->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(protwin->window), protwin->window_title);
    gtk_container_border_width (GTK_CONTAINER (protwin->window), 5);
    gtk_signal_connect( GTK_OBJECT (protwin->window), "delete_event",
                        GTK_SIGNAL_FUNC(close_application), NULL );
    gtk_widget_show(protwin->window);

    /**** XpmIcon���줾��� GdkPixmap* ���֥������Ȥ��Ѵ����Ƥ��� ****/
    /**** (�طʿ�������٤� gtk_window_new()�θ�Ǥʤ��Ȥ����ʤ�) ****/
    //��������θĿ�ʬ��������
    protwin->pixmap_icon_ary = malloc(sizeof(PixmapMask) * (protwin->nr_icon));
    //����˥������������
    for (i=0; i < (protwin->nr_icon) ; i++) {
       (protwin->pixmap_icon_ary)[i]
           = create_pixmap_with_mask(protwin->xpm_icon_ary[i], protwin->window);
    }

    /**** ��ľ�ѥå��󥰥ܥå���(���֤ο�)�Ȥ�����Ȥ��Ѱ� ****/
    vbox = gtk_vbox_new(FALSE /*homogeneous?*/,  10 /*spacing*/);
    /* 1 */
    hbox = gtk_hbox_new(FALSE /*homogeneous?*/,  10 /*spacing*/);
    gtk_widget_show(hbox);
    /* 2 */
    separator = gtk_hseparator_new();
    gtk_widget_show(separator);
    /* 3 */
    button_box = create_button_box(protwin);
    gtk_widget_show(button_box);

    /**** grid�Υơ��֥����� ****/
    protwin->gridtable = create_grid_table(protwin);
    gtk_widget_show(protwin->gridtable);

    //gridtable ���ȤǰϤ�
    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
    gtk_widget_show(frame);
    gtk_container_add(GTK_CONTAINER(frame), protwin->gridtable);
    //frame ���� hbox �������
    gtk_box_pack_start( GTK_BOX(hbox), frame, TRUE, FALSE, 10 );

    /**** Vbox�� ���� ������ ****/
    gtk_box_pack_start( GTK_BOX(vbox), hbox,  TRUE, FALSE, 5 );
    gtk_box_pack_start( GTK_BOX(vbox), separator, TRUE, FALSE, 0 );
    gtk_box_pack_start( GTK_BOX(vbox), button_box, TRUE, FALSE, 10 );
    gtk_widget_show(vbox);

    gtk_container_add( GTK_CONTAINER(protwin->window), vbox);
    protwin_refresh();
}
