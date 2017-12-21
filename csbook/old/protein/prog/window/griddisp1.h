#ifndef GRIDDISP_H_INCLUDED
#define GRIDDISP_H_INCLUDED






/****************************************************************************
                                Data types
 ****************************************************************************/
typedef char **XpmIcon;

/* ***** XPM のアイコンデータの初期化用マクロ *****
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


typedef struct _pixmap_with_mask PixmapMask;
struct _pixmap_with_mask {
    GdkBitmap *mask;
    GdkPixmap *pixmap;
};


typedef struct _ProtWindow ProtWindow;
struct _ProtWindow {
    /* ユーザが独自に(ウィンドウ毎に)保持させておきたいデータ(へのポインタ)
     * ウィンドウが一つだけなら, global でもなんでも良いんじゃん?  */
    void      *user_data;
    int        width, height;   //作成する grid の縦と横
    char      *window_title;
    char      *button_label;    //default is " Start ".
    char      *default_text;
    void     (*button_callback)(ProtWindow *protwin, char *input_text);

    /***** 以下は直接弄らないこと *****/
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


/* 構造体の初期化を行う */
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
