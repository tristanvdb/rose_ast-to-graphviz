
#include "Viz/traversal.hpp"

#ifndef __ROSE_VIZ_TRAVERSALS_DEFAULT_HPP__
#define __ROSE_VIZ_TRAVERSALS_DEFAULT_HPP__

namespace Rose {

namespace Viz {

namespace Traversals {

struct Default {
  class InheritedAttr {};
  class SynthesizedAttr {};

  static InheritedAttr evaluateInheritedAttribute(SgNode * node, InheritedAttr attr);
  static SynthesizedAttr evaluateSynthesizedAttribute(SgNode * node, InheritedAttr attr, StackFrameVector<SynthesizedAttr> attrs);

  struct Policies {
    static bool stopAfter(SgNode *);
    static bool skip(SgNode *);

    static Traversal<Default>::graph_t * startOn(SgNode *);
    static Traversal<Default>::graph_t * getSubgraph(SgNode * node, Traversal<Default>::graph_t * graph);
  };

  struct GraphViz {
    static void edit(SgNode * node, GraphVizObjects::node_desc_t & desc, const InheritedAttr & inhr_attr, const SynthesizedAttr & synth_attr);
    static void edit(SgNode * node, SgNode * parent, GraphVizObjects::edge_desc_t & desc);
    static void edit(Traversal<Default>::graph_t * graph, GraphVizObjects::cluster_desc_t & desc);
    static void edit(GraphVizObjects::graph_desc_t & desc);
  };
};

}

}

}

#endif /* __ROSE_VIZ_TRAVERSALS_DEFAULT_HPP__ */

