program add1
1: $1 = $1 + 1
program end.

program main
10: if ($1 != 0) goto 30
20: $1 = 5
30: print($1)
40: if ( $1 == 0 ) goto 70
50:   $1 = $1 - 1
60: goto 40
70: &add1
80: return($1)
program end.

