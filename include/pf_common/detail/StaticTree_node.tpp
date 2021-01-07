

template<typename T, unsigned int ChildCount>
Leaf<T, ChildCount>::Leaf(value_type value) : value(std::move(value)) {}
template<typename T, unsigned int ChildCount>
Leaf<T, ChildCount>::Leaf(const Leaf &other) : parent(nullptr), value(other.value) {}
template<typename T, unsigned int ChildCount>
Leaf<T, ChildCount> &Leaf<T, ChildCount>::operator=(const Leaf &other) {
  if (&other == this) { return *this; }
  value = other.value;
  parent = nullptr;
  return *this;
}
template<typename T, unsigned int ChildCount>
Leaf<T, ChildCount>::Leaf(Leaf &&other) noexcept : parent(other.parent), value(std::move(other.value)) {}
template<typename T, unsigned int ChildCount>
Leaf<T, ChildCount> &Leaf<T, ChildCount>::operator=(Leaf &&other) noexcept {
  value = std::move(other.value);
  parent = other.parent;
  return *this;
}
template<typename T, unsigned int ChildCount>
template<typename F>
void Leaf<T, ChildCount>::traverseDepthFirst(F &&callable) {
  detail::traverseDepthFirstImpl(this, callable);
}
template<typename T, unsigned int ChildCount>
template<typename F>
void Leaf<T, ChildCount>::traverseDepthFirstIf(F &&callable) {
  detail::traverseDepthFirstIfImpl(this, callable);
}
template<typename T, unsigned int ChildCount>
template<typename F>
void Leaf<T, ChildCount>::traverseBreadthFirst(F &&callable) {
  detail::traverseBreadthFirstImpl(this, callable);
}
template<typename T, unsigned int ChildCount>
template<typename F, unsigned int C, typename>
void Leaf<T, ChildCount>::preorder(F &&callable) {
  detail::preorderImpl(this, callable);
}
template<typename T, unsigned int ChildCount>
template<typename F, unsigned int C, typename>
void Leaf<T, ChildCount>::inorder(F &&callable) {
  detail::preorderImpl(this, callable);
}
template<typename T, unsigned int ChildCount>
template<typename F, unsigned int C, typename>
void Leaf<T, ChildCount>::postorder(F &&callable) {
  detail::preorderImpl(this, callable);
}
template<typename T, unsigned int ChildCount>
Node<T, ChildCount> &Leaf<T, ChildCount>::getParent() {
  return *parent;
}
template<typename T, unsigned int ChildCount>
const Node<T, ChildCount> &Leaf<T, ChildCount>::getParent() const {
  return *parent;
}
template<typename T, unsigned int ChildCount>
Node<T, ChildCount> &Leaf<T, ChildCount>::asNode() {
  assert(getType() == NodeType::Node);
  return reinterpret_cast<Node<T, ChildCount> &>(*this);
}
template<typename T, unsigned int ChildCount>
const Node<T, ChildCount> &Leaf<T, ChildCount>::asNode() const {
  assert(getType() == NodeType::Node);
  return reinterpret_cast<const Node<T, ChildCount> &>(*this);
}
template<typename T, unsigned int ChildCount>
template<typename F>
void Leaf<T, ChildCount>::traverseDepthFirstIfNode(F &&callable) {
  detail::traverseDepthFirstIfNodeImpl(this, callable);
}
template<typename T, unsigned int ChildCount>
bool Leaf<T, ChildCount>::isRoot() const {
  return parent == nullptr;
}
template<typename T, unsigned int ChildCount>
template<typename F>
void Leaf<T, ChildCount>::traverseBreadthFirstNode(F &&callable) {
  detail::traverseBreadthFirstNodeImpl(this, callable);
}
template<typename T, unsigned int ChildCount>
template<typename F>
void Leaf<T, ChildCount>::traverseDepthFirstNode(F &&callable) {
  detail::traverseDepthFirstNodeImpl(this, callable);
}

