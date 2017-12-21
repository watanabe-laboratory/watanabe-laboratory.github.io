/* window ver. animation:$BL5$7(B, $BCGB3E*C5:w(B:$BM-$j(B */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*************************** Window$BI=<(MQ(B  ***************************/
#include <gtk/gtk.h>
#include "griddisp2.h"

//$B;H$$$?$$%"%$%3%s$N(B XPM$B%U%!%$%kL>(B. include$B$9$k(B. ($BL>A0$N>WFM$KCm0U(B!)
#include "icon_L.xpm"
#include "icon_R.xpm"
#include "icon_U.xpm"
#include "icon_D.xpm"
#include "icon_P.xpm"
#include "icon_H.xpm"
#include "icon_NULL.xpm"
#include "icon_G.xpm"

//$B>e$G(B #include $B$7$?%"%$%3%s$=$l$>$l$N(B static char* $BJQ?tL>(B[].
#define ICONS    {icon_L_xpm, icon_R_xpm, icon_U_xpm, icon_D_xpm, \
                  icon_P_xpm, icon_H_xpm, icon_NULL_xpm, icon_G_xpm}
//$B$=$l$>$l$N%"%$%3%s$K$D$1$?$$%(%$%j%"%9(B(enum).
#define ICON_ID  {ICON_L,     ICON_R,     ICON_U,     ICON_D, \
                  ICON_P,     ICON_H,     ICON_NULL,  ICON_G}
//$B"-$3$3$G(B xpm_icons_ary $B$H(B nr_icon $B$,Dj5A$5$l$k(B.($BD>@\O.$i$J$$(B) */
ICONS_init(ICONS, ICON_ID);

void my_button1_callback(ProtWindow *protwin);
void my_button2_callback(ProtWindow *protwin);
void my_button3_callback(ProtWindow *protwin);
void my_button4_callback(ProtWindow *protwin);

/*********************************************************************/

#define SIZE   100           /* HP $BNs$ND9$5$N>e8B(B              */
#define DSIZE   27           /* $BG[CV2hLL(B(lat)$B$NBg$-$5$N>e8B(B    */
#define NOEXIST -1           /* $BG[CVIT2DG=;~$N(B HH $B%3%s%?%/%H?t(B */

/* $B3F<o(B heuristics parameters */
int Asize;                   /* $BG[CV2hLL!J@5J}7A!K$N0lJU(B */
float Hweight;               /* H 1 $B$DJ,$N=E$_!%(B0 < Hweight <= 2$B!$(B0 $B$O%A%'%C%/$J$7(B */
int Hsw;                     /* Hweight $B$rMxMQ$9$k$+$I$&$+$N(B Switch                */
int STleng;                  /* $BD>@~$rB3$1$k>e8B!%(B0 $B$O>e8B$J$7(B                     */
float Dlimit;                /* $B=PH/$H$N5wN%$N>e8B!%(B0 < Dlimit <= 1$B!$(B0 $B$O>e8B$J$7(B  */
int Dsw;                     /* Dlimit $B$rMxMQ$9$k$+$I$&$+$N(B Switch                 */
int Dlimitint;               /* Dlimit $B$r<B:]$N5wN%$KD>$7$?$b$N(B                    */
int CRsw;                    /* Close Region Check Switch, SW = 0 or 1             */
int Tsw;                     /* Tight Cut Switch, SW = 0 or 1                      */
int ParaSet;                 /* Parameter$B$,(BSet$B$5$l$F$$$k$+(B                         */
int StepSet;                 /* $BL\I8%9%F%C%W?t(Bstep$B$,(BSet$B$5$l$F$$$k$+(B                */
int Gsw;                     /* Goal Switch, SW = 0 or 1                           */
int GoalSet;                 /* Goal Point$B$,(BSet$B$5$l$F$$$k$+(B                        */

/* Console $BI=<(7O(B */
float par;                   /* $BC5:w$N?JEY(B,0$B!A(B100(%) */

/* $BBP>]$H$9$k(B HP $BNs$NI=$o$7J}(B */
char inhp[SIZE];                    /* $BF~NO(B HP $BJ8;zNs(B */
enum elmtyp {NELM, PELM, HELM, WALL};
enum elmtyp hp[SIZE];               /* HP $BNs(B */
int len;                            /* HP $BNsD9(B length >= 1 */

