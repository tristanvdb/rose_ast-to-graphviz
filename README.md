AST to GraphViz
===============

A programmable GraphViz (dot) generator for ROSE Compiler, it uses ROSE's [AstTopDownProcessing](http://rosecompiler.org/ROSE_HTML_Reference/classAstTopDownProcessing.html).

Prototype of the top-down GraphViz Generator:
```c++
template <class DownAttr_tpl>
class TopDown : public AstTopDownProcessing<DownAttr<DownAttr_tpl> > {
  protected:
    std::map<SgNode *, NodeDesc> p_nodes;
    std::map<std::pair<SgNode *, SgNode *>, EdgeDesc> p_edges;

    void printNodeName(std::ostream & out, const SgNode * node) const;
    void toDot(std::ostream & out, const SgNode * node, const NodeDesc & desc) const;
    void toDot(std::ostream & out, const std::pair<SgNode *, SgNode *> & edge, const EdgeDesc & desc) const;

  public:
    TopDown();

    void toDot(std::ostream & out) const;

    NodeDesc & addNode(SgNode * node);

    EdgeDesc & addEdge(SgNode * node, SgNode * parent);

    void clear();

  protected:
    virtual bool skipNode(SgNode * node, const DownAttr_tpl & attr_in);
    virtual void computeAttr(SgNode * node, const DownAttr_tpl & attr_in, DownAttr_tpl & attr_out);

    virtual bool stopAtNode(SgNode * node, const DownAttr_tpl & attr_in, const DownAttr_tpl & attr_out);

    virtual void editNodeDesc(SgNode * node, const DownAttr_tpl & attr_in, const DownAttr_tpl & attr_out, NodeDesc & node_desc) = 0;
    virtual void editEdgeDesc(SgNode * node, const DownAttr_tpl & attr_in, const DownAttr_tpl & attr_out, SgNode * parent, EdgeDesc & edge_desc) = 0;

    DownAttr<DownAttr_tpl> evaluateInheritedAttribute(SgNode * node, DownAttr<DownAttr_tpl> attr_in);
};
```
Node and edge descriptors:
```c++
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
```

A minimal implementation:
```c++
struct EmptyDownAttr {};

class Basic : public AST2GraphViz::TopDown<EmptyDownAttr> {
  public:
    typedef AST2GraphViz::DownAttr<EmptyDownAttr> DownAttr;
    typedef AST2GraphViz::NodeDesc NodeDesc;
    typedef AST2GraphViz::EdgeDesc EdgeDesc;

  protected:
    void editNodeDesc(SgNode * node, const EmptyDownAttr & attr_in, const EmptyDownAttr & attr_out, NodeDesc & node_desc) {
      node_desc.label = node->class_name().getString();
      node_desc.shape = "box";
      node_desc.color = "black";
      node_desc.fillcolor = "white";
    }

    void editEdgeDesc(SgNode * node, const EmptyDownAttr & attr_in, const EmptyDownAttr & attr_out, SgNode * parent, EdgeDesc & edge_desc) {
      edge_desc.label = "";
      edge_desc.color = "black";
      edge_desc.constraint = true;
    }
};
```

Using ROSE's interface, we can extract the information we want from each node, and we edit the descriptors accordingly.
Other virtual method can be reimplemented for more control:
* skipNode: skips node in the traversal. In the resulting graph, the children of the skipped node will have the parent of the skipped node for parent.
* stopAtNode: stops exporting the AST at this node
* computeAttr: enables to graft a top-down analysis. It is called before all other overloadable methods. These method receive both input and output version of the user defined attribute.



