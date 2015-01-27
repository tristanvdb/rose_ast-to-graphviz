
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

      ~graph_t();
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
    Traversal();
    ~Traversal();

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

    static std::string getNodeName(SgNode * node);

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

}

}

#include "Viz/traversal.tpp"

#endif /* __ROSE_VIZ_TRAVERSAL_HPP__ */

