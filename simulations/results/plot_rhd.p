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

set key bottom right
set style fill solid
set boxwidth 0.5

set ylabel "Delay (s)"
#set xrange [0:150]
set yrange [0:0.22]

set title "Delay per hop in relaying of CSP"
set xlabel "sleepInterval"
set output 'rhd2.eps'
plot "rhd.csv" every ::3::3 u 1:3:xtic(2) notitle with boxes ls 1, \
     "rhd.csv" every ::4::4 u 1:3:xtic(2) notitle with boxes ls 3, \
     "rhd.csv" every ::5::5 u 1:3:xtic(2) notitle with boxes ls 4, \
     "rhd.csv" every ::6::6 u 1:3:xtic(2) notitle with boxes ls 9, \
     "rhd.csv" every ::7::7 u 1:3:xtic(2) notitle with boxes ls 7

set title "Delay per hop in relaying"
set xlabel "Configuration"
set output 'rhd3.eps'
plot "rhd.csv" every ::0::0 u 1:3:xtic(2) notitle with boxes ls 4, \
     "rhd.csv" every ::1::1 u 1:3:xtic(2) notitle with boxes ls 2, \
     "rhd.csv" every ::2::2 u 1:3:xtic(2) notitle with boxes ls 8
