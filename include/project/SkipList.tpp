#pragma once

#include <stdexcept>

// Lab 23: Skip Lists; Probabilistic Search Structures
// Implement the functions below. Do not change the public API.

template <typename Key, typename Value>
SkipList<Key, Value>::SkipList(std::size_t maxLevel, double probability, unsigned int seed)
    : head_(nullptr), size_(0), maxLevel_(maxLevel), probability_(probability),
      generator_(seed), distribution_(0.0, 1.0)
{
    if (maxLevel_ == 0)
    {
        throw std::invalid_argument("Maximum level must be greater than 0");
    }
    if (probability_ <= 0.0 || probability_ >= 1.0)
    {
        throw std::invalid_argument("Probability must be between 0 and 1");
    }
    head_ = new Node(Key{}, Value{}, maxLevel_);
}

template <typename Key, typename Value>
SkipList<Key, Value>::~SkipList() { clear(); }

template <typename Key, typename Value>
SkipList<Key, Value>::SkipList(SkipList &&other) noexcept
    : head_(other.head_), size_(other.size_), maxLevel_(other.maxLevel_),
      probability_(other.probability_), generator_(std::move(other.generator_)),
      distribution_(other.distribution_)
{
    other.head_ = nullptr;
    other.size_ = 0;
}

template <typename Key, typename Value>
SkipList<Key, Value> &SkipList<Key, Value>::operator=(SkipList &&other) noexcept
{
    if (this != &other)
    {
        clear();
        head_ = other.head_;
        size_ = other.size_;
        maxLevel_ = other.maxLevel_;
        probability_ = other.probability_;
        generator_ = std::move(other.generator_);
        distribution_ = other.distribution_;
        other.head_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename Key, typename Value>
std::size_t SkipList<Key, Value>::randomLevel()
{
    std::size_t level = 0;
    while (level < maxLevel_ && distribution_(generator_) < probability_)
    {
        ++level;
    }
    return level;
}

template <typename Key, typename Value>
void SkipList<Key, Value>::clear()
{
    if (head_ == nullptr)
    {
        size_ = 0;
        return;
    }
    Node *current = head_->forward[0];
    while (current != nullptr)
    {
        Node *next = current->forward[0];
        delete current;
        current = next;
    }
    delete head_;
    head_ = nullptr;
    size_ = 0;
}

template <typename Key, typename Value>
bool SkipList<Key, Value>::insert(const Key &key, const Value &value)
{
    std::vector<Node *> update(maxLevel_ + 1, nullptr);
    Node *current = head_;
    for (int i = static_cast<int>(maxLevel_); i >= 0; --i)
    {
        while (current->forward[i] != nullptr &&
               current->forward[i]->key < key)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    Node *candidate = update[0]->forward[0];
    if (candidate != nullptr && !(key < candidate->key) &&
        !(candidate->key < key))
    {
        candidate->value = value;
        return false;
    }

    std::size_t level = randomLevel();
    Node *newNode = new Node(key, value, level);
    for (std::size_t i = 0; i <= level; ++i)
    {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
    ++size_;
    return true;
}

template <typename Key, typename Value>
bool SkipList<Key, Value>::erase(const Key &key)
{
    if (head_ == nullptr)
    {
        return false;
    }

    std::vector<Node *> update(maxLevel_ + 1, nullptr);
    Node *current = head_;
    for (int i = static_cast<int>(maxLevel_); i >= 0; --i)
    {
        while (current->forward[i] != nullptr &&
               current->forward[i]->key < key)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    Node *target = update[0]->forward[0];
    if (target == nullptr || key < target->key ||
        target->key < key)
    {
        return false;
    }

    for (std::size_t i = 0; i < target->forward.size(); ++i)
    {
        update[i]->forward[i] = target->forward[i];
    }
    delete target;
    --size_;
    return true;
}

template <typename Key, typename Value>
bool SkipList<Key, Value>::contains(const Key &key) const { return find(key) != nullptr; }

template <typename Key, typename Value>
Value *SkipList<Key, Value>::find(const Key &key)
{
    if (head_ == nullptr)
    {
        return nullptr;
    }

    Node *current = head_;
    for (int i = static_cast<int>(maxLevel_); i >= 0; --i)
    {
        while (current->forward[i] != nullptr &&
               current->forward[i]->key < key)
        {
            current = current->forward[i];
        }
    }

    Node *candidate = current->forward[0];
    if (candidate != nullptr && !(key < candidate->key) &&
        !(candidate->key < key))
    {
        return &candidate->value;
    }
    return nullptr;
}

template <typename Key, typename Value>
const Value *SkipList<Key, Value>::find(const Key &key) const
{
    if (head_ == nullptr)
    {
        return nullptr;
    }

    const Node *current = head_;
    for (int i = static_cast<int>(maxLevel_); i >= 0; --i)
    {
        while (current->forward[i] != nullptr &&
               current->forward[i]->key < key)
        {
            current = current->forward[i];
        }
    }

    const Node *candidate = current->forward[0];
    if (candidate != nullptr && !(key < candidate->key) &&
        !(candidate->key < key))
    {
        return &candidate->value;
    }
    return nullptr;
}

template <typename Key, typename Value>
std::size_t SkipList<Key, Value>::size() const noexcept { return size_; }

template <typename Key, typename Value>
bool SkipList<Key, Value>::empty() const noexcept { return size_ == 0; }

template <typename Key, typename Value>
std::vector<std::pair<Key, Value>> SkipList<Key, Value>::items() const
{
    std::vector<std::pair<Key, Value>> result;
    if (head_ == nullptr)
    {
        return result;
    }

    Node *current = head_->forward[0];
    while (current != nullptr)
    {
        result.push_back({current->key, current->value});
        current = current->forward[0];
    }
    return result;
}

template <typename Key, typename Value>
std::size_t SkipList<Key, Value>::levelCount() const noexcept
{
    return head_ ? head_->forward.size() : 0;
}
