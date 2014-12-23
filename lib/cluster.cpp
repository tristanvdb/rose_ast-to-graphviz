
#include "AST2GraphViz/cluster.hpp"

namespace AST2GraphViz {

Cluster::Cluster() : Default() {}
Cluster::~Cluster() {}

Cluster::graph_t * Cluster::getGraphInherited(
                   graph_t * suggested_graph, SgNode * node, SgNode * parent,
                   const UserInheritedAttr & inherited_attr
                 ) {
/*std::cerr << "Cluster::getGraphInherited(" << std::endl;
  std::cerr << "    graph  = " << suggested_graph << std::endl;
  std::cerr << "    node   = " << node << " (" << node->class_name() << ")" << std::endl;
  if (parent != NULL)
    std::cerr << "    parent = " << parent << " (" << parent->class_name() << ")" << std::endl;
  else
    std::cerr << "    parent = NULL" << std::endl;
  std::cerr << ")" << std::endl;
*/
  SgDeclarationStatement * decl_stmt = isSgDeclarationStatement(node);
  SgDeclarationStatement * decl_stmt_parent = isSgDeclarationStatement(parent);
  SgStatement * stmt = isSgStatement(node);
  SgStatement * stmt_parent = isSgStatement(parent);
  SgExpression * expr = isSgExpression(node);
  SgExpression * expr_parent = isSgExpression(parent);

  if (decl_stmt != NULL) {
    if (decl_stmt_parent == NULL) {
      std::ostringstream oss; oss << "decl_" << node;
      return suggested_graph->getSubgraph(oss.str());
    }
  }
  else if (stmt != NULL) {
    if (stmt_parent == NULL) {
      std::ostringstream oss; oss << "stmt_" << node;
      return suggested_graph->getSubgraph(oss.str());
    }
  }
  else if (expr != NULL) {
    if (expr_parent == NULL) {
      std::ostringstream oss; oss << "expr_" << node;
      return suggested_graph->getSubgraph(oss.str());
    }
  }
  return suggested_graph;
}

Cluster::graph_t * Cluster::getGraphSynthesized(
                   graph_t * suggested_graph, SgNode * node, SgNode * parent,
                   const UserInheritedAttr & inherited_attr, const UserSynthesizedAttr & synthetized_attr
                 ) {
/*std::cerr << "Cluster::getGraphSynthesized(" << std::endl;
  std::cerr << "    graph  = " << suggested_graph << std::endl;
  std::cerr << "    node   = " << node << " (" << node->class_name() << ")" << std::endl;
  if (parent != NULL)
    std::cerr << "    parent = " << parent << " (" << parent->class_name() << ")" << std::endl;
  else
    std::cerr << "    parent = NULL" << std::endl;
  std::cerr << ")" << std::endl;
*/
  return suggested_graph;
}

}