/* $BG[CV2hLL$NI=$o$7J}(B */
/* $B:BI87O$O:8>e$,(B (1,1) $B1&2<$,(B (Asize, Asize) */
enum elmtyp lat[DSIZE+2][DSIZE+2]; /* $B8=:_$NG[CV2hLL!J(Bx $B:BI8!$(By $B:BI8!K(B*/
char outlat[2*DSIZE+3][2*DSIZE+3]; /* $B=PNOMQG[CV2hLL!J(Bx $B:BI8!$(By $B:BI8!K(B*/

/* $BG[CVJ}8~$NI=$o$7J}(B */
/* (x,y) $B$NAH$GI=8=!%(BDIRCH $B$rMQ$$$k$H$-$O!$(B2*x+y+2 $B$G;2>H!%(B              */
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
struct dirtyp maxvec[SIZE];   /* $B8=;~E@$G$N:GNIJ}8~%Y%/%H%k(B */
struct dirtyp vec[SIZE];      /* $B8=:_C5:wCf$NJ}8~%Y%/%H%k(B   */
int maxhh;                    /* $B8=;~E@$G$N:GBg(B HH $BF@E@(B     */

int  sx, sy;                  /* $B=PH/:BI8(B: (x, y)     */
int  gx, gy;                  /* $BL\I8:BI8(B: (x, y)     */
int  step;                    /* $BL\I8%9%F%C%W?t(B       */
int  index[SIZE];
int  top;                     /* index[top]=$BC5:w:Q$_$N=*C<(Bindex */
int  topx, topy;              /* $BC5:w:Q$_$N=*C<:BI8(B             */
int  cthh[SIZE];              /* (topx,topy) $BKx$G$N9g7W(B HH $BF@E@(B */ 

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

  /* $B9=B$BN$N=i4|2=$r$7$F$+$i(B... */
  PROTWIN_INIT(my_protwin);
  /* $BI,MW$JCM$rF~$l$F$d$k(B($B$3$l0J30$OO.$i$J$$(B!) */
  my_protwin.window_title = "Protwin Ver.2.0";
  my_protwin.button1_label = "Go>>";
  my_protwin.button1_callback = &my_button1_callback;
  my_protwin.button2_label = "Back<<";
  my_protwin.button2_callback = &my_button2_callback;
  my_protwin.button3_label = "Parameter";
  my_protwin.button3_callback = &my_button3_callback;
  my_protwin.button4_label = "Step/Goal";
  my_protwin.button4_callback = &my_button4_callback;

  /* $B2hLL$N=i4|2=(B */
  protwin_init(argc, argv, &my_protwin, 2*Asize - 1, 2*Asize - 1);
  assignelm(sx, sy, hp[0]);

  /* $BF)L@$N%"%$%3%s$rA^F~$7$F(B gridtable$B$NBg$-$5$r3NDj$9$k(B. */
  protwin_put_icon(&my_protwin, ICON_NULL, 0, 0);
  protwin_main_loop();
  return 0;
}
/***********************************************************************/

