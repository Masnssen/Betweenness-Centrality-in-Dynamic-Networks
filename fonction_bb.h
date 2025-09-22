#pragma once
#include "fonction.h"
#include <algorithm>

/* ======= Path family operations (64-bit bitsets) ======= */

// Return: 2 = equal, 1 = incomparable, 0 = NEW dominates OLD (i.e., old ⊂ new), -1 = OLD dominates NEW (new ⊂ old)
static inline int compare_paths(const word_t* A, const word_t* B, int nWords){
    bool A_subset_B = true, B_subset_A = true;
    for(int k=0;k<nWords;k++){
        word_t Ak=A[k], Bk=B[k];
        if((Ak & Bk) != Ak) A_subset_B=false;
        if((Ak & Bk) != Bk) B_subset_A=false;
        if(!A_subset_B && !B_subset_A) return 1; // incomparable
    }
    if(A_subset_B && B_subset_A) return 2; // equal
    if(A_subset_B) return -1;              // old ⊇ new -> new is subset (old dominates)
    if(B_subset_A) return 0;               // new ⊇ old -> new dominates old
    return 1;
}

// Try to insert `bits` into list `chemins` at node `id`, keeping an inclusion-minimal family.
// Returns updated head pointer. May delete dominated entries.
static inline ListChemin* insertPathMinimal(ListChemin* chemins, word_t* bits, int id, int nWords){
    // Set the bit of the destination node in the candidate
    set_bit(bits, id);

    // First pass: detect if candidate is dominated by any existing -> abort
    for(ListChemin* p=chemins; p; p=p->suivant){
        int cmp = compare_paths(p->listChem, bits, nWords);
        if(cmp == 2 || cmp == -1){ // equal OR old dominates new
            return chemins; // do not insert
        }
    }
    // Second pass: remove any existing entries dominated by candidate
    ListChemin *prev=nullptr, *cur=chemins;
    while(cur){
        int cmp = compare_paths(cur->listChem, bits, nWords);

        if(cmp == 0){ // candidate dominates cur
            ListChemin* bye = cur;
            cur = cur->suivant;
            if(prev) prev->suivant = cur; else chemins = cur;
            std::free(bye->listChem); std::free(bye);
        }else{
            prev = cur; cur = cur->suivant;
        }
    }
    // Insert candidate at head (own copy)
    ListChemin* neu = (ListChemin*)std::malloc(sizeof(ListChemin));
    neu->listChem = (word_t*)std::malloc(sizeof(word_t)*nWords);
    for(int k=0;k<nWords;k++) neu->listChem[k]=bits[k];
    neu->suivant = chemins;
    return neu;
}

// Deep copy of a list of bitsets
static inline ListChemin* copyChemins64(ListChemin *head, int nWords){
    ListChemin *dst=nullptr;
    for(ListChemin *p=head; p; p=p->suivant){
        ListChemin *neu=(ListChemin*)std::malloc(sizeof(ListChemin));
        neu->listChem = (word_t*)std::malloc(sizeof(word_t)*nWords);
        for(int k=0;k<nWords;k++) neu->listChem[k]=p->listChem[k];
        neu->suivant = dst; dst = neu;
    }
    return dst;
}

/* ======= Fast traversal: bucketed queue by time ======= */
static inline Path paths_bb_v1(Graphe_1 *g, int src, int paths[], ListChemin *chemins[]){
    Path ret; ret.dateArrive=-1; ret.listchemin=nullptr;

    const int nWords = (g->nbNoued + WORD_BITS - 1) / WORD_BITS;

    // buckets[time] contains time-nodes to process at that date
    std::vector<std::vector<NoeudTemp*>> buckets(g->dateMax);

    // Initialize: for each existing (src, t)
    for(int t=0; t<g->dateMax; ++t){
        NoeudTemp* u = g->noeuds[src + (long long)t * g->nbNoued];
        if(!u) continue;
        // path bitset with only src set
        ListChemin* lc = (ListChemin*)std::malloc(sizeof(ListChemin));
        lc->listChem = (word_t*)std::calloc(nWords, sizeof(word_t));
        set_bit(lc->listChem, u->id);
        lc->suivant=nullptr;
        u->listChemins = lc;
        buckets[t].push_back(u);
    }

    for(int curT=0; curT<g->dateMax; ++curT){
        printf("curT=%d\n", curT);
        auto &Q = buckets[curT];
        for(size_t qi=0; qi<Q.size(); ++qi){
            NoeudTemp* u = Q[qi];
            if(!u) continue;

            // Relax earliest arrival for u->id (except the source itself at time 0)
            if(paths[u->id] == -1 || (u->date < paths[u->id] && u->id != src)){
                paths[u->id] = u->date;
                // store a copy of path families
                if(chemins[u->id]) {
                    // free previous family
                    ListChemin* c = chemins[u->id];
                    while(c){ ListChemin* nx=c->suivant; std::free(c->listChem); std::free(c); c=nx; }
                }
                chemins[u->id] = copyChemins64(u->listChemins, nWords);
            }

            // Propagate to children
            for(FilsNoeud* e=u->first; e; e=e->suivant){
                NoeudTemp* v = e->noeud;
                if(!v || v->date<0 || v->date>=g->dateMax) continue;

                // For each path family in u, try to extend to v with inclusion-minimal rule
                bool inserted=false;
                for(ListChemin* c = u->listChemins; c; c=c->suivant){
                    // clone wordset once per candidate
                    word_t* tmp = (word_t*)std::malloc(sizeof(word_t)*nWords);
                    for(int k=0;k<nWords;k++) tmp[k]=c->listChem[k];
                    ListChemin* headBefore = v->listChemins;
                    v->listChemins = insertPathMinimal(v->listChemins, tmp, v->id, nWords);
                    if(v->listChemins != headBefore) inserted=true;
                    std::free(tmp); // insertPathMinimal copied / or rejected
                }
                if(inserted){
                    buckets[v->date].push_back(v);
                }
            }

            // free u->listChemins (consumed)
            ListChemin* c = u->listChemins;
            while(c){ ListChemin* nx=c->suivant; std::free(c->listChem); std::free(c); c=nx; }
            u->listChemins = nullptr;
        }
    }
    return ret;
}

/* ======= Convert path families to betweenness increments ======= */
static inline int fromBitToId_v1(ListChemin *head, int nbNoeud, int src, int dst, double *bc){
    if(!head) return 0;
    int nbPaths=0;

    // Gather counts per node only for those appearing at least once
    std::vector<int> idx; idx.reserve(128);
    std::vector<int> cnt(nbNoeud, 0);

    for(ListChemin* p=head; p; ){
        nbPaths++;
        // scan bitset
        const word_t* A = p->listChem;
        for(int i=0;i<nbNoeud;i++){
            if(i==src || i==dst) continue;
            if(test_bit(A, i)){
                if(cnt[i]==0) idx.push_back(i);
                cnt[i]++;
            }
        }
        // free on the fly
        ListChemin* bye=p; p=p->suivant;
        std::free(bye->listChem); std::free(bye);
    }

    if(nbPaths>0){
        for(int k: idx){
            bc[k] += double(cnt[k]) / double(nbPaths);
        }
    }
    return nbPaths;
}
