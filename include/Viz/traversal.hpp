
#ifndef __ROSE_VIZ_TRAVERSAL_HPP__
#define __ROSE_VIZ_TRAVERSAL_HPP__

#include "rose.h"

#include <string>
#include <sstream>
#include <map>
#include <utility>

namespace Rose {

namespace Viz {

struct GraphVizObjects {
  struct desc_t {
    std::string label;
    std::string color;
    std::string style;
  };

  struct node_desc_t : public desc_t {
    std::string shape;
    std::string fillcolor;
    float penwidth;
    std::string url;
  };

  struct edge_desc_t : public desc_t {
    int minlen;
    float penwidth;
    bool constraint;
  };

  struct cluster_desc_t : public desc_t {
    float penwidth;
  };

  struct graph_desc_t : public desc_t {
    std::string rankdir;
    float ranksep;
  };
};

template <class UserTraversal>
class Traversal;

template <class UserTraversal, class UserAttr>
struct TraversalAttrWrapper {
    struct viz_attr_t {
      SgNode * node;
      SgNode * parent;
      typename Traversal<UserTraversal>::graph_t * graph;

      SgNode * stopped_by;
      SgNode * skipped_since;

    };

    viz_attr_t viz_attr;
    UserAttr user_attr;

    TraversalAttrWrapper();
    TraversalAttrWrapper(const UserAttr & user_attr_);
};

template <class UserTraversal>
class Traversal : public AstTopDownBottomUpProcessing<
                           TraversalAttrWrapper<UserTraversal, typename UserTraversal::InheritedAttr>,
                           typename UserTraversal::SynthesizedAttr
                         > {
  public:
    struct graph_t {
      std::string tag;

      graph_t * parent;
      std::map<std::string, graph_t *> children;

      std::vector<SgNode *> ast_nodes;

      graph_t * subgraph(const std::string & tag);
    };

    typedef TraversalAttrWrapper<UserTraversal, typename UserTraversal::InheritedAttr> InheritedAttr;
    typedef typename UserTraversal::SynthesizedAttr SynthesizedAttr;
    typedef typename UserTraversal::InheritedAttr UserInheritedAttr;
    typedef typename TraversalAttrWrapper<UserTraversal, typename UserTraversal::InheritedAttr>::viz_attr_t VizAttr;

  protected:
    std::map<SgNode *, InheritedAttr> inherited_attr_map;
    std::map<SgNode *, SynthesizedAttr> synthesized_attr_map;
    graph_t * universe_graph;

  public:
    Traversal() :
      AstTopDownBottomUpProcessing<InheritedAttr, SynthesizedAttr>(),
      inherited_attr_map(), synthesized_attr_map(), universe_graph(new graph_t())
    {
      universe_graph->tag = "universe";
    }

    graph_t * getGraph(const std::string & tag, graph_t * parent = NULL);

    graph_t * mergeGraph(graph_t * g1, graph_t * g2, bool fuse = false);

    graph_t * ancestorGraph(graph_t * g1, graph_t * g2);

    InheritedAttr makeAttribute(const std::string & tag);
    InheritedAttr makeAttribute(const std::string & tag, const UserInheritedAttr & user_attr);

    void collectGraphElements(
           graph_t * graph,
           std::vector<graph_t *> & graphs,
           std::map<graph_t *, std::set<std::pair<SgNode *, SgNode *> > > & edge_map,
           std::map<SgNode *, graph_t *> node_graph_map
    );

    void editGraphElement(
           graph_t * graph,
           const std::vector<graph_t *> & clusters,
           const std::map<graph_t *, std::set<std::pair<SgNode *, SgNode *> > > & edge_map,
           std::map<graph_t *, GraphVizObjects::cluster_desc_t> & cluster_desc,
           std::map<SgNode *, GraphVizObjects::node_desc_t> & node_desc,
           std::map<std::pair<SgNode *, SgNode *>, GraphVizObjects::edge_desc_t> & edge_desc
         );

    void toGraphViz(const GraphVizObjects::graph_desc_t & desc, std::ostream & out, std::string indent = std::string("  "));

    void toGraphViz(const GraphVizObjects::cluster_desc_t & desc, std::ostream & out, std::string indent = std::string("  "));

    void toGraphViz(SgNode * node, const GraphVizObjects::node_desc_t & desc, std::ostream & out, std::string indent = std::string("  "));

    void toGraphViz(std::pair<SgNode *, SgNode *> edge, const GraphVizObjects::edge_desc_t & desc, std::ostream & out, std::string indent = std::string("  "));

    void toGraphViz(
           graph_t * graph,
           const std::vector<graph_t *> & clusters,
           const std::map<graph_t *, std::set<std::pair<SgNode *, SgNode *> > > & edge_map,
           const std::map<graph_t *, GraphVizObjects::cluster_desc_t> & cluster_desc,
           const std::map<SgNode *, GraphVizObjects::node_desc_t> & node_desc,
           const std::map<std::pair<SgNode *, SgNode *>, GraphVizObjects::edge_desc_t> & edge_desc,
           std::ostream & out, std::string indent = std::string("  ")
         );

    void toGraphViz(const std::string & tag, std::ostream & out);

  protected:
    const InheritedAttr & getInheritedAttribute(SgNode *) const;
    const VizAttr & getVizAttribute(SgNode *) const;

    const SynthesizedAttr & getSynthesizedAttribute(SgNode *) const;

    InheritedAttr evaluateInheritedAttribute(SgNode * node, InheritedAttr attr);

    SynthesizedAttr evaluateSynthesizedAttribute(SgNode * node, InheritedAttr attr, StackFrameVector<SynthesizedAttr> attrs);

    VizAttr evaluateVizAttribute(SgNode * node, VizAttr);
};

template <class UserTraversal, class UserAttr>
TraversalAttrWrapper<UserTraversal, UserAttr>::TraversalAttrWrapper() :
  viz_attr(),
  user_attr()
{}

template <class UserTraversal, class UserAttr>
TraversalAttrWrapper<UserTraversal, UserAttr>::TraversalAttrWrapper(const UserAttr & user_attr_) :
  viz_attr(),
  user_attr(user_attr_)
{}

template <class UserTraversal>
typename Traversal<UserTraversal>::graph_t * Traversal<UserTraversal>::graph_t::subgraph(const std::string & tag) {
  typename Traversal<UserTraversal>::graph_t * res = children[tag];
  if (res == NULL) {
    res = new Traversal<UserTraversal>::graph_t();
      res->tag = tag;
      res->parent = this;
    children[tag] = res;
  }
  return res;
}

template <class UserTraversal>
typename Traversal<UserTraversal>::graph_t * Traversal<UserTraversal>::getGraph(const std::string & tag, typename Traversal<UserTraversal>::graph_t * parent) {
  if (parent != NULL)
    return parent->subgraph(tag);
  else if (universe_graph != NULL)
    return universe_graph->subgraph(tag);
  else assert(false);
}

template <class UserTraversal>
typename Traversal<UserTraversal>::graph_t * Traversal<UserTraversal>::mergeGraph(typename Traversal<UserTraversal>::graph_t * g1, typename Traversal<UserTraversal>::graph_t * g2, bool fuse) {
  typename Traversal<UserTraversal>::graph_t * res = new Traversal<UserTraversal>::graph_t();
  if (fuse && g1->tag == g2->tag) {
    res->tag = g1->tag;

    res->ast_nodes.insert(res->ast_nodes.begin(), g1->ast_nodes.begin(), g1->ast_nodes.end());
    res->ast_nodes.insert(res->ast_nodes.begin(), g2->ast_nodes.begin(), g2->ast_nodes.end());

    typename std::map<std::string, typename Traversal<UserTraversal>::graph_t *>::iterator it_1 = g1->children.begin();
    typename std::map<std::string, typename Traversal<UserTraversal>::graph_t *>::iterator it_2 = g2->children.begin();
    while (it_1 != g1->children.end() && it_2 != g2->children.end()) {
      if (it_1->second->tag == it_2->second->tag) {
        graph_t * merged = mergeGraph(it_1->second, it_2->second, true);
        res->children[merged->tag] = merged;
        it_1++; it_2++;
      }
      else if (it_1->second->tag < it_2->second->tag) {
        res->children[it_1->second->tag] = it_1->second;
        it_1++;
      }
      else if (it_1->second->tag > it_2->second->tag) {
        res->children[it_2->second->tag] = it_2->second;
        it_2++;
      }
      else assert(false);
    }
    while (it_1 != g1->children.end()) {
      res->children[it_1->second->tag] = it_1->second;
      it_1++;
    }
    while (it_2 != g2->children.end()) {
      res->children[it_2->second->tag] = it_2->second;
      it_2++;
    }
  }
  else {
    res->tag = g1->tag + "_" + g2->tag;

    res->children[g1->tag] = g1;
    g1->parent->children.erase(g1->tag);
    g1->parent = res;

    res->children[g2->tag] = g2;
    g2->parent->children.erase(g2->tag);
    g2->parent = res;
  }
  return res;
}

template <class UserTraversal>
typename Traversal<UserTraversal>::graph_t * Traversal<UserTraversal>::ancestorGraph(graph_t * g1, graph_t * g2) {
  assert(g1 != NULL);
  assert(g2 != NULL);

  std::vector<typename Traversal<UserTraversal>::graph_t *> l1;
  while (g1 != NULL) {
    l1.push_back(g1);
    g1 = g1->parent;
  }

  std::vector<typename Traversal<UserTraversal>::graph_t *> l2;
  while (g2 != NULL) {
    l2.push_back(g2);
    g2 = g2->parent;
  }

  assert(l1.size() > 0 && l2.size() > 0);

  if (l1[0] != l2[0]) return NULL;

  typename std::vector<typename Traversal<UserTraversal>::graph_t *>::iterator it1 = l1.begin();
  typename std::vector<typename Traversal<UserTraversal>::graph_t *>::iterator it2 = l2.begin();
  while (it1 != l1.end() && it2 != l2.end() && *it1 == *it2) { it1++; it2++; };
  assert(it1 != l1.end() && it2 != l2.end());
  it1--;
  it2--;
  assert(*it1 == *it2);
  return *it1;
}

template <class UserTraversal>
typename Traversal<UserTraversal>::InheritedAttr Traversal<UserTraversal>::makeAttribute(const std::string & tag) {
  typename Traversal<UserTraversal>::InheritedAttr res;

  res.viz_attr.node   = NULL;
  res.viz_attr.parent = NULL;
  res.viz_attr.graph  = getGraph(tag, NULL);
  res.viz_attr.stopped_by    = NULL;
  res.viz_attr.skipped_since = NULL;

  return res;
}

template <class UserTraversal>
typename Traversal<UserTraversal>::InheritedAttr Traversal<UserTraversal>::makeAttribute(const std::string & tag, const typename UserTraversal::InheritedAttr & user_attr) {
  typename Traversal<UserTraversal>::InheritedAttr res;

  res.viz_attr.node   = NULL;
  res.viz_attr.parent = NULL;
  res.viz_attr.graph  = getGraph(tag, NULL);
  res.viz_attr.stopped_by    = NULL;
  res.viz_attr.skipped_since = NULL;

  res.user_attr = user_attr;

  return res;
}

template <class UserTraversal>
void Traversal<UserTraversal>::collectGraphElements(
  graph_t * graph,
  std::vector<graph_t *> & clusters,
  std::map<graph_t *, std::set<std::pair<SgNode *, SgNode *> > > & edge_map,
  std::map<SgNode *, graph_t *> node_graph_map
) {
  edge_map[graph];
  std::vector<SgNode *>::iterator it_node;
  for (it_node = graph->ast_nodes.begin(); it_node != graph->ast_nodes.end(); it_node++) {
    node_graph_map[*it_node] = graph;
    SgNode * parent = getVizAttribute(*it_node).parent;
    graph_t * parent_graph = node_graph_map[parent];
    edge_map[parent_graph].insert(std::pair<SgNode *, SgNode *>(parent, *it_node));
  }

  typename std::map<std::string, graph_t *>::iterator it_cluster;
  for (it_cluster = graph->children.begin(); it_cluster != graph->children.end(); it_cluster++) {
    clusters.push_back(it_cluster->second);
    collectGraphElements(it_cluster->second, clusters, edge_map, node_graph_map);
  }
}

template <class UserTraversal>
void Traversal<UserTraversal>::editGraphElement(
  graph_t * graph,
  const std::vector<graph_t *> & clusters,
  const std::map<graph_t *, std::set<std::pair<SgNode *, SgNode *> > > & edge_map,
  std::map<graph_t *, GraphVizObjects::cluster_desc_t> & cluster_desc,
  std::map<SgNode *, GraphVizObjects::node_desc_t> & node_desc,
  std::map<std::pair<SgNode *, SgNode *>, GraphVizObjects::edge_desc_t> & edge_desc
) {

  std::vector<SgNode *>::iterator it_node;
  for (it_node = graph->ast_nodes.begin(); it_node != graph->ast_nodes.end(); it_node++) {
    UserTraversal::GraphViz::edit(*it_node, node_desc[*it_node], getInheritedAttribute(*it_node).user_attr, getSynthesizedAttribute(*it_node));
    SgNode * parent = getVizAttribute(*it_node).parent;
    UserTraversal::GraphViz::edit(parent, *it_node, edge_desc[std::pair<SgNode *, SgNode *>(parent, *it_node)]);
  }

  typename std::map<std::string, graph_t *>::iterator it_cluster;
  for (it_cluster = graph->children.begin(); it_cluster != graph->children.end(); it_cluster++) {
    UserTraversal::GraphViz::edit(it_cluster->second, cluster_desc[it_cluster->second]);
    editGraphElement(it_cluster->second, clusters, edge_map, cluster_desc, node_desc, edge_desc);
  }
}

template <class UserTraversal>
void Traversal<UserTraversal>::toGraphViz(const GraphVizObjects::graph_desc_t & desc, std::ostream & out, std::string indent) {
  out << indent << "label=\"" << desc.label << "\";" << std::endl;
  out << indent << "color=\"" << desc.color << "\";" << std::endl;
  out << indent << "style=\"" << desc.style << "\";" << std::endl;
  out << indent << "rankdir=\"" << desc.rankdir << "\";" << std::endl;
  out << indent << "ranksep=" << desc.ranksep << ";" << std::endl;
}

template <class UserTraversal>
void Traversal<UserTraversal>::toGraphViz(const GraphVizObjects::cluster_desc_t & desc, std::ostream & out, std::string indent) {
  out << indent << "label=\"" << desc.label << "\";" << std::endl;
  out << indent << "color=\"" << desc.color << "\";" << std::endl;
  out << indent << "style=\"" << desc.style << "\";" << std::endl;
  out << indent << "penwidth=" << desc.penwidth << ";" << std::endl;
}

void print_node_name(SgNode * node, std::ostream & out) {
  out << "node_" << node->class_name() << "_" << std::hex << node;
}

template <class UserTraversal>
void Traversal<UserTraversal>::toGraphViz(SgNode * node, const GraphVizObjects::node_desc_t & desc, std::ostream & out, std::string indent) {

  print_node_name(node, out);

  out << " [label=\"" << desc.label << "\",";
  out << "  color=\"" << desc.color << "\",";
  out << "  style=\"" << desc.style << "\",";
  out << "  shape=\"" << desc.shape << "\",";
  out << "  fillcolor=\"" << desc.fillcolor << "\",";
  out << "  penwidth=" << desc.penwidth << "," << std::endl;
  out << "  URL=\"" << desc.url << "\"];" << std::endl;
}

template <class UserTraversal>
void Traversal<UserTraversal>::toGraphViz(std::pair<SgNode *, SgNode *> edge, const GraphVizObjects::edge_desc_t & desc, std::ostream & out, std::string indent) {

  print_node_name(edge.first, out);
  out << " -> ";
  print_node_name(edge.second, out);

  out << " [label=\"" << desc.label << "\",";
  out << "  color=\"" << desc.color << "\",";
  out << "  style=\"" << desc.style << "\",";
  out << "  minlen=" << desc.minlen << ",";
  out << "  penwidth=" << desc.penwidth << ",";
  out << "  constraint=" << desc.constraint << "];" << std::endl;
}

template <class UserTraversal>
void Traversal<UserTraversal>::toGraphViz(
  graph_t * graph,
  const std::vector<graph_t *> & clusters,
  const std::map<graph_t *, std::set<std::pair<SgNode *, SgNode *> > > & edge_map,
  const std::map<graph_t *, GraphVizObjects::cluster_desc_t> & cluster_desc,
  const std::map<SgNode *, GraphVizObjects::node_desc_t> & node_desc,
  const std::map<std::pair<SgNode *, SgNode *>, GraphVizObjects::edge_desc_t> & edge_desc,
  std::ostream & out, std::string indent
) {
  out << indent << "subgraph cluster_" << graph->tag << " {" << std::endl;

  toGraphViz(cluster_desc.at(graph), out);

  typename std::map<std::string, typename Traversal<UserTraversal>::graph_t *>::iterator it_child;
  for (it_child = graph->children.begin(); it_child != graph->children.end(); it_child++)
    toGraphViz(it_child->second, clusters, edge_map, cluster_desc, node_desc, edge_desc, out, indent + "  ");

  std::vector<SgNode *>::iterator it_node;
  for (it_node = graph->ast_nodes.begin(); it_node != graph->ast_nodes.end(); it_node++) {
    toGraphViz(*it_node, node_desc.at(*it_node), out, indent + "  ");
  }

  std::set<std::pair<SgNode *, SgNode *> >::iterator it_edge;
  for (it_edge = edge_map.at(graph).begin(); it_edge != edge_map.at(graph).end(); it_edge++) {
    toGraphViz(*it_edge, edge_desc.at(*it_edge), out, indent + "  ");
  }

  out << "}" << std::endl;
}

template <class UserTraversal>
void Traversal<UserTraversal>::toGraphViz(const std::string & tag, std::ostream & out) {
  typename Traversal<UserTraversal>::graph_t * graph = getGraph(tag, NULL);

  std::vector<graph_t *> clusters;
  std::map<graph_t *, std::set<std::pair<SgNode *, SgNode *> > > edge_map;

  collectGraphElements(graph, clusters, edge_map, std::map<SgNode *, graph_t *>());

  GraphVizObjects::graph_desc_t graph_desc;
  std::map<graph_t *, GraphVizObjects::cluster_desc_t> cluster_desc;
  std::map<SgNode *, GraphVizObjects::node_desc_t> node_desc;
  std::map<std::pair<SgNode *, SgNode *>, GraphVizObjects::edge_desc_t> edge_desc;

  UserTraversal::GraphViz::edit(graph_desc);
  editGraphElement(graph, clusters, edge_map, cluster_desc, node_desc, edge_desc);

  out << "digraph graph_" << graph->tag << " {" << std::endl;

  toGraphViz(graph_desc, out);

  typename std::map<std::string, typename Traversal<UserTraversal>::graph_t *>::iterator it_child;
  for (it_child = graph->children.begin(); it_child != graph->children.end(); it_child++)
    toGraphViz(it_child->second, clusters, edge_map, cluster_desc, node_desc, edge_desc, out);

  std::vector<SgNode *>::iterator it_node;
  for (it_node = graph->ast_nodes.begin(); it_node != graph->ast_nodes.end(); it_node++) {
    toGraphViz(*it_node, node_desc.at(*it_node), out);
  }

  std::set<std::pair<SgNode *, SgNode *> >::iterator it_edge;
  for (it_edge = edge_map.at(graph).begin(); it_edge != edge_map.at(graph).end(); it_edge++) {
    toGraphViz(*it_edge, edge_desc.at(*it_edge), out);
  }

  out << "}" << std::endl;
}

template <class UserTraversal>
const typename Traversal<UserTraversal>::InheritedAttr & Traversal<UserTraversal>::getInheritedAttribute(SgNode * node) const {
  return inherited_attr_map.at(node);
}

template <class UserTraversal>
const typename Traversal<UserTraversal>::VizAttr & Traversal<UserTraversal>::getVizAttribute(SgNode * node) const {
  return inherited_attr_map.at(node).viz_attr;
}

template <class UserTraversal>
const typename Traversal<UserTraversal>::SynthesizedAttr & Traversal<UserTraversal>::getSynthesizedAttribute(SgNode * node) const {
  return synthesized_attr_map.at(node);
}

template <class UserTraversal>
typename Traversal<UserTraversal>::VizAttr Traversal<UserTraversal>::evaluateVizAttribute(
    SgNode * node, typename Traversal<UserTraversal>::VizAttr attr
) {
  typename Traversal<UserTraversal>::VizAttr res;
    res.node          = node;
    res.parent        = NULL;
    res.graph         = NULL;
    res.stopped_by    = NULL;
    res.skipped_since = NULL;

  assert(!(attr.stopped_by != NULL && attr.skipped_since != NULL));

  // Traversal is stopped
  if (attr.stopped_by != NULL) {
    // Does the user want to start a new graph for this subtree?
    res.graph = UserTraversal::Policies::startOn(node);

    if (res.graph == NULL) {
      // Stay stopped
      res.stopped_by = attr.stopped_by;
      return res;
    }
  }

  // parent can be: NULL if new graph, the closest parent added to the graph, or simply the previous node
  if (attr.stopped_by != NULL)
    res.parent = NULL;
  else if (attr.skipped_since != NULL)
    res.parent = attr.skipped_since;
  else
    res.parent = attr.node;

  // This should happen when starting the traversal
  if (res.parent == NULL && res.graph == NULL) {
    assert(attr.graph != NULL);
    res.graph = attr.graph;
  }

  // Only one of the parent and graph can be filled
  assert((res.parent != NULL) xor (res.graph != NULL));

  // Does the user want to skip this node? (cannot skip a node if new graph)
  if (res.parent != NULL && UserTraversal::Policies::skip(node)) {
    res.skipped_since = res.parent; // last node added to the graph
    res.parent = NULL;
    res.graph  = NULL;
    return res;
  }

  // Does the user want to stop after this node?
  if (UserTraversal::Policies::stopAfter(node)) {
    res.stopped_by = node;
  }

  // Get graph from parent
  if (res.graph == NULL)
    res.graph = getVizAttribute(res.parent).graph;
  assert(res.graph != NULL);

  // Let the user create a cluster for the subtree if he wants. 
  res.graph = UserTraversal::Policies::getSubgraph(node, res.graph);
  assert(res.graph != NULL);

  res.graph->ast_nodes.push_back(node);

  return res;
}

template <class UserTraversal>
typename Traversal<UserTraversal>::InheritedAttr Traversal<UserTraversal>::evaluateInheritedAttribute(SgNode * node, typename Traversal<UserTraversal>::InheritedAttr attr) {
  typename UserTraversal::InheritedAttr user_attr = UserTraversal::evaluateInheritedAttribute(node, attr.user_attr);

  typename Traversal<UserTraversal>::InheritedAttr res(user_attr);

  res.viz_attr = evaluateVizAttribute(node, attr.viz_attr);

  inherited_attr_map[node] = res;
  return res;
}

template <class UserTraversal>
typename Traversal<UserTraversal>::SynthesizedAttr Traversal<UserTraversal>::evaluateSynthesizedAttribute(SgNode * node, InheritedAttr attr, StackFrameVector<SynthesizedAttr> attrs) {
  typename UserTraversal::SynthesizedAttr res = UserTraversal::evaluateSynthesizedAttribute(node, attr.user_attr, attrs);
  synthesized_attr_map[node] = res;
  return res;
}

}

}

#endif /* __ROSE_VIZ_TRAVERSAL_HPP__ */