template<typename T, unsigned int ChildCount>
Node<T, ChildCount>::Node() {
  std::generate(children.begin(), children.end(), [] { return nullptr; });
}
template<typename T, unsigned int ChildCount>
Node<T, ChildCount>::Node(value_type value) : Leaf<T, ChildCount>(value) {
  std::generate(children.begin(), children.end(), [] { return nullptr; });
}
template<typename T, unsigned int ChildCount>
Node<T, ChildCount>::Node(const Node &other) : Leaf<T, ChildCount>(other) {
  for (std::size_t i = 0; i < ChildCount; ++i) {
    if (other.children[i]->getType() == NodeType::Node) {
      children[i] = std::make_unique<Node<T, ChildCount>>(other.children[i]->asNode());
    } else {
      children[i] = std::make_unique<Leaf<T, ChildCount>>(*other.children[i]);
    }
  }
}
template<typename T, unsigned int ChildCount>
Node<T, ChildCount> &Node<T, ChildCount>::operator=(const Node &other) {
  if (&other == this) { return *this; }
  *this = Leaf<T, ChildCount>::operator=(other);
  children = other.children;
  return *this;
}
template<typename T, unsigned int ChildCount>
Node<T, ChildCount>::Node(Node &&other) noexcept : Leaf<T, ChildCount>(other), children(std::move(other.children)) {}
template<typename T, unsigned int ChildCount>
Node<T, ChildCount> &Node<T, ChildCount>::operator=(Node &&other) noexcept {
  *this = Leaf<T, ChildCount>::operator=(other);
  children = std::move(other.children);
  return *this;
}
template<typename T, unsigned int ChildCount>
bool Node<T, ChildCount>::hasChildAtIndex(std::size_t index) const {
  assert(index < ChildCount);
  return children[index] != nullptr;
}
template<typename T, unsigned int ChildCount>
typename Node<T, ChildCount>::Child &Node<T, ChildCount>::setChildAtIndex(std::size_t index, NodeType nodeType) {
  if (nodeType == NodeType::Leaf) {
    children[index] = std::make_unique<Leaf<T, ChildCount>>();
  } else if (nodeType == NodeType::Node) {
    children[index] = std::make_unique<Node>();
  }
  children[index]->parent = this;
  return *children[index];
}
template<typename T, unsigned int ChildCount>
void Node<T, ChildCount>::setChildrenValues(const_reference value, NodeType nodeType) {
  if (nodeType == NodeType::Leaf) {
    std::generate(children.begin(), children.end(), [&value] { return std::make_unique<Leaf<T, ChildCount>>(value); });
  } else if (nodeType == NodeType::Node) {
    std::generate(children.begin(), children.end(), [&value] { return std::make_unique<Node<T, ChildCount>>(value); });
  }
  for (auto &child : children) { child->parent = this; }
}
template<typename T, unsigned int ChildCount>
typename Node<T, ChildCount>::Child &Node<T, ChildCount>::childAtIndex(std::size_t index) {
  assert(index < ChildCount);
  return *children[index];
}
template<typename T, unsigned int ChildCount>
const typename Node<T, ChildCount>::Child &Node<T, ChildCount>::childAtIndex(std::size_t index) const {
  assert(index < ChildCount);
  return *children[index];
}
template<typename T, unsigned int ChildCount>
template<unsigned int C, typename>
typename Node<T, ChildCount>::Child &Node<T, ChildCount>::leftChild() {
  return *children[0];
}
template<typename T, unsigned int ChildCount>
template<unsigned int C, typename>
typename Node<T, ChildCount>::Child &Node<T, ChildCount>::rightChild() {
  return *children[1];
}
template<typename T, unsigned int ChildCount>
template<unsigned int C, typename>
const typename Node<T, ChildCount>::Child &Node<T, ChildCount>::leftChild() const {
  return *children[0];
}
template<typename T, unsigned int ChildCount>
template<unsigned int C, typename>
const typename Node<T, ChildCount>::Child &Node<T, ChildCount>::rightChild() const {
  return *children[1];
}
