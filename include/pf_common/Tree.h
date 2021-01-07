//
// Created by petr on 1/7/21.
//

#ifndef PF_COMMON_INCLUDE_PF_COMMON_TREE_H
#define PF_COMMON_INCLUDE_PF_COMMON_TREE_H

#include <memory>
#include <queue>
#include <range/v3/action/sort.hpp>
#include <range/v3/view/transform.hpp>

namespace pf {

//TODO: STL like iterators
template<typename T>
class Node {
 public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using rvalue = T &&;
  using size_type = std::size_t;

  Node() requires std::is_default_constructible_v<T> = default;
  explicit Node(rvalue val) : value_(std::move(val)) {}
  explicit Node(const_reference val) : value_(val) {}
  Node(const Node &) = delete;
  Node &operator=(const Node &) = delete;
  Node(Node &&) noexcept = default;
  Node &operator=(Node &&) noexcept = default;

  [[nodiscard]] reference operator*() { return value_; }
  [[nodiscard]] const_reference operator*() const { return value_; }

  [[nodiscard]] pointer operator->() { return &value_; }
  [[nodiscard]] const_pointer operator->() const { return &value_; }

  [[nodiscard]] reference value() { return value_; }
  [[nodiscard]] const_reference value() const { return value_; }

  Node<T> &appendChild(const_reference val) {
    children_.template emplace_back(std::make_unique<Node>(val));
    return *children_.back();
  }
  Node<T> &appendChild() requires std::is_default_constructible_v<T> {
    children_.template emplace_back(std::make_unique<Node>());
    return *children_.back();
  }
  Node<T> &appendChild(rvalue val) {
    children_.template emplace_back(std::make_unique<Node>(std::move(val)));
    return *children_.back();
  }
  Node<T> &appendChild(std::unique_ptr<Node<T>> &&child) {
    children_.template emplace_back(std::move(child));
    return *children_.back();
  }

  Node<T> &insertChild(size_type idx, const_reference val) {
    assert(idx < childrenSize());
    return **children_.insert(children_.begin() + idx, std::make_unique<Node>(val));
  }
  Node<T> &insertChild(size_type idx) requires std::is_default_constructible_v<T> {
    assert(idx < childrenSize());
    return **children_.insert(children_.begin() + idx, std::make_unique<Node>());
  }
  Node<T> &insertChild(size_type idx, rvalue val) {
    assert(idx < childrenSize());
    return **children_.insert(children_.begin() + idx, std::make_unique<Node>(std::move(val)));
  }
  Node<T> &insertChild(size_type idx, std::unique_ptr<Node<T>> &&child) {
    assert(idx < childrenSize());
    return **children_.insert(children_.begin() + idx, std::move(child));
  }

  std::unique_ptr<Node<T>> removeChild(size_type idx) {
    assert(idx < childrenSize());
    auto result = std::move(children_[idx]);
    children_.erase(children_.begin() + idx);
    return result;
  }

  [[nodiscard]] auto children() {
    return children_ | ranges::views::transform([](const auto &child) -> Node & { return *child; });
  }
  [[nodiscard]] auto children() const {
    return children_ | ranges::views::transform([](const auto &child) -> const Node & { return *child; });
  }

  void sortChildren(std::predicate<T, T> auto pred) {
    std::ranges::sort(children_, [pred](const auto &lhs, const auto &rhs) { return pred(*lhs, *rhs); });
  }

  [[nodiscard]] size_type childrenSize() const { return children_.size(); }

 private:
  T value_;
  std::vector<std::unique_ptr<Node>> children_;
};

template<typename T>
class Tree {
 public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;
  using rvalue = T &&;

  Tree() = default;
  explicit Tree(std::unique_ptr<Node<T>> &&initRoot) : root(std::move(initRoot)) {}
  explicit Tree(rvalue val) : root(std::make_unique<Node<T>>(std::move(val))) {}
  explicit Tree(const_reference val) : root(std::make_unique<Node<T>>(val)) {}

  Node<T> &initRoot(rvalue val) {
    root = std::make_unique<Node<T>>(std::move(val));
    return *root;
  }
  Node<T> &initRoot(const_reference val) {
    root = std::make_unique<Node<T>>(val);
    return *root;
  }

  [[nodiscard]] Node<T> &getRoot() { return *root; }
  [[nodiscard]] const Node<T> &getRoot() const { return *root; }

  [[nodiscard]] bool hasRoot() const { return root != nullptr; }

 private:
  std::unique_ptr<Node<T>> root = nullptr;
};

namespace tree_traversal {

template<typename T>
void depthFirst(Node<T> &node, std::invocable<Node<T> &> auto callable) {
  callable(node);
  auto children = node.children();
  for (const auto &child : children) { depthFirst(child, callable); }
}
template<typename T>
void depthFirst(Tree<T> &tree, std::invocable<Node<T> &> auto callable) {
  if (!tree.hasRoot()) { return; }
  depthFirst(tree.getRoot(), callable);
}

template<typename T>
void breadthFirst(Node<T> &node, std::invocable<Node<T> &> auto callable) {
  auto queue = std::queue<const Node<T> *>();
  queue.push(&node);
  while (!queue.empty()) {
    auto currentNode = queue.front();
    queue.pop();
    callable(*currentNode);
    auto children = currentNode->children();
    for (const auto &child : children) { queue.push(&child); }
  }
}
template<typename T>
void BreadthFirst(Tree<T> &tree, std::invocable<Node<T> &> auto callable) {
  if (!tree.hasRoot()) { return; }
  breadthFirst(tree.getRoot(), callable);
}

template<typename T>
void cDepthFirst(const Node<T> &node, std::invocable<const Node<T> &> auto callable) {
  callable(node);
  auto children = node.children();
  for (const auto &child : children) { depthFirst(child, callable); }
}
template<typename T>
void cDepthFirst(const Tree<T> &tree, std::invocable<const Node<T> &> auto callable) {
  if (!tree.hasRoot()) { return; }
  depthFirst(tree.getRoot(), callable);
}

template<typename T>
void cBreadthFirst(const Node<T> &node, std::invocable<const Node<T> &> auto callable) {
  auto queue = std::queue<const Node<T> *>();
  queue.push(&node);
  while (!queue.empty()) {
    auto currentNode = queue.front();
    queue.pop();
    callable(*currentNode);
    auto children = currentNode->children();
    for (const auto &child : children) { queue.push(&child); }
  }
}
template<typename T>
void cBreadthFirst(const Tree<T> &tree, std::invocable<const Node<T> &> auto callable) {
  if (!tree.hasRoot()) { return; }
  breadthFirst(tree.getRoot(), callable);
}

}// namespace tree_traversal

}// namespace pf

#endif//PF_COMMON_INCLUDE_PF_COMMON_TREE_H
