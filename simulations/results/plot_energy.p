reset

set border linewidth 0.5
set pointsize 1

set style line 1 lt 1 lw 0.5 lc rgb '#ff0000' pt 1 pi 2
set style line 2 lt 1 lw 0.5 lc rgb '#ff3333' pt 2 pi 2
set style line 3 lt 1 lw 0.5 lc rgb '#ff6666' pt 3 pi 2
set style line 4 lt 1 lw 0.5 lc rgb '#00ff00' pt 4 pi 2
set style line 5 lt 1 lw 0.5 lc rgb '#33ff33' pt 5 pi 2
set style line 6 lt 1 lw 0.5 lc rgb '#66ff66' pt 6 pi 2
set style line 7 lt 1 lw 0.5 lc rgb '#0000ff' pt 7 pi 2
set style line 8 lt 1 lw 0.5 lc rgb '#3333ff' pt 8 pi 2
set style line 9 lt 1 lw 0.5 lc rgb '#6666ff' pt 9 pi 2
set style line 10 lt 1 lw 0.5 lc rgb '#000000' pt 10 pi 2

set terminal postscript eps enhanced color font 'Helvetica,20' lw 8

set key bottom right
set xlabel "Time (s)"
set ylabel "Total residual energy (mWh)"
#set xrange [0:150]
set yrange [0:1300]

set title "Total residual energy"
set output 'energy1.eps'
plot "Config21_energy.csv" u 1:2 t "XT-MAC" w lp ls 4, \
     "Config6_energy.csv" u 1:2 t "X-MAC" w lp ls 2, \
     "Config3_energy.csv" u 1:2 t "B-MAC" w lp ls 8

set title "Total residual energy of XT-MAC"
set output 'energy2.eps'
plot "Config2_energy.csv" u 1:2 t "sleepInterval = 0.1 s" w lp ls 1, \
     "Config21_energy.csv" u 1:2 t "sleepInterval = 0.15 s" w lp ls 4, \
     "Config22_energy.csv" u 1:2 t "sleepInterval = 0.2 s" w lp ls 7
