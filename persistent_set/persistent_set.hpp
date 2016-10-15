//
//  persistent_set.hpp
//  plusi2v2
//
//  Created by Иван Трофимов on 13.10.16.
//  Copyright © 2016 Иван Трофимов. All rights reserved.
//

#ifndef persistent_set_hpp
#define persistent_set_hpp

#include <iostream>
#include <vector>

struct persistent_set {
    struct node;
    struct iterator;
    
    typedef int value_type;
    
    persistent_set();                                       //covered by tests
    persistent_set(persistent_set const&);                  //covered by tests
    ~persistent_set();                                      //covered by tests
    persistent_set& operator=(persistent_set const&);       //covered by tests
    
    iterator find(value_type);                              //covered by tests
    std::pair<iterator, bool> insert(value_type);           //covered by tests
    void erase(iterator);                                   //covered by tests
    
    iterator begin() const;                                 //covered by tests
    iterator end() const;                                   //covered by tests
private:
    std::shared_ptr<node> root;
};

struct persistent_set::iterator {
    friend struct persistent_set;
    iterator(std::shared_ptr<node>, std::shared_ptr<node>);
    iterator(iterator const&);
    iterator& operator=(iterator const&);
    
    value_type const& operator*() const;
    iterator& operator++();
    iterator operator++(int);
    iterator& operator--();
    iterator operator--(int);
    
    friend bool operator==(iterator, iterator);
    friend bool operator!=(iterator, iterator);
private:
    std::shared_ptr<node> current, root;
};

struct persistent_set::node : std::enable_shared_from_this<node> {
    node(value_type);
    node(std::shared_ptr<node> const&);
    node& operator=(const node&);
    std::shared_ptr<node> find_node(value_type);
    std::shared_ptr<node> insert_node(value_type);
    std::shared_ptr<node> next_node (std::shared_ptr<node>);
    std::shared_ptr<node> prev_node (std::shared_ptr<node>);
    void erase_node(std::shared_ptr<node>, std::shared_ptr<node>);
    void print_node(std::shared_ptr<node>);
    
public:
    value_type data;
    std::shared_ptr<node> left;
    std::shared_ptr<node> right;
};

#endif /* persistent_set_hpp */
