//-----------------------------------------------
// Copyright 2009 Wellcome Trust Sanger Institute
// Written by Jared Simpson (js18@sanger.ac.uk)
// Released under the GPL
//-----------------------------------------------
//
// sga - Main assembler driver program
//
#include <string>
#include <iostream>
#include "index.h" 
#include "overlap.h"
#include "assemble.h"
#include "oview.h"
#include "rmdup.h"
#include "preprocess.h"
#include "merge.h"
#include "correct.h"
#include "subgraph.h"
#include "scaffold.h"
#include "connect.h"
#include "walk.h"
#include "gmap.h"
#include "filter.h"
#include "fm-merge.h"
#include "scaffold2fasta.h"
#include "stats.h"
#include "filterBAM.h"
#include "cluster.h"

#define PROGRAM_BIN "sga"
#define AUTHOR "Jared Simpson"

static const char *SGA_VERSION_MESSAGE =
"String Graph Assembler (sga) Version " PACKAGE_VERSION "\n"
"Written by Jared Simpson.\n"
"\n"
"Copyright 2009 Wellcome Trust Sanger Institute\n";

static const char *SGA_USAGE_MESSAGE =
"Program: " PACKAGE_NAME "\n"
"Version: " PACKAGE_VERSION "\n"
"Contact: " AUTHOR " [" PACKAGE_BUGREPORT "]\n"
"Usage: " PROGRAM_BIN " <command> [options]\n\n"
"Commands:\n"
"           preprocess      filter and quality-trim reads\n"
"           index           build the BWT and FM-index for a set of reads\n"
"           merge           merge multiple BWT/FM-index files into a single index\n"
"           correct         correct sequencing errors in a set of reads\n"
"           fm-merge        merge unambiguously overlapped sequences using the FM-index\n"
"           overlap         compute overlaps between reads\n"
"           assemble        generate contigs from an assembly graph\n"
"           oview           view overlap alignments\n"
"           subgraph        extract a subgraph from a graph\n"
"           filter          remove reads from a data set\n"
"\n\nExperimental commands:\n"
"           stats           print useful statistics about the read set\n"
"           connect         resolve the complete sequence of a paired-end fragment\n"
"           scaffold        generate ordered sets of contigs using distance estimates\n"
"           scaffold2fasta  convert the output of the scaffold subprogram into a fasta file\n"
"           filterBAM       filter out contaminating mate-pair data in a BAM file\n"
"           cluster         find clusters of reads belonging to the same connected component\n"
"\n\nDeprecated commands:\n"
"           rmdup           duplicate read removal - superceded by sga filter\n"
"\nReport bugs to " PACKAGE_BUGREPORT "\n\n";

int main(int argc, char** argv)
{
    if(argc <= 1)
    {
        std::cout << SGA_USAGE_MESSAGE;
        return 0;
    }
    else
    {
        std::string command(argv[1]);
        if(command == "help" || command == "--help")
        {
            std::cout << SGA_USAGE_MESSAGE;
            return 0;
        }
        else if(command == "version" || command == "--version")
        {
            std::cout << SGA_VERSION_MESSAGE;
            return 0;
        }

        if(command == "preprocess")
            preprocessMain(argc - 1, argv + 1);
        else if(command == "index")
            indexMain(argc - 1, argv + 1);
        else if(command == "merge")
            mergeMain(argc - 1, argv + 1);
        else if(command == "filter")
            filterMain(argc - 1, argv + 1);
        else if(command == "stats")
            statsMain(argc - 1, argv + 1);
        else if(command == "rmdup")
            rmdupMain(argc - 1, argv + 1);
        else if(command == "fm-merge")
            FMMergeMain(argc - 1, argv + 1);
        else if(command == "overlap")
            overlapMain(argc - 1, argv + 1);
        else if(command == "correct")
            correctMain(argc - 1, argv + 1);
        else if(command == "assemble")
            assembleMain(argc - 1, argv + 1);
        else if(command == "connect")
            connectMain(argc - 1, argv + 1);
        else if(command == "gmap")
            gmapMain(argc - 1, argv + 1);
        else if(command == "subgraph")
            subgraphMain(argc - 1, argv + 1);
        else if(command == "walk")
            walkMain(argc - 1, argv + 1);
        else if(command == "oview")
            oviewMain(argc - 1, argv + 1);
        else if(command == "scaffold")
            scaffoldMain(argc - 1, argv + 1);
        else if(command == "scaffold2fasta")
            scaffold2fastaMain(argc - 1, argv + 1);
        else if(command == "filterBAM")
            filterBAMMain(argc - 1, argv + 1);
        else if(command == "cluster")
            clusterMain(argc - 1, argv + 1);
        else
        {
            std::cerr << "Unrecognized command: " << command << "\n";
            return 1;
        }
    }

    return 0;
}
