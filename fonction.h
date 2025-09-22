#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>

/* ======= Public types (kept compatible) ======= */
struct NoeudTemp;
struct ListMin;    // not used in v1 (kept for compat)
struct Graphe_1;
struct FilsNoeud;  // kept for compat (adjacency as linked list)
struct ListChemin;
struct Path;
struct aretes;

using word_t = uint64_t;      // 64-bit words for bitsets
static constexpr int WORD_BITS = 64;

struct Path{
    int dateArrive = -1;
    ListChemin* listchemin = nullptr;
};

struct ListChemin{
    word_t *listChem = nullptr; // 64-bit bitset
    ListChemin* suivant = nullptr;
};

struct FilsNoeud{
    NoeudTemp* noeud = nullptr;
    FilsNoeud* suivant = nullptr;
};

struct NoeudTemp{
    int id  = -1; 
    int date = -1;
    FilsNoeud* first = nullptr;
    FilsNoeud* last  = nullptr;
    ListChemin* listChemins = nullptr; // list of path bitsets that arrive here
};

struct Graphe_1{
    NoeudTemp** noeuds = nullptr; // grid of size (dateMax * nbNoued)
    int nbNoued = -1;             // number of physical nodes
    int dateMax = -1;             // number of time steps (e.g., 1440)
    int nbNoeudTemps = 0;         // materialized time-nodes
};

struct aretes{
    int *dates = nullptr;
    int *srcs  = nullptr;
    int *dsts  = nullptr;
    int *couts = nullptr; 
    int taille = 0; 
};

/* ======= Helpers ======= */
static inline void set_bit(word_t* A, int i){
    A[i / WORD_BITS] |= (word_t(1) << (i % WORD_BITS));
}
static inline bool test_bit(const word_t* A, int i){
    return (A[i / WORD_BITS] >> (i % WORD_BITS)) & 1u;
}

/* ======= Safe reader: file -> arrays (auto-resize) ======= */
static inline aretes *fileToTables(const char *fileName){
    FILE *f = std::fopen(fileName, "r");
    if (!f) { std::printf("Error opening file %s\n", fileName); return nullptr; }

    aretes *a = (aretes*)std::malloc(sizeof(aretes));
    if (!a) { std::fclose(f); return nullptr; }

    int cap = 1'000'000;
    a->dates = (int*)std::malloc(sizeof(int)*cap);
    a->srcs  = (int*)std::malloc(sizeof(int)*cap);
    a->dsts  = (int*)std::malloc(sizeof(int)*cap);
    a->couts = (int*)std::malloc(sizeof(int)*cap);
    a->taille = 0;
    if(!a->dates || !a->srcs || !a->dsts || !a->couts){
        std::printf("Initial allocation failed\n");
        std::fclose(f);
        std::free(a->dates); std::free(a->srcs); std::free(a->dsts); std::free(a->couts);
        std::free(a); return nullptr;
    }

    int date, src, dst, cout;
    while (std::fscanf(f, "%d %d %d %d", &date, &src, &dst, &cout) == 4){
        if (a->taille >= cap){
            int newcap = cap * 2;
            int *nd  = (int*)std::realloc(a->dates, sizeof(int)*newcap);
            int *ns  = (int*)std::realloc(a->srcs , sizeof(int)*newcap);
            int *ndd = (int*)std::realloc(a->dsts , sizeof(int)*newcap);
            int *nc  = (int*)std::realloc(a->couts, sizeof(int)*newcap);
            if(!nd || !ns || !ndd || !nc){
                std::printf("Realloc failed (%d -> %d)\n", cap, newcap);
                std::fclose(f);
                if(nd) a->dates=nd; if(ns) a->srcs=ns; if(ndd)a->dsts=ndd; if(nc)a->couts=nc;
                return a; // return partial for debugging
            }
            a->dates=nd; a->srcs=ns; a->dsts=ndd; a->couts=nc; cap=newcap;
        }
        a->dates[a->taille]=date; a->srcs[a->taille]=src;
        a->dsts[a->taille]=dst;   a->couts[a->taille]=cout;
        a->taille++;
    }
    std::fclose(f);
    return a;
}

/* ======= Arrays -> time expanded graph (bounds checked) ======= */
static inline Graphe_1 *tableToGraphe(const int *dates, const int *srcs, const int *dsts,
                                      const int *couts, int taille, int nbNoued, int dateMax){
    Graphe_1 *g = (Graphe_1*)std::malloc(sizeof(Graphe_1));
    if(!g) return nullptr;
    const long long cells = (long long)nbNoued * (long long)dateMax;
    g->noeuds = (NoeudTemp**)std::malloc(sizeof(NoeudTemp*) * (size_t)cells);
    if(!g->noeuds){ std::free(g); return nullptr; }

    for(long long i=0;i<cells;i++) g->noeuds[i]=nullptr;
    g->nbNoued=nbNoued; g->dateMax=dateMax; g->nbNoeudTemps=0;

    for(int idx=0; idx<taille; ++idx){
        const int date=dates[idx], src=srcs[idx], dst=dsts[idx], cout=couts[idx];
        if(date<0 || date>=dateMax || cout<=0) continue;
        const int date2 = date + cout;
        if(date2<0 || date2>=dateMax) continue;
        if(src<0 || src>=nbNoued || dst<0 || dst>=nbNoued) continue;

        const long long iSrc = (long long)src + (long long)date  * nbNoued;
        const long long iDst = (long long)dst + (long long)date2 * nbNoued;

        NoeudTemp *u = g->noeuds[iSrc];
        NoeudTemp *v = g->noeuds[iDst];
        if(!u){
            u = (NoeudTemp*)std::malloc(sizeof(NoeudTemp));
            if(!u) return g;
            u->id=src; u->date=date; u->first=u->last=nullptr; u->listChemins=nullptr;
            g->noeuds[iSrc]=u; g->nbNoeudTemps++;
        }
        if(!v){
            v = (NoeudTemp*)std::malloc(sizeof(NoeudTemp));
            if(!v) return g;
            v->id=dst; v->date=date2; v->first=v->last=nullptr; v->listChemins=nullptr;
            g->noeuds[iDst]=v; g->nbNoeudTemps++;
        }
        FilsNoeud* e = (FilsNoeud*)std::malloc(sizeof(FilsNoeud));
        if(!e) return g;
        e->noeud = v; e->suivant=nullptr;
        if(!u->first){ u->first=u->last=e; } else { u->last->suivant=e; u->last=e; }
    }
    return g;
}

/* ======= Cleanup ======= */
static inline void freeGraphe(Graphe_1 *g){
    if(!g) return;
    const long long cells = (long long)g->nbNoued * (long long)g->dateMax;
    for(long long i=0;i<cells;i++){
        NoeudTemp* u = g->noeuds[i];
        if(!u) continue;
        FilsNoeud* cur = u->first;
        while(cur){ FilsNoeud* nxt=cur->suivant; std::free(cur); cur=nxt; }
        while(u->listChemins){
            ListChemin* c=u->listChemins; u->listChemins=c->suivant;
            std::free(c->listChem); std::free(c);
        }
        std::free(u);
    }
    std::free(g->noeuds);
    std::free(g);
}
