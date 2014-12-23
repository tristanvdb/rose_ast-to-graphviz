
#include "AST2GraphViz/cluster.hpp"

int main(int argc, char ** argv) {
//CommandlineProcessing::extraCppSourceFileSuffixes.push_back("h");

  SgProject * project = new SgProject::SgProject(argc, argv);

  std::ofstream dot_file;

  dot_file.open("whole_ast_default.dot");
  assert(dot_file.is_open());
  AST2GraphViz::Default ast_to_graphviz_default;
  ast_to_graphviz_default.traverse(project, ast_to_graphviz_default.makeAttribute());
  ast_to_graphviz_default.toDot(dot_file, "whole_ast_default");
  dot_file.close();

  dot_file.open("whole_ast_cluster.dot");
  assert(dot_file.is_open());
  AST2GraphViz::Cluster ast_to_graphviz_cluster;
  ast_to_graphviz_cluster.traverse(project, ast_to_graphviz_cluster.makeAttribute());
  ast_to_graphviz_cluster.toDot(dot_file, "whole_ast_cluster");
  dot_file.close();

  return 0;
}

