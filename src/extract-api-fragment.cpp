
#include "AST2GraphViz/cluster.hpp"

int main(int argc, char ** argv) {
  CommandlineProcessing::extraCppSourceFileSuffixes.push_back("h");
  CommandlineProcessing::extraCppSourceFileSuffixes.push_back("hpp");
  SgProject * project = new SgProject::SgProject(argc, argv);

  std::ofstream dot_file;
  dot_file.open("whole_ast.dot");
  assert(dot_file.is_open());
  AST2GraphViz::Cluster ast_to_graphviz;
  AST2GraphViz::Cluster::graph_t * graph = ast_to_graphviz.getSubgraph("ast");

  graph = graph->getSubgraph("ast__function");

  std::vector<SgFunctionDeclaration *> func_decls = SageInterface::querySubTree<SgFunctionDeclaration>(project);
  std::vector<SgFunctionDeclaration *>::iterator it_func_decl;
  for (it_func_decl = func_decls.begin(); it_func_decl != func_decls.end(); it_func_decl++) {
    if ((*it_func_decl)->get_name().getString() != "foo") continue;

    std::ostringstream oss; oss << "ast__function__" << (*it_func_decl)->get_name().getString();
    graph = graph->getSubgraph(oss.str());

    ast_to_graphviz.traverse(*it_func_decl, ast_to_graphviz.makeAttribute(graph));

    graph = graph->parent;
  }

  graph = graph->parent;
/*
  graph = graph->getSubgraph("ast__class");

  std::vector<SgClassDeclaration *> class_decls = SageInterface::querySubTree<SgClassDeclaration>(project);
  std::vector<SgClassDeclaration *>::iterator it_class_decl;
  for (it_class_decl = class_decls.begin(); it_class_decl != class_decls.end(); it_class_decl++) {
    std::ostringstream oss; oss << "ast__class__" << (*it_class_decl)->get_name().getString();
    graph = graph->getSubgraph(oss.str());

    ast_to_graphviz.traverse(*it_class_decl, ast_to_graphviz.makeAttribute(graph));

    graph = graph->parent;
  }

  graph = graph->parent;
*/
  ast_to_graphviz.toDot(dot_file, "mfb");

  return 0;
}

