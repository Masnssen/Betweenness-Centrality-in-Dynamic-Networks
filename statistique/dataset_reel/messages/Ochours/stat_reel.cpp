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
    int dateMax = 5000;
    int coutMax = 1;
    int tabS[nbNoeud];
    int tabD[nbNoeud];
    int nbLigne; 60000;
    int tailleS = 0,  tailleD = 0;
    int cc[2000];

    char filename[] = "OCnodeslinks_clean_hours";
    char fileName[200];
    
    char statName[200];
    char stat[200];
    char text[200];
    FILE *statFile = NULL, *moyFile = NULL;
    clock_t debut, fin;
    float temps;
    float tempsMoy = 0;
    
    sprintf(statName, "stat_%s.moy", filename);
    sprintf(stat, "stat_%s.result", filename);

    sprintf(text , "Src\tdst\tTemps\tPath");
    writeFile(stat, text, 0, "w");
    sprintf(text , "src\tTemps_moy");
    writeFile(statName, text, 0, "w");


    sprintf(fileName, "./dataset_reel/%s.src", filename);
    FILE* fichier = NULL;
    fichier = fopen(fileName, "r");
    if(fichier == NULL){
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
    }else{
        int id = 0;
        while(!feof(fichier)){
            fscanf(fichier, "%d\n", &id);
            tabS[tailleS] = id;
            tailleS++;
        }
        fclose(fichier);
    }

    sprintf(fileName, "./dataset_reel/%s.dst", filename);
    fichier = fopen(fileName, "r");
    if(fichier == NULL){
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
    }else{
        int id = 0;
        while(!feof(fichier)){
            fscanf(fichier, "%d\n", &id);
            tabD[tailleD] = id;
            tailleD++;
        }
        fclose(fichier);
    }

    aretes *a;
    sprintf(fileName, "dataset_reel/%s_copy.txt", filename);
    
    a = fileToTables(fileName, nbLigne);


    printf("Taille d est %d\n", tailleD);
    for(int i = 0; i < a->taille; i++){
        printf("%d %d %d %d\n", a->dates[i], a->srcs[i], a->dsts[i], a->couts[i]);
    }
    Graphe_1* g1 = NULL;
    g1 = tableToGraphe(a->dates, a->srcs, a->dsts, a->couts, a->taille, nbNoeud, dateMax);
    for(int i = 0; i < 1; i++){   
        tempsMoy = 0; 
        int nombre = 0;
        
        for(int j = 0; j < tailleD; j++){
            if(tabS[i] == tabD[j]){
                continue;
            }
            
            debut = clock();
            Path chemin = path(g1, 1339, tabD[j]);
            fin = clock();
            
            //printf("Calculer %d\n", chemin.dateArrive);
            //printf("Le chemin est : %d\n", chemin.dateArrive);
            //temps = (fin.tv_usec/1000)-(debut.tv_usec/1000);
            temps = (float)(fin-debut)/ CLOCKS_PER_SEC;
            tempsMoy += temps; 
            nombre++;
            printf("%d %d %d\n\n", tabS[i], tabD[j], chemin.dateArrive);
            printf("Le temps d'execution est %f\n", temps);
            if(chemin.dateArrive != -1){
                //printf("CHEMIN %d\n", chemin.dateArrive);
                cc[i] += chemin.dateArrive;
                //sprintf(text , "%d\t%d\t%f\t%d", tabS[i], tabD[j], temps, chemin.dateArrive);
                //writeFile(stat, text, -1, "a");
            }
            

        }
        //tempsMoy /= nombre;
        //sprintf(text , "%d\t%f", nbNoeud, tempsMoy);
        //writeFile(statName, text, -1, "a");
        printf("%d %d %f\n",tabS[i], cc[i], tempsMoy);
        
    }
    /*
    sprintf(fileName, "cc_%s.test", filename);
    statFile = fopen(fileName, "w");
    for(int i = 0; i < tailleS; i++){
        fprintf(statFile, "%d %d\n", tabS[i], cc[i]);
    }
    */
    //fclose(statFile);
    return 0;
}