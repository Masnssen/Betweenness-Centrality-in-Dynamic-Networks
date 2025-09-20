#include "fonction_cc.h"
#include <time.h>
#include <math.h>


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

    int nbNoeud = 62;
    int dateMax = 9981;
    
    int tabS[nbNoeud];
    int tabD[nbNoeud];

    int tailleS = 0,  tailleD = 0;


    char filename[] = "rollernet";
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


    sprintf(fileName, "./%s.id", filename);
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
     
    sprintf(fileName, "./%s.id", filename);
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
    sprintf(fileName, "./%s.txt", filename);
    printf("Hello how are you\n");
    a = fileToTables(fileName);
    
    Graphe_1* g1 = NULL;
    g1 = tableToGraphe(a->dates, a->srcs, a->dsts, a->couts, a->taille, nbNoeud, dateMax);
    printf("Arriver ici\n");

    double tempsTable[nbNoeud];
    double cc[nbNoeud];

    for(int i = 0; i < tailleS; i++){
        int chemins[nbNoeud];
        
        for(int a = 0; a < nbNoeud; a++){
            chemins[a] = -1; 
        }
        
        tempsMoy = 0; 
        //printf("Start calcule\n");
        debut = clock();
        Path chemin = paths_cc(g1, tabS[i], chemins);
        fin = clock();
        
        temps = (float)(fin-debut)/ CLOCKS_PER_SEC;
        tempsTable[tabS[i]] = temps;

        //printf("Le temps est %lf\n", temps);

        cc[tabS[i]] = 0.0;

        for(int j = 0; j < nbNoeud; j++){
            if(chemins[j] != -1 && chemins[j] != 0){
                cc[tabS[i]] += (1.0/(double)chemins[j]);
                if(isinf(cc[tabS[i]])){
                    printf("Hello %d", chemins[j]);
                    exit(0);
                }
                //printf("%d %d %d\n", 0, j, chemins[j]);
            }
        }

        /*
        for(int b = 0; b < 2000; b++){            
            
            if(chemins[b] != -1){
                cc[tabS[i]] += (1.0/(double)chemins[b]); 
                if(b == 1){
                    printf("Hello %lf\n", cc[tabS[0]]);
                }
            }

        }
        */
        printf("%d %lf %lf\n",i, cc[tabS[i]], tempsTable[tabS[i]]);

    }
    
            
            //printf("Calculer %d\n", chemin.dateArrive);
            //printf("Le chemin est : %d\n", chemin.dateArrive);
            //temps = (fin.tv_usec/1000)-(debut.tv_usec/1000);
            
       
        //printf("%d %d %d\n\n", tabS[i], tabD[j], coumpte);
        //printf("Le temps d'execution est %f\n", temps);
    

    int degrerS[nbNoeud]; 
    int degrerE[nbNoeud];

    for(int i = 0; i < nbNoeud; i++){
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
        fprintf(statFile, "%d %d %d %lf\n", tabS[i], degrerS[tabS[i]], degrerS[tabS[i]], (1.0/11949.0)*100*cc[tabS[i]]);        
    }
    fclose(fichier);
    //printf("%lf\n", cc[tabS[0]]);
    //fclose(statFile);
    
    return 0;
}