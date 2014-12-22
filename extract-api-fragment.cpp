
#include "ast-to-graphviz.hpp"

int main(int argc, char ** argv) {
  CommandlineProcessing::extraCppSourceFileSuffixes.push_back("h");
  CommandlineProcessing::extraCppSourceFileSuffixes.push_back("hpp");
  SgProject * project = new SgProject::SgProject(argc, argv);

  AST2GraphViz::Basic ast_to_graphviz;
  AST2GraphViz::Basic::DownAttr attr;

  std::cout << "digraph {" << std::endl;

  std::cout << "subgraph cluster_function {" << std::endl;
  std::vector<SgFunctionDeclaration *> func_decls = SageInterface::querySubTree<SgFunctionDeclaration>(project);
  std::vector<SgFunctionDeclaration *>::iterator it_func_decl;
  for (it_func_decl = func_decls.begin(); it_func_decl != func_decls.end(); it_func_decl++) {
    std::cout << "subgraph cluster_" << *it_func_decl << " {" << std::endl;
    ast_to_graphviz.traverse(*it_func_decl, attr);
    ast_to_graphviz.toDot(std::cout);
    std::cout << "}" << std::endl;
    ast_to_graphviz.clear();
  }
  std::cout << "}" << std::endl;

  std::cout << "subgraph cluster_class {" << std::endl;
  std::vector<SgClassDeclaration *> class_decls = SageInterface::querySubTree<SgClassDeclaration>(project);
  std::vector<SgClassDeclaration *>::iterator it_class_decl;
  for (it_class_decl = class_decls.begin(); it_class_decl != class_decls.end(); it_class_decl++) {
    std::cout << "subgraph cluster_" << *it_class_decl << " {" << std::endl;
    ast_to_graphviz.traverse(*it_class_decl, attr);
    ast_to_graphviz.toDot(std::cout);
    std::cout << "}" << std::endl;
    ast_to_graphviz.clear();
  }
  std::cout << "}" << std::endl;

  std::cout << "subgraph cluster_api_func {" << std::endl;
  std::vector<SgFunctionRefExp *> func_refs = SageInterface::querySubTree<SgFunctionRefExp>(project);
  std::vector<SgFunctionRefExp *>::iterator it_func_ref;
  for (it_func_ref = func_refs.begin(); it_func_ref != func_refs.end(); it_func_ref++) {
    SgFunctionRefExp      * func_ref  = *it_func_ref;
    assert(func_ref != NULL);
    SgFunctionSymbol      * func_sym  = func_ref->get_symbol_i();
    assert(func_sym != NULL);
    SgFunctionDeclaration * func_decl = func_sym->get_declaration();
    assert(func_decl != NULL);

    AST2GraphViz::NodeDesc & node_desc = ast_to_graphviz.addNode(func_sym);
      node_desc.label = ""; // func_sym->get_name().getString();
      node_desc.shape = "diamond";
      node_desc.color = "black";
      node_desc.fillcolor = "red";
    AST2GraphViz::EdgeDesc &  ref_edge_desc = ast_to_graphviz.addEdge(func_ref, func_sym);
      ref_edge_desc.color = "red";
      ref_edge_desc.constraint = false;
    AST2GraphViz::EdgeDesc & decl_edge_desc = ast_to_graphviz.addEdge(func_decl, func_sym);
      decl_edge_desc.color = "olivedrab";
      decl_edge_desc.constraint = true;
  }
  ast_to_graphviz.toDot(std::cout);
  ast_to_graphviz.clear();
  std::cout << "}" << std::endl;

  std::cout << "subgraph cluster_api_var {" << std::endl;
  std::vector<SgInitializedName *> init_names = SageInterface::querySubTree<SgInitializedName>(project);
  std::vector<SgInitializedName *>::iterator it_init_name;
  for (it_init_name = init_names.begin(); it_init_name != init_names.end(); it_init_name++) {
    SgInitializedName  * init_name  = *it_init_name;
    assert(init_name != NULL);
    SgClassType * class_type = isSgClassType(SageInterface::getElementType(init_name->get_type()));
    if (class_type == NULL) continue;
    SgClassDeclaration * class_decl = isSgClassDeclaration(class_type->get_declaration());
    assert(class_decl != NULL);
    class_decl = isSgClassDeclaration(class_decl->get_definingDeclaration());
    assert(class_decl != NULL);

    AST2GraphViz::NodeDesc & node_desc = ast_to_graphviz.addNode(class_type);
      node_desc.label = ""; // class_type->get_name().getString();
      node_desc.shape = "diamond";
      node_desc.color = "black";
      node_desc.fillcolor = "olivedrab";
    AST2GraphViz::EdgeDesc &  ref_edge_desc = ast_to_graphviz.addEdge(init_name, class_type);
      ref_edge_desc.color = "red";
      ref_edge_desc.constraint = false;
    AST2GraphViz::EdgeDesc & decl_edge_desc = ast_to_graphviz.addEdge(class_decl, class_type);
      decl_edge_desc.color = "olivedrab";
      decl_edge_desc.constraint = true;
  }
  ast_to_graphviz.toDot(std::cout);
  ast_to_graphviz.clear();
  std::cout << "}" << std::endl;

  std::cout << "}" << std::endl;

  return 0;
}

