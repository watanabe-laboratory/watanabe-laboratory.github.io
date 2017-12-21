/* window ver. animation:無し, 断続的探索:有り */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*************************** Window表示用  ***************************/
#include <gtk/gtk.h>
#include "griddisp2.h"

//使いたいアイコンの XPMファイル名. includeする. (名前の衝突に注意!)
#include "icon_L.xpm"
#include "icon_R.xpm"
#include "icon_U.xpm"
#include "icon_D.xpm"
#include "icon_P.xpm"
#include "icon_H.xpm"
#include "icon_NULL.xpm"
#include "icon_G.xpm"

//上で #include したアイコンそれぞれの static char* 変数名[].
#define ICONS    {icon_L_xpm, icon_R_xpm, icon_U_xpm, icon_D_xpm, \
                  icon_P_xpm, icon_H_xpm, icon_NULL_xpm, icon_G_xpm}
//それぞれのアイコンにつけたいエイリアス(enum).
#define ICON_ID  {ICON_L,     ICON_R,     ICON_U,     ICON_D, \
                  ICON_P,     ICON_H,     ICON_NULL,  ICON_G}
//↓ここで xpm_icons_ary と nr_icon が定義される.(直接弄らない) */
ICONS_init(ICONS, ICON_ID);

void my_button1_callback(ProtWindow *protwin);
void my_button2_callback(ProtWindow *protwin);
void my_button3_callback(ProtWindow *protwin);
void my_button4_callback(ProtWindow *protwin);

/*********************************************************************/

#define SIZE   100           /* HP 列の長さの上限              */
#define DSIZE   27           /* 配置画面(lat)の大きさの上限    */
#define NOEXIST -1           /* 配置不可能時の HH コンタクト数 */

/* 各種 heuristics parameters */
int Asize;                   /* 配置画面（正方形）の一辺 */
float Hweight;               /* H 1 つ分の重み．0 < Hweight <= 2，0 はチェックなし */
int Hsw;                     /* Hweight を利用するかどうかの Switch                */
int STleng;                  /* 直線を続ける上限．0 は上限なし                     */
float Dlimit;                /* 出発との距離の上限．0 < Dlimit <= 1，0 は上限なし  */
int Dsw;                     /* Dlimit を利用するかどうかの Switch                 */
int Dlimitint;               /* Dlimit を実際の距離に直したもの                    */
int CRsw;                    /* Close Region Check Switch, SW = 0 or 1             */
int Tsw;                     /* Tight Cut Switch, SW = 0 or 1                      */
int ParaSet;                 /* ParameterがSetされているか                         */
int StepSet;                 /* 目標ステップ数stepがSetされているか                */
int Gsw;                     /* Goal Switch, SW = 0 or 1                           */
int GoalSet;                 /* Goal PointがSetされているか                        */

/* Console 表示系 */
float par;                   /* 探索の進度,0〜100(%) */

/* 対象とする HP 列の表わし方 */
char inhp[SIZE];                    /* 入力 HP 文字列 */
enum elmtyp {NELM, PELM, HELM, WALL};
enum elmtyp hp[SIZE];               /* HP 列 */
int len;                            /* HP 列長 length >= 1 */

/* 配置画面の表わし方 */
/* 座標系は左上が (1,1) 右下が (Asize, Asize) */
enum elmtyp lat[DSIZE+2][DSIZE+2]; /* 現在の配置画面（x 座標，y 座標）*/
char outlat[2*DSIZE+3][2*DSIZE+3]; /* 出力用配置画面（x 座標，y 座標）*/

