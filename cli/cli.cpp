#include "../src/DeepWalk.h"
#include "../src/Walklets.h"
#include "../src/LINE.h"
#include "../src/HPE.h"
#include "../src/FINE.h"


int ArgPos(char *str, int argc, char **argv) {
    int a;
    for (a = 1; a < argc; a++) if (!strcmp(str, argv[a])) {
        if (a == argc - 1) {
            printf("Argument missing for %s\n", str);
            exit(1);
        }
        return a;
    }
    return -1;
}

int main(int argc, char **argv){
    
    int i;

    if (argc == 1) {
        printf("[proRec]\n");
        printf("\tcommand line interface for proRec\n\n");
        printf("Options:\n");
        printf("\t-model <string>\n");
        printf("\t\tAvaliable models: <DeepWalk>, <LINE>, <Walklets>, <HPE>, <FINE>\n");
        printf("\t-train <string>\n");
        printf("\t\tTrain the Network data\n");
        printf("\t-field <string>\n");
        printf("\t\tField meta of the vertices\n");
        printf("\t-save <string>\n");
        printf("\t\tSave the representation data\n");
        printf("\t-dimenstions <int>\n");
        printf("\t\tDimension of vertex representation; default is 64\n");
        printf("\t-negative_samples <int>\n");
        printf("\t\tNumber of negative examples; default is 5\n");
        printf("\t-window_size <int>\n");
        printf("\t\tSize of skip-gram window; default is 5\n");
        printf("\t-walk_times <int>\n");
        printf("\t\tTimes of being staring vertex; default is 10\n");
        printf("\t-walk_steps <int>\n");
        printf("\t\tStep of random walk; default is 5\n");
        printf("\t-sample_times <int>\n");
        printf("\t\tNumber of training samples *Million; default is 10\n");
        printf("\t-threads <int>\n");
        printf("\t\tNumber of training threads (default 1)\n");
        printf("\t-alpha <float>\n");
        printf("\t\tInit learning rate; default is 0.025\n");
        printf("\nExample Usage:\n");
        printf("./cli -model DeepWalk -train net.txt -output rep.txt -window_size 64 -negative_samples 5 -alpha 0.025 -threads 20\n\n");
        return 0;
    }
    
    char model[100], network_file[100], rep_file[100], field_file[100];
    int dimenstions=64, window_size=5, negative_samples=5, walk_times=10, walk_steps=5, sample_times=10, threads=1;
    double init_alpha=0.025;

    if ((i = ArgPos((char *)"-model", argc, argv)) > 0) strcpy(model, argv[i + 1]);
    if ((i = ArgPos((char *)"-train", argc, argv)) > 0) strcpy(network_file, argv[i + 1]);
    if ((i = ArgPos((char *)"-field", argc, argv)) > 0) strcpy(field_file, argv[i + 1]);
    if ((i = ArgPos((char *)"-save", argc, argv)) > 0) strcpy(rep_file, argv[i + 1]);
    if ((i = ArgPos((char *)"-dimenstions", argc, argv)) > 0) dimenstions = atoi(argv[i + 1]);
    if ((i = ArgPos((char *)"-window_size", argc, argv)) > 0) window_size = atoi(argv[i + 1]);
    if ((i = ArgPos((char *)"-negative_samples", argc, argv)) > 0) negative_samples = atoi(argv[i + 1]);
    if ((i = ArgPos((char *)"-walk_times", argc, argv)) > 0) walk_times = atoi(argv[i + 1]);
    if ((i = ArgPos((char *)"-walk_steps", argc, argv)) > 0) walk_steps = atoi(argv[i + 1]);
    if ((i = ArgPos((char *)"-sample_times", argc, argv)) > 0) sample_times = atoi(argv[i + 1]);
    if ((i = ArgPos((char *)"-alpha", argc, argv)) > 0) init_alpha = atof(argv[i + 1]);
    if ((i = ArgPos((char *)"-threads", argc, argv)) > 0) threads = atoi(argv[i + 1]);
    
    char model_dw[10] = "DeepWalk";
    char model_line[10] = "LINE";
    char model_wl[10] = "Walklets";
    char model_hpe[10] = "HPE";
    char model_fine[10] = "FINE";
    if (!strcmp(model_dw, model))
    {
        DeepWalk *dw;
        dw = new DeepWalk();
        dw->LoadEdgeList(network_file, 1);
        dw->Init(dimenstions);
        dw->Train(walk_times, walk_steps, window_size, negative_samples, init_alpha, threads);
        dw->SaveWeights(rep_file);
    }
    else if (!strcmp(model_line, model))
    {
        LINE *line;
        line = new LINE();
        line->LoadEdgeList(network_file, 1);
        line->Init(dimenstions);
        line->Train(sample_times, negative_samples, init_alpha, threads);
        line->SaveWeights(rep_file);
    }
    else if (!strcmp(model_hpe, model))
    {
        HPE *hpe;
        hpe = new HPE();
        hpe->LoadEdgeList(network_file, 1);
        hpe->Init(dimenstions);
        hpe->Train(sample_times, negative_samples, walk_steps, init_alpha, threads);
        hpe->SaveWeights(rep_file);
    }
    else if (!strcmp(model_wl, model))
    {
        int window_left=1, window_right=2;
        Walklets *wl;
        wl = new Walklets();
        wl->LoadEdgeList(network_file, 1);
        wl->Init(dimenstions);
        wl->Train(walk_times, walk_steps, window_left, window_right, negative_samples, init_alpha, threads);
        wl->SaveWeights(rep_file);
    }
    else if (!strcmp(model_fine, model))
    {
        FINE *fine;
        fine = new FINE();
        fine->LoadEdgeList(network_file, 1);
        fine->LoadFieldMeta(field_file);
        fine->Init(dimenstions);
        fine->Train(sample_times, walk_steps, negative_samples, init_alpha, threads);
        fine->SaveWeights(rep_file);
    }

   return 0;


}