void my_button1_callback(ProtWindow *protwin)
{
  char rp;                   /* $B<!$N;n9T$r9T$&$+(B (y/n) */
  int resulthh;              /* $B:G=*E*$J:GBg(B HH $BF@E@(B */
  int hnum;                  /* $BC5:w6h4V(B H $B?t!J=PH/E@$O4^$a$:!K(B*/
  int cx, cy;
  int s, i, indtop;

  if(ParaSet == 0 || StepSet == 0 || (Gsw == 1 && GoalSet == 0)) return;

  maxhh = NOEXIST;
  hnum = 0;
  for(s = 1; s <= step; s++)
    if(hp[0+s] == HELM)
    hnum++;

  /* HP $BNs$NG[CV(B */
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

  /* $B:G=*E*$J:G9bF@E@G[CV(B,$B5Z$S$=$NI=<((B */
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

  /* $BC5:w:Q$_$N=*C<>pJs$N99?7$J$I(B */
  top++;
  index[top] = index[top-1] + step;
  topx = cx; topy = cy;
  StepSet = 0;  GoalSet = 0;
  par = 0;

  /* $BF~NO(BHP$BNs$NA4$F$rG[CV$7=*$($?>l9g$N=hM}(B */
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
/* $B2hLL$X$NMWAG$NG[CV!J4^!'G[CV$N>C5n!K(B*/
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
/* $BJ}8~$N@_Dj(B */
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
/* $BJ}8~@_Dj$N>C5n(B */
void erasedir(int x, int y, struct dirtyp dir) {

  int offset_x, offset_y;

  offset_x = dir.x;
  offset_y = dir.y;

  protwin_put_icon(&my_protwin, ICON_NULL,
     (x-1)*2 + offset_x, (y-1)*2 + offset_y);
}
/**********************************************************************/
/* $B?7$?$KCV$/0LCV$KNY@\$7$F$$$k(B H $B$N8D?t$rJV$9(B */
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
/* $B:GE,G[CV$NC5:w!J4^!'(Bmaxvec $B$N7W;;!K(B*/
int find(int cx, int cy, int ci, int cs, int hh,
  int hnum, int stnum, struct dirtyp pdir) {

  int cmaxhh;               /* $B$3$N@h$K?J$a$F9T$C$?>l9g$N:GBgF@E@(B */
  struct dirtyp dir;
  int rs;
  int diri, nx, ny, nhh, nhnum, nstnum, nmaxhh;

  rs = step - cs;           /* $B;D$j%9%F%C%W?t(B                     */
                            /* $B$3$l$+$iF'$_=P$9J,(B (nx,ny) $B$+$i@h(B  */
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

    /* $B<!G[CVE@$N7W;;(B */
    nx = cx + dir.x;  ny = cy + dir.y;

    /* $B<!G[CVE@$K$*$1$k$+!)(B */
    if(lat[nx][ny] != NELM)
      continue;

    /* $B<!G[CVE@$+$iE~C#2DG=$+!)(B */
    if(GoalSet==1)
      if(abs(gx - nx) + abs(gy - ny) > rs + 1)
	continue;

    /* $B<!G[CVE@$O(B close region $B$r7A@.$9$k$+!)(B */
    if(CRsw != 0)
      if(closereg(nx, ny, diri))
	continue;

    /* $B<!G[CVE@$GF1$8J}8~!JD>@~!K$,B3$-$9$.$?$+!)(B */
    if(STleng != 0) {
      if(pdir.x == dir.x && pdir.y == dir.y) {
        nstnum = stnum + 1;
        if(nstnum > STleng)
          continue;
      } else
        nstnum = 1;
    } else
      nstnum = 0;

    /* $B<!G[CVE@$OK>$_$J$7$+!)(B */
    if(hp[ci+1] == HELM) {
      nhnum = hnum - 1;
      nhh = hh + hhpoint(cx, cy, nx, ny);
      if(Hsw != 0)
	if(maxhh > nhh + (int)((float)nhnum * Hweight + 0.5))
	  continue;
    } else {
      nhnum = hnum;  nhh = hh;
    }

    /* $B<!G[CVE@$O=PH/E@$+$iN%$l$9$.$?$+!)(B */
    if(Dsw != 0)
      if(abs(nx - topx) +  abs(ny - topy) > Dlimitint)
      continue;

    /* $BG[CV2DG=$+$DK>$_$"$j(B */
    vec[index[top]+cs] = dir;

    if(GoalSet==1)
      if(abs(gx - nx) + abs(gy - ny) >= rs) {
	/* $B:GC;5wN%$G?J$`$7$+$J$$>l9g(B                        */
	/* $BCm!'$3$N;~E@$G$O(B  dist = rs or dist = rs + 1      */
	/* findT $B8F$S=P$7;~(B  dist = rs + 1 or dist = rs + 2  */
	nmaxhh = findT(nx, ny, ci + 1, cs + 1, nhh);
      }
      else
	/* $BDL>o$NC5:w(B */
	nmaxhh = find(nx, ny, ci + 1, cs + 1, nhh, nhnum, nstnum, dir);
    else
      /* Goal$B$,L5$$>l9g$O!"(BfindT()$B$r$H$P$7$F(Bleaf()$B$r<B9T$9$k(B */
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
/* Close Region $B$r7A@.$9$k$+$NH=Dj(B */
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
/* $B:GE,G[CV$NC5:w!J:GC;$G?J$^$J$1$l$P$J$i$J$$>l9g!K(B*/
/* $BA0Ds>r7o!'(Bdistx + disty = rs + 1 or rs + 2      */
int findT(int cx, int cy, int ci, int cs, int hh) {

  int find1(int cx, int cy, int ci, int cs, int hh,
     int d1, int d2, struct dirtyp dir1, struct dirtyp dir2);
  int find2(int cx, int cy, int ci, int cs, int hh,
     int dx, int dy, struct dirtyp dirx, struct dirtyp diry);

  struct dirtyp dirx, diry;
  int hh1, hh2;
  int rs, dist, distx, disty;

  rs = step - cs;           /* $B;D$j%9%F%C%W?t!J$3$l$+$iF'$_=P$9J,$O4^$^$:!K(B*/
  distx = abs(gx - cx);  disty = abs(gy - cy);
  dist = distx + disty;

  /* $B8~$-$N7hDj(B */
  if(gy > cy)
    diry = DOWN;
  else
    diry = UP;
  if(gx > cx)
    dirx = RIGHT;
  else
    dirx = LEFT;

  if(distx == 0)
    /* y $B<4J}8~$K$7$+?J$a$J$$>l9g(B */
    return(find1(cx, cy, ci, cs, hh, rs + 1, 0, diry, diry));
  else if(disty == 0)
    /* y $B<4J}8~$K$7$+?J$a$J$$>l9g(B */
    return(find1(cx, cy, ci, cs, hh, rs + 1, 0, dirx, diry));
  else if(dist == rs + 1) {
    /* $B%4!<%k$,(B 1 $B$D$7$+$J$$>l9g(B */
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
    /* $B%4!<%k$,(B 2 $B$D$"$k>l9g(B */
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
/* $BL\I80LCV$X0l0U$K?J$`>l9g(B */
int find1(int cx, int cy, int ci, int cs, int hh,
   int d1, int d2, struct dirtyp dir1, struct dirtyp dir2) {

  int backpoint;             /* $B%P%C%/%H%i%C%/$GLa$k>l=j(B */
  int nx, ny;

  backpoint = cs;

  lat[cx][cy] = hp[ci];

  /* $B$^$:(B dir1 $BJ}8~$X$N0\F0(B */
  while(d1 > 0) {
    /* $B<!G[CVE@$N7W;;(B */
    nx = cx + dir1.x;  ny = cy + dir1.y;
    /* $B<!G[CVE@$K$*$1$k$+!)(B */
    if(lat[nx][ny] == NELM) {
      /* $B<!G[CVE@$K$*$1$k>l9g(B */
      vec[index[top]+cs] = dir1;
      if(hp[ci+1] == HELM)
        hh = hh + hhpoint(cx, cy, nx, ny);
      ci++; cx = nx;  cy = ny;
      lat[cx][cy] = hp[ci];
      d1--;  cs++;
    } else {
    /* $B<!G[CVE@$K$J$$>l9g!'(Bdir2 $BJ}8~$rD4$Y$k(B */
      nx = cx + dir2.x;  ny = cy + dir2.y;
      if(d2 > 0 && lat[nx][ny] == NELM) {
        /* dir2 $BJ}8~$K$*$1$k>l9g(B */
        vec[index[top]+cs] = dir2;
        if(hp[ci+1] == HELM)
          hh = hh + hhpoint(cx, cy, nx, ny);
        ci++; cx = nx;  cy = ny;
        lat[cx][cy] = hp[ci];
        d2--;  cs++;
      } else {
        /* $BE~C#IT2DG=(B */
        hh = NOEXIST;  d2 = 0;
        break;
      }
    }
  }
  /* $BB3$$$F(B dir2 $BJ}8~$X$N0\F0(B */
  while(d2 > 0) {
    /* $B<!G[CVE@$N7W;;(B */
    nx = cx + dir2.x;  ny = cy + dir2.y;
    /* $B<!G[CVE@$K$*$1$k$+!)(B */
    if(lat[nx][ny] == NELM) {
      /* $B<!G[CVE@$K$*$1$k>l9g(B */
      vec[index[top]+cs] = dir2;
      if(hp[ci+1] == HELM)
        hh = hh + hhpoint(cx, cy, nx, ny);
      ci++; cx = nx;  cy = ny;
      lat[cx][cy] = hp[ci];
      d2--;  cs++;
    } else {
      /* $BE~C#IT2DG=(B */
      hh = NOEXIST;
      break;
    }
  }

  cs--;
  if(hh != NOEXIST)
    leaf(cs, hh, cx, cy);

  /* $B8e;OKv(B */
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
/* $B:GC;7PO)$r$9$Y$FC5:w$9$k>l9g(B */
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

  /* x $B<4J}8~$X$N0\F0(B */
  hh1 = NOEXIST;
  if(dx > 0) {
    nx = cx + dirx.x;  ny = cy;
    if(lat[nx][ny] == NELM) {
      /* $B<!G[CVE@$K$*$1$k>l9g(B */
      vec[index[top]+cs] = dirx;
      if(hp[ci+1] == HELM)
        nhh = hh + hhpoint(cx, cy, nx, ny);
      else
        nhh = hh;
      hh1 = find2(nx, ny, ci + 1, cs + 1, nhh, dx - 1, dy, dirx, diry);
      vec[index[top]+cs] = BLNK;
    }
  }

  /* y $B<4J}8~$X$N0\F0(B */
  hh2 = NOEXIST;
  if(dy > 0) {
    nx = cx;  ny = cy + diry.y;
    if(lat[nx][ny] == NELM) {
      /* $B<!G[CVE@$K$*$1$k>l9g(B */
      vec[index[top]+cs] = diry;
      if(hp[ci+1] == HELM)
        nhh = hh + hhpoint(cx, cy, nx, ny);
      else
        nhh = hh;
      hh2 = find2(nx, ny, ci + 1, cs + 1, nhh, dx, dy - 1, dirx, diry);
      vec[index[top]+cs] = BLNK;
    }
  }

  /* $B8e;OKv$H=*N;(B */
  lat[cx][cy] = NELM;
  if(hh1 > hh2)
    return(hh1);
  else
    return(hh2);
}
/**********************************************************************/
/* $BC5:wLZ$NMU!JL\I80LCV$XE~C#$7$?>l9g!K$N=hM}(B */
void leaf(int s, int hh, int x, int y) {

  int i, indtop;

  if(hh > maxhh) {
    maxhh = hh;
    indtop = index[top];
    for(i = 1; i <= s; i++)
      maxvec[indtop+i] = vec[indtop+i];
    /* $B>pJs$N=PNO(B */
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
/* index[top-1]$B$N>u67$^$GLa$k(B */
void my_button2_callback(ProtWindow *protwin) {

  int cx, cy, i;

  if(!top) return; /* top$B$,(B0$B"N(Bindex[top]=index[0]=0$B"N$=$l0J>eMzNr$J$7(B */

  /* $BA02s$N:G8e$h$j8e$r>C$9(B */
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
/* ParameterSet$B$N(BMENU */
void my_button3_callback(ProtWindow *protwin) {

  float err;

  if(index[top] == len-1) return; /* $B:G8e$^$GC5:w:Q$_(B */

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
/* GoalPoint$B$r7h$a$k(BMENU */
void my_button4_callback(ProtWindow *protwin) {

  int ogx = 0, ogy = 0;

  if(!ParaSet || index[top] == len-1) return;
  /* Parameter$B$,L$@_Dj(B or $B:G8e$^$GC5:w:Q$_(B*/

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
/* $B=i4|2=(B */
void start(){
  int i, x, y;
  FILE *fp;

  fp = fopen("hp.dat", "r");
  if(!fp){
    printf("File open error!\n");
    exit(1);
  }

  fgets(inhp, SIZE + 1, fp);  /* SIZE $B$h$jD9$$ItJ,$OFI$_Ht$P$5$l$k(B */
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

  /* $BC5:w6&DLJQ?t$N=i4|2=(B */
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
