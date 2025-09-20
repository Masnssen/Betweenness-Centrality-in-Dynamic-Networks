#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char const *argv[])
{
    FILE *fichier = NULL, *fichierOut = NULL;
    char fileName[] = "./rollernet.data";
    char fileOut[] = "./rollernet_1.txt";
    char fileNameId[] = "./rollernet.id";

    int idMax = 100;
    int tabS[idMax];
    int tabD[idMax];
    int tabId[idMax];

    int tailleSrc = 0;
    int tailleDst = 0;
    int src, dst, dateD, dateF, cout, autre1, autre2, autre3, autre4, taux = 0; 
    int idSrc, idDst, tailleId = 0;
    
    int debut = 1, fin = 2; //L'intervale de connection. 
    char dateDStr[20];
    char dateFStr[20];
    int testSrc, testDest; 

    fichier = fopen(fileName, "r");
    fichierOut = fopen(fileOut, "w");
    char ignore[200];

    tailleId = 0;
    if(fichier != NULL){

        while(!feof(fichier)){
            fscanf(fichier, "%d %d %d\n", &src, &dst, &dateD);
            
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
            cout = debut; 
            while (cout <= fin){
                if(idSrc != 1 && idDst != 1){
                    fprintf(fichierOut, "%d %d %d %d\n", dateD, idSrc, idDst, cout);
                    fprintf(fichierOut, "%d %d %d %d\n", dateD, idDst, idSrc, cout);
                    cout++; 
                }
                
            }
            if(dateD+cout-1 > taux){
                taux = dateD+cout-1;
            }
        }
        
        printf("Le taux est %d\n", taux);
        fclose(fichier);
        fclose(fichierOut);
    }else{
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
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
