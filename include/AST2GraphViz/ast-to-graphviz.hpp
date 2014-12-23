
#ifndef __AST_TO_GRAPHVIZ_HPP__
#define __AST_TO_GRAPHVIZ_HPP__

#include "rose.h"

#include <string>
#include <sstream>
#include <map>
#include <utility>

namespace AST2GraphViz {

struct NodeDesc {
  std::string label;
  std::string shape;
  std::string color;
  std::string fillcolor;
};

struct EdgeDesc {
  std::string label;
  std::string color;
  bool constraint;
};

template <class TraversalAttr_tpl>
class GraphVizTraversal;

template <class TraversalAttr_tpl>
struct GraphVizAttr {
  struct Inherited {
    Inherited();

    Inherited(SgNode * node_, const typename TraversalAttr_tpl::Inherited & user_attr_);

    SgNode * node;
    SgNode * parent;
    typename GraphVizTraversal<TraversalAttr_tpl>::graph_t * graph;
    typename TraversalAttr_tpl::Inherited user_attr;
  };

  struct Synthesized {
    Synthesized(SgNode * node_ = NULL);

    SgNode * node;
    SgNode * parent;
    typename GraphVizTraversal<TraversalAttr_tpl>::graph_t * graph;
    typename TraversalAttr_tpl::Synthesized user_attr;
  };
};

template <class TraversalAttr_tpl>
class GraphVizTraversal : public AstTopDownBottomUpProcessing<
                                     typename GraphVizAttr<TraversalAttr_tpl>::Inherited,
                                     typename GraphVizAttr<TraversalAttr_tpl>::Synthesized
                                 > {
  public:
    typedef typename GraphVizAttr<TraversalAttr_tpl>::Inherited   InheritedAttr;
    typedef typename GraphVizAttr<TraversalAttr_tpl>::Synthesized SynthesizedAttr;
    typedef typename TraversalAttr_tpl::Inherited UserInheritedAttr;
    typedef typename TraversalAttr_tpl::Synthesized UserSynthesizedAttr;

    struct graph_t {
      graph_t(graph_t * parent_);

      ~graph_t();

      graph_t * parent;
      std::vector<SgNode *> nodes;
      std::map<std::string, graph_t *> subgraphs;

      graph_t * getSubgraph(const std::string & label);
    };

  protected:
    graph_t * p_graph;
    std::map<SgNode *, NodeDesc> p_nodes;
    std::map<std::pair<SgNode *, SgNode *>, EdgeDesc> p_edges;

  protected:
    InheritedAttr evaluateInheritedAttribute(SgNode * node, InheritedAttr attr);

    SynthesizedAttr evaluateSynthesizedAttribute(SgNode * node, InheritedAttr attr, StackFrameVector<SynthesizedAttr> attrs);

    virtual void evaluateInheritedAttribute(SgNode * node, const UserInheritedAttr & attr, UserInheritedAttr & result) {}

    virtual void evaluateSynthesizeAttribute(
                   SgNode * node,
                   const UserInheritedAttr & attr, const std::vector<UserSynthesizedAttr> & attrs,
                   UserSynthesizedAttr & result
                 ) {}

  protected:
    virtual void editNodeDesc(
                   SgNode * node,
                   const UserInheritedAttr & inherited_attr, const UserSynthesizedAttr & synthetized_attr,
                   NodeDesc & node_desc
                 ) = 0;

    virtual void editParentEdgeDesc(
                   SgNode * node, SgNode * parent,
                   const UserInheritedAttr & inherited_attr, const UserSynthesizedAttr & synthetized_attr,
                   EdgeDesc & edge_desc
                 ) = 0;

    virtual SgNode * getParentInherited(
                   SgNode * suggested_parent, SgNode * node,
                   const UserInheritedAttr & inherited_attr
                 ) { return suggested_parent; };

    virtual SgNode * getParentSynthesized(
                   SgNode * suggested_parent, SgNode * node,
                   const UserInheritedAttr & inherited_attr, const UserSynthesizedAttr & synthetized_attr
                 ) { return suggested_parent; };

    virtual graph_t * getGraphInherited(
                   graph_t * suggested_graph, SgNode * node, SgNode * parent,
                   const UserInheritedAttr & inherited_attr
                 ) { return suggested_graph; }

    virtual graph_t * getGraphSynthesized(
                   graph_t * suggested_graph, SgNode * node, SgNode * parent,
                   const UserInheritedAttr & inherited_attr, const UserSynthesizedAttr & synthetized_attr
                 ) { return suggested_graph; }

  public:
    GraphVizTraversal();

    ~GraphVizTraversal();

    void toDot(std::ostream & out, const std::string & label) const;

    void reset();

    InheritedAttr makeAttribute(bool set_graph = true, SgNode * parent = NULL);

    InheritedAttr makeAttribute(graph_t * graph, SgNode * parent = NULL);

    graph_t * getSubgraph(const std::string & label);

  protected:
    void printNodeName(std::ostream & out, const SgNode * node) const;

    void toDot(std::ostream & out, const SgNode * node, const NodeDesc & desc, std::string indent) const;
    void toDot(std::ostream & out, const std::pair<SgNode *, SgNode *> & edge, const EdgeDesc & desc, std::string indent) const;
    void toDot(std::ostream & out, const graph_t * graph, const std::string & label, std::string indent) const;
};

}

#include "AST2GraphViz/ast-to-graphviz.tpp"

#endif /* __AST_TO_GRAPHVIZ_HPP__ */

