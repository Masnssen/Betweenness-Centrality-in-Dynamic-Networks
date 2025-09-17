#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>


typedef struct NoeudTemp NoeudTemp;
typedef struct ListMin ListMin;

typedef struct Graphe_1 Graphe_1;
typedef struct FilsNoeud FilsNoeud;

typedef struct ListChemin ListChemin;

typedef struct Path Path;
typedef struct aretes aretes;

struct Path{
    int dateArrive = 0;
    ListChemin* listchemin;
};


struct ListChemin{
    unsigned char *listChem = NULL;
    ListChemin* suivant = NULL;
};



struct FilsNoeud{
    NoeudTemp* noeud = NULL;
    FilsNoeud* suivant = NULL;
};

struct NoeudTemp{
    int id = -1; 
    int date = -1;
    FilsNoeud* first = NULL;
    FilsNoeud* last = NULL;
    ListChemin* listChemins = NULL;
};

struct Graphe_1{
    NoeudTemp** noeuds = NULL; //Un tableau des pointeurs vers les listes des noeuds temporel pour chaque noeuds. 
    int nbNoued = -1; //Le nombre de noeuds.
    int dateMax = -1; //Le taux 
    int nbNoeudTemps = 0; //Le nombre de noeud temporels. 
};

struct ListMin{
    NoeudTemp* noeud = NULL;
    ListMin* next_noeud = NULL;
};



struct aretes
{
    int *dates = NULL;
    int *srcs = NULL;
    int *dsts = NULL;
    int *couts = NULL; 
    int taille = 0; 
};


Graphe_1 *fileToGraphe_1(char *fileName, int nbNoued, int dateMax){
    Graphe_1 *g = NULL;
    FILE *fichier = NULL;
    int date = -1, src = -1, dst = -1, cout = -1; 
    NoeudTemp *noeud = NULL;
    NoeudTemp *noeudDest = NULL;
    FilsNoeud* fils = NULL;
    fichier = fopen(fileName, "r");
    
    if(fichier != NULL){
        g = (Graphe_1*)malloc(sizeof(Graphe_1));
        g->noeuds = (NoeudTemp**)malloc(sizeof(NoeudTemp*)*(nbNoued*dateMax));
        
        for(int i = 0; i < nbNoued*dateMax; i++){
            *((g->noeuds)+i) = NULL;
        }
    
        g->nbNoued = nbNoued;   
        g->dateMax = dateMax;
        
        while(!feof(fichier)){

            fscanf(fichier, "%d %d %d %d\n",&date, &src, &dst, &cout);
            //printf("%d %d %d %d\n", date, src, dst, cout);
            noeud = (*((g->noeuds)+src+(date*nbNoued)));
            noeudDest = (*((g->noeuds)+dst+((date+cout)*nbNoued)));

            if( noeud == NULL ){//Le noeud temporel n'est pas déjà étais crée.
                
                noeud = (NoeudTemp*)malloc(sizeof(NoeudTemp));
                noeud->id = src; 
                noeud->date = date;
                noeud->first = NULL;
                noeud->last = NULL;
                noeud->listChemins = NULL;
                g->nbNoeudTemps++;
                (*((g->noeuds)+src+(date*nbNoued))) = noeud;

                
            }

             if( noeudDest == NULL ){//Le noeud temporel n'est pas déjà étais crée.
                
                noeudDest = (NoeudTemp*)malloc(sizeof(NoeudTemp));
                
                noeudDest->id = dst; 
                noeudDest->date = date+cout;
                noeudDest->first = NULL;
                noeudDest->last = NULL;
                noeudDest->listChemins = NULL;
                g->nbNoeudTemps++;
                (*((g->noeuds)+dst+((date+cout)*nbNoued))) = noeudDest;
            }

            fils = (FilsNoeud*)malloc(sizeof(FilsNoeud));
            fils->noeud = noeudDest; 
            fils->suivant = NULL;

            if(noeud->first == NULL){
                noeud->first = fils;
                noeud->last = fils;
            }else{
                noeud->last->suivant = fils;
                noeud->last = fils;
            }
    
        }
        fclose(fichier);
        return g;
    }else{
        printf("Erreur lors de l'ouverture du fichier");
        return NULL;
    }
}


void printGraphe_1(Graphe_1 g){
    NoeudTemp* noeud; 
    FilsNoeud* fils = NULL;
    int j = 0;

    for(int i = 0; i < g.nbNoued*g.dateMax; i++){
        noeud = g.noeuds[i];

        if(noeud != NULL){
            fils = noeud->first;
        }
        
        while(fils != NULL){
        
            printf("hello %d %d %d %d\n", noeud->date, noeud->id, fils->noeud->id, fils->noeud->date-noeud->date);
            j++;

            fils = fils->suivant;
        }
    }

    printf("%d\n", j);
}



void freeGraphe(Graphe_1 *g){

    NoeudTemp* noeud = NULL;
    FilsNoeud* fils = NULL;
    FilsNoeud* par = NULL;
    ListChemin *chemin = NULL;

    for(int i = 0; i < g->nbNoued*g->dateMax; i++){

        noeud = *(g->noeuds+i);

        if(noeud != NULL){
            
            fils = noeud->first; 
            par = noeud->first;
            while(par != NULL){
                fils = fils->suivant;
                free(par);
                par = fils;
            }
    
            while(noeud->listChemins != NULL){
                chemin = noeud->listChemins;
                noeud->listChemins = noeud->listChemins->suivant;
                free(chemin->listChem);
                free(chemin);
            }
            free(noeud);
        }
    }

    free(g->noeuds);
    free(g);
}

