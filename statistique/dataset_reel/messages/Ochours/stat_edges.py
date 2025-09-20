import dfs_iterat
import time

nbNoeud = 10000
dateMax = 1000
coutMax = 1


nbGraph = 1
nbEdges = 10000000
step = 1000
nbStep = 10

i = 0

fichier = open("stat_"+str(nbNoeud)+"_"+str(nbEdges)+"_"+str(coutMax)+".result", "w")
fichier.write("Nb_arêtes \t Num_graph \t Temps \t date_min \t Path \n")
fichier.close()

fichier = open("stat_"+str(nbNoeud)+"_"+str(nbEdges)+"_"+str(coutMax)+".moy", "w")
fichier.write("Nb_arêtes \t Temps_moy \n")
fichier.close()

while i < nbStep:
    j = 0
    moy = 0
    moy1 = 0
    while j < nbGraph:
        #graph, verTime = dfs_iterat.getGraph("dataset/graph_"+str(nbEdges)+"_"+str(nbNoeud)+"_"+str(dateMax)+"_"+str(j)+".gr")
    
        #debut = time.time()
        #path, minArrive = naivePath.shortestPath(graph, verTime, "0", "399") 
        #fin = time.time()
        #temps = fin - debut
        #moy += temps
        #del graph
        #del verTime

        graph, verTime = dfs_iterat.getGraph("graph_edges_"+str(nbEdges)+"_"+str(nbNoeud)+"_"+str(dateMax)+"_"+str(j)+".gr")
        
        debut1 = time.time()
        path1, minArrive1 = dfs_iterat.dfs(graph, verTime, "0", str(nbNoeud-1))
        fin1 = time.time()
        temps1 = fin1 - debut1
        moy1 += temps1
        
        del graph
        del verTime

        fichier = open("stat_"+str(nbNoeud)+"_"+str(nbEdges)+"_"+str(coutMax)+".result", "a")
        fichier.write(str(dateMax)+"\t"+str(j)+"\t"+str(temps1)+"\t"+str(minArrive1)+"\t"+str(path1)+"\n")
        fichier.close()

        j += 1
    
    #moy /= nbGraph
    moy1 /= nbGraph
    fichier = open("stat_"+str(nbNoeud)+"_"+str(nbEdges)+"_"+str(coutMax)+".moy", "a")
    fichier.write(str(dateMax)+" \t"+str(moy1)+"\n")
    fichier.close()
    i+=1
    dateMax += step