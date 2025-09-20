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
    int dateMax = 500;
    int coutMax = 1;
    int nbGraphs = 1;
    int nbEdges = 1000000;


    int step = 1; //Le saut.

    char filename[] = "Ocnodeslinks_clean_hours";
    char fileName[200];
    
    char statName[200];
    char stat[200];
    char text[200];
    FILE *statFile = NULL, *moyFile = NULL;
    clock_t debut, fin;
    float temps;
    float tempsMoy = 0;

    sprintf(statName, "stat_a_%d_%d_%d.moy", nbEdges, dateMax, coutMax);
    sprintf(stat, "stat_a_%d_%d_%d.result", nbEdges, dateMax, coutMax);

    sprintf(text , "Nb_noeuds\tNum_graph\tTemps\tPath");
    writeFile(stat, text, 0, "w");
    sprintf(text , "Nb_noeuds\tTemps_moy");
    writeFile(statName, text, 0, "w");

    for(int i = 0; i < nbStep; i++){   
        tempsMoy = 0; 

        for(int j = 0; j < nbGraphs; j++){

            sprintf(fileName, "%s.gr", filename);
            Graphe_1* g1 = NULL;
            g1 = fileToGraphe_1(fileName, nbNoeud, dateMax);
            debut = clock();
            printf("Hello\n");
            Path chemin = path(g1, 1, 7);
            fin = clock();
            freeGraphe(g1);
            printf("Le chemin est : %d\n", chemin.dateArrive);
            //temps = (fin.tv_usec/1000)-(debut.tv_usec/1000);
            temps = (float)(fin-debut)/ CLOCKS_PER_SEC;
            tempsMoy += temps; 
            printf("Le temps d'execution est %f\n", temps);
            sprintf(text , "%d\t%d\t%f\t%d", nbNoeud, j, temps, chemin.dateArrive);
            writeFile(stat, text, -1, "a");

        }
        tempsMoy /= nbGraphs;
        sprintf(text , "%d\t%f", nbNoeud, tempsMoy);
        writeFile(statName, text, -1, "a");
        
        nbNoeud += step;
    }

    return 0;
}