ListMin* insertListMin(ListMin* first, NoeudTemp* noeud, int gool){

    ListMin* last = NULL, *par = NULL;
    ListMin* newNoued;

    newNoued = (ListMin*)malloc(sizeof(ListMin));
    newNoued->noeud = noeud;
   
    par = first;
    while(par != NULL){

        if((par->noeud->date) == (noeud->date)){
            if(par->noeud->id == noeud->id){
                return first;
            }
            if(noeud->id == gool)
                break;
        }
        if(par->noeud->date > (noeud->date)){
            break;
        }

        last = par;
        par = par->next_noeud;
    }
    
        
    if(first == NULL){  
        newNoued->next_noeud = NULL;
        first = newNoued;
        return first; 
    }else{
        newNoued->next_noeud = par;
    }

    if(last != NULL)
        last->next_noeud = newNoued;
    else{
        return newNoued;
    }
    
    return first;
}


void posBit(unsigned char* var, int pos){
    int index = int(pos/8);
    int offset = pos%8;

    var[index] |= (1 << offset);
}




void printOpenList(ListMin* openList, int taille){
    ListMin *par;
    par = openList;
    
    while(par != NULL){
        
        printf("La date : %d, l'id: %d \n",par->noeud->date, par->noeud->id);
        printf("Le chemin est : ");
        for(int i = 0; i < taille; i++){
            printf("%02X ", *((par->noeud->listChemins->listChem)+i));
        }
        printf("\n\n");
        par = par->next_noeud;
    }
}




aretes *fileToTables(char *fileName){

    aretes *a = NULL;
    FILE *fichier = NULL;
    int date = -1, src = -1, dst = -1, cout = -1; 

    fichier = fopen(fileName, "r");
    a = (aretes*)malloc(sizeof(aretes));

    (a->dates) = (int*)malloc((sizeof(int)*2000000));
    (a->srcs) = (int*)malloc((sizeof(int)*2000000));
    (a->dsts) = (int*)malloc((sizeof(int)*2000000));
    (a->couts) = (int*)malloc((sizeof(int)*2000000));
    (a->taille) = 0;
    if(fichier != NULL){

        while(fscanf(fichier, "%d %d %d %d\n",&date, &src, &dst, &cout) == 4){
            //printf("%d %d %d %d\n", date, src, dst, cout);
            
           *((*a).dates + (*a).taille) = date;
          
            (a->srcs)[(a->taille)] = src;
            (a->dsts)[(a->taille)] = dst;
            (a->couts)[(a->taille)] = cout;
            //printf("%d \n", a->taille);
            (a->taille) += 1;
            
        }

        fclose(fichier);
        return a;
    }else{
        printf("Erreur lors de l'ouverture du fichier %s\n", fileName);
        return a;
    }
}



Graphe_1 *tableToGraphe(int *dates, int *srcs, int *dsts, int *couts, int taille, int nbNoued, int dateMax){
    Graphe_1 *g = NULL;
    int date = -1, src = -1, dst = -1, cout = -1; 
    NoeudTemp *noeud = NULL;
    NoeudTemp *noeudDest = NULL;
    FilsNoeud* fils = NULL;
    
    
    g = (Graphe_1*)malloc(sizeof(Graphe_1));
    g->noeuds = (NoeudTemp**)malloc(sizeof(NoeudTemp*)*(nbNoued*dateMax));
        
    for(int i = 0; i < nbNoued*dateMax; i++){
            *((g->noeuds)+i) = NULL;
    }
    
    g->nbNoued = nbNoued;   
    g->dateMax = dateMax;
        
    for(int index = 0; index < taille; index++){
        
        date = dates[index];
        src = srcs[index];
        dst = dsts[index];
        cout = couts[index];

        //printf("%d %d %d %d\n", date, src, dst, cout);

        noeud = (*((g->noeuds)+src+(date*nbNoued)));
        noeudDest = (*((g->noeuds)+dst+((date+cout)*nbNoued)));

        if( noeud == NULL ){//Le noeud temporel n'est pas déjà étais crée.    
            noeud = (NoeudTemp*)malloc(sizeof(NoeudTemp));
            noeud->id = src; 
            noeud->date = date;
            noeud->first = NULL;
            noeud->last = NULL;
            noeud->listChemins = NULL;
            g->nbNoeudTemps++;
            (*((g->noeuds)+src+(date*nbNoued))) = noeud;
        }

        if( noeudDest == NULL ){//Le noeud temporel n'est pas déjà étais crée.
                
            noeudDest = (NoeudTemp*)malloc(sizeof(NoeudTemp));
                
            noeudDest->id = dst; 
            noeudDest->date = date+cout;
            noeudDest->first = NULL;
            noeudDest->last = NULL;
            noeudDest->listChemins = NULL;
            g->nbNoeudTemps++;
            (*((g->noeuds)+dst+((date+cout)*nbNoued))) = noeudDest;
        }

        fils = (FilsNoeud*)malloc(sizeof(FilsNoeud));
        fils->noeud = noeudDest; 
        fils->suivant = NULL;

        if(noeud->first == NULL){
            noeud->first = fils;
            noeud->last = fils;
        }else{
            noeud->last->suivant = fils;
            noeud->last = fils;
        }
    }
    
    return g;
}