/* 配置方向の表わし方 */
/* (x,y) の組で表現．DIRCH を用いるときは，2*x+y+2 で参照．              */
/* UP = (0,-1), DOWN = (0,1), LEFT = (-1,0), RIGHT = (1,0), BLNK = (0,0) */
/* DIRCH: 1              3              0              4           2     */
/* DIRCH: ^              v              <              >          blnk   */
struct dirtyp { int x; int y; };
struct dirtyp UP = { 0, -1 };
struct dirtyp DOWN  = { 0, 1 };
struct dirtyp LEFT  = { -1, 0 };
struct dirtyp RIGHT = { 1, 0 };
struct dirtyp BLNK  = { 0, 0 };
struct dirtyp DIRS[4] = { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } };
struct dirtyp maxvec[SIZE];   /* 現時点での最良方向ベクトル */
struct dirtyp vec[SIZE];      /* 現在探索中の方向ベクトル   */
int maxhh;                    /* 現時点での最大 HH 得点     */

int  sx, sy;                  /* 出発座標: (x, y)     */
int  gx, gy;                  /* 目標座標: (x, y)     */
int  step;                    /* 目標ステップ数       */
int  index[SIZE];
int  top;                     /* index[top]=探索済みの終端index */
int  topx, topy;              /* 探索済みの終端座標             */
int  cthh[SIZE];              /* (topx,topy) 迄での合計 HH 得点 */ 

void assignelm(int x, int y, enum elmtyp token);
void drawdir(int x, int y, struct dirtyp dir);
void erasedir(int x, int y, struct dirtyp dir);

void leaf(int s, int hh, int x, int y);
int  hhpoint(int cx, int cy, int nx, int ny);
int  find(int cx, int cy, int ci, int cs, int hh,
   int hnum, int stnum, struct dirtyp pdir);
int  findT(int cx, int cy, int ci, int cs, int hh);
int  closereg(int nx, int ny, int diri);
void start();

ProtWindow my_protwin; 
/**********************************************************************/
int main(int argc, char **argv)
{
  start();

  /* 構造体の初期化をしてから... */
  PROTWIN_INIT(my_protwin);
  /* 必要な値を入れてやる(これ以外は弄らない!) */
  my_protwin.window_title = "Protwin Ver.2.0";
  my_protwin.button1_label = "Go>>";
  my_protwin.button1_callback = &my_button1_callback;
  my_protwin.button2_label = "Back<<";
  my_protwin.button2_callback = &my_button2_callback;
  my_protwin.button3_label = "Parameter";
  my_protwin.button3_callback = &my_button3_callback;
  my_protwin.button4_label = "Step/Goal";
  my_protwin.button4_callback = &my_button4_callback;

  /* 画面の初期化 */
  protwin_init(argc, argv, &my_protwin, 2*Asize - 1, 2*Asize - 1);
  assignelm(sx, sy, hp[0]);

  /* 透明のアイコンを挿入して gridtableの大きさを確定する. */
  protwin_put_icon(&my_protwin, ICON_NULL, 0, 0);
  protwin_main_loop();
  return 0;
}
/***********************************************************************/

void my_button1_callback(ProtWindow *protwin)
{
  char rp;                   /* 次の試行を行うか (y/n) */
  int resulthh;              /* 最終的な最大 HH 得点 */
  int hnum;                  /* 探索区間 H 数（出発点は含めず）*/
  int cx, cy;
  int s, i, indtop;

  if(ParaSet == 0 || StepSet == 0 || (Gsw == 1 && GoalSet == 0)) return;

  maxhh = NOEXIST;
  hnum = 0;
  for(s = 1; s <= step; s++)
    if(hp[0+s] == HELM)
    hnum++;

  /* HP 列の配置 */
  resulthh = find(topx, topy, index[top], 1, 0, hnum, 0, vec[top]);
  printf(" 100%%\n\n");
  if(resulthh == NOEXIST){
    printf("Not reachable!\n\n");
    return;
  }
  else {
    cthh[top+1] = cthh[top] + resulthh;
    printf("Total Max HH contact = %d\n\n", cthh[top+1]);
  }

  /* 最終的な最高得点配置,及びその表示 */
  cx = topx;  cy = topy;
  indtop = index[top];
  assignelm(cx, cy, hp[indtop]);
  for(i=1; i<=step; i++) {
    drawdir(cx, cy, maxvec[indtop+i]);
    cx = cx + maxvec[indtop+i].x;  cy = cy + maxvec[indtop+i].y;
    assignelm(cx, cy, hp[indtop+i]);
  }
  if(GoalSet == 1)
    if(lat[gx][gy]==NELM)
      protwin_put_icon(&my_protwin, ICON_NULL, (gx-1)*2, (gy-1)*2);

  /* 探索済みの終端情報の更新など */
  top++;
  index[top] = index[top-1] + step;
  topx = cx; topy = cy;
  StepSet = 0;  GoalSet = 0;
  par = 0;

  /* 入力HP列の全てを配置し終えた場合の処理 */
  if(index[top] == len-1){
    while(1) {
      printf("HP has completely placed. Will you Restart? (y/n): ");
      fflush(stdin);
      scanf("%c", &rp);
      if(rp=='y'){
	cx = sx; cy = sy;
	for(i=1; i<len; i++) {
	  erasedir(cx, cy, maxvec[i]);
	  cx = cx + maxvec[i].x;  cy = cy + maxvec[i].y;
	  assignelm(cx, cy, NELM);
	}
	start();
	assignelm(sx, sy, hp[0]);
	return;
      } else if(rp=='n'){
	printf("You can select [Back]Track or [Quit].\n\n");
	break;
      }
      else{
	printf("Reply by [y] or [n]!\n");
      }
    }
  }
}

