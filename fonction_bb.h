#include"fonction.h"


typedef struct PathList PathList;


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
   
    if(egale0 == 1 && egale1 == 1){
        //printf("Egale0 et egale 1\n");
        return 2; //Les deux chemins sont egaux. Ne pas sauvegarder
    }

    /*
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
    */
    return 1; //Le nouveau chemin n'est pas ajouter 
}   


ListChemin *gestionChemin(ListChemin *chemins, unsigned char* chemin, int id, int taille){
    int pos = (int)(id/8);
    int offset = id%8;
    //printf("exit\n");
    ListChemin *par = NULL; 
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
            if(fin == 2){
                *(chemin+pos) &= ~(1 << offset);
                return chemins;
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
        }
    }
    
    return par; 
}

ListChemin* copyChemins(ListChemin *chemin, int nbOctet){
    ListChemin* newChem, *par, *chem; 
   // printf("Exit\n");
    chem = NULL;
    par = chemin;
    while(par !=NULL){
        
        newChem = (ListChemin *)malloc(sizeof(ListChemin));
        newChem->listChem = (unsigned char *)malloc(sizeof(unsigned char)*nbOctet);
        for(int i = 0; i < nbOctet; i++){
            newChem->listChem[i] = par->listChem[i];
        }
        newChem->suivant = chem;
        chem = newChem; 

        par = par->suivant;
    }

    return chem;
}

Path paths_bb(Graphe_1 *g, int src, int paths[], ListChemin *chemins[]){

    ListMin *openList = NULL, *sauv = NULL;
    NoeudTemp* noeud;
    ListChemin* chemin;
    FilsNoeud* fils;
    Path path; 
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

        //La date d'arriver minimume au noeud, qui a comme id noeud->id.
        //printf("Path = %d\n", paths[noeud->id]);
        //printf("Heeee\n");
        if(paths[noeud->id] == -1 || noeud->date < paths[noeud->id] && noeud->id != src){
            
           paths[noeud->id] = noeud->date;
           
           chemins[noeud->id] = copyChemins(noeud->listChemins, nbOctet); 
        }
        
        fils = noeud->first;//Le premier fils du noeud temporel actuel.

        while(fils != NULL && fils->noeud != NULL){
            
            chemin = noeud->listChemins; //La liste des chemins du vers le départ connue par le noeud actuel. 
            
            while(chemin != NULL){
               // printf("Hello2\n");
                //printf("%d\n", fils->noeud->date);
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


int fromBitToId(ListChemin *chemins, int nbNoeud, int src, int dst, double *bc){
    int taille = 0;
    ListChemin *par = chemins, *sup;

    int pos = 0;
    int offset = 0;
    int nbChemin = 0;
    int bb[nbNoeud]; 
    int index[nbNoeud]; 
    int length = 0; 

    for(int i = 0; i < nbNoeud; i++){
        bb[i] = 0;
    }
    while(par != NULL){
        nbChemin++;
        //printf("Un des chemins est : \n");
        for(int i = 0; i < nbNoeud; i++){
            pos = int(i/8);
            offset = (i % 8);
            if(par->listChem[pos] & (1 << offset)){
                if(i != src & i != dst){
                    if(bb[i] == 0){
                        index[length] = i;
                        length++; 
                    }
                    bb[i]++;
                }
                //printf("%d ", i); 
            }
            
        }
        //fprintf(fichier, "\t");
        //printf("\n");
        sup = par;
        par = par->suivant;
        free(sup->listChem);
        free(sup);
    }
    //fprintf(fichier, "\t\t%d\n", nbChemin);
    //printf("Le nombre de chemin est %d\n\n\n", nbChemin);
    
    if(nbChemin > 0){

        for(int i = 0; i < length; i++){
            bc[index[i]] += (double)(bb[index[i]]/nbChemin);
        }

    }

    return nbChemin;
}