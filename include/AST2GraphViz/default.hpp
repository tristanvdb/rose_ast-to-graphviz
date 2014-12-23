
#ifndef __AST_TO_GRAPHVIZ__DEFAULT_HPP__
#define __AST_TO_GRAPHVIZ__DEFAULT_HPP__

#include "AST2GraphViz/ast-to-graphviz.hpp"

namespace AST2GraphViz {

struct EmptyAttr {
  struct Inherited {};
  struct Synthesized {};
};

class Default : public GraphVizTraversal<EmptyAttr> {
  public:
    typedef GraphVizAttr<EmptyAttr>::Inherited   Inherited;
    typedef GraphVizAttr<EmptyAttr>::Synthesized Synthesized;
    typedef EmptyAttr::Inherited   UserInherited;
    typedef EmptyAttr::Synthesized UserSynthesized;

    typedef GraphVizTraversal<EmptyAttr>::graph_t graph_t;

  public:
    Default();
    ~Default();

  protected:
    void editNodeDesc(
                   SgNode * node,
                   const UserInherited & inherited_attr, const UserSynthesized & synthetized_attr,
                   NodeDesc & node_desc
                 );

    void editParentEdgeDesc(
                   SgNode * node, SgNode * parent,
                   const UserInherited & inherited_attr, const UserSynthesized & synthetized_attr,
                   EdgeDesc & edge_desc
                 );
};

}

#endif /* __AST_TO_GRAPHVIZ__DEFAULT_HPP__ */

