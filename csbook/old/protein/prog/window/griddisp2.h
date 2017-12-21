#ifndef GRIDDISP_H_INCLUDED
#define GRIDDISP_H_INCLUDED

#define LABEL_FONT "-*-fixed-medium-r-normal--7-*-*-*-*-*-*-*"

/****************************************************************************
                                Data types
 ****************************************************************************/
typedef char **XpmIcon;

/**** XPM $B$N%"%$%3%s%G!<%?$N=i4|2=MQ%^%/%m(B ****
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


/**** XPM$B$rF)2a?'(Bbitmap$B$H%;%C%H$G9=B$BN$H$7$F07$&(B ****/
typedef struct _pixmap_with_mask PixmapMask;
struct _pixmap_with_mask {
    GdkBitmap *mask;
    GdkPixmap *pixmap;
};

/**** $BI=<($9$k(BWindow$B$rJ];}$9$k(B ****/
typedef struct _ProtWindow ProtWindow;
struct _ProtWindow {
    /* $B%f!<%6$,FH<+$K(B($B%&%#%s%I%&Kh$K(B)$BJ];}$5$;$F$*$-$?$$%G!<%?(B($B$X$N%]%$%s%?(B)
     * $B%&%#%s%I%&$,0l$D$@$1$J$i(B, global $B$G$b$J$s$G$bNI$$$s$8$c$s(B?  */
    void      *user_data;
    int        width, height;    //$B:n@.$9$k(B grid $B$N=D$H2#(B
    char      *window_title;
    char      *button1_label;
    char      *button2_label;
    char      *button3_label;
    char      *button4_label;
    void     (*button1_callback)(ProtWindow *protwin);
    void     (*button2_callback)(ProtWindow *protwin);
    void     (*button3_callback)(ProtWindow *protwin);
    void     (*button4_callback)(ProtWindow *protwin);

    /***** $B0J2<$OD>@\O.$i$J$$$3$H(B *****/
    XpmIcon      *xpm_icon_ary;
    int           nr_icon;
    PixmapMask   *pixmap_icon_ary;
    GtkWidget    *window;
    GtkWidget   **grid_ary;      //table(ary of ary) of GtkPixmap object. can be NULL
    GtkWidget    *gridtable;     //GtkTable object itself
    GtkWidget    *quit_button, *button1, *button2, *button3, *button4;
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
     protwin  . user_data        = NULL;          \
     protwin  . width            = 10;            \
     protwin  . height           = 10;            \
     protwin  . window_title     = "Window Title";\
     protwin  . window_title     = NULL;          \
     protwin  . button1_label    = NULL;          \
     protwin  . button1_callback = NULL;          \
     protwin  . button2_label    = NULL;          \
     protwin  . button2_callback = NULL;          \
     protwin  . button3_label    = NULL;          \
     protwin  . button3_callback = NULL;          \
     protwin  . button4_label    = NULL;          \
     protwin  . button4_callback = NULL;          \
     protwin  . nr_icon          = nr_icon;       \
     protwin  . xpm_icon_ary     = xpm_icon_ary;  \
}


#endif //EOF
