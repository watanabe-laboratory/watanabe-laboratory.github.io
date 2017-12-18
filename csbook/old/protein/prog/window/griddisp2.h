#ifndef GRIDDISP_H_INCLUDED
#define GRIDDISP_H_INCLUDED

#define LABEL_FONT "-*-fixed-medium-r-normal--7-*-*-*-*-*-*-*"

/****************************************************************************
                                Data types
 ****************************************************************************/
typedef char **XpmIcon;

/**** XPM のアイコンデータの初期化用マクロ ****
 * #define ICONS {icon1_var, icon2_var, ...}
        *.xpm ファイルで指定されている, それぞれの変数名の配列.
 * #define ICONS_ID {ICON1_NAME, ICON2_NAME, ...}
        それぞれの変数に対応するアイコンのエイリアス名(enumされる)の定義.
 * ICONS_init(ICONS, ICONS_ID)
      ICONS, ICONS_ID を #define してから ICONS_init() すると
      icon_array[ICONS_ID] で各アイコンの XPM (static char *xpm_data[])
      が得られるようになる.  */
#define ICONS_init(ICONS, ICON_ID);                               \
enum enum_IconID ICON_ID;                                         \
XpmIcon xpm_icon_ary[] =  ICONS ;                                 \
int nr_icon = (sizeof(xpm_icon_ary)/sizeof(XpmIcon));


/**** XPMを透過色bitmapとセットで構造体として扱う ****/
typedef struct _pixmap_with_mask PixmapMask;
struct _pixmap_with_mask {
    GdkBitmap *mask;
    GdkPixmap *pixmap;
};

/**** 表示するWindowを保持する ****/
typedef struct _ProtWindow ProtWindow;
struct _ProtWindow {
    /* ユーザが独自に(ウィンドウ毎に)保持させておきたいデータ(へのポインタ)
     * ウィンドウが一つだけなら, global でもなんでも良いんじゃん?  */
    void      *user_data;
    int        width, height;    //作成する grid の縦と横
    char      *window_title;
    char      *button1_label;
    char      *button2_label;
    char      *button3_label;
    char      *button4_label;
    void     (*button1_callback)(ProtWindow *protwin);
    void     (*button2_callback)(ProtWindow *protwin);
    void     (*button3_callback)(ProtWindow *protwin);
    void     (*button4_callback)(ProtWindow *protwin);

    /***** 以下は直接弄らないこと *****/
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


/* 構造体の初期化を行う */
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
