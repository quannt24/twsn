reset

set border linewidth 0.5
set pointsize 1

set style line 10 lt 1 lw 0.5 lc rgb '#000000' pt 1 pi 20

set style line 1 lt 1 lw 0.5 lc rgb '#ff0000' pt 7 pi 20
set style line 2 lt 1 lw 0.5 lc rgb '#ff3333' pt 9 pi 20
set style line 3 lt 1 lw 0.5 lc rgb '#ff6666' pt 5 pi 20

set style line 4 lt 1 lw 0.5 lc rgb '#00ff00' pt 7 pi 20
set style line 5 lt 1 lw 0.5 lc rgb '#33ff33' pt 9 pi 20
set style line 6 lt 1 lw 0.5 lc rgb '#66ff66' pt 5 pi 20

set style line 7 lt 1 lw 0.5 lc rgb '#0000ff' pt 7 pi 20
set style line 8 lt 1 lw 0.5 lc rgb '#3333ff' pt 9 pi 20
set style line 9 lt 1 lw 0.5 lc rgb '#6666ff' pt 5 pi 20

set key top right
set xlabel "Timestamp (s)"
set ylabel "Delay (s)"
set xrange [0:150]
#set yrange [90:100]

set terminal postscript eps enhanced color font 'Helvetica,20' lw 8

set title "End-to-end delay"
set output 'delay.eps'
plot "Config1_delay.csv" u 1:2 t "without duty cycling" w p ls 10, \
     "Config2_delay.csv" u 1:2 t "with XT-MAC" w p ls 1, \
     "Config3_delay.csv" u 1:2 t "with B-MAC" w p ls 8
