#pragma once

#include <string>

using namespace std; 

template<typename K, typename V>
class HashNode {
    public:
        HashNode(const K & key, const V & value) : key(key), value(value), next(NULL) {}

        K getKey() const {
            return key;
        }

        void setKey(K key) {
            HashNode::key = key;
        }

        V getValue() {
            return value;
        }

        void setValue(V value) {
            HashNode::value = value;
        }

        HashNode * getNext() const {
            return next;
        }

        void setNext(HashNode * next) {
            HashNode::next = next;
        }
    private:
        K key = key;
        V value;
        HashNode * next;
};
