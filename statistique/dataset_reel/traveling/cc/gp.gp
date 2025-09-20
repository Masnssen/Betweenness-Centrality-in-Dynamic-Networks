set terminal pngcairo
set output "grahe_1.png"

set ylabel "Le cout du sommet"
set xlabel "Closeness centrality"

# Obtenir les valeurs minimale et maximale de l'axe x
stats "cc_network_temporal_day.test" using 4 nooutput
min_x = STATS_min
max_x = STATS_max

stats "cc_network_temporal_day.test" using 3 nooutput 
min_y = STATS_min
max_y = STATS_max

# Définir l'échelle de l'axe x
set xrange [-1:110]
set yrange [-1:110]

# Définir la fonction de normalisation pour l'axe x
normalize_x(x) = 100 * (x - min_x) / (max_x - min_x)
normalize_y(y) = 100 * (y - min_y) / (max_y - min_y)


unset key

plot "cc_network_temporal_day.test" using (normalize_x($4)):(normalize_y($3)) with points pointtype 7