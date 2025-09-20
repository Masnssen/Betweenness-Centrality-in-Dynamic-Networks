set terminal pngcairo
set output "paris_time.png"

set ylabel "Execution time"
set xlabel "Betweeness centrality"

# Obtenir les valeurs minimale et maximale de l'axe x
stats "bc_network_temporal_day_1.test" using 4 nooutput
min_x = STATS_min
max_x = STATS_max

stats "bc_network_temporal_day_1.test" using 5 nooutput 
min_y = STATS_min
max_y = STATS_max

# Définir l'échelle de l'axe x
set xrange [-1:110]
#set yrange [-1:110]

# Définir la fonction de normalisation pour l'axe x
normalize_x(x) = 100 * (x - min_x) / (max_x - min_x)
normalize_y(y) = 100 * (y - min_y) / (max_y - min_y)


unset key

plot "bc_network_temporal_day_1.test" using (normalize_x($4)):5 with points pointtype 7
#plot "bc_network_temporal_day_1.test" using (normalize_x($5)):(normalize_y($3)) with points pointtype 7