reset

set border linewidth 0.2
set pointsize 1

set style line 1 lt 1 lw 0.2 pt 1

set title "Sequenced traces"

set xlabel "x (m)"
set ylabel "y (m)"
set zlabel "t (s)"

set xrange [-50:450]
set yrange [-50:450]
set ticslevel 0

unset key

#set terminal qt
set terminal postscript eps enhanced color font 'Helvetica,20' lw 8

set output 'Config1_trace_all_3d.eps'
splot 'bs_output/Config1_trace_all.data' w lp ls 1 lt palette

set output 'Config2_trace_all_3d.eps'
splot 'bs_output/Config2_trace_all.data' w lp ls 1 lt palette

set output 'Config3_trace_all_3d.eps'
splot 'bs_output/Config3_trace_all.data' w lp ls 1 lt palette
