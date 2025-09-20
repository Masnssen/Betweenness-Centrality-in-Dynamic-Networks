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

int saveChemin(unsigned char* chemin0, unsigned char* chemin1, int del, int taille){
   
    unsigned char result[taille];
   
    int egale0 = 1, egale1 =1;
    //printf("dans saveChemin la taille est %d\n", taille);
    for(int i = 0; i < taille; i++){
        *(result+i) = (*(chemin0+i) & (*(chemin1+i)));

        if(*(result+i) != (*(chemin0+i))){
            egale0 = 0;
        }

        if(*(result+i) != (*(chemin1+i))){
            egale1 = 0;
        }
        
    }
    //printf("Exit ici %d\n",taille);
   
    if(egale0 == 1){
        //printf("Egale0 \n");
        return 2; //Le nouveau est plus grand que le chemin 0. ne pas tester les autres chemins
    }

    if(egale1 == 1){ //A & B  = A. 
        
        if(del){ //A déjà ajouter 
            return -1; //Il faut supprimer le chemin0. 
        }

        for(int i = 0; i < taille; i++){
            *(chemin0+i) = *(chemin1+i); //Sauvegarder A a la place de B. 
        }
        //printf("%02X\n",*result);
        return 0; //Le nouveau chemin est ajouter
    }

    return 1; //Le nouveau chemin n'est pas ajouter 
}   


