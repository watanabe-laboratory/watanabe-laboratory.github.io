#!/bin/tcsh -f

@ total = 0
@ x = $1

while ( $x <= $2 )
    @ y = 2 * $x + 1
    @ t = `./gcd2T $x $y`
    echo $x $y $t
    @ total = $total + $t
    @ x = $x + 1
end

@ ave = $total / (($2 - $1) + 1)
echo $total $ave
