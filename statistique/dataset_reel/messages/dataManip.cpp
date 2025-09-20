#include<stdio.h>
#include<stdlib.h>


int main(int argc, char const *argv[])
{
    FILE *fichier = NULL, *fichierOut = NULL;
    char fileName[] = "../dataset_reel/OCnodeslinks_clean_hours.txt";
    char fileNameOut[] = "../dataset_reel/OCnodeslinks_clean_hours.src";
    char fileNameDst[] = "../dataset_reel/OCnodeslinks_clean_hours.dst";

    int tabS[2000];
    int tabD[2000];

    int tailleSrc = 0;
    int tailleDst = 0;
    int src, dst, date, cout; 
    int testSrc, testDest; 
    fichier = fopen(fileName, "r");
    if(fichier != NULL){
        while(!feof(fichier)){
            
            fscanf(fichier, "%d %d %d %d\n", &date, &src, &dst, &cout);

            testSrc = 0;
            testDest = 0;

            for(int i = 0; i < tailleSrc; i++){
                if(tabS[i] == src){
                    testSrc = 1;
                }
               
            }
            for(int i = 0; i < tailleDst; i++){
                if(tabD[i] == dst){
                    testDest = 1;
                }
            }

            if(testSrc == 0){
                tabS[tailleSrc] = src;
                tailleSrc++;
            }
            if(testDest == 0){
                tabD[tailleDst] = dst;
                tailleDst++;
            }
        }

        fclose(fichier);
    }else{
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
    }

    fichier = fopen(fileNameOut, "w");
    if(fichier != NULL){
        for(int i = 0; i < tailleSrc; i++){
            fprintf(fichier, "%d\n", tabS[i]);
        }

        fclose(fichier);
    }else{
        printf("Erreur lors de l'ouverture du fichier \n");
    }

    
    fichier = fopen(fileNameDst, "w");
    if(fichier != NULL){
        for(int i = 0; i < tailleDst; i++){
            fprintf(fichier, "%d\n", tabD[i]);
        }

        fclose(fichier);
    }else{
        printf("Erreur lors de l'ouverture du fichier \n");
    }

    return 0;
}
