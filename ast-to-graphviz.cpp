
#include "ast-to-graphviz.hpp"

namespace AST2GraphViz {

bool Basic::skipNode(SgNode * node, const EmptyDownAttr & attr_in) {
  if (isSgProject(node) || isSgFileList(node) ||isSgFile(node))
    return true;
  else
    return false;
}

void Basic::editNodeDesc(SgNode * node, const EmptyDownAttr & attr_in, const EmptyDownAttr & attr_out, NodeDesc & node_desc) {
  std::ostringstream oss_label;

  SgDeclarationStatement  * decl_stmt  = isSgDeclarationStatement(node);
  SgScopeStatement        * scope_stmt = isSgScopeStatement(node);
  SgInitializedName       * init_name  = isSgInitializedName(node);
  SgFunctionParameterList * param_list = isSgFunctionParameterList(node);
  SgFunctionDeclaration   * func_decl  = isSgFunctionDeclaration(node);
  SgClassDeclaration      * class_decl = isSgClassDeclaration(node);
  SgExpression            * expr       = isSgExpression(node);
  SgStatement             * stmt       = isSgStatement(node);
  SgLocatedNode           * loc_node   = isSgLocatedNode(node);

  if (param_list != NULL) {;
    node_desc.shape = "diamond";
    node_desc.color = "black";
    node_desc.fillcolor = "aquamarine";
  }
  else if (func_decl != NULL) {
    oss_label << "Function:\\n" << func_decl->get_name().getString();
    node_desc.shape = "diamond";
    node_desc.color = "black";
    node_desc.fillcolor = "cyan";
  }
  else if (class_decl != NULL) {
    oss_label << "Class:\\n" << class_decl->get_name().getString();
    node_desc.shape = "diamond";
    node_desc.color = "black";
    node_desc.fillcolor = "cyan";
  }
  else if (init_name != NULL) {
    oss_label << "Variable:\\n" << init_name->get_name().getString();
    node_desc.shape = "box";
    node_desc.color = "black";
    node_desc.fillcolor = "lightsalmon";
  }
  else if (decl_stmt != NULL) {
    oss_label << node->class_name();
    node_desc.shape = "diamond";
    node_desc.color = "black";
    node_desc.fillcolor = "cyan";
  }
  else if (scope_stmt != NULL) {
    oss_label << node->class_name();
    node_desc.shape = "box";
    node_desc.color = "black";
    node_desc.fillcolor = "indianred1";
  }
  else if (stmt != NULL) {
    oss_label << node->class_name();
    node_desc.shape = "box";
    node_desc.color = "black";
    node_desc.fillcolor = "darkgoldenrod1";
  }
  else if (expr != NULL) {
    oss_label << node->class_name();
    node_desc.shape = "oval";
    node_desc.color = "black";
    node_desc.fillcolor = "chartreuse1";
  }
  else if (loc_node != NULL) {
    oss_label << node->class_name();
    node_desc.shape = "box";
    node_desc.color = "black";
    node_desc.fillcolor = "white";
  }
  else {
    node_desc.shape = "diamond";
    node_desc.color = "black";
    node_desc.fillcolor = "black";
  }
  node_desc.label = oss_label.str();
}

void Basic::editEdgeDesc(SgNode * node, const EmptyDownAttr & attr_in, const EmptyDownAttr & attr_out, SgNode * parent, EdgeDesc & edge_desc) {
  edge_desc.label = "";
  edge_desc.color = "black";
  edge_desc.constraint = true;
}

}

