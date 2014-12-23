
#ifndef __AST_TO_GRAPHVIZ__CLUSTER_HPP__
#define __AST_TO_GRAPHVIZ__CLUSTER_HPP__

#include "AST2GraphViz/default.hpp"

namespace AST2GraphViz {

class Cluster : public Default {
  public:
    typedef GraphVizAttr<EmptyAttr>::Inherited   Inherited;
    typedef GraphVizAttr<EmptyAttr>::Synthesized Synthesized;
    typedef EmptyAttr::Inherited   UserInherited;
    typedef EmptyAttr::Synthesized UserSynthesized;

    typedef GraphVizTraversal<EmptyAttr>::graph_t graph_t;

  public:
    Cluster();
    ~Cluster();

  protected:
    virtual graph_t * getGraphInherited(
                   graph_t * suggested_graph, SgNode * node, SgNode * parent,
                   const UserInheritedAttr & inherited_attr
                 );

    virtual graph_t * getGraphSynthesized(
                   graph_t * suggested_graph, SgNode * node, SgNode * parent,
                   const UserInheritedAttr & inherited_attr, const UserSynthesizedAttr & synthetized_attr
                 );
};

}

#endif /* __AST_TO_GRAPHVIZ__CLUSTER_HPP__ */

