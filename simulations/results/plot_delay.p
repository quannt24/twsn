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

set terminal postscript eps enhanced color font 'Helvetica,20' lw 8

set key top right
set xlabel "Timestamp (s)"
set ylabel "Delay (s)"
#set xrange [0:150]
#set yrange [90:100]

set title "End-to-end delay"
set output 'delay1.eps'
plot "Config21_delay.csv" u 1:2 t "XT-MAC" w p ls 4, \
     "Config6_delay.csv" u 1:2 t "X-MAC" w p ls 2, \
     "Config3_delay.csv" u 1:2 t "B-MAC" w p ls 8

set title "End-to-end delay of CSP"
set output 'delay2.eps'
plot "Config2_delay.csv" u 1:2 t "sleepInterval = 0.1 s" w p ls 1, \
     "Config23_delay.csv" u 1:2 t "sleepInterval = 0.125 s" w p ls 3, \
     "Config21_delay.csv" u 1:2 t "sleepInterval = 0.15 s" w p ls 4, \
     "Config24_delay.csv" u 1:2 t "sleepInterval = 0.175 s" w p ls 9, \
     "Config22_delay.csv" u 1:2 t "sleepInterval = 0.2 s" w p ls 7

set title "End-to-end delay"
set output 'delay3.eps'
plot "Config21_delay.csv" u 1:2 t "CSP" w p ls 4, \
     "Config3_delay.csv" u 1:2 t "CSP with B-MAC" w p ls 2, \
     "Config5_delay.csv" u 1:2 t "CSP with ARPEES" w p ls 8