/**********************************************************************/
/* 画面への要素の配置（含：配置の消去）*/
void assignelm(int x, int y, enum elmtyp hp) {

  lat[x][y] = hp;
  if (hp == NELM) {
    protwin_put_icon(&my_protwin, ICON_NULL, (x-1)*2, (y-1)*2);
  } else if(hp == PELM) {
    protwin_put_icon(&my_protwin, ICON_P, (x-1)*2, (y-1)*2);
  } else if(hp == HELM) {
    protwin_put_icon(&my_protwin, ICON_H, (x-1)*2, (y-1)*2);
  }
  protwin_refresh();
}

/**********************************************************************/
/* 方向の設定 */
void drawdir(int x, int y, struct dirtyp dir) {

  int dirx, diry;
  int icon = 0;
  dirx = dir.x;  diry = dir.y;

  if      ( dirx ==  0 && diry == -1) { icon = ICON_U; }//up
  else if ( dirx ==  0 && diry ==  1) { icon = ICON_D; }//down
  else if ( dirx == -1 && diry ==  0) { icon = ICON_L; }//left
  else if ( dirx ==  1 && diry ==  0) { icon = ICON_R; }//right

  protwin_put_icon(&my_protwin, icon,
     (x-1)*2 + dirx, (y-1)*2 + diry);
}

