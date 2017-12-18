/* console ver. animation有り, last map有り */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE    40           /* HP 列の長さの上限              */
#define DSIZE   60           /* 配置画面の大きさの上限         */
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

/* 対象とする HP 列の表わし方 */
enum elmtyp {NELM, PELM, HELM, WALL};
enum elmtyp hp[SIZE];         /* HP 列 */
int len;                      /* HP 列長 length >= 1 */

/* 配置画面の表わし方 */
/* 座標系は左上が (1,1) 右下が (Asize, Asize) */
enum elmtyp lat[DSIZE+1][DSIZE+1]; /* 現在の配置画面（x 座標，y 座標）*/
char outlat[2*DSIZE+1][2*DSIZE+1]; /* 出力用配置画面（x 座標，y 座標）*/

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
char DIRCH[5] = { '<', '^', ' ', 'v', '>' };
struct dirtyp maxvec[SIZE];   /* 現時点での最良方向ベクトル */
struct dirtyp vec[SIZE];      /* 現在探索中の方向ベクトル   */
int maxhh;                    /* 現時点での最大 HH 得点     */

int  sx, sy;                  /* 出発座標: (x, y) */
int  gx, gy;                  /* 目標座標: (x, y) */
int  step;                    /* 目標ステップ数   */


void assignelm(int x, int y, enum elmtyp token);
void assigndir(int s, int x, int y, struct dirtyp dir);
void unassigndir(int s, int x, int y, struct dirtyp dir);

void leaf(int s, int hh, int x, int y);
int  hhpoint(int cx, int cy, int nx, int ny);
int  find(int cx, int cy, int ci, int cs, int hh,
   int hnum, int stnum, struct dirtyp pdir);
int  findT(int cx, int cy, int ci, int cs, int hh);
int  closereg(int nx, int ny, int diri);

/***********************************************************************/
int main(void) {
  
  char inhp[SIZE];           /* 入力 HP 列 */
  char rp;                   /* 次の試行を行うか (y/n) */
  int resulthh;              /* 最終的な最大 HH 得点 */
  int hnum;                  /* 探索区間 H 数（出発点は含めず）*/
  int cx, cy, dirx, diry;
  int x, y, s;
  int i, j, k;
  float err;

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

  rp = 'y';
  while(rp =='y'){
    printf("HP: ");
    for(i = 0; i < len; i++)
      printf("%c", inhp[i]);
    printf("\n");
    
    Asize = (int)((float)len / 2) * 2 + 1;  step = len - 1;
    printf("Set basic parameter.\n");
    printf("   Area Size: %d\n", Asize);
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

    printf("Set stesp and the goal point.\n");
    while(1) {
      printf("   Steps;     %d\n", step);
      if(step > len - 1)
	printf("Illegal steps.  Steps must be at most <= HP - 1.\n");
      else
	break;
    }
    Dlimitint = Dlimit * (float)step + 0.5;
    sx = (int)(Asize/2);  sy = (int)(Asize/2);
    printf("   Start Point: x, y = %d, %d\n", sx, sy);
    while(1) {
      fflush(stdin);
      printf("   Goal Point:  x, y = ");  scanf("%d, %d", &gx, &gy);
      if(abs(gx - sx) + abs(gy - sy) > step + 1)
 printf("Not enough steps!\n");
      else if(Dsw != 0 && abs(gx - sx) + abs(gy - sy) > Dlimitint)
 printf("Out of Distance Limit!\n");
      else
 break;
    }
    
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
    maxhh = NOEXIST;
    hnum = 0;
    for(s = 1; s <= step; s++)
      if(hp[0+s] == HELM)
 hnum++;

    /* HP 列の配置 */
    resulthh = find(sx, sy, 0, 1, 0, hnum, 0, BLNK);
    printf("\n\n");
    if(resulthh == NOEXIST)
      printf("Not reachable!\n\n");
    else {
      printf("Max HH contact = %d\n\n", resulthh);
    }

  /* 最終的な方向 vector (maxvec) に従い配置 */
  j = 1;
  for(y = 1; y <= Asize; y++) {
    i = 1;
    for(x = 1; x <= Asize; x++) {
      outlat[i][j] = '.';
      outlat[i][j+1] = '.';
      outlat[i+1][j] = '.';
      outlat[i+1][j+1] = '.';
      i = i + 2;
    }
    j = j + 2;
  }

  cx = sx;  cy = sy;
  for(s = 1; s <= step; s++) {
    /* PH 要素の挿入 */
    if(hp[s-1] == PELM)
      outlat[2*cx-1][2*cy-1] = 'P';
    else if(hp[s-1] == HELM)
      outlat[2*cx-1][2*cy-1] = 'H';

    /* 方向の挿入 */
    dirx = maxvec[s].x;  diry = maxvec[s].y;
    outlat[2*cx-1+dirx][2*cy-1+diry] = DIRCH[2*dirx+diry+2];
    cx = cx + dirx;  cy = cy + diry;
  }
  if(hp[step] == PELM)
    outlat[2*cx-1][2*cy-1] = 'P';
  else if(hp[step] == HELM)
    outlat[2*cx-1][2*cy-1] = 'H';




    /* 最終的な配置の表示 */

  printf("     *");
  for(i = 1; i <= Asize - 1; i++)
    printf("%1d.", i % 10);
  printf("%1d", Asize % 10);
  printf("*\n");
  for(j = 1; j <= 2 * Asize - 1; j++) {
    if(j % 2 == 1)
      printf("     %1d", ((j + 1) / 2) % 10);
    else
      printf("      ");
    for(i = 1; i <= 2 * Asize - 1; i++)
      printf("%c", outlat[i][j]);
    if(j % 2 == 1)
      printf("%1d", ((j + 1) / 2) % 10);
    printf("\n");
  }
  printf("     *");
  for(i = 1; i <= Asize - 1; i++)
    printf("%1d.", i % 10);
  printf("%d*\n\n", Asize % 10);

    while(1) {
      printf("   Repeart? (y/n): ");
      fflush(stdin);
      scanf("%c", &rp);
      if(rp!='y' && rp!='n')
	printf("Reply by [y] or [n]!\n");
      else
	break;
    }
    printf("\n");
  }

  printf("Good Bye!!\n\n");
  exit(0);
}

