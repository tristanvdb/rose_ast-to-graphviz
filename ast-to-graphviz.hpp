
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

template <class DownAttr_tpl>
struct DownAttr {

  DownAttr() :
    node(NULL),
    stopped(false),
    down_attr()
  {}

  DownAttr(const DownAttr_tpl & down_attr_) :
    node(NULL),
    stopped(false),
    down_attr(down_attr_)
  {}

  DownAttr(SgNode * node_, bool stopped_) :
    node(node_),
    stopped(stopped_),
    down_attr()
  {}

  DownAttr(SgNode * node_, bool stopped_, const DownAttr_tpl & down_attr_) :
    node(node_),
    stopped(stopped_),
    down_attr(down_attr_)
  {}

  SgNode * node;
  bool stopped;
  DownAttr_tpl down_attr;
};

template <class DownAttr_tpl>
class TopDown : public AstTopDownProcessing<DownAttr<DownAttr_tpl> > {
  protected:
    std::map<SgNode *, NodeDesc> p_nodes;
    std::map<std::pair<SgNode *, SgNode *>, EdgeDesc> p_edges;

    void printNodeName(std::ostream & out, const SgNode * node) const {
      out << "node_" << node;
    }

    void toDot(std::ostream & out, const SgNode * node, const NodeDesc & desc) const {
      printNodeName(out, node);
      out << " [label=\"" << desc.label << "\", shape=" << desc.shape << ", color=\"" << desc.color << "\", style=filled, fillcolor=" << desc.fillcolor << "];" << std::endl;
    }

    void toDot(std::ostream & out, const std::pair<SgNode *, SgNode *> & edge, const EdgeDesc & desc) const {
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

  public:
    TopDown() : p_nodes(), p_edges() {}

    void toDot(std::ostream & out) const {
      typename std::map<SgNode *, NodeDesc>::const_iterator it_node;
      for (it_node = p_nodes.begin(); it_node != p_nodes.end(); it_node++)
        toDot(out, it_node->first, it_node->second);
      typename std::map<std::pair<SgNode *, SgNode *>, EdgeDesc>::const_iterator it_edge;
      for (it_edge = p_edges.begin(); it_edge != p_edges.end(); it_edge++)
        toDot(out, it_edge->first, it_edge->second);
    }

    NodeDesc & addNode(SgNode * node) {
      return p_nodes[node];
    }

    EdgeDesc & addEdge(SgNode * node, SgNode * parent) {
      std::pair<SgNode *, SgNode *> edge(parent, node);
      return p_edges[edge];
    }

    void clear() {
      p_nodes.clear();
      p_edges.clear();
    }

  protected:
    virtual bool skipNode(SgNode * node, const DownAttr_tpl & attr_in) { return false; }
    virtual void computeAttr(SgNode * node, const DownAttr_tpl & attr_in, DownAttr_tpl & attr_out) {}

    virtual bool stopAtNode(SgNode * node, const DownAttr_tpl & attr_in, const DownAttr_tpl & attr_out) { return false; }

    virtual void editNodeDesc(SgNode * node, const DownAttr_tpl & attr_in, const DownAttr_tpl & attr_out, NodeDesc & node_desc) = 0;
    virtual void editEdgeDesc(SgNode * node, const DownAttr_tpl & attr_in, const DownAttr_tpl & attr_out, SgNode * parent, EdgeDesc & edge_desc) = 0;

    DownAttr<DownAttr_tpl> evaluateInheritedAttribute(SgNode * node, DownAttr<DownAttr_tpl> attr_in) {
//    std::cerr << node->class_name() << " = " << skipNode(node, attr_in.down_attr) << std::endl;

      DownAttr<DownAttr_tpl> attr_out(node, false);
      computeAttr(node, attr_in.down_attr, attr_out.down_attr);

      if (attr_in.stopped || skipNode(node, attr_in.down_attr))
        return DownAttr<DownAttr_tpl>(attr_in.node, attr_in.stopped, attr_in.down_attr);

      editNodeDesc(node, attr_in.down_attr, attr_out.down_attr, addNode(node));

      if (attr_in.node != NULL)
        editEdgeDesc(node, attr_in.down_attr, attr_out.down_attr, attr_in.node, addEdge(node, attr_in.node));

      attr_out.stopped = stopAtNode(node, attr_in.down_attr, attr_out.down_attr);

      return attr_out;
    }
};

struct EmptyDownAttr {};

class Basic : public AST2GraphViz::TopDown<EmptyDownAttr> {
  public:
    typedef AST2GraphViz::DownAttr<EmptyDownAttr> DownAttr;
    typedef AST2GraphViz::NodeDesc NodeDesc;
    typedef AST2GraphViz::EdgeDesc EdgeDesc;

  protected:
    virtual bool skipNode(SgNode * node, const EmptyDownAttr & attr_in);

    virtual void editNodeDesc(SgNode * node, const EmptyDownAttr & attr_in, const EmptyDownAttr & attr_out, NodeDesc & node_desc);

    virtual void editEdgeDesc(SgNode * node, const EmptyDownAttr & attr_in, const EmptyDownAttr & attr_out, SgNode * parent, EdgeDesc & edge_desc);
};

}

#endif /* __AST_TO_GRAPHVIZ_HPP__ */

