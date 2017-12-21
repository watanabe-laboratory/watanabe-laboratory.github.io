/* console ver. animation$BM-$j(B, last map$BM-$j(B */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE    40           /* HP $BNs$ND9$5$N>e8B(B              */
#define DSIZE   60           /* $BG[CV2hLL$NBg$-$5$N>e8B(B         */
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

/* $BBP>]$H$9$k(B HP $BNs$NI=$o$7J}(B */
enum elmtyp {NELM, PELM, HELM, WALL};
enum elmtyp hp[SIZE];         /* HP $BNs(B */
int len;                      /* HP $BNsD9(B length >= 1 */

/* $BG[CV2hLL$NI=$o$7J}(B */
/* $B:BI87O$O:8>e$,(B (1,1) $B1&2<$,(B (Asize, Asize) */
enum elmtyp lat[DSIZE+1][DSIZE+1]; /* $B8=:_$NG[CV2hLL!J(Bx $B:BI8!$(By $B:BI8!K(B*/
char outlat[2*DSIZE+1][2*DSIZE+1]; /* $B=PNOMQG[CV2hLL!J(Bx $B:BI8!$(By $B:BI8!K(B*/

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
char DIRCH[5] = { '<', '^', ' ', 'v', '>' };
struct dirtyp maxvec[SIZE];   /* $B8=;~E@$G$N:GNIJ}8~%Y%/%H%k(B */
struct dirtyp vec[SIZE];      /* $B8=:_C5:wCf$NJ}8~%Y%/%H%k(B   */
int maxhh;                    /* $B8=;~E@$G$N:GBg(B HH $BF@E@(B     */

int  sx, sy;                  /* $B=PH/:BI8(B: (x, y) */
int  gx, gy;                  /* $BL\I8:BI8(B: (x, y) */
int  step;                    /* $BL\I8%9%F%C%W?t(B   */


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
  
  char inhp[SIZE];           /* $BF~NO(B HP $BNs(B */
  char rp;                   /* $B<!$N;n9T$r9T$&$+(B (y/n) */
  int resulthh;              /* $B:G=*E*$J:GBg(B HH $BF@E@(B */
  int hnum;                  /* $BC5:w6h4V(B H $B?t!J=PH/E@$O4^$a$:!K(B*/
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
    maxhh = NOEXIST;
    hnum = 0;
    for(s = 1; s <= step; s++)
      if(hp[0+s] == HELM)
 hnum++;

    /* HP $BNs$NG[CV(B */
    resulthh = find(sx, sy, 0, 1, 0, hnum, 0, BLNK);
    printf("\n\n");
    if(resulthh == NOEXIST)
      printf("Not reachable!\n\n");
    else {
      printf("Max HH contact = %d\n\n", resulthh);
    }

  /* $B:G=*E*$JJ}8~(B vector (maxvec) $B$K=>$$G[CV(B */
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
    /* PH $BMWAG$NA^F~(B */
    if(hp[s-1] == PELM)
      outlat[2*cx-1][2*cy-1] = 'P';
    else if(hp[s-1] == HELM)
      outlat[2*cx-1][2*cy-1] = 'H';

    /* $BJ}8~$NA^F~(B */
    dirx = maxvec[s].x;  diry = maxvec[s].y;
    outlat[2*cx-1+dirx][2*cy-1+diry] = DIRCH[2*dirx+diry+2];
    cx = cx + dirx;  cy = cy + diry;
  }
  if(hp[step] == PELM)
    outlat[2*cx-1][2*cy-1] = 'P';
  else if(hp[step] == HELM)
    outlat[2*cx-1][2*cy-1] = 'H';




    /* $B:G=*E*$JG[CV$NI=<((B */

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
/* $B2hLL$X$NMWAG$NG[CV!J4^!'G[CV$N>C5n!K(B*/
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
/* $BJ}8~$N@_Dj(B */
void assigndir(int s, int x, int y, struct dirtyp dir) {

  int dirx, diry;
  int icon;
  vec[s] = dir;
  dirx = dir.x;  diry = dir.y;

 printf("%c", DIRCH[2*dirx+diry+2]);
 fflush(stdout);

}

