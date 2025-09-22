#ifndef DATA_DIR
#define DATA_DIR "../../Experiment/Paris/dataset_reel"
#endif

#ifndef DATASET_NAME
#define DATASET_NAME "network_temporal_day"
#endif

#include "fonction_bb.h"
#include <ctime>
#include <cstdio>
#include <cstdlib>

/* small util for writing headers */
static void writeFile(const char* fileName, const char* text, const char* mode){
    FILE* f = std::fopen(fileName, mode);
    if(!f){ std::printf("Error opening %s\n", fileName); return; }
    std::fprintf(f, "%s\n", text);
    std::fclose(f);
}
static void initStats(const char *baseName){
    char statName[256], stat[256];
    std::snprintf(statName, sizeof(statName), "stat_%s.moy", baseName);
    std::snprintf(stat,     sizeof(stat),     "stat_%s.result", baseName);
    writeFile(stat,    "Src\tdst\tPath",   "w");
    writeFile(statName,"src\tcc\tTemps_moy","w");
}

int main(){
    /* === CONFIG === */
    int   nbNoeud = 35370;     // set to (max_id + 1)
    int   dateMax = 1440;      // minutes 0..1439
    const char dataPrefix[] = DATA_DIR "/" DATASET_NAME;
    const char statPrefix[] = DATASET_NAME;
    initStats(statPrefix);

    /* === Read .src / .dst === */
    std::vector<int> tabS; tabS.reserve(10000);
    std::vector<int> tabD; tabD.reserve(10000);
    {
        char path[512];
        std::snprintf(path, sizeof(path), "%s.src", dataPrefix);
        FILE* f = std::fopen(path, "r");
        if(!f){ std::printf("Error opening %s\n", path); return 1; }
        int id;
        while(std::fscanf(f, "%d", &id)==1) tabS.push_back(id);
        std::fclose(f);

        std::snprintf(path, sizeof(path), "%s.dst", dataPrefix);
        f = std::fopen(path, "r");
        if(!f){ std::printf("Error opening %s\n", path); return 1; }
        while(std::fscanf(f, "%d", &id)==1) tabD.push_back(id);
        std::fclose(f);
    }

    /* === Read edges & build TE graph === */
    char edgesPath[512];
    std::snprintf(edgesPath, sizeof(edgesPath), "%s.txt", dataPrefix);
    std::printf("Reading edges: %s\n", edgesPath);
    aretes *a = fileToTables(edgesPath);
    if(!a){ std::printf("fileToTables failed.\n"); return 1; }
    std::printf("Edges read: %d\n", a->taille);

    Graphe_1* g = tableToGraphe(a->dates, a->srcs, a->dsts, a->couts, a->taille, nbNoeud, dateMax);
    if(!g){ std::printf("tableToGraphe failed.\n"); return 1; }
    std::printf("TE graph built. time-nodes=%d\n", g->nbNoeudTemps);

    /* === Arrays for results === */
    std::vector<double> bc(nbNoeud, 0.0);
    std::vector<double> tempsTable(nbNoeud, 0.0);
    std::vector<int>    paths(nbNoeud, -1);

    /* === Compute BC per source (sequential; parallelize if needed) === */
    for(size_t si=0; si<tabS.size(); ++si){
        int s = tabS[si];
        std::fill(paths.begin(), paths.end(), -1);
        std::vector<ListChemin*> chemins(nbNoeud, nullptr);

        std::clock_t t0 = std::clock();
        Path info = paths_bb_v1(g, s, paths.data(), chemins.data());
        (void)info;

        // accumulate BC from families
        for(int v=0; v<nbNoeud; ++v){
            if(chemins[v]){
                (void)fromBitToId_v1(chemins[v], nbNoeud, s, v, bc.data());
                chemins[v] = nullptr; // freed inside
            }
        }
        std::clock_t t1 = std::clock();
        tempsTable[s] = double(t1 - t0) / double(CLOCKS_PER_SEC);
        std::printf("src %d: %.6f s\n", s, tempsTable[s]);
    }

    /* === Degrees (single pass over file) === */
    std::vector<int> outdeg(nbNoeud,0), indeg(nbNoeud,0);
    {
        FILE* f = std::fopen(edgesPath, "r");
        if(!f){ std::printf("Error opening %s\n", edgesPath); return 1; }
        int date, u, v, w;
        while(std::fscanf(f, "%d %d %d %d", &date, &u, &v, &w)==4){
            if(u>=0 && u<nbNoeud) outdeg[u]++;
            if(v>=0 && v<nbNoeud) indeg[v]++;
        }
        std::fclose(f);
    }

    /* === Write BC results === */
    char outPath[512];
    std::snprintf(outPath, sizeof(outPath), "bc_%s_1.test", statPrefix);
    FILE* fout = std::fopen(outPath, "w");
    if(!fout){ std::printf("Error opening %s\n", outPath); return 1; }
    for(int i=0;i<(int)tabS.size();++i){
        int node = tabS[i];
        std::fprintf(fout, "%d %d %d %lf %lf\n",
            node, outdeg[node], indeg[node], bc[node], tempsTable[node]);
    }
    std::fclose(fout);

    // Optionally free graph (can be large)
    // freeGraphe(g);

    std::printf("Done. Wrote %s\n", outPath);
    return 0;
}
