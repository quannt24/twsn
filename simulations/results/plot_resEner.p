reset

set border linewidth 0.5
set pointsize 1.5

set xlabel "Row (corresponding to y coordinate)"
set ylabel "Column (corresponding to x coordinate)"

set cbrange [0:5]
set cblabel "Energy (mWh)"
set xrange [-0.5:15.5]
set yrange [-0.5:15.5]

set terminal postscript eps enhanced color font 'Helvetica,20' lw 8

set title "Energy distribution with XT-MAC (sleepInterval = 0.1 s)"
set output 'Config2_resEner.eps'
plot "Config2_resEner.data" using 1:2:3 with image

set title "Energy distribution with XT-MAC (sleepInterval = 0.15 s)"
set output 'Config21_resEner.eps'
plot "Config21_resEner.data" using 1:2:3 with image

set title "Energy distribution with XT-MAC (sleepInterval = 0.2 s)"
set output 'Config22_resEner.eps'
plot "Config22_resEner.data" using 1:2:3 with image

set title "Energy distribution with B-MAC"
set output 'Config3_resEner.eps'
plot "Config3_resEner.data" using 1:2:3 with image

set title "Energy distribution with X-MAC"
set output 'Config6_resEner.eps'
plot "Config6_resEner.data" using 1:2:3 with image
