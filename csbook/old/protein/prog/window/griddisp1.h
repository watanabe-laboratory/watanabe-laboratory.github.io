#ifndef GRIDDISP_H_INCLUDED
#define GRIDDISP_H_INCLUDED






/****************************************************************************
                                Data types
 ****************************************************************************/
typedef char **XpmIcon;

/* ***** XPM $B$N%"%$%3%s%G!<%?$N=i4|2=MQ%^%/%m(B *****
 * #define ICONS {icon1_var, icon2_var, ...}
        *.xpm $B%U%!%$%k$G;XDj$5$l$F$$$k(B, $B$=$l$>$l$NJQ?tL>$NG[Ns(B.
 * #define ICONS_ID {ICON1_NAME, ICON2_NAME, ...}
        $B$=$l$>$l$NJQ?t$KBP1~$9$k%"%$%3%s$N%(%$%j%"%9L>(B(enum$B$5$l$k(B)$B$NDj5A(B.
 * ICONS_init(ICONS, ICONS_ID)
      ICONS, ICONS_ID $B$r(B #define $B$7$F$+$i(B ICONS_init() $B$9$k$H(B
      icon_array[ICONS_ID] $B$G3F%"%$%3%s$N(B XPM (static char *xpm_data[])
      $B$,F@$i$l$k$h$&$K$J$k(B.  */
#define ICONS_init(ICONS, ICON_ID);                               \
enum enum_IconID ICON_ID;                                         \
XpmIcon xpm_icon_ary[] =  ICONS ;                                 \
int nr_icon = (sizeof(xpm_icon_ary)/sizeof(XpmIcon));


typedef struct _pixmap_with_mask PixmapMask;
struct _pixmap_with_mask {
    GdkBitmap *mask;
    GdkPixmap *pixmap;
};


typedef struct _ProtWindow ProtWindow;
struct _ProtWindow {
    /* $B%f!<%6$,FH<+$K(B($B%&%#%s%I%&Kh$K(B)$BJ];}$5$;$F$*$-$?$$%G!<%?(B($B$X$N%]%$%s%?(B)
     * $B%&%#%s%I%&$,0l$D$@$1$J$i(B, global $B$G$b$J$s$G$bNI$$$s$8$c$s(B?  */
    void      *user_data;
    int        width, height;   //$B:n@.$9$k(B grid $B$N=D$H2#(B
    char      *window_title;
    char      *button_label;    //default is " Start ".
    char      *default_text;
    void     (*button_callback)(ProtWindow *protwin, char *input_text);

    /***** $B0J2<$OD>@\O.$i$J$$$3$H(B *****/
    XpmIcon      *xpm_icon_ary;
    int           nr_icon;
    PixmapMask   *pixmap_icon_ary;
    GtkWidget    *window;
    GtkWidget   **grid_ary;      //table(ary of ary) of GtkPixmap object. can be NULL
    GtkWidget    *gridtable;     //GtkTable object itself
    GtkWidget    *textentry;     // User inputs "HPHHPPP..."
    GtkWidget    *quit_button, *start_button;
    GtkWidget    *spin_h_weight, *spin_st_length, *spin_goal_x, *spin_goal_y; /* spin button */
    GtkWidget    *check_close, *check_tight; /* check box */
};


/****************************************************************************
                      Prototypes and Macro Function
 ****************************************************************************/
void protwin_init(int argc, char **argv_p, ProtWindow *protwin, int width, int height);
void protwin_put_icon(ProtWindow *protwin, int icon_id, int x, int y);

#define protwin_refresh()\
   { while (gtk_events_pending()) gtk_main_iteration(); }
#define protwin_main_loop()   gtk_main()

#define ARY_TO_TBL_IDX(width, x, y)  (width*y + x)


/* $B9=B$BN$N=i4|2=$r9T$&(B */
#define PROTWIN_INIT(protwin)                     \
{                                                 \
     protwin  . user_data       = NULL;           \
     protwin  . width           = 10;             \
     protwin  . height          = 10;             \
     protwin  . window_title    = NULL;           \
     protwin  . button_label    = NULL;           \
     protwin  . default_text    = NULL;           \
     protwin  . button_callback = NULL;           \
     protwin  . nr_icon         = nr_icon;        \
     protwin  . xpm_icon_ary    = xpm_icon_ary;   \
}


#endif //EOF