/**********************************************************************/
/* 方向設定の消去 */
void erasedir(int x, int y, struct dirtyp dir) {

  int offset_x, offset_y;

  offset_x = dir.x;
  offset_y = dir.y;

  protwin_put_icon(&my_protwin, ICON_NULL,
     (x-1)*2 + offset_x, (y-1)*2 + offset_y);
}
/**********************************************************************/
/* 新たに置く位置に隣接している H の個数を返す */
int hhpoint(int cx, int cy, int nx, int ny) {

  int p;

  p = 0;
  if(lat[nx+1][ny] == HELM) p++;
  if(lat[nx-1][ny] == HELM) p++;
  if(lat[nx][ny+1] == HELM) p++;
  if(lat[nx][ny-1] == HELM) p++;
  if(lat[cx][cy] == HELM) p--;
  return(p);
}
/**********************************************************************/
/* 最適配置の探索（含：maxvec の計算）*/
int find(int cx, int cy, int ci, int cs, int hh,
  int hnum, int stnum, struct dirtyp pdir) {

  int cmaxhh;               /* この先に進めて行った場合の最大得点 */
  struct dirtyp dir;
  int rs;
  int diri, nx, ny, nhh, nhnum, nstnum, nmaxhh;

  rs = step - cs;           /* 残りステップ数                     */
                            /* これから踏み出す分 (nx,ny) から先  */
  lat[cx][cy] = hp[ci];
  cmaxhh = NOEXIST;

  for(diri = 0; diri < 4; diri++) {
    if(cs == 2){
      printf("%d%% ",(int)par);
      par = par + 100/16;
    }
    if(cs == 3)
      printf(".");
    fflush(stdout);
    dir = DIRS[diri];

    /* 次配置点の計算 */
    nx = cx + dir.x;  ny = cy + dir.y;

    /* 次配置点におけるか？ */
    if(lat[nx][ny] != NELM)
      continue;

    /* 次配置点から到達可能か？ */
    if(GoalSet==1)
      if(abs(gx - nx) + abs(gy - ny) > rs + 1)
	continue;

    /* 次配置点は close region を形成するか？ */
    if(CRsw != 0)
      if(closereg(nx, ny, diri))
	continue;

    /* 次配置点で同じ方向（直線）が続きすぎたか？ */
    if(STleng != 0) {
      if(pdir.x == dir.x && pdir.y == dir.y) {
        nstnum = stnum + 1;
        if(nstnum > STleng)
          continue;
      } else
        nstnum = 1;
    } else
      nstnum = 0;

    /* 次配置点は望みなしか？ */
    if(hp[ci+1] == HELM) {
      nhnum = hnum - 1;
      nhh = hh + hhpoint(cx, cy, nx, ny);
      if(Hsw != 0)
	if(maxhh > nhh + (int)((float)nhnum * Hweight + 0.5))
	  continue;
    } else {
      nhnum = hnum;  nhh = hh;
    }

    /* 次配置点は出発点から離れすぎたか？ */
    if(Dsw != 0)
      if(abs(nx - topx) +  abs(ny - topy) > Dlimitint)
      continue;

    /* 配置可能かつ望みあり */
    vec[index[top]+cs] = dir;

    if(GoalSet==1)
      if(abs(gx - nx) + abs(gy - ny) >= rs) {
	/* 最短距離で進むしかない場合                        */
	/* 注：この時点では  dist = rs or dist = rs + 1      */
	/* findT 呼び出し時  dist = rs + 1 or dist = rs + 2  */
	nmaxhh = findT(nx, ny, ci + 1, cs + 1, nhh);
      }
      else
	/* 通常の探索 */
	nmaxhh = find(nx, ny, ci + 1, cs + 1, nhh, nhnum, nstnum, dir);
    else
      /* Goalが無い場合は、findT()をとばしてleaf()を実行する */
      if(cs == step){
	leaf(cs, nhh, cx, cy);
	nmaxhh = maxhh;
      }
      else
	nmaxhh = find(nx, ny, ci + 1, cs + 1, nhh, nhnum, nstnum, dir);

    if(nmaxhh > cmaxhh)
      cmaxhh = nmaxhh;
    vec[index[top]+cs] = BLNK;
  }
  lat[cx][cy] = NELM;
  return(cmaxhh);
}

