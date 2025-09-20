#include<stdio.h>
#include<stdlib.h>



int main(int argc, char const *argv[])
{
    FILE *fichier = NULL;
    char fileName[] = "cc_network_temporal_day_1.test";
    int id, nbE, nbS;
    double cc;
    double maxCc = -1;
    int maxCcId; 
    int maxE = -1, maxEId; 
    int maxS = -1, maxSId;   
    char authers[50]; 

    fichier = fopen(fileName, "r");

    if(fichier != NULL){
        while(!feof(fichier)){
            fscanf(fichier, "%d %d %d %lf\n", &id, &nbE, &nbS, &cc);
            printf("%d %d %d %lf\n", id, nbE, nbS, cc);
            
            if(maxCc == -1 || maxCc < cc){
                maxCc = cc;
                maxCcId = id;
            }

            if(maxE == -1 || maxE < nbE){
                maxE = nbE;
                maxEId = id;
            }

            if(maxS == -1 || maxS < nbS){
                maxS = nbS;
                maxSId = id;
            }

        }

        fclose(fichier);
        printf("Le max cc est %lf, l'id du noeud est %d\n", maxCc, maxCcId);
        printf("Le max entrant est %d, l'id du noeud est %d\n", maxE, maxEId);
        printf("Le max sortant est %d, l'id du noeud est %d\n", maxS, maxSId);
    }else{
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
    }
    
    return 0;
}
