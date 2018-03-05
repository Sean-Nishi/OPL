//This is the header that contains the nodes

#include <vector>

class Node {
 public:
  //values
  Node* parent;
  vector<Node> children;

  //constructor and destructor
  Node ();
  ~Node();

  //adding and retreiving a child
  void _add_child(Node* child);
  void _get_child();
  
};

Node::Node () {
  parent = NULL;
  children = NULL;
}

Node::~Node () {

}

void Node::_add_child(Node* child) {
  children.push(child);
}

void Node::_get_child() {
  
}
