#include "fonction_bb.h"
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

void statTime(char *fileName, int etat){ //Si etat = 0 alors initialisation, si etat = 1 alors insertion.
    char statName[200], stat[200], text[200];
    
    if(etat == 0){
        sprintf(statName, "stat_%s.moy", fileName);
        sprintf(stat, "stat_%s.result", fileName);

        sprintf(text , "Src\tdst\tPath");

        writeFile(stat, text, 0, "w");
        sprintf(text , "src\tcc\tTemps_moy");
        writeFile(statName, text, 0, "w");
    }else{

    }

}


int main(){

    int nbNoeud = 65;
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
    
    
    statTime(filename, 0);

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
    printf("%d\n", a->taille);
    g1 = tableToGraphe(a->dates, a->srcs, a->dsts, a->couts, a->taille, nbNoeud, dateMax);
    printf("Arriver ici\n");
   
    double tempsTable[nbNoeud];
    double cc[nbNoeud];
    double bc[nbNoeud];
    int nb = 0; 
    //FILE *fichier_bc = NULL;
    //fichier_bc = fopen("s_t_chemins.paris", "w");
    int chemins[nbNoeud];
    for(int i = 0; i < tailleS; i++){
        
        
        for(int a = 0; a < nbNoeud; a++){
            chemins[a] = -1; 
        }
        
        
        ListChemin *pathsChemin[nbNoeud];

        for(int i = 0; i < nbNoeud; i++){
            pathsChemin[i] = NULL;
        }
        tempsMoy = 0; 
        //printf("Start calcule\n");
        debut = clock();
        Path chemin = paths_bb(g1, tabS[i], chemins, pathsChemin);
        for(int a = 0; a < nbNoeud; a++){
            if(pathsChemin[a] != NULL){
                //printf("%d vers le noeud %d le plus cours est %d\n",tabS[15], a, chemins[a]);
                //fprintf(fichier_bc, "%d %d ", tabS[i], a); 
                nb = fromBitToId(pathsChemin[a], nbNoeud, tabS[i], a, bc);
            }
        }
        
       fin = clock();
    
        
        
        temps = (float)(fin-debut)/ CLOCKS_PER_SEC;
        tempsTable[tabS[i]] = temps;

        printf("Le temps est %f %d\n",temps, tabS[i]);
        //printf("Le temps est %lf\n", temps);
    }

    //fclose(fichier_bc);
            
            //printf("Calculer %d\n", chemin.dateArrive);
            //printf("Le chemin est : %d\n", chemin.dateArrive);
            //temps = (fin.tv_usec/1000)-(debut.tv_usec/1000);
            
       
        //printf("%d %d %d\n\n", tabS[i], tabD[j], coumpte);
        //printf("Le temps d'execution est %f\n", temps);
    printf("Fin\n");

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
    
    sprintf(fileName, "bc_%s_1.test", filename);
    statFile = fopen(fileName, "w");
    
    for(int i = 0; i < tailleS; i++){
        fprintf(statFile, "%d %d %d %lf %lf\n", i, degrerS[i], degrerS[i], bc[i], tempsTable[i]);        
    }
    fclose(fichier);
    /*
    //printf("%lf\n", cc[tabS[0]]);
    //fclose(statFile);
    */
    return 0;
}



/*
 cc[tabS[i]] = 0.0;
        
        char statName[200];
        sprintf(statName, "%s_result.stat", filename);
        fichier = fopen(statName, "a");
        for(int j = 0; j < nbNoeud; j++){
            if(chemins[j] != -1 && chemins[j] != 0){
                
                fprintf(fichier, "%d %d %d\n", tabS[i], j, chemins[j]);

                cc[tabS[i]] += (1.0/(double)chemins[j]);
                if(isinf(cc[tabS[i]])){
                    printf("Hello %d", chemins[j]);
                    exit(0);
                }
                //printf("%d %d %d\n", 0, j, chemins[j]);
            }
        }
        fclose(fichier);
        /*
        for(int b = 0; b < 2000; b++){            
            
            if(chemins[b] != -1){
                cc[tabS[i]] += (1.0/(double)chemins[b]); 
                if(b == 1){
                    printf("Hello %lf\n", cc[tabS[0]]);
                }
            }

        }
        
        printf("%d %lf %lf\n",i, cc[tabS[i]], tempsTable[tabS[i]]);

*/