reset

set border linewidth 0.5
set pointsize 1

set style line 1 lt 1 lw 0.5 lc rgb '#ff0000' pt 1
set style line 2 lt 1 lw 0.5 lc rgb '#ff8800' pt 2
set style line 3 lt 1 lw 0.5 lc rgb '#ff0088' pt 3
set style line 4 lt 1 lw 0.5 lc rgb '#00ff00' pt 4
set style line 5 lt 1 lw 0.5 lc rgb '#88ff00' pt 5
set style line 6 lt 1 lw 0.5 lc rgb '#00ff88' pt 6
set style line 7 lt 1 lw 0.5 lc rgb '#0000ff' pt 7
set style line 8 lt 1 lw 0.5 lc rgb '#0088ff' pt 8
set style line 9 lt 1 lw 0.5 lc rgb '#8800ff' pt 9
set style line 10 lt 1 lw 0.5 lc rgb '#000000' pt 10

set title "A segment of tracked path"
set key bottom right
set xlabel "x (m)"
set ylabel "y (m)"
set xrange [300:400]
set yrange [50:150]

set terminal postscript eps enhanced color font 'Helvetica,20' lw 8

set output 'trace_part1.eps'
plot "../path1.txt" u 1:2 t "True path" w l ls 10, \
     "bs_output/Config2_trace_all.data" u 1:2 t "with XT-MAC" w l ls 1, \
     "bs_output/Config3_trace_all.data" u 1:2 t "with B-MAC" w l ls 8

set output 'trace_part3.eps'
plot "../path1.txt" u 1:2 t "True path" w l ls 10, \
     "bs_output/Config21_trace_all.data" u 1:2 t "CSP" w l ls 4, \
     "bs_output/Config3_trace_all.data" u 1:2 t "CSP with B-MAC" w l ls 2, \
     "bs_output/Config5_trace_all.data" u 1:2 t "CSP with ARPEES" w l ls 8