/**********************************************************************/
/* Close Region を形成するかの判定 */
int closereg(int nx, int ny, int diri) {

  switch(diri){
  case 0:
    if(((lat[nx-1][ny-1] != NELM || lat[nx][ny-1] != NELM || lat[nx+1][ny-1] != NELM)
      && lat[nx-1][ny] == NELM && lat[nx+1][ny] == NELM)
       ||
       (lat[nx][ny-1]==NELM && ((lat[nx-1][ny-1]!=NELM && lat[nx-1][ny]==NELM)
     ||(lat[nx+1][ny-1]!=NELM && lat[nx+1][ny]==NELM))))
      return(1);
    break; 
  case 1:
    if(((lat[nx-1][ny+1]!=NELM || lat[nx][ny+1]!=NELM || lat[nx+1][ny+1]!=NELM)
      && lat[nx-1][ny]==NELM && lat[nx+1][ny]==NELM)
       ||
       (lat[nx][ny+1]==NELM && ((lat[nx-1][ny+1]!=NELM && lat[nx-1][ny]==NELM)
     ||(lat[nx+1][ny+1]!=NELM && lat[nx+1][ny]==NELM))))
      return(1);
    break;
  case 2:
    if(((lat[nx-1][ny-1]!=NELM || lat[nx-1][ny]!=NELM || lat[nx-1][ny+1]!=NELM)
      && lat[nx][ny-1]==NELM && lat[nx][ny+1]==NELM)
       ||
       (lat[nx-1][ny]==NELM && ((lat[nx-1][ny-1]!=NELM && lat[nx][ny-1]==NELM)
     ||(lat[nx-1][ny+1]!=NELM && lat[nx][ny+1]==NELM))))
      return(1);
    break;
  case 3:
    if(((lat[nx+1][ny-1]!=NELM || lat[nx+1][ny]!=NELM || lat[nx+1][ny+1]!=NELM)
      && lat[nx][ny-1]==NELM && lat[nx][ny+1]==NELM)
       ||
       (lat[nx+1][ny]==NELM && ((lat[nx+1][ny-1]!=NELM && lat[nx][ny-1]==NELM)
     ||(lat[nx+1][ny+1]!=NELM && lat[nx][ny+1]==NELM))))
      return(1);
    break;
  }
  return(0);
}
/**********************************************************************/
/* 最適配置の探索（最短で進まなければならない場合）*/
/* 前提条件：distx + disty = rs + 1 or rs + 2      */
int findT(int cx, int cy, int ci, int cs, int hh) {

  int find1(int cx, int cy, int ci, int cs, int hh,
     int d1, int d2, struct dirtyp dir1, struct dirtyp dir2);
  int find2(int cx, int cy, int ci, int cs, int hh,
     int dx, int dy, struct dirtyp dirx, struct dirtyp diry);

  struct dirtyp dirx, diry;
  int hh1, hh2;
  int rs, dist, distx, disty;

  rs = step - cs;           /* 残りステップ数（これから踏み出す分は含まず）*/
  distx = abs(gx - cx);  disty = abs(gy - cy);
  dist = distx + disty;

  /* 向きの決定 */
  if(gy > cy)
    diry = DOWN;
  else
    diry = UP;
  if(gx > cx)
    dirx = RIGHT;
  else
    dirx = LEFT;

  if(distx == 0)
    /* y 軸方向にしか進めない場合 */
    return(find1(cx, cy, ci, cs, hh, rs + 1, 0, diry, diry));
  else if(disty == 0)
    /* y 軸方向にしか進めない場合 */
    return(find1(cx, cy, ci, cs, hh, rs + 1, 0, dirx, diry));
  else if(dist == rs + 1) {
    /* ゴールが 1 つしかない場合 */
    if(Tsw == 0)
      return(find2(cx, cy, ci, cs, hh, distx, disty, dirx, diry));
    else {
      hh1 = find1(cx, cy, ci, cs, hh, distx, disty, dirx, diry);
      hh2 = find1(cx, cy, ci, cs, hh, disty, distx, diry, dirx);
      if(hh1 > hh2)
        return(hh1);
      else
        return(hh2);
    }
  } else { /* dist = rs + 2 */
    /* ゴールが 2 つある場合 */
    if(Tsw == 0) {
      hh1 = find2(cx, cy, ci, cs, hh, distx - 1, disty, dirx, diry);
      hh2 = find2(cx, cy, ci, cs, hh, distx, disty - 1, dirx, diry);
    } else {
      hh1 = find1(cx, cy, ci, cs, hh, distx - 1, disty, dirx, diry);
      hh2 = find1(cx, cy, ci, cs, hh, distx, disty - 1, dirx, diry);
      if(hh2 > hh1)
        hh1 = hh2;
      hh2 = find1(cx, cy, ci, cs, hh, disty - 1, distx, diry, dirx);
      if(hh2 > hh1)
        hh1 = hh2;
      hh2 = find1(cx, cy, ci, cs, hh, disty, distx - 1, diry, dirx);
    }
    if(hh1 > hh2)
      return(hh1);
    else
      return(hh2);
  }
}
/**********************************************************************/
/* 目標位置へ一意に進む場合 */
int find1(int cx, int cy, int ci, int cs, int hh,
   int d1, int d2, struct dirtyp dir1, struct dirtyp dir2) {

  int backpoint;             /* バックトラックで戻る場所 */
  int nx, ny;

  backpoint = cs;

  lat[cx][cy] = hp[ci];

  /* まず dir1 方向への移動 */
  while(d1 > 0) {
    /* 次配置点の計算 */
    nx = cx + dir1.x;  ny = cy + dir1.y;
    /* 次配置点におけるか？ */
    if(lat[nx][ny] == NELM) {
      /* 次配置点における場合 */
      vec[index[top]+cs] = dir1;
      if(hp[ci+1] == HELM)
        hh = hh + hhpoint(cx, cy, nx, ny);
      ci++; cx = nx;  cy = ny;
      lat[cx][cy] = hp[ci];
      d1--;  cs++;
    } else {
    /* 次配置点にない場合：dir2 方向を調べる */
      nx = cx + dir2.x;  ny = cy + dir2.y;
      if(d2 > 0 && lat[nx][ny] == NELM) {
        /* dir2 方向における場合 */
        vec[index[top]+cs] = dir2;
        if(hp[ci+1] == HELM)
          hh = hh + hhpoint(cx, cy, nx, ny);
        ci++; cx = nx;  cy = ny;
        lat[cx][cy] = hp[ci];
        d2--;  cs++;
      } else {
        /* 到達不可能 */
        hh = NOEXIST;  d2 = 0;
        break;
      }
    }
  }
  /* 続いて dir2 方向への移動 */
  while(d2 > 0) {
    /* 次配置点の計算 */
    nx = cx + dir2.x;  ny = cy + dir2.y;
    /* 次配置点におけるか？ */
    if(lat[nx][ny] == NELM) {
      /* 次配置点における場合 */
      vec[index[top]+cs] = dir2;
      if(hp[ci+1] == HELM)
        hh = hh + hhpoint(cx, cy, nx, ny);
      ci++; cx = nx;  cy = ny;
      lat[cx][cy] = hp[ci];
      d2--;  cs++;
    } else {
      /* 到達不可能 */
      hh = NOEXIST;
      break;
    }
  }

  cs--;
  if(hh != NOEXIST)
    leaf(cs, hh, cx, cy);

  /* 後始末 */
  lat[cx][cy] = NELM;
  while(cs >= backpoint) {
    cx = cx - vec[index[top]+cs].x;  cy = cy - vec[index[top]+cs].y;
    vec[index[top]+cs] = BLNK;
    lat[cx][cy] = NELM;
    cs--;
  }
  return(hh);
}
/**********************************************************************/
/* 最短経路をすべて探索する場合 */
int find2(int cx, int cy, int ci, int cs, int hh,
   int dx, int dy, struct dirtyp dirx, struct dirtyp diry) {

  int nx, ny, nhh;
  int hh1, hh2;

  lat[cx][cy] = hp[ci];

  if(dx == 0 && dy == 0) {
    if(hh != NOEXIST) {
      leaf(cs - 1, hh, cx, cy);
    }
    lat[cx][cy] = NELM;
    return(hh);
  }

  /* x 軸方向への移動 */
  hh1 = NOEXIST;
  if(dx > 0) {
    nx = cx + dirx.x;  ny = cy;
    if(lat[nx][ny] == NELM) {
      /* 次配置点における場合 */
      vec[index[top]+cs] = dirx;
      if(hp[ci+1] == HELM)
        nhh = hh + hhpoint(cx, cy, nx, ny);
      else
        nhh = hh;
      hh1 = find2(nx, ny, ci + 1, cs + 1, nhh, dx - 1, dy, dirx, diry);
      vec[index[top]+cs] = BLNK;
    }
  }

  /* y 軸方向への移動 */
  hh2 = NOEXIST;
  if(dy > 0) {
    nx = cx;  ny = cy + diry.y;
    if(lat[nx][ny] == NELM) {
      /* 次配置点における場合 */
      vec[index[top]+cs] = diry;
      if(hp[ci+1] == HELM)
        nhh = hh + hhpoint(cx, cy, nx, ny);
      else
        nhh = hh;
      hh2 = find2(nx, ny, ci + 1, cs + 1, nhh, dx, dy - 1, dirx, diry);
      vec[index[top]+cs] = BLNK;
    }
  }

  /* 後始末と終了 */
  lat[cx][cy] = NELM;
  if(hh1 > hh2)
    return(hh1);
  else
    return(hh2);
}
/**********************************************************************/
/* 探索木の葉（目標位置へ到達した場合）の処理 */
void leaf(int s, int hh, int x, int y) {

  int i, indtop;

  if(hh > maxhh) {
    maxhh = hh;
    indtop = index[top];
    for(i = 1; i <= s; i++)
      maxvec[indtop+i] = vec[indtop+i];
    /* 情報の出力 */
    /*
    printf("     %d: ", hh);
    for(i = 1; i <= s; i++){
      if(hp[indtop+i-1] == PELM)
        printf("P");
      else
        printf("H");
      printf("%c", DIRCH[2*vec[indtop+i].x+vec[indtop+i].y+2]);
    }
    if(hp[indtop+s] == PELM)
      printf("P\n");
    else
      printf("H\n");
      */
  }
}
/*********************************************************************/
/* index[top-1]の状況まで戻る */
void my_button2_callback(ProtWindow *protwin) {

  int cx, cy, i;

  if(!top) return; /* topが0⇔index[top]=index[0]=0⇔それ以上履歴なし */

  /* 前回の最後より後を消す */
  cx = topx; cy = topy;
  for(i=index[top]; i>index[top-1]; i--){
    assignelm(cx, cy, NELM);
    cx = cx - maxvec[i].x;  cy = cy - maxvec[i].y;
    erasedir(cx, cy, maxvec[i]);
  }

  topx = cx; topy = cy;
  top--;
}

