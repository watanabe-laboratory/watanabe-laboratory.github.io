#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "griddisp1.h"


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
GtkWidget* create_select_box(ProtWindow *protwin);

void button_callback( GtkWidget *widget, gpointer data );
void close_application( GtkWidget *widget, GdkEvent *event, gpointer data );



/****************************************************************************
                                Functions
 ****************************************************************************/

/***************************** Signal Handers *******************************/

/*
 * Call User Interface of the Signal Handler
 */
void
button_callback( GtkWidget *widget, gpointer data )
{
    ProtWindow *protwin;
    char * input_text;
    protwin = (ProtWindow*) data;

    /* disable buttons */
    gtk_widget_set_sensitive(protwin->quit_button,  FALSE);
    gtk_widget_set_sensitive(protwin->start_button, FALSE);

    /* get input value, button status, etc */
    input_text = "Textentry Disabled(This is Dummy String)!!\n";

    if (protwin->button_callback == NULL) {
        printf("button_clicked (No action is defined)\n");
        printf("  protwin->window_title is \"%s\"\n", protwin->window_title);
    }else{
        /* Call real callback function */
        protwin->button_callback(protwin, input_text);
    }

    /* enable buttons */
    gtk_widget_set_sensitive(protwin->quit_button,  TRUE);
    gtk_widget_set_sensitive(protwin->start_button, TRUE);
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
 * ���� GtkPixmap �����äƤ����, �����˳����Ƥ��Ƥ� pixmap ��񴹤���. */
void
protwin_put_icon(ProtWindow *protwin, int icon_id, int x, int y)
{
    int index = ARY_TO_TBL_IDX(protwin->width, x, y);

    /* */
    if (  x > protwin->width  ||  y > protwin->height )
        fprintf(stderr, "Error: in protwin_put_icon(): exceed window size\n");

    if ( protwin->grid_ary[index]  !=  NULL ) {
        /* pixmap already created on the grid. */
        gtk_grid_set(  (protwin->grid_ary)[index],
                       (protwin->pixmap_icon_ary)[icon_id]  );
    } else {
        /* pixmap not created */
        (protwin->grid_ary)[index] = gtk_grid_new( (protwin->pixmap_icon_ary)[icon_id] );

        gtk_widget_show(  (protwin->grid_ary)[index]  );

        gtk_table_attach_icon( GTK_TABLE(protwin->gridtable),
                               (protwin->grid_ary)[index], x, y );
    }

}


/************************ �ƥ��������åȤκ�����ɽ��  ***********************/
/*
 * �ᥤ�󥦥���ɥ��β�¦�˺��ܥ����ΰ���Ѱդ���
 */
GtkWidget *
create_button_box (ProtWindow *protwin)
{
    GtkWidget *button_box;     //button container(��ʿ�ܥå���)
    GtkWidget *centering_hbox; //����˶��������֤���Τ˻���.
    GtkWidget *quit_button, *start_button;

    /**** �桼������ܥ����Quit�ܥ��� ****/
    if ( protwin->button_label != NULL ){
        start_button = gtk_button_new_with_label(protwin->button_label);
    } else {
        start_button = gtk_button_new_with_label(" Start ");
    }
    gtk_widget_show(start_button);
    quit_button     = gtk_button_new_with_label(" Quit ");
    gtk_widget_show(quit_button);
    gtk_signal_connect (GTK_OBJECT(quit_button), "clicked",
                        GTK_SIGNAL_FUNC(close_application), (gpointer)NULL);
    gtk_signal_connect (GTK_OBJECT(start_button), "clicked",
                        GTK_SIGNAL_FUNC(button_callback), (gpointer)protwin);

    /* hbox���Ѱդ���, ��ä��ܥ���������. */
    button_box     = gtk_hbox_new(TRUE   /*homogeneous?*/,  5 /*spacing*/);
    centering_hbox = gtk_hbox_new(FALSE  /*homogeneous?*/,  5 /*spacing*/);
    /* �ޤ�centering_hbox������, */
    gtk_box_pack_start( GTK_BOX(centering_hbox),
                        start_button,       TRUE /*expand*/, FALSE /*fill*/ , 5);
    gtk_box_pack_start( GTK_BOX(centering_hbox),
                        quit_button,        TRUE /*expand*/, FALSE /*fill*/ , 5);
    /* ����� button_box �������. */
    gtk_box_pack_start( GTK_BOX(button_box),
                        centering_hbox,     TRUE /*expand*/, FALSE /*fill*/ , 5);
    gtk_widget_show(centering_hbox);

    protwin->quit_button  = quit_button;
    protwin->start_button = start_button;
    return button_box;
}


GtkWidget* create_select_box(ProtWindow *protwin)
{
    GtkWidget *select_hbox;
    GtkWidget *check_close, *check_tight; /* check box */

    select_hbox = gtk_hbox_new(TRUE   /*homogeneous?*/,  5 /*spacing*/);

    /* ckeck button ���Ѱ� */
    check_close = gtk_check_button_new_with_label ("Close?");
    check_tight = gtk_check_button_new_with_label ("Tight?");
    gtk_widget_show(check_close);
    gtk_widget_show(check_tight);

    /* hbox �˵ͤ�� */
    gtk_box_pack_start( GTK_BOX(select_hbox),
                        check_close,        TRUE /*expand*/, FALSE /*fill*/ , 5);
    gtk_box_pack_start( GTK_BOX(select_hbox),
                        check_tight,        TRUE /*expand*/, FALSE /*fill*/ , 5);

    protwin->check_close=check_close;
    protwin->check_tight=check_tight;
    return select_hbox;
}



GtkWidget*
create_grid_table(ProtWindow *protwin)
{
    int i;
    int square_size =  (protwin->width) * (protwin->height); /* ���� */
    /*��table ��������֤����ġ��� grid(�ºݤ� Pixmap ���֥�������)�ؤΥݥ��󥿡�
       ���������ݤ���. �Ȥ����ϰ켡�������ޥ��� ARY_TO_TBL���Ѵ����Ƥ� */
    protwin->grid_ary = malloc( (sizeof (GtkWidget*)) * square_size   );

    for (i=0; i < square_size; i++){
        (protwin->grid_ary)[i] = NULL;   /* NULL�ǽ���� */
    }

    /* gridtable widget�κ���, ���Τޤޤ���� return ���� */
    return gtk_table_new(protwin->width, protwin->height, TRUE);
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

    /**** ��ľ�ѥå��󥰥ܥå���(���֤ο�)�Ȥ�����Ȥ��Ѱ� ****/
    vbox = gtk_vbox_new(FALSE /*homogeneous?*/,  10 /*spacing*/);
    /* 1 */
    hbox = gtk_hbox_new(FALSE /*homogeneous?*/,  10 /*spacing*/);
    gtk_widget_show(hbox);
    /* 2 */
    separator = gtk_hseparator_new();
    gtk_widget_show(separator);
    /* 3 */
    /* 4 */
    button_box = create_button_box(protwin);
    gtk_widget_show(button_box);

    /**** XpmIcon���줾��� GdkPixmap* ���֥������Ȥ��Ѵ����Ƥ��� ****/
    /* ��������θĿ�ʬ�������� */
    protwin->pixmap_icon_ary = malloc(sizeof(PixmapMask) * (protwin->nr_icon));

    for (i=0; i < (protwin->nr_icon) ; i++) {
       (protwin->pixmap_icon_ary)[i]
           = create_pixmap_with_mask(protwin->xpm_icon_ary[i], protwin->window);
    }

    /**** grid�Υơ��֥����� ****/
    protwin->gridtable = create_grid_table(protwin);
    gtk_widget_show(protwin->gridtable);
    /* gridtable ���ȤǰϤ� */
    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
    gtk_widget_show(frame);
    gtk_container_add(GTK_CONTAINER(frame), protwin->gridtable);
    /* frame ���� hbox ������� */
    gtk_box_pack_start( GTK_BOX(hbox), frame, TRUE, FALSE, 10 );

    /**** Vbox�� ���� ������ ****/
    gtk_box_pack_start( GTK_BOX(vbox), hbox,  TRUE, FALSE, 5 );
    gtk_box_pack_start( GTK_BOX(vbox), separator, TRUE, FALSE, 0 );
    gtk_box_pack_start( GTK_BOX(vbox), button_box, TRUE, FALSE, 10 );
    gtk_widget_show(vbox);

    gtk_container_add( GTK_CONTAINER(protwin->window), vbox);
    protwin_refresh();
}
