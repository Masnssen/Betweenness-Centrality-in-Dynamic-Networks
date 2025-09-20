set terminal pngcairo
set output "message_time.png"

set ylabel "Execution time"
set xlabel "Path length"

# Obtenir les valeurs minimale et maximale de l'axe x
#stats "cc_OCnodeslinks_clean_hours.result" using 4 nooutput
#min_x = STATS_min
#max_x = STATS_max

#stats "cc_OCnodeslinks_clean_hours.result" using 3 nooutput 
#min_y = STATS_min
#max_y = STATS_max

# Définir l'échelle de l'axe x
#set xrange [0:100]
#set yrange [0:100]

# Définir la fonction de normalisation pour l'axe x
#normalize_x(x) = 100 * (x - min_x) / (max_x - min_x)
#normalize_y(y) = 100 * (y - min_y) / (max_y - min_y)


unset key

plot "stat_h_OCnodeslinks_clean_hours.result" using 4:3 with points pointtype 7