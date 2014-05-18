reset

set border linewidth 0.5
set pointsize 1.5
set style line 1 lc rgb '#ff0000' pt 7   # circle
set style line 2 lc rgb '#00ff00' pt 9   # triangle
set style line 3 lc rgb '#0000ff' pt 5   # square

set title "Residual energy at end of simulation"

set xlabel "Row (corresponding to y coordinate)"
set ylabel "Column (corresponding to x coordinate)"

set cbrange [0:5]
set cblabel "Energy (mWh)"
set xrange [-0.5:15.5]
set yrange [-0.5:15.5]

set terminal postscript eps enhanced color font 'Helvetica,20' lw 8

set output 'Config1_resEner.eps'
plot "Config1_resEner.data" using 1:2:3 with image

set output 'Config2_resEner.eps'
plot "Config2_resEner.data" using 1:2:3 with image

set output 'Config3_resEner.eps'
plot "Config3_resEner.data" using 1:2:3 with image