/*********************************************************************/
/* ParameterSetのMENU */
void my_button3_callback(ProtWindow *protwin) {

  float err;

  if(index[top] == len-1) return; /* 最後まで探索済み */

  printf("Set search parameters.\n");
  fflush(stdin);
  while(1){
    printf("   H Weight  (0 or <= 2): ");   scanf("%f", &Hweight);
    if(Hweight < 0 || Hweight > 2)
      printf("Set Hweight=0(off) or 0<Hweight<=2(on)!\n");
    else
      break;
  }
  while(1) {
    printf("   ST Length (0 or 1 <=): ");   scanf("%f", &err);
    if(floor(err) != err || STleng < 0)
      printf("Set STLength=0(off) or STLength=1,2,...(on)!\n");
    else{
      STleng = err;
      break;
    }
  }
  while(1) {
    printf("   Dist Limit(0 or <= 1): ");   scanf("%f", &Dlimit);
    if(Dlimit < 0 || Dlimit > 1)
      printf("Set DistLimit=0(off) or 0<DistLimit<=1(on)!\n");
    else
      break;
  }
  while(1) {
    printf("   Close SW  (0, 1):      ");   scanf("%f", &err);
    if(err != 0 && err != 1)
      printf("Set CloseSW=0(off) or 1(on)!\n");
    else{
      CRsw = (int)err;
      break;
    }
  }
  while(1) {
    printf("   Goal  SW  (0, 1):      ");   scanf("%f", &err);
    if(err != 0 && err != 1 )
      printf("Set GoalSW=0(off) or 1(on)!\n");
    else{
      Gsw = (int)err;
      break;
    }
  }
  if(Gsw == 1)
    while(1) {
      printf("   Tight SW  (0, 1):      ");   scanf("%f", &err);
      if(err != 0 && err != 1 )
      printf("Set GoalSW=0(off) or 1(on)!\n");
      else{
      Tsw = (int)err;
      break;
      }
    }

  if(Hweight == 0)
    Hsw = 0;
  else
    Hsw = 1;
  if(Dlimit == 0)
    Dsw = 0;
  else
    Dsw = 1;
  printf("Search parameters were set.\n\n");
  ParaSet = 1;

}