ListChemin *gestionChemin(ListChemin *chemins, unsigned char* chemin, int id, int taille){
    int pos = (int)(id/8);
    int offset = id%8;
   
    ListChemin *par; 
    ListChemin *avant = NULL;
    unsigned char* newChem = NULL;
    par = chemins;
    
    if((*(chemin+pos) & (1<<offset)) == 0){
        
        *(chemin+pos) |= (1<<offset);
        int fin = 1; 
        int add = 1;
        int del = 0;

        while(par != NULL){
            //printf("hello gestion chemin\n");
            fin = saveChemin(par->listChem, chemin, del, taille);
            //printf("hello gestion chemin 1\n");
            if(fin != 1){
                add = 0;

                if(fin == -1){
                    if(avant == NULL){
                        avant = par->suivant;
                        free(par->listChem);
                        free(par);
                        par = avant;
                        avant = NULL;
                    }else{
                        avant->suivant = par->suivant;
                        free(par->listChem);
                        free(par);
                        par = avant->suivant;
                    }
                    continue;
                }else{
                    if(fin == 2){
                        *(chemin+pos) &= ~(1 << offset);
                        return chemins;
                    }
                    del = 1;
                }
            }
            
            avant = par;
            par = par->suivant;  
            
            
        }
        
        if(add){
            par = (ListChemin*)malloc(sizeof(ListChemin)); 
            par->listChem = (unsigned char*)malloc(sizeof(unsigned char)*taille);
            for(int i = 0; i < taille; i++){
                *((par->listChem)+i) = *(chemin+i);
            }
            if(chemins == NULL){
                par->suivant = NULL;
            }else{
                par->suivant = chemins;
            }
            
            *(chemin+pos) &= (~(1 << offset));

            return par;
        }else{
            *(chemin+pos) &= (~(1 << offset));
            return chemins;
        }
    }
    
    return par; 
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


Path path(Graphe_1 *g, int src, int dst){


    ListMin *openList = NULL, *sauv = NULL;
    NoeudTemp* noeud;
    ListChemin* chemin;
    FilsNoeud* fils;
    Path path; 
    path.dateArrive = -1;
    int nbOctet = (g->nbNoued/8)+1; 
    //printf("Le nombre d'octets est %d\n", nbOctet);
    for(int i = 0; i < g->dateMax; i++){ 
        noeud = *((g->noeuds)+src+(i*g->nbNoued));
        
        if(noeud != NULL){
            
            chemin =(ListChemin*)malloc(sizeof(ListChemin));
            
            chemin->listChem = (unsigned char*)malloc(sizeof(unsigned char)*nbOctet);
            for(int i = 0; i < nbOctet; i++){
                chemin->listChem[i] = 0;
            }
            
            posBit(chemin->listChem, noeud->id);
            
            chemin->suivant = NULL;

            noeud->listChemins = chemin;
            
            openList = insertListMin(openList, noeud, dst); 
           
        }
    }
   
   
    while(openList != NULL){
        //printOpenList(openList, nbOctet);
        noeud = openList->noeud; //Le noeud actuel, en exploration.
        sauv = openList; //Pour supprimer la structure allouer. 
        openList = openList->next_noeud;//Le premier element pointe vers le prochain dans la liste chainer. 
        free(sauv);//Liberer la mémoire. 
        
        //printf("%d\n", noeud->date);
        if(noeud->id == dst){//Si le fils est la destination alors retourner la date minimume.
            
            while(openList != NULL){
                sauv = openList;
                openList = openList->next_noeud;
                free(sauv);
            }

            path.dateArrive = noeud->date;
            path.listchemin = noeud->listChemins;

            return path;
        }


        fils = noeud->first;//Le premier fils du noeud temporel actuel.

        while(fils != NULL){
            
            chemin = noeud->listChemins; //La liste des chemins du vers le départ connue par le noeud actuel. 
            while(chemin != NULL){
                
                fils->noeud->listChemins = gestionChemin(fils->noeud->listChemins, chemin->listChem, fils->noeud->id, nbOctet);
                chemin = chemin->suivant;
               
            }
            
            if(fils->noeud->listChemins != NULL){
                //printf("hello\n");
                openList = insertListMin(openList, fils->noeud, dst);
                 //printf("hello1\n");
            }
            
            //noeud->first = fils->suivant;
            //free(fils);
            fils = fils->suivant;
        }
        
        chemin = noeud->listChemins;
        while(chemin != NULL){
            //free(chemin->listChem);
            noeud->listChemins = noeud->listChemins->suivant;
            free(chemin->listChem);
            free(chemin);
            chemin = noeud->listChemins;
        }
        
    } 
    
    while(openList != NULL){
        sauv = openList;
        openList = openList->next_noeud;
        free(sauv);
    }

    return path;
}


typedef struct aretes aretes;

struct aretes
{
    int *dates = NULL;
    int *srcs = NULL;
    int *dsts = NULL;
    int *couts = NULL; 
    int taille = 0; 
};



aretes *fileToTables(char *fileName){

    aretes *a = NULL;
    FILE *fichier = NULL;
    int date = -1, src = -1, dst = -1, cout = -1; 

    fichier = fopen(fileName, "r");
    a = (aretes*)malloc(sizeof(aretes));

    (a->dates) = (int*)malloc((sizeof(int)*5000000));
    (a->srcs) = (int*)malloc((sizeof(int)*5000000));
    (a->dsts) = (int*)malloc((sizeof(int)*5000000));
    (a->couts) = (int*)malloc((sizeof(int)*5000000));
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



Path paths_cc(Graphe_1 *g, int src, int* paths){


    ListMin *openList = NULL, *sauv = NULL;
    NoeudTemp* noeud;
    ListChemin* chemin;
    FilsNoeud* fils;
    Path path; 
    int nbNoeud = g->nbNoued;
    printf("Le nombre de noeuds est %d\n", nbNoeud);
    int nbPaths = 0;
    path.dateArrive = -1;
    int nbOctet = (g->nbNoued/8)+1; 
    //printf("Le nombre d'octets est %d %d\n", nbOctet, g->nbNoued);
    for(int i = 0; i < g->dateMax; i++){ 
        
        noeud = *((g->noeuds)+src+(i*g->nbNoued));
        
        if(noeud != NULL){
            
            chemin =(ListChemin*)malloc(sizeof(ListChemin));
            
            chemin->listChem = (unsigned char*)malloc(sizeof(unsigned char)*nbOctet);

            for(int j = 0; j < nbOctet; j++){
                chemin->listChem[j] = 0;
            }
            //printf("L'id est %d\n", noeud->id);
            posBit(chemin->listChem, noeud->id);
            
            chemin->suivant = NULL;

            noeud->listChemins = chemin;
           
            openList = insertListMin(openList, noeud, -1); 
            
        }
        
    }
   
     
    while(openList != NULL){
        //printOpenList(openList, nbOctet);
        noeud = openList->noeud; //Le noeud actuel, en exploration.
        sauv = openList; //Pour supprimer la structure allouer. 
        openList = openList->next_noeud;//Le premier element pointe vers le prochain dans la liste chainer. 
        free(sauv);//Liberer la mémoire. 
        
        //printf("%d %d\n", noeud->date, noeud->id);
        if(noeud->id != src && (paths[noeud->id] == -1 || noeud->date < paths[noeud->id])){
            printf("Le chemin de %d vers %d est %d %d\n",src, noeud->id, noeud->date, paths[noeud->id]);
            paths[noeud->id] = noeud->date;
            nbPaths++;
            if(nbPaths >= nbNoeud-1){
                break;
            }
        }

        fils = noeud->first;//Le premier fils du noeud temporel actuel.

        while(fils != NULL){
            
            chemin = noeud->listChemins; //La liste des chemins du vers le départ connue par le noeud actuel. 
            while(chemin != NULL){
                
                fils->noeud->listChemins = gestionChemin(fils->noeud->listChemins, chemin->listChem, fils->noeud->id, nbOctet);
                chemin = chemin->suivant;
               
            }
            
            if(fils->noeud->listChemins != NULL){
                //printf("hello\n");
                openList = insertListMin(openList, fils->noeud, -1);
                 //printf("hello1\n");
            }
            
            //noeud->first = fils->suivant;
            //free(fils);
            fils = fils->suivant;
        }
        paths[src] = -1;

        chemin = noeud->listChemins;
        while(chemin != NULL){
            //free(chemin->listChem);
            noeud->listChemins = noeud->listChemins->suivant;
            free(chemin->listChem);
            free(chemin);
            chemin = noeud->listChemins;
        }
        
    } 
    
    while(openList != NULL){
        sauv = openList;
        openList = openList->next_noeud;
        free(sauv);
    }
    //printf("hello\n");
    return path;
}


