#include "fonction_cc.h"
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

    printf("Heloo how are you \n");

    int nbNoeud = 2000;
    int dateMax = 5000;
    int coutMax = 1;
    int tabS[nbNoeud];
    int tabD[nbNoeud];
    int nbLigne; 60000;
    int tailleS = 0,  tailleD = 0;


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


    float tempsTable[2000];
    double cc[2000]; 
    for(int i = 0; i < tailleS; i++){
        int chemins[2000];
    
        for(int a = 0; a < 2000; a++){
            chemins[a] = -1; 
        }
    
        tempsMoy = 0; 
        
        debut = clock();
        Path chemin = paths_cc(g1, tabS[i], chemins);
        fin = clock();
        
        temps = (float)(fin-debut)/ CLOCKS_PER_SEC;
        tempsTable[tabS[i]] = temps; 
       
        cc[tabS[i]] = 0.0;
        
        for(int b = 0; b < 2000; b++){            
            
            if(chemins[b] != -1){
                cc[tabS[i]] += (1.0/(double)chemins[b]); 
                if(b == 1){
                    printf("Hello %lf\n", cc[tabS[0]]);
                }
            }

        }
        //printf("%f\n", cc[tabS[i]]);

    }
    
            
            //printf("Calculer %d\n", chemin.dateArrive);
            //printf("Le chemin est : %d\n", chemin.dateArrive);
            //temps = (fin.tv_usec/1000)-(debut.tv_usec/1000);
            
       
        //printf("%d %d %d\n\n", tabS[i], tabD[j], coumpte);
        //printf("Le temps d'execution est %f\n", temps);
    
    int degrerS[2000]; 
    int degrerE[2000];
    for(int i = 0; i < 2000; i++){
        degrerS[i] = 0;
        degrerE[i] = 0;
    }
    printf("fin\n");
    fichier = fopen(fileName, "r"); 
    int date, ids, idd, cout; 

    while(fscanf(fichier, "%d %d %d %d\n", &date, &ids, &idd, &cout) == 4){
        degrerS[ids]++;
        degrerE[idd]++; 
    }
    fclose(fichier);
    sprintf(fileName, "cc_%s_1.test", filename);
    statFile = fopen(fileName, "w");
    
    for(int i = 0; i < tailleS; i++){
        fprintf(statFile, "%d %d %d %lf\n", tabS[i], degrerS[tabS[i]], degrerS[tabS[i]], (1.0/1899)*100*cc[tabS[i]]);
        
    }
    fclose(fichier);
    printf("%lf\n", cc[tabS[0]]);
    //fclose(statFile);
    return 0;
}