/*********************************************************************/
/* GoalPointを決めるMENU */
void my_button4_callback(ProtWindow *protwin) {

  int ogx = 0, ogy = 0;

  if(!ParaSet || index[top] == len-1) return;
  /* Parameterが未設定 or 最後まで探索済み*/

  printf("Set steps and the goal point.\n");
  fflush(stdin);
  while(1) {
    printf("     Steps(<=%2d): ",len-index[top]-1);  scanf("%d", &step);
    if(step > len - (index[top]+1))
      printf("Illegal steps.  Total Steps must be at most <= HP - 1.\n");
    else {
      StepSet = 1;
      break;
    }
  }
  Dlimitint = Dlimit * (float)step + 0.5;

  printf("   Current Point: x, y = %d, %d\n", topx, topy);

  if(GoalSet==1){
  ogx = gx; ogy = gy;
  }

  if(Gsw) {
    while(1) {
      fflush(stdin);
      printf("      Next Point: x, y = ");  scanf("%d, %d", &gx, &gy);
      if(abs(gx - topx) + abs(gy - topy) > step + 1)
        printf("Not enough steps!\n");
      else if(Dsw != 0 && abs(gx - topx) + abs(gy - topy) > Dlimitint)
        printf("Out of Distance Limit!\n");
      else if(lat[gx][gy]!=NELM)
	printf("Already placed!\n");
      else if(gx < 1 || gx > Asize || gy < 1 || gy > Asize)
	printf("Out of Area Size!\n");
      else{
	if(GoalSet==1)
	  protwin_put_icon(&my_protwin, ICON_NULL, (ogx-1)*2, (ogy-1)*2);
        GoalSet = 1;
	protwin_put_icon(&my_protwin, ICON_G, (gx-1)*2, (gy-1)*2);
        break;
      }
    }
  } else
    printf("      Next Point: Free.\n");
  printf("Steps and the goal point were set.\n\n");
}

