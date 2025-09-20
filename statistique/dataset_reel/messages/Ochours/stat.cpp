#include "fonction.h"
#include <time.h>


void writeFile(char* fileName, char* text, int pos, const char* mode){//Si po == fin alors ajouter a la fin 
    FILE* file; 
    file = fopen(fileName, mode);
    if(file != NULL){
        if(pos == 0)
            fseek(file, pos, SEEK_SET); 

        fprintf(file, "%s\n", text);
        fclose(file);
    }else{
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
    }
}



int main(){

    int nbNoeud = 2000;
    int dateMax = 400;
    int coutMax = 1;
    int nbGraphs = 1;
    int nbEdges = 10000;
    int nbStep = 10;
    char filename[] = "graph_edges";
    char fileName[200];
    
    char statName[200];
    char stat[200];
    char text[200];
    FILE *statFile = NULL, *moyFile = NULL;
    clock_t debut, fin;
    float temps;
    float tempsMoy = 0;

    sprintf(statName, "stat_a_%d_%d_%d.moy", nbNoeud, dateMax, coutMax);
    sprintf(stat, "stat_a_%d_%d_%d.result", nbNoeud, dateMax, coutMax);
    sprintf(text , "Nb_edges\tNum_graph\tTemps\tPath");
    writeFile(stat, text, 0, "r+");
    sprintf(text , "Nb_edges\tTemps_moy");
    writeFile(statName, text, 0, "r+");

    for(int i = 0; i < nbStep; i++){   
        tempsMoy = 0; 

        for(int j = 0; j < nbGraphs; j++){

            sprintf(fileName, "%s_%d_%d_%d_%d.gr", filename, nbEdges, nbNoeud, dateMax, j);
            Graphe_1* g1 = NULL;
            g1 = fileToGraphe_1(fileName, nbNoeud, dateMax);

            debut = clock();
            Path chemin = path(g1, 0, nbNoeud-1);
            fin = clock();
            freeGraphe(g1);
            printf("Le chemin est : %d\n", chemin.dateArrive);
            //temps = (fin.tv_usec/1000)-(debut.tv_usec/1000);
            temps = (float)(fin-debut)/ CLOCKS_PER_SEC;
            tempsMoy += temps; 
            printf("Le temps d'execution est %f\n", temps);
            sprintf(text , "%d\t%d\t%f\t%d", nbEdges, j, temps, chemin.dateArrive);
            writeFile(stat, text, -1, "a");

        }
        tempsMoy /= nbGraphs;
        sprintf(text , "%d\t%f", nbEdges, tempsMoy);
        writeFile(statName, text, -1, "a");
        
        nbEdges += 10000;
    }

    return 0;
}