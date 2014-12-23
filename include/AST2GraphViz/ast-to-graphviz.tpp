
namespace AST2GraphViz {

template <class TraversalAttr_tpl>
GraphVizAttr<TraversalAttr_tpl>::Inherited::Inherited() :
  node(NULL), parent(NULL), graph(NULL), user_attr()
{}

template <class TraversalAttr_tpl>
GraphVizAttr<TraversalAttr_tpl>::Inherited::Inherited(SgNode * node_, const typename TraversalAttr_tpl::Inherited & user_attr_) :
  node(node_), parent(NULL), graph(NULL), user_attr(user_attr_)
{}

template <class TraversalAttr_tpl>
GraphVizAttr<TraversalAttr_tpl>::Synthesized::Synthesized(SgNode * node_) :
  node(node_), user_attr()
{}

template <class TraversalAttr_tpl>
GraphVizTraversal<TraversalAttr_tpl>::graph_t::graph_t(graph_t * parent_) :
  parent(parent_),
  nodes(),
  subgraphs()
{}

template <class TraversalAttr_tpl>
GraphVizTraversal<TraversalAttr_tpl>::graph_t::~graph_t() {
  for (typename std::map<std::string, graph_t *>::iterator it = subgraphs.begin(); it != subgraphs.end(); it++)
    delete it->second;
}

template <class TraversalAttr_tpl>
typename GraphVizTraversal<TraversalAttr_tpl>::graph_t * GraphVizTraversal<TraversalAttr_tpl>::graph_t::getSubgraph(const std::string & label) {
  graph_t *& res = subgraphs[label];
  if (res == NULL) res = new graph_t(this);
    return res;
}

template <class TraversalAttr_tpl>
typename GraphVizTraversal<TraversalAttr_tpl>::InheritedAttr GraphVizTraversal<TraversalAttr_tpl>::evaluateInheritedAttribute(SgNode * node, InheritedAttr attr) {
  InheritedAttr result(node, attr.user_attr);

  evaluateInheritedAttribute(node, attr.user_attr, result.user_attr);

  result.parent = getParentInherited(attr.node, node, attr.user_attr);
  result.graph  = getGraphInherited(attr.graph, node, result.parent, attr.user_attr);

  assert(result.graph != NULL);

  return result;
}

template <class TraversalAttr_tpl>
typename GraphVizTraversal<TraversalAttr_tpl>::SynthesizedAttr GraphVizTraversal<TraversalAttr_tpl>::evaluateSynthesizedAttribute(SgNode * node, InheritedAttr attr, StackFrameVector<SynthesizedAttr> attrs) {
  SynthesizedAttr result(node);

  std::vector<UserSynthesizedAttr> user_attrs;
  for (typename StackFrameVector<SynthesizedAttr>::iterator it = attrs.begin(); it != attrs.end(); it++)
    user_attrs.push_back(it->user_attr);
  evaluateSynthesizeAttribute(node, attr.user_attr, user_attrs, result.user_attr);

  result.parent = getParentSynthesized(attr.parent, node, attr.user_attr, result.user_attr);
  result.graph  = getGraphSynthesized(attr.graph, node, result.parent, attr.user_attr, result.user_attr);

  if (result.graph != NULL) {
    result.graph->nodes.push_back(node);

    NodeDesc & node_desc = p_nodes[node];
    editNodeDesc(node, attr.user_attr, result.user_attr, node_desc);

    if (result.parent != NULL) {
      std::pair<SgNode *, SgNode *> edge(result.parent, node);
      EdgeDesc & edge_desc = p_edges[edge];
      editParentEdgeDesc(node, result.parent, attr.user_attr, result.user_attr, edge_desc);
    }
  }

  return result;
}

template <class TraversalAttr_tpl>
GraphVizTraversal<TraversalAttr_tpl>::GraphVizTraversal() : 
  AstTopDownBottomUpProcessing<typename GraphVizAttr<TraversalAttr_tpl>::Inherited, typename GraphVizAttr<TraversalAttr_tpl>::Synthesized>(),
  p_graph(new graph_t(NULL)),
  p_nodes(),
  p_edges()
{}

template <class TraversalAttr_tpl>
GraphVizTraversal<TraversalAttr_tpl>::~GraphVizTraversal() {
  if (p_graph != NULL) delete p_graph;
}

template <class TraversalAttr_tpl>
void GraphVizTraversal<TraversalAttr_tpl>::toDot(std::ostream & out, const std::string & label) const {
  if (p_graph != NULL) toDot(out, p_graph, label, std::string());
}

template <class TraversalAttr_tpl>
void GraphVizTraversal<TraversalAttr_tpl>::reset() {
  if (p_graph != NULL) delete p_graph;
  p_nodes.clear();
  p_edges.clear();
  p_graph = new graph_t(NULL);
}

template <class TraversalAttr_tpl>
typename GraphVizTraversal<TraversalAttr_tpl>::InheritedAttr GraphVizTraversal<TraversalAttr_tpl>::makeAttribute(bool set_graph, SgNode * parent) {
  InheritedAttr res;
  res.parent = parent;
  if (set_graph)
    res.graph = p_graph;
  return res;
}

template <class TraversalAttr_tpl>
typename GraphVizTraversal<TraversalAttr_tpl>::InheritedAttr GraphVizTraversal<TraversalAttr_tpl>::makeAttribute(graph_t * graph, SgNode * parent) {
  InheritedAttr res;
  res.parent = parent;
  res.graph = graph;
  return res;
}

template <class TraversalAttr_tpl>
typename GraphVizTraversal<TraversalAttr_tpl>::graph_t * GraphVizTraversal<TraversalAttr_tpl>::getSubgraph(const std::string & label) {
  if (p_graph != NULL)
    return p_graph->getSubgraph(label);
  else return NULL;
}

template <class TraversalAttr_tpl>
void GraphVizTraversal<TraversalAttr_tpl>::printNodeName(std::ostream & out, const SgNode * node) const {
  out << "node_" << node;
}

template <class TraversalAttr_tpl>
void GraphVizTraversal<TraversalAttr_tpl>::toDot(std::ostream & out, const SgNode * node, const NodeDesc & desc, std::string indent) const {
   out << indent;
   printNodeName(out, node);
   out << " [label=\"" << desc.label << "\", shape=" << desc.shape << ", color=\"" << desc.color << "\", style=filled, fillcolor=" << desc.fillcolor << "];" << std::endl;
}

template <class TraversalAttr_tpl>
void GraphVizTraversal<TraversalAttr_tpl>::toDot(std::ostream & out, const std::pair<SgNode *, SgNode *> & edge, const EdgeDesc & desc, std::string indent) const {
  out << indent;
  printNodeName(out, edge.first);
  out << " -> ";
  printNodeName(out, edge.second);
  out << " [label=\"" << desc.label << "\", color=\"" << desc.color << "\", constraint=\"";
  if (desc.constraint)
    out << "true";
  else
    out << "false";
  out << "\"];" << std::endl;
}

template <class TraversalAttr_tpl>
void GraphVizTraversal<TraversalAttr_tpl>::toDot(std::ostream & out, const graph_t * graph, const std::string & label, std::string indent) const {
  assert(graph != NULL);

  if (graph->parent == NULL)
    out << indent << "digraph graph_" << label << "_" << graph << " {" << std::endl;
  else
    out << indent << "subgraph cluster_" << label << "_" << graph << " {" << std::endl;

  std::vector<SgNode *>::const_iterator it_node;
  for (it_node = graph->nodes.begin(); it_node != graph->nodes.end(); it_node++) {
    std::map<SgNode *, NodeDesc>::const_iterator it_node_desc = p_nodes.find(*it_node);
    assert(it_node_desc != p_nodes.end());
    toDot(out, *it_node, it_node_desc->second, indent + "  ");
  }

  typename std::map<std::string, graph_t *>::const_iterator it_graph;
  for (it_graph = graph->subgraphs.begin(); it_graph != graph->subgraphs.end(); it_graph++)
    toDot(out, it_graph->second, it_graph->first, indent + "  ");

  if (graph->parent == NULL) {
    typename std::map<std::pair<SgNode *, SgNode *>, EdgeDesc>::const_iterator it_edge;
    for (it_edge = p_edges.begin(); it_edge != p_edges.end(); it_edge++)
      toDot(out, it_edge->first, it_edge->second, indent + "  ");
  }

  out << indent << "}" << std::endl;
}

}

