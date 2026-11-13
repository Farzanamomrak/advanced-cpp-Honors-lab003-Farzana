#include <algorithm>
namespace project
{

    // ========================= AVLTree =========================

    template <typename Key, typename Value, typename Compare>
    int AVLTree<Key, Value, Compare>::nodeHeight(const Node *n)
    {
        if (n == nullptr)
        {
            return 0;
        }
        return n->height;
    }

    template <typename Key, typename Value, typename Compare>
    int AVLTree<Key, Value, Compare>::balanceFactor(const Node *n)
    {
        if (n == nullptr)
        {
            return 0;
        }
        return nodeHeight(n->left) - nodeHeight(n->right);
    }

    template <typename Key, typename Value, typename Compare>
    void AVLTree<Key, Value, Compare>::destroy(Node *n)
    {
        if (n == nullptr)
        {
            return;
        }
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    template <typename Key, typename Value, typename Compare>
    AVLTree<Key, Value, Compare>::~AVLTree()
    {
        destroy(root_);
    }

    template <typename Key, typename Value, typename Compare>
    AVLTree<Key, Value, Compare>::AVLTree(AVLTree &&other) noexcept
        : root_(other.root_), size_(other.size_), comp_(std::move(other.comp_))
    {
        other.root_ = nullptr;
        other.size_ = 0;
    }

    template <typename Key, typename Value, typename Compare>
    AVLTree<Key, Value, Compare> &
    AVLTree<Key, Value, Compare>::operator=(AVLTree &&other) noexcept
    {
        if (this != &other)
        {
            destroy(root_);
            root_ = other.root_;
            size_ = other.size_;
            comp_ = std::move(other.comp_);
            other.root_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    template <typename Key, typename Value, typename Compare>
    bool AVLTree<Key, Value, Compare>::empty() const noexcept
    {
        return size_ == 0;
    }

    template <typename Key, typename Value, typename Compare>
    std::size_t AVLTree<Key, Value, Compare>::size() const noexcept
    {
        return size_;
    }

    template <typename Key, typename Value, typename Compare>
    int AVLTree<Key, Value, Compare>::height() const noexcept
    {
        return nodeHeight(root_);
    }

    template <typename Key, typename Value, typename Compare>
    typename AVLTree<Key, Value, Compare>::Node *
    AVLTree<Key, Value, Compare>::rotateLeft(Node *x)
    {
        Node *y = x->right;
        Node *t2 = y->left;
        y->left = x;
        x->right = t2;
        x->height = 1 + std::max(nodeHeight(x->left), nodeHeight(x->right));
        y->height = 1 + std::max(nodeHeight(y->left), nodeHeight(y->right));
        return y;
    }

    template <typename Key, typename Value, typename Compare>
    typename AVLTree<Key, Value, Compare>::Node *
    AVLTree<Key, Value, Compare>::rotateRight(Node *y)
    {
        Node *x = y->left;
        Node *t2 = x->right;
        y->left = t2;
        x->right = y;
        y->height = 1 + std::max(nodeHeight(y->left), nodeHeight(y->right));
        x->height = 1 + std::max(nodeHeight(x->left), nodeHeight(x->right));
        return x;
    }

    template <typename Key, typename Value, typename Compare>
    typename AVLTree<Key, Value, Compare>::Node *
    AVLTree<Key, Value, Compare>::rebalance(Node *n)
    {
        n->height = 1 + std::max(nodeHeight(n->left), nodeHeight(n->right));
        int balance = balanceFactor(n);
        if (balance > 1 && balanceFactor(n->left) >= 0)
        {
            return rotateRight(n);
        }
        if (balance > 1)
        {
            n->left = rotateLeft(n->left);
            return rotateRight(n);
        }
        if (balance < -1 && balanceFactor(n->right) <= 0)
        {
            return rotateLeft(n);
        }
        if (balance < -1)
        {
            n->right = rotateRight(n->right);
            return rotateLeft(n);
        }
        return n;
    }

    template <typename Key, typename Value, typename Compare>
    typename AVLTree<Key, Value, Compare>::Node *
    AVLTree<Key, Value, Compare>::insert(Node *n, const Key &key,
                                         const Value &value, bool &inserted)
    {
        if (n == nullptr)
        {
            inserted = true;
            return new Node(key, value);
        }
        if (comp_(key, n->key))
        {
            n->left = insert(n->left, key, value, inserted);
        }
        else if (comp_(n->key, key))
        {
            n->right = insert(n->right, key, value, inserted);
        }
        else
        {
            n->value = value;
            return n;
        }
        return rebalance(n);
    }

    template <typename Key, typename Value, typename Compare>
    void AVLTree<Key, Value, Compare>::insert(const Key &key, const Value &value)
    {
        bool inserted = false;
        root_ = insert(root_, key, value, inserted);
        if (inserted)
        {
            ++size_;
        }
    }

    template <typename Key, typename Value, typename Compare>
    typename AVLTree<Key, Value, Compare>::Node *
    AVLTree<Key, Value, Compare>::minimum(Node *n)
    {
        while (n != nullptr && n->left != nullptr)
        {
            n = n->left;
        }
        return n;
    }

    template <typename Key, typename Value, typename Compare>
    typename AVLTree<Key, Value, Compare>::Node *
    AVLTree<Key, Value, Compare>::erase(Node *n, const Key &key, bool &removed)
    {
        if (n == nullptr)
        {
            return nullptr;
        }
        if (comp_(key, n->key))
        {
            n->left = erase(n->left, key, removed);
        }
        else if (comp_(n->key, key))
        {
            n->right = erase(n->right, key, removed);
        }
        else
        {
            removed = true;
            if (n->left == nullptr || n->right == nullptr)
            {
                Node *child = (n->left != nullptr) ? n->left : n->right;
                delete n;
                return child;
            }

            Node *successor = minimum(n->right);
            n->key = successor->key;
            n->value = successor->value;
            bool successorRemoved = false;
            n->right = erase(n->right, successor->key, successorRemoved);
        }
        return rebalance(n);
    }

    template <typename Key, typename Value, typename Compare>
    bool AVLTree<Key, Value, Compare>::erase(const Key &key)
    {
        bool removed = false;
        root_ = erase(root_, key, removed);
        if (removed)
        {
            --size_;
        }
        return removed;
    }

    template <typename Key, typename Value, typename Compare>
    const typename AVLTree<Key, Value, Compare>::Node *
    AVLTree<Key, Value, Compare>::findNode(const Node *n, const Key &key) const
    {
        while (n != nullptr)
        {
            if (comp_(key, n->key))
            {
                n = n->left;
            }
            else if (comp_(n->key, key))
            {
                n = n->right;
            }
            else
            {
                return n;
            }
        }
        return nullptr;
    }

    template <typename Key, typename Value, typename Compare>
    bool AVLTree<Key, Value, Compare>::contains(const Key &key) const
    {
        return findNode(root_, key) != nullptr;
    }

    template <typename Key, typename Value, typename Compare>
    const Value *AVLTree<Key, Value, Compare>::find(const Key &key) const
    {
        const Node *n = findNode(root_, key);
        return n ? &n->value : nullptr;
    }

    template <typename Key, typename Value, typename Compare>
    void AVLTree<Key, Value, Compare>::inorder(const Node *n,
                                               std::vector<Key> &out)
    {
        if (n == nullptr)
        {
            return;
        }
        inorder(n->left, out);
        out.push_back(n->key);
        inorder(n->right, out);
    }

    template <typename Key, typename Value, typename Compare>
    std::vector<Key> AVLTree<Key, Value, Compare>::inorder() const
    {
        std::vector<Key> out;
        inorder(root_, out);
        return out;
    }

    template <typename Key, typename Value, typename Compare>
    bool AVLTree<Key, Value, Compare>::validate(const Node *n, const Key *lo,
                                                const Key *hi, int &computedHeight) const
    {
        if (n == nullptr)
        {
            computedHeight = 0;
            return true;
        }

        if (lo != nullptr && comp_(n->key, *lo))
        {
            return false;
        }
        if (hi != nullptr && comp_(*hi, n->key))
        {
            return false;
        }

        int leftHeight = 0;
        int rightHeight = 0;
        if (!validate(n->left, lo, &n->key, leftHeight) ||
            !validate(n->right, &n->key, hi, rightHeight))
        {
            return false;
        }

        if (n->height != 1 + (leftHeight > rightHeight ? leftHeight : rightHeight) ||
            leftHeight - rightHeight < -1 || leftHeight - rightHeight > 1)
        {
            return false;
        }
        computedHeight = 1 + std::max(leftHeight, rightHeight);
        if (n->height != computedHeight)
        {
            return false;
        }
        return (leftHeight - rightHeight >= -1 && leftHeight - rightHeight <= 1);
    }

    template <typename Key, typename Value, typename Compare>
    bool AVLTree<Key, Value, Compare>::isValidAVL() const
    {
        int h = 0;
        return validate(root_, nullptr, nullptr, h);
    }

    // ========================= SplayTree =========================

    template <typename Key, typename Value, typename Compare>
    void SplayTree<Key, Value, Compare>::destroy(Node *n)
    {
        if (n == nullptr)
        {
            return;
        }
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    template <typename Key, typename Value, typename Compare>
    SplayTree<Key, Value, Compare>::~SplayTree()
    {
        destroy(root_);
    }

    template <typename Key, typename Value, typename Compare>
    SplayTree<Key, Value, Compare>::SplayTree(SplayTree &&other) noexcept
        : root_(other.root_), size_(other.size_), comp_(std::move(other.comp_))
    {
        other.root_ = nullptr;
        other.size_ = 0;
    }

    template <typename Key, typename Value, typename Compare>
    SplayTree<Key, Value, Compare> &
    SplayTree<Key, Value, Compare>::operator=(SplayTree &&other) noexcept
    {
        if (this != &other)
        {
            destroy(root_);
            root_ = other.root_;
            size_ = other.size_;
            comp_ = std::move(other.comp_);
            other.root_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    template <typename Key, typename Value, typename Compare>
    bool SplayTree<Key, Value, Compare>::empty() const noexcept
    {
        return size_ == 0;
    }

    template <typename Key, typename Value, typename Compare>
    std::size_t SplayTree<Key, Value, Compare>::size() const noexcept
    {
        return size_;
    }

    template <typename Key, typename Value, typename Compare>
    void SplayTree<Key, Value, Compare>::rotateLeft(Node *x)
    {
        Node *y = x->right;
        if (y == nullptr)
        {
            return;
        }
        x->right = y->left;
        if (y->left != nullptr)
        {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            root_ = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    template <typename Key, typename Value, typename Compare>
    void SplayTree<Key, Value, Compare>::rotateRight(Node *x)
    {
        Node *y = x->left;
        if (y == nullptr)
        {
            return;
        }
        x->left = y->right;
        if (y->right != nullptr)
        {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            root_ = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        y->right = x;
        x->parent = y;
    }

    template <typename Key, typename Value, typename Compare>
    void SplayTree<Key, Value, Compare>::splay(Node *x)
    {
        while (x != nullptr && x->parent != nullptr)
        {
            Node *parent = x->parent;
            Node *grandparent = parent->parent;
            // Zig case: x's parent is the root.
            if (grandparent == nullptr)
            {
                if (x == parent->left)
                {
                    rotateRight(parent);
                }
                else
                {
                    rotateLeft(parent);
                }
            }
            // Zig-zig case: x and parent are both left children.
            else if (x == parent->left && parent == grandparent->left)
            {
                rotateRight(grandparent);
                rotateRight(parent);
            }
            // Zig-zig case: x and parent are both right children.
            else if (x == parent->right && parent == grandparent->right)
            {
                rotateLeft(grandparent);
                rotateLeft(parent);
            }
            // Zig-zag case: x is a right child and parent is a left child.
            else if (x == parent->right && parent == grandparent->left)
            {
                rotateLeft(parent);
                rotateRight(grandparent);
            }
            // Zig-zag case: x is a left child and parent is a right child.
            else
            {
                rotateRight(parent);
                rotateLeft(grandparent);
            }
        }
    }

    template <typename Key, typename Value, typename Compare>
    typename SplayTree<Key, Value, Compare>::Node *
    SplayTree<Key, Value, Compare>::findNode(const Key &key)
    {
        Node *current = root_;
        Node *last = nullptr;

        while (current != nullptr)
        {
            last = current;
            if (comp_(key, current->key))
            {
                current = current->left;
            }
            else if (comp_(current->key, key))
            {
                current = current->right;
            }
            else
            {
                splay(current);
                return current;
            }
        }

        if (last != nullptr)
        {
            splay(last);
        }
        return nullptr;
    }

    template <typename Key, typename Value, typename Compare>
    bool SplayTree<Key, Value, Compare>::contains(const Key &key)
    {
        return findNode(key) != nullptr;
    }

    template <typename Key, typename Value, typename Compare>
    const Value *SplayTree<Key, Value, Compare>::find(const Key &key)
    {
        Node *n = findNode(key);
        return n ? &n->value : nullptr;
    }

    template <typename Key, typename Value, typename Compare>
    void SplayTree<Key, Value, Compare>::insert(const Key &key,
                                                const Value &value)
    {
        if (root_ == nullptr)
        {
            root_ = new Node(key, value);
            ++size_;
            return;
        }

        Node *current = root_;
        Node *parent = nullptr;
        while (current != nullptr)
        {
            parent = current;
            if (comp_(key, current->key))
            {
                current = current->left;
            }
            else if (comp_(current->key, key))
            {
                current = current->right;
            }
            else
            {
                current->value = value;
                splay(current);
                return;
            }
        }

        Node *newNode = new Node(key, value);
        newNode->parent = parent;
        if (comp_(key, parent->key))
        {
            parent->left = newNode;
        }
        else
        {
            parent->right = newNode;
        }
        ++size_;
        splay(newNode);
    }

    template <typename Key, typename Value, typename Compare>
    void SplayTree<Key, Value, Compare>::transplant(Node *u, Node *v)
    {
        if (u->parent == nullptr)
        {
            root_ = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }

        if (v != nullptr)
        {
            v->parent = u->parent;
        }
    }

    template <typename Key, typename Value, typename Compare>
    typename SplayTree<Key, Value, Compare>::Node *
    SplayTree<Key, Value, Compare>::subtreeMinimum(Node *n)
    {
        while (n != nullptr && n->left != nullptr)
        {
            n = n->left;
        }
        return n;
    }

    template <typename Key, typename Value, typename Compare>
    bool SplayTree<Key, Value, Compare>::erase(const Key &key)
    {
        Node *target = findNode(key);
        if (target == nullptr)
        {
            return false;
        }

        Node *leftSubtree = root_->left;
        Node *rightSubtree = root_->right;

        if (leftSubtree != nullptr)
        {
            leftSubtree->parent = nullptr;
        }
        if (rightSubtree != nullptr)
        {
            rightSubtree->parent = nullptr;
        }

        delete root_;
        --size_;
        if (leftSubtree == nullptr)
        {
            root_ = rightSubtree;
        }
        else
        {
            root_ = leftSubtree;
            Node *maximum = leftSubtree;
            while (maximum->right != nullptr)
            {
                maximum = maximum->right;
            }
            splay(maximum);
            root_->right = rightSubtree;
            if (rightSubtree != nullptr)
            {
                rightSubtree->parent = root_;
            }
        }
        return true;
    }

    template <typename Key, typename Value, typename Compare>
    const Key *SplayTree<Key, Value, Compare>::rootKey() const noexcept
    {
        return root_ ? &root_->key : nullptr;
    }

    template <typename Key, typename Value, typename Compare>
    void SplayTree<Key, Value, Compare>::inorder(const Node *n,
                                                 std::vector<Key> &out)
    {
        if (n == nullptr)
        {
            return;
        }
        inorder(n->left, out);
        out.push_back(n->key);
        inorder(n->right, out);
    }

    template <typename Key, typename Value, typename Compare>
    std::vector<Key> SplayTree<Key, Value, Compare>::inorder() const
    {
        std::vector<Key> out;
        inorder(root_, out);
        return out;
    }

    template <typename Key, typename Value, typename Compare>
    std::size_t SplayTree<Key, Value, Compare>::depth(const Node *n)
    {
        if (n == nullptr)
        {
            return 0;
        }
        return 1 + std::max(depth(n->left), depth(n->right));
    }

    template <typename Key, typename Value, typename Compare>
    std::size_t SplayTree<Key, Value, Compare>::depth() const noexcept
    {
        return depth(root_);
    }

} // namespace project