/**********************************************************************/
/* $BJ}8~@_Dj$N>C5n(B */
void unassigndir(int s, int x, int y, struct dirtyp dir) {

  int offset_x, offset_y;
  vec[s] = BLNK;

  offset_x = dir.x;
  offset_y = dir.y;

  printf("\b \b");
  fflush(stdout);

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
  int rs, lasthh;
  int diri, nx, ny, nhh, nhnum, nstnum, nmaxhh;

  rs = step - cs;           /* $B;D$j%9%F%C%W?t(B                     */
                            /* $B$3$l$+$iF'$_=P$9J,(B (nx,ny) $B$+$i@h(B  */

  assignelm(cx, cy, hp[ci]);
  cmaxhh = NOEXIST;

  for(diri = 0; diri < 4; diri++) {
    dir = DIRS[diri];

    /* $B<!G[CVE@$N7W;;(B */
    nx = cx + dir.x;  ny = cy + dir.y;

    /* $B<!G[CVE@$K$*$1$k$+!)(B */
    if(lat[nx][ny] != NELM)
      continue;
      
    /* $B<!G[CVE@$+$iE~C#2DG=$+!)(B */
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
      if(abs(nx - sx) +  abs(ny - sy) > Dlimitint)
 continue;

    /* $BG[CV2DG=$+$DK>$_$"$j(B */
    assigndir(cs, cx, cy, dir);

    if(abs(gx - nx) + abs(gy - ny) >= rs) {
      /* $B:GC;5wN%$G?J$`$7$+$J$$>l9g(B                        */
      /* $BCm!'$3$N;~E@$G$O(B  dist = rs or dist = rs + 1      */
      /* findT $B8F$S=P$7;~(B  dist = rs + 1 or dist = rs + 2  */
      nmaxhh = findT(nx, ny, ci + 1, cs + 1, nhh);
    }
    else
      /* $BDL>o$NC5:w(B */
      nmaxhh = find(nx, ny, ci + 1, cs + 1, nhh, nhnum, nstnum, dir);

    if(nmaxhh > cmaxhh)
      cmaxhh = nmaxhh;
    unassigndir(cs, cx, cy, dir);
  }
  assignelm(cx, cy, NELM);
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
/* $B:GE,G[CV$NC5:w!J:GC;$G?J$^$J$1$l$P$J$i$J$$>l9g!K(B*/
/* $BA0Ds>r7o!'(Bdistx + disty = rs + 1 or rs + 2      */
findT(int cx, int cy, int ci, int cs, int hh) {

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

  assignelm(cx, cy, hp[ci]);

  /* $B$^$:(B dir1 $BJ}8~$X$N0\F0(B */
  while(d1 > 0) {
    /* $B<!G[CVE@$N7W;;(B */
    nx = cx + dir1.x;  ny = cy + dir1.y;
    /* $B<!G[CVE@$K$*$1$k$+!)(B */
    if(lat[nx][ny] == NELM) {
      /* $B<!G[CVE@$K$*$1$k>l9g(B */
      assigndir(cs, cx, cy, dir1);
      if(hp[ci+1] == HELM)
 hh = hh + hhpoint(cx, cy, nx, ny);
      ci++; cx = nx;  cy = ny;
      assignelm(cx, cy, hp[ci]);
      d1--;  cs++;
    } else {
      /* $B<!G[CVE@$K$J$$>l9g!'(Bdir2 $BJ}8~$rD4$Y$k(B */
      nx = cx + dir2.x;  ny = cy + dir2.y;
      if(d2 > 0 && lat[nx][ny] == NELM) {
 /* dir2 $BJ}8~$K$*$1$k>l9g(B */
 assigndir(cs, cx, cy, dir2);
 if(hp[ci+1] == HELM)
   hh = hh + hhpoint(cx, cy, nx, ny);
 ci++; cx = nx;  cy = ny;
 assignelm(cx, cy, hp[ci]);
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
      assigndir(cs, cx, cy, dir2);
      if(hp[ci+1] == HELM)
 hh = hh + hhpoint(cx, cy, nx, ny);
      ci++; cx = nx;  cy = ny;
      assignelm(cx, cy, hp[ci]);
      d2--;  cs++;
    } else {
      /* $BE~C#IT2DG=(B */
      hh = NOEXIST;
      break;
    }
  }

  cs--;
  if(hh != NOEXIST) {
    leaf(cs, hh, cx, cy);
  }

  /* $B8e;OKv(B */
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
/* $B:GC;7PO)$r$9$Y$FC5:w$9$k>l9g(B */
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

  /* x $B<4J}8~$X$N0\F0(B */
  hh1 = NOEXIST;
  if(dx > 0) {
    nx = cx + dirx.x;  ny = cy;
    if(lat[nx][ny] == NELM) {
      /* $B<!G[CVE@$K$*$1$k>l9g(B */
      assigndir(cs, cx, cy, dirx);
      if(hp[ci+1] == HELM)
 nhh = hh + hhpoint(cx, cy, nx, ny);
      else
 nhh = hh;
      hh1 = find2(nx, ny, ci + 1, cs + 1, nhh, dx - 1, dy, dirx, diry);
      unassigndir(cs, cx, cy, dirx);
    }
  }

  /* y $B<4J}8~$X$N0\F0(B */
  hh2 = NOEXIST;
  if(dy > 0) {
    nx = cx;  ny = cy + diry.y;
    if(lat[nx][ny] == NELM) {
      /* $B<!G[CVE@$K$*$1$k>l9g(B */
      assigndir(cs, cx, cy, diry);
      if(hp[ci+1] == HELM)
 nhh = hh + hhpoint(cx, cy, nx, ny);
      else
 nhh = hh;
      hh2 = find2(nx, ny, ci + 1, cs + 1, nhh, dx, dy - 1, dirx, diry);
      unassigndir(cs, cx, cy, diry);
    }
  }

  /* $B8e;OKv$H=*N;(B */
  assignelm(cx, cy, NELM);
  if(hh1 > hh2)
    return(hh1);
  else
    return(hh2);
}
/**********************************************************************/
/* $BC5:wLZ$NMU!JL\I80LCV$XE~C#$7$?>l9g!K$N=hM}(B */
void leaf(int s, int hh, int x, int y) {

  int i;

  if(hh > maxhh) {
    maxhh = hh;
    for(i = 1; i <= s; i++)
      maxvec[i] = vec[i];
    /* $B>pJs$N=PNO(B */
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
