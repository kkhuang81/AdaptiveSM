#define HEAD_INFO

#include "sfmt/SFMT.h"
#include "head.h"

class Argument{
public:
	double Q;		// the percentage of total node number taken as quota.	
    string dataset;
    double epsilon;
    string model;    
	unsigned int batch;
	string seedfile;
    int time;  //this is the number of times needed to measure the result.	
};

#include "graph.h"
#include "infgraph.h"
#include "asm.h"

void OutputSeedSetToFile(vector<int> seed_set, string seedfile)
{
    ofstream of;
    of.open(seedfile);
    for (int seed: seed_set)
    {
        of << seed << " ";
    }
	of << endl;
    of.close();
}

void run_with_parameter(InfGraph &g, const Argument & arg)
{         
	ASM::SeedMinimize(g, arg);

    //INFO(g.seedSet);
    OutputSeedSetToFile(g.seedSet, arg.seedfile);        
}
void Run(int argn, char **argv)
{
    Argument arg;

    arg.Q = 0.1;    
	arg.batch = 1;	

    for (int i = 0; i < argn; i++)
    {
        if (argv[i] == string("-help") || argv[i] == string("--help") || argn == 1)
        {
            cout << "./imm -dataset *** -epsilon *** -k ***  -model IC|LT -seedfile *** -time *** -batch ***" << endl;
            return ;
        }	

		if (argv[i] == string("-dataset"))
			arg.dataset = argv[i + 1];
		if (argv[i] == string("-epsilon"))
			arg.epsilon = atof(argv[i + 1]);
		if (argv[i] == string("-Q"))
			arg.Q = atof(argv[i + 1]);		
		if (argv[i] == string("-model"))
			arg.model = argv[i + 1];
		if (argv[i] == string("-seedfile"))
			arg.seedfile = argv[i + 1];
		if (argv[i] == string("-batch"))
			arg.batch = atoi(argv[i + 1]);
		if (argv[i] == string("-time"))
			arg.time = atoi(argv[i + 1]);
    }
    ASSERT(arg.dataset != "");
    ASSERT(arg.model == "IC" || arg.model == "LT");
    arg.dataset = arg.dataset + "/";
    string graph_file;
    if (arg.model == "IC")
        graph_file = arg.dataset + "graph_ic.inf";
    else if (arg.model == "LT")
        graph_file = arg.dataset + "graph_lt.inf";      
    else
        ASSERT(false);
    
    InfGraph g(arg.dataset, graph_file);        

    if (arg.model == "IC")
        g.setInfuModel(InfGraph::IC);
    else if (arg.model == "LT")
        g.setInfuModel(InfGraph::LT);
    else
        ASSERT(false);

	ASSERT(arg.Q < 1.0);	
	arg.Q = ceil(arg.Q*g.n);			//convert it to the number of nodes in graph G.	

    run_with_parameter(g, arg);
}


int main(int argn, char **argv)
{
    __head_version = "v1";
    OutputInfo info(argn, argv);
    
    Run( argn, argv );
    // disp_mem_usage();
    cout<<"Memory(MB) "<<getProcMemory()<<endl;
}

