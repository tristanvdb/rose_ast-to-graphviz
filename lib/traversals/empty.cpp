
#include "Viz/Traversals/empty.hpp"

namespace Rose {

namespace Viz {

namespace Traversals {

Empty::InheritedAttr Empty::evaluateInheritedAttribute(SgNode * node, InheritedAttr attr) {
  InheritedAttr res;
  return res;
}

Empty::SynthesizedAttr Empty::evaluateSynthesizedAttribute(SgNode * node, InheritedAttr attr, StackFrameVector<SynthesizedAttr> attrs) {
  SynthesizedAttr res;
  return res;
}

bool Empty::Policies::stopAfter(SgNode *) {
  return false;
}

bool Empty::Policies::skip(SgNode *) {
  return false;
}

Traversal<Empty>::graph_t * Empty::Policies::startOn(SgNode *) {
  return NULL;
}

Traversal<Empty>::graph_t * Empty::Policies::getSubgraph(SgNode * node, Traversal<Empty>::graph_t * graph) {
  return graph;
}

void Empty::GraphViz::edit(SgNode * node, GraphVizObjects::node_desc_t & desc, const InheritedAttr & inhr_attr, const SynthesizedAttr & synth_attr) {
  desc.label="";
  desc.color="black";
  desc.style="";
  desc.shape="box";
  desc.fillcolor="";
  desc.penwidth=1;
}

void Empty::GraphViz::edit(SgNode * node, SgNode * parent, GraphVizObjects::edge_desc_t & desc) {
  desc.label="";
  desc.color="";
  desc.style="";
  desc.minlen=1;
  desc.penwidth=1;
  desc.constraint=true;
}

void Empty::GraphViz::edit(Traversal<Empty>::graph_t * graph, GraphVizObjects::cluster_desc_t & desc) {
  desc.label="";
  desc.color="black";
  desc.style="";
  desc.penwidth=1;
}

void Empty::GraphViz::edit(GraphVizObjects::graph_desc_t & desc) {
  desc.label="";
  desc.color="black";
  desc.style="";
  desc.rankdir="TB";
  desc.ranksep=1;
}

}

}

}

