#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char const *argv[])
{
    FILE *fichier = NULL, *fichierOut = NULL;
    char fileName[] = "./data/adelaide/network_temporal_day.csv";
    char fileOut[] = "./data/adelaide/network_temporal_day.txt";
    char fileNameOut[] = "./data/adelaide/network_temporal_day.src";
    char fileNameDst[] = "./data/adelaide/network_temporal_day.dst";
    char fileNameId[] = "./data/adelaide/network_temporal_day.id";

    int tabS[20000];
    int tabD[20000];
    int tabId[30000];

    int tailleSrc = 0;
    int tailleDst = 0;
    int src, dst, dateD, dateF, cout, autre1, autre2, autre3, autre4, taux = 0; 
    int idSrc, idDst, tailleId = 0;
    time_t debut, fin;
    char dateDStr[20];
    char dateFStr[20];

    struct tm *date_info;

    int testSrc, testDest; 
    fichier = fopen(fileName, "r");
    fichierOut = fopen(fileOut, "w");
    char ignore[200];

    tailleId = 0;
    if(fichier != NULL){

        while(!feof(fichier)){
            fscanf(fichier, "%d;%d;%d;%d;%s\n", &src, &dst, &dateD, &dateF, ignore);
            //printf("%s\n", ignore);
            debut = dateD;
            fin = dateF;
            
            date_info = localtime(&debut);
            dateD = (date_info->tm_hour * 60) + date_info->tm_min;
            
            date_info = localtime(&fin);
            dateF = (date_info->tm_hour * 60) + date_info->tm_min;

            if(dateD > dateF){
                cout = (1440 - dateD) +dateF;
                
            }else{
                cout = dateF - dateD; 
            }

            if(cout >= 0){
                int j = 0;
                testSrc = 0;
                testDest = 0;

                while (j < tailleId && (testSrc == 0 || testDest == 0)){
                    
                    if(tabId[j] == src){
                        testSrc = 1;
                        idSrc = j;
                    }
                    if(tabId[j] == dst){
                        testDest = 1; 
                        idDst = j;
                    }
                    j++;
                }

                if(testSrc == 0){
                    tabId[tailleId] = src;
                    idSrc = tailleId;
                    tailleId++;
                }
                if(testDest == 0){
                    tabId[tailleId] = dst;
                    idDst = tailleId;
                    tailleId++; 
                }

                fprintf(fichierOut, "%d %d %d %d\n", dateD, idSrc, idDst, cout);
            
                if(dateD+cout > taux){
                    taux = dateD+cout;
                }

                testSrc = 0;
                testDest = 0;

                for(int i = 0; i < tailleSrc; i++){
                    if(tabS[i] == idSrc){
                        testSrc = 1;
                    }
                }

                for(int i = 0; i < tailleDst; i++){
                    if(tabD[i] == idDst){
                        testDest = 1;
                    }
                }

                if(testSrc == 0){
                    tabS[tailleSrc] = idSrc;
                    tailleSrc++;
                }
                if(testDest == 0){
                    tabD[tailleDst] = idDst;
                    tailleDst++;
                }
            }
    
        }

        fclose(fichier);
        fclose(fichierOut);
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
        fprintf(fichier, "Le taux est %d\n", taux);
        for(int i = 0; i < tailleDst; i++){
            fprintf(fichier, "%d\n", tabD[i]);
        }

        fclose(fichier);
    }else{
        printf("Erreur lors de l'ouverture du fichier \n");
    }

    fichier = fopen(fileNameId, "w");
    if(fichier != NULL){
        
        for(int i = 0; i < tailleId; i++){
            fprintf(fichier, "%d\n", tabId[i]);
        }

        fclose(fichier);
    }else{
        printf("Erreur lors de l'ouverture du fichier \n");
    }
    return 0;
}
