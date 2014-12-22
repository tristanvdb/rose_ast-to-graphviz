
#include "ast-to-graphviz.hpp"

int main(int argc, char ** argv) {
  CommandlineProcessing::extraCppSourceFileSuffixes.push_back("h");
  CommandlineProcessing::extraCppSourceFileSuffixes.push_back("hpp");
  SgProject * project = new SgProject::SgProject(argc, argv);

  AST2GraphViz::Basic ast_to_graphviz;
  AST2GraphViz::Basic::DownAttr attr;

  ast_to_graphviz.traverse(project, attr);

  std::cout << "digraph {" << std::endl;
  ast_to_graphviz.toDot(std::cout);
  std::cout << "}" << std::endl;

  return 0;
}

