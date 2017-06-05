#Executar primer:
#./placaCN 100 100 100 1 1 1 1e-6 1000
#després anar a gnuplot:
# $ gnuplot
#i dins gnuplot carregar el fitxer amb:
#load 'placaCN.gnu'

#gràfics projectats amb colors
unset hidden3d
set pm3d
unset surface
set view map

# fonts
set xtic font ',16'
set ytic font ',16'
set cbtic font ',16'
set title font ',20'

#escala del plot
set cbrange [0:25]

#format output
set terminal postscript eps enhanced color font 'Helvetica,10'

#Fem tots els plots del pdf:
#Plot 1
set output 't0.eps'
set title 't=0.000'
splot 'placaCN.txt' index 0 w l title ''

#Plot 2
set output 't006.eps'
set title 't=0.060'
splot 'placaCN.txt' index 6 w l title ''

#nota que el pas és 0.01, i si volem arribar a 0.06 negressitem 6 passos
#per tant index=6, similarment es dedueix index 25, index 62 i index 100.

#Plot 3
set output 't025.eps'
set title 't=0.250'
splot 'placaCN.txt' index 25 w l title ''

#Plot 4
set output 't062.eps'
set title 't=0.620'
splot 'placaCN.txt' index 62 w l title ''

#Plot 5
set output 't1.eps'
set title 't=1.000'
splot 'placaCN.txt' index 100 w l title ''