/**********************************************************************/
/* 初期化 */
void start(){
  int i, x, y;
  FILE *fp;

  fp = fopen("hp.dat", "r");
  if(!fp){
    printf("File open error!\n");
    exit(1);
  }

  fgets(inhp, SIZE + 1, fp);  /* SIZE より長い部分は読み飛ばされる */
  len = strlen(inhp);
  close(fp);

  for(i = 0; i < len; i++) {
    if(inhp[i] == 'H')
      hp[i] = HELM;
    else if(inhp[i] == 'P')
      hp[i] = PELM;
    else {
      len = i;
      break;
    }
  }
  printf("\nHP data obtained.  HP length = %d\n", len);
  printf("HP: ");
  for(i = 0; i < len; i++)
    printf("%c", inhp[i]);
  printf("\n");

  Asize = (int)((float)len / 2) * 2 + 1;
  if(Asize > DSIZE){
    Asize = DSIZE;
  }
  printf("Set basic parameter.\n");
  printf("   Area Size: %d\n", Asize);

  sx = (int)(Asize/2) + 1;  sy = (int)(Asize/2) + 1;
  topx = sx, topy = sy;
  top = 0; index[top] = 0;
  vec[top] = BLNK;
  ParaSet = 0;
  StepSet = 0;
  GoalSet = 0;
  cthh[top] = 0;
  par = 0;

  /* 探索共通変数の初期化 */
  for(x = 1; x <= Asize; x++) {
    lat[x][0] = WALL; lat[x][Asize+1] = WALL;
  }
  for(y = 1; y <= Asize; y++) {
    lat[0][y] = WALL;  lat[Asize+1][y] = WALL;
  }
  for(y = 1; y <= Asize; y++)
    for(x = 1; x <= Asize; x++)
    lat[x][y] = NELM;
}
