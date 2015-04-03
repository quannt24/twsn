reset

set border linewidth 0.5
set pointsize 1

set style line 1 lt 1 lw 0.5 lc rgb '#ff0000' pt 1 pi 3
set style line 2 lt 1 lw 0.5 lc rgb '#ff8800' pt 2 pi 3
set style line 3 lt 1 lw 0.5 lc rgb '#ff0088' pt 3 pi 3
set style line 4 lt 1 lw 0.5 lc rgb '#00ff00' pt 4 pi 3
set style line 5 lt 1 lw 0.5 lc rgb '#88ff00' pt 5 pi 3
set style line 6 lt 1 lw 0.5 lc rgb '#00ff88' pt 6 pi 3
set style line 7 lt 1 lw 0.5 lc rgb '#0000ff' pt 7 pi 3
set style line 8 lt 1 lw 0.5 lc rgb '#0088ff' pt 8 pi 3
set style line 9 lt 1 lw 0.5 lc rgb '#8800ff' pt 9 pi 3
set style line 10 lt 1 lw 0.5 lc rgb '#000000' pt 10 pi 3

set terminal postscript eps enhanced color font 'Helvetica,20' lw 8

set key bottom right
set xlabel "Time (s)"
set ylabel "Total residual energy (mWh)"
#set xrange [0:150]
set yrange [800:1300]

set title "Total residual energy"
set output 'energy1.eps'
plot "Config21_energy.csv" u 1:2 t "XT-MAC" w lp ls 4, \
     "Config6_energy.csv" u 1:2 t "X-MAC" w lp ls 2, \
     "Config3_energy.csv" u 1:2 t "B-MAC" w lp ls 8

set title "Total residual energy of CSP"
set output 'energy2.eps'
plot "Config2_energy.csv" u 1:2 t "sleepInterval = 0.1 s" w lp ls 1, \
     "Config23_energy.csv" u 1:2 t "sleepInterval = 0.125 s" w lp ls 3, \
     "Config21_energy.csv" u 1:2 t "sleepInterval = 0.15 s" w lp ls 4, \
     "Config24_energy.csv" u 1:2 t "sleepInterval = 0.175 s" w lp ls 9, \
     "Config22_energy.csv" u 1:2 t "sleepInterval = 0.2 s" w lp ls 7

set title "Total residual energy"
set output 'energy3.eps'
plot "Config21_energy.csv" u 1:2 t "CSP" w lp ls 4, \
     "Config3_energy.csv" u 1:2 t "CSP with B-MAC" w lp ls 2, \
     "Config5_energy.csv" u 1:2 t "CSP with ARPEES" w lp ls 8

