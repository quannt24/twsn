set title "Tracked paths"
set key outside right
set xlabel "x (m)"
set ylabel "y (m)"
#set xrange [100:220]
#set yrange [90:100]

set terminal postscript eps enhanced color font 'Helvetica,20' lw 8
set output 'trace.eps'

set border linewidth 0.5
set pointsize 1

set style line 10 lt 1 lw 0.5 lc rgb '#000000' pt 9 pi 20

set style line 1 lt 1 lw 0.5 lc rgb '#ff0000' pt 9 pi 20
set style line 2 lt 1 lw 0.5 lc rgb '#ff3333' pt 7 pi 20
set style line 3 lt 1 lw 0.5 lc rgb '#ff6666' pt 5 pi 20

set style line 4 lt 1 lw 0.5 lc rgb '#00ff00' pt 9 pi 20
set style line 5 lt 1 lw 0.5 lc rgb '#33ff33' pt 7 pi 20
set style line 6 lt 1 lw 0.5 lc rgb '#66ff66' pt 5 pi 20

set style line 7 lt 1 lw 0.5 lc rgb '#0000ff' pt 9 pi 20
set style line 8 lt 1 lw 0.5 lc rgb '#3333ff' pt 7 pi 20
set style line 9 lt 1 lw 0.5 lc rgb '#6666ff' pt 5 pi 20

plot "../path1.txt" u 1:2 t "True path" w l ls 10, \
     "bs_output/Config1_trace_all.data" u 1:2 t "Tracked path 1" w l ls 1, \
     "bs_output/Config2_trace_all.data" u 1:2 t "Tracked path 2" w l ls 4, \
     "bs_output/Config3_trace_all.data" u 1:2 t "Tracked path 2" w l ls 7
