
#include "Viz/traversal.hpp"

namespace Rose {

namespace Viz {

namespace Traversals {

struct Programmable {
  class InheritedAttr {};
  class SynthesizedAttr {};

  static InheritedAttr evaluateInheritedAttribute(SgNode * node, InheritedAttr attr) { InheritedAttr res; return res; }
  static SynthesizedAttr evaluateSynthesizedAttribute(SgNode * node, InheritedAttr attr, StackFrameVector<SynthesizedAttr> attrs) { SynthesizedAttr res; return res; }

  struct Policies {
    static bool stopAfter(SgNode *) { return false; }
    static bool skip(SgNode *) { return false; }

    static Traversal<Programmable>::graph_t * startOn(SgNode *) { return NULL; }
    static Traversal<Programmable>::graph_t * getSubgraph(SgNode * node, Traversal<Programmable>::graph_t * graph) { return graph; }
  };

  struct GraphViz {
    static void edit(SgNode * node, GraphVizObjects::node_desc_t & desc, const InheritedAttr & inhr_attr, const SynthesizedAttr & synth_attr) {  
  desc.label="";
  desc.color="black";
  desc.style="";
  desc.shape="box";
  desc.fillcolor="";
  desc.penwidth=1;
    }
    static void edit(SgNode * node, SgNode * parent, GraphVizObjects::edge_desc_t & desc) {
  desc.label="";
  desc.color="";
  desc.style="";
  desc.minlen=1;
  desc.penwidth=1;
  desc.constraint=true;
    }
    static void edit(Traversal<Programmable>::graph_t * graph, GraphVizObjects::cluster_desc_t & desc) {
  desc.label="";
  desc.color="black";
  desc.style="";
  desc.penwidth=1;
    }
    static void edit(GraphVizObjects::graph_desc_t & desc) {
  desc.label="";
  desc.color="black";
  desc.style="";
  desc.rankdir="TB";
  desc.ranksep=1;
    }
  };
};

}

}

}

int main(int argc, char ** argv) {
  CommandlineProcessing::extraCppSourceFileSuffixes.push_back("h");
  CommandlineProcessing::extraCppSourceFileSuffixes.push_back("hpp");

  std::vector<std::string> args;
  std::string prefix_opt("-prefix");
  std::string prefix;

  for (int i = 0; i < argc; i++) {
    if (prefix_opt == argv[i]) {
      i++;
      prefix = argv[i];
    }
    else {
      args.push_back(argv[i]);
    }
  }

  SgProject * project = new SgProject::SgProject(args);

  Rose::Viz::Traversal<Rose::Viz::Traversals::Programmable> traversal;

  std::vector<SgSourceFile *> files = SageInterface::querySubTree<SgSourceFile>(project);
  std::vector<SgSourceFile *>::iterator it_file;
  for (it_file = files.begin(); it_file != files.end(); it_file++) {
    std::string filename = (*it_file)->get_sourceFileNameWithoutPath();
    size_t dotpos = filename.find_last_of('.');
    std::string basename = filename.substr(0, dotpos);
    std::string extension = filename.substr(dotpos+1);
    std::cerr << "basename = " << basename << ", prefix = " << prefix << ", ext = " << extension << std::endl;

    traversal.traverseWithinFile(*it_file, traversal.makeAttribute(basename));

    std::ostringstream oss;
    oss << prefix << basename << ".dot";

    std::ofstream dot_file;
    dot_file.open(oss.str().c_str());
    assert(dot_file.is_open());

    traversal.toGraphViz(basename, dot_file);

    dot_file.close();
  }

  return 0;
}

