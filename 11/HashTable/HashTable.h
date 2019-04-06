#pragma once
#include <string>
#include <functional>
#include "HashNode.h"

template<typename K, typename V>
class HashTable {
    public:
        HashTable(const int & table_size) {
            this->table_size = table_size;
            table = new HashNode<K, V> * [table_size](); 
        }

        ~HashTable() {
            for (int i = 0; i < table_size; ++i) {
                HashNode<K, V> * entry = table[i];
                while (entry != NULL) {
                    HashNode<K, V> * prev = entry;
                    entry = entry->getNext();
                    delete prev;
                }
                table[i] = NULL;
            }
            delete [] table;
        }
        
        bool get(const K &key, V & value) {
            unsigned long hash = hasher(key) % table_size;
            HashNode<K, V> * entry = table[hash];
            while (entry != NULL) {
                if (hasher(entry->getKey()) == hasher(key)) {
                    value = entry->getValue();
                    return true;
                }
                entry = entry->getNext();
            }
            return false;
        }
        
        void put(const K &key, const V &value) {
            unsigned long hash = hasher(key) % table_size;
            HashNode<K, V> * prev = NULL;
            HashNode<K, V> * entry = table[hash]; 
            
            while (entry != NULL && entry->getKey() != key) {
                prev = entry;
                entry = entry->getNext();
            }
            
            if (entry == NULL) {
                entry = new HashNode<K, V>(key, value);
                if (prev == NULL) {
                    table[hash] = entry;
                } else {
                    prev->setNext(entry);
                }
            } else {
                //entry->setKey(key);
                entry->setValue(value);
            }
        }

        void remove(const K &key) {
            unsigned long hash = hasher(key) % table_size;
            HashNode<K, V> * prev = NULL;
            HashNode<K, V> * entry = table[hash];
            
            while (entry != NULL && entry->getKey() !=key) {
                prev = entry;
                entry = entry->getNext();
            }
            if (entry == NULL) {
                return;
            } else {
                if (prev == NULL) {
                    table[hash] = entry->getNext();
                } else {
                    prev->setNext(entry->getNext());
                }
                delete entry;
            }
        }
 
        void print_all() {
            for (int i=0; i < table_size; ++i) {
                HashNode<K, V> * entry = table[i];
                while(entry != NULL ) {
                    cout << entry->getKey() << " " << entry->getValue() << endl;
                    entry = entry->getNext();
                }
            }
        }
    
    private:
        HashNode<K, V> ** table;
        int table_size;
        hash<string> hasher;
};