/**********************************************************************/
/* 画面への要素の配置（含：配置の消去）*/
void assignelm(int x, int y, enum elmtyp hp) {

  lat[x][y] = hp;
  if (hp == NELM) {
    printf("\b \b");
  } else if(hp == PELM) {
    printf("P");
  } else if(hp == HELM) {
    printf("H");
  }
  fflush(stdout);
}

/**********************************************************************/
/* 方向の設定 */
void assigndir(int s, int x, int y, struct dirtyp dir) {

  int dirx, diry;
  int icon;
  vec[s] = dir;
  dirx = dir.x;  diry = dir.y;

 printf("%c", DIRCH[2*dirx+diry+2]);
 fflush(stdout);

}

/**********************************************************************/
/* 方向設定の消去 */
void unassigndir(int s, int x, int y, struct dirtyp dir) {

  int offset_x, offset_y;
  vec[s] = BLNK;

  offset_x = dir.x;
  offset_y = dir.y;

  printf("\b \b");
  fflush(stdout);

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
  int rs, lasthh;
  int diri, nx, ny, nhh, nhnum, nstnum, nmaxhh;

  rs = step - cs;           /* 残りステップ数                     */
                            /* これから踏み出す分 (nx,ny) から先  */

  assignelm(cx, cy, hp[ci]);
  cmaxhh = NOEXIST;

  for(diri = 0; diri < 4; diri++) {
    dir = DIRS[diri];

    /* 次配置点の計算 */
    nx = cx + dir.x;  ny = cy + dir.y;

    /* 次配置点におけるか？ */
    if(lat[nx][ny] != NELM)
      continue;
      
    /* 次配置点から到達可能か？ */
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
      if(abs(nx - sx) +  abs(ny - sy) > Dlimitint)
 continue;

    /* 配置可能かつ望みあり */
    assigndir(cs, cx, cy, dir);

    if(abs(gx - nx) + abs(gy - ny) >= rs) {
      /* 最短距離で進むしかない場合                        */
      /* 注：この時点では  dist = rs or dist = rs + 1      */
      /* findT 呼び出し時  dist = rs + 1 or dist = rs + 2  */
      nmaxhh = findT(nx, ny, ci + 1, cs + 1, nhh);
    }
    else
      /* 通常の探索 */
      nmaxhh = find(nx, ny, ci + 1, cs + 1, nhh, nhnum, nstnum, dir);

    if(nmaxhh > cmaxhh)
      cmaxhh = nmaxhh;
    unassigndir(cs, cx, cy, dir);
  }
  assignelm(cx, cy, NELM);
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
    ||(lat[nx-11][ny+1]!=NELM && lat[nx][ny+1]==NELM))))
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
findT(int cx, int cy, int ci, int cs, int hh) {

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

  assignelm(cx, cy, hp[ci]);

  /* まず dir1 方向への移動 */
  while(d1 > 0) {
    /* 次配置点の計算 */
    nx = cx + dir1.x;  ny = cy + dir1.y;
    /* 次配置点におけるか？ */
    if(lat[nx][ny] == NELM) {
      /* 次配置点における場合 */
      assigndir(cs, cx, cy, dir1);
      if(hp[ci+1] == HELM)
 hh = hh + hhpoint(cx, cy, nx, ny);
      ci++; cx = nx;  cy = ny;
      assignelm(cx, cy, hp[ci]);
      d1--;  cs++;
    } else {
      /* 次配置点にない場合：dir2 方向を調べる */
      nx = cx + dir2.x;  ny = cy + dir2.y;
      if(d2 > 0 && lat[nx][ny] == NELM) {
 /* dir2 方向における場合 */
 assigndir(cs, cx, cy, dir2);
 if(hp[ci+1] == HELM)
   hh = hh + hhpoint(cx, cy, nx, ny);
 ci++; cx = nx;  cy = ny;
 assignelm(cx, cy, hp[ci]);
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
      assigndir(cs, cx, cy, dir2);
      if(hp[ci+1] == HELM)
 hh = hh + hhpoint(cx, cy, nx, ny);
      ci++; cx = nx;  cy = ny;
      assignelm(cx, cy, hp[ci]);
      d2--;  cs++;
    } else {
      /* 到達不可能 */
      hh = NOEXIST;
      break;
    }
  }

  cs--;
  if(hh != NOEXIST) {
    leaf(cs, hh, cx, cy);
  }

  /* 後始末 */
  assignelm(cx, cy, NELM);
  while(cs >= backpoint) {
    cx = cx - vec[cs].x;  cy = cy - vec[cs].y;
    unassigndir(cs, cx, cy, vec[cs]);
    assignelm(cx, cy, NELM);
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

  assignelm(cx, cy, hp[ci]);

  if(dx == 0 && dy == 0) {
    if(hh != NOEXIST) {
      leaf(cs - 1, hh, cx, cy);
    }
    assignelm(cx, cy, NELM);
    return(hh);
  }

  /* x 軸方向への移動 */
  hh1 = NOEXIST;
  if(dx > 0) {
    nx = cx + dirx.x;  ny = cy;
    if(lat[nx][ny] == NELM) {
      /* 次配置点における場合 */
      assigndir(cs, cx, cy, dirx);
      if(hp[ci+1] == HELM)
 nhh = hh + hhpoint(cx, cy, nx, ny);
      else
 nhh = hh;
      hh1 = find2(nx, ny, ci + 1, cs + 1, nhh, dx - 1, dy, dirx, diry);
      unassigndir(cs, cx, cy, dirx);
    }
  }

  /* y 軸方向への移動 */
  hh2 = NOEXIST;
  if(dy > 0) {
    nx = cx;  ny = cy + diry.y;
    if(lat[nx][ny] == NELM) {
      /* 次配置点における場合 */
      assigndir(cs, cx, cy, diry);
      if(hp[ci+1] == HELM)
 nhh = hh + hhpoint(cx, cy, nx, ny);
      else
 nhh = hh;
      hh2 = find2(nx, ny, ci + 1, cs + 1, nhh, dx, dy - 1, dirx, diry);
      unassigndir(cs, cx, cy, diry);
    }
  }

  /* 後始末と終了 */
  assignelm(cx, cy, NELM);
  if(hh1 > hh2)
    return(hh1);
  else
    return(hh2);
}
/**********************************************************************/
/* 探索木の葉（目標位置へ到達した場合）の処理 */
void leaf(int s, int hh, int x, int y) {

  int i;

  if(hh > maxhh) {
    maxhh = hh;
    for(i = 1; i <= s; i++)
      maxvec[i] = vec[i];
    /* 情報の出力 */
    printf("     %d; (%d, %d)\n", hh, x, y);
    for(i = 1; i <= s; i++){
      if(hp[i-1] == PELM)
	printf("P");
      else
	printf("H");
      printf("%c", DIRCH[2*vec[i].x+vec[i].y+2]);
    }
    if(hp[s] == PELM)
      printf("P");
    else
      printf("H");
  }
}
