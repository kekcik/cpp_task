//
//  persistent_set.cpp
//  plusi2v2
//
//  Created by Иван Трофимов on 13.10.16.
//  Copyright © 2016 Иван Трофимов. All rights reserved.
//

#include "persistent_set.hpp"
#define ps persistent_set

//node operations
//return node by parametrs
int foo () {
    return 6;
}
ps::node& ps::node::operator=(const ps::node& param) {
    data = param.data;
    left = param.left;
    right = param.right;
    return *this;
}
std::shared_ptr<ps::node> ps::node::find_node(ps::value_type param) {
    if (param > data) {
        if (right == nullptr) {
            return nullptr;
        } else {
            return right -> find_node(param);
        }
    }
    
    if (param < data) {
        if (left == nullptr) {
            return nullptr;
        } else {
            return left -> find_node(param);
        }
    }
    return shared_from_this();
}
std::shared_ptr<ps::node> ps::node::insert_node(value_type param) {
    if (param > data) {
        if (right == nullptr) {
            right = std::make_shared<node>(param);
            return right;
            
        } else {
            right = std::make_shared<node>(right);
            return right -> insert_node(param);
        }
    }
    if (param < data) {
        if (left == nullptr) {
            left = std::make_shared<node>(param);
            return left;
        } else {
            left = std::make_shared<node>(left);
            return left -> insert_node(param);
        }
    }
    return shared_from_this();
}
std::shared_ptr<ps::node> ps::node::next_node(std::shared_ptr<ps::node> param) {
    // first greater param
    if (data < param -> data) {
        if (right == nullptr) return shared_from_this();
        return right -> next_node(param);
    } else {
        std::shared_ptr<ps::node> ans;
        if (left == nullptr) {
            ans = shared_from_this();
        } else {
            ans = left -> next_node (param);
        }
        if (ans -> data <= param -> data) ans = shared_from_this();
        return ans;
    }
}
std::shared_ptr<ps::node> ps::node::prev_node(std::shared_ptr<ps::node> param) {
    // first less param
    if (data > param -> data) {
        if (left == nullptr) return shared_from_this();
        return left -> prev_node(param);
    } else {
        std::shared_ptr<ps::node> ans;
        if (right == nullptr) {
            ans = shared_from_this();
        } else {
            ans = right -> prev_node(param);
        }
        if (ans -> data >= param -> data) ans = shared_from_this();
        return ans;
    }
}
void ps::node::erase_node(std::shared_ptr<ps::node> param, std::shared_ptr<ps::node> parent) {
    if (data == param -> data) {
        if (left != nullptr) {
            std::shared_ptr<node> it;
            left = std::make_shared<node>(left);
            it = left;
            while (it -> right != nullptr) {
                it -> right = std::make_shared<node>(it -> right);
                it = it -> right;
            }
            it -> right = right;
            right = nullptr;
        } else {
            
        }
        if  (parent -> right != shared_from_this()) {
            if (left == nullptr) {
                parent -> left = right;
            } else {
                parent -> left = left;
            }
        } else {
            if (right == nullptr) {
                parent -> right = left;
            } else {
                parent -> right = right;
            }
        }
    } else if (param -> data < data) {
        left = std::make_shared<node>(left);
        left -> erase_node (param, shared_from_this());
    } else if (param -> data > data){
        right = std::make_shared<node>(right);
        right -> erase_node (param, shared_from_this());
    }
}
void ps::node::print_node(std::shared_ptr<ps::node> param) {
    if (param == nullptr) return;
    std::cout << '(';
    if (param -> left != nullptr) print_node(param -> left);
    std::cout << param -> data;
    if (param -> right != nullptr) print_node(param -> right);
    std::cout << ')';
}

//iterator operations
ps::iterator& ps::iterator::operator=(ps::iterator const& param) {
    current = param.current;
    root = param.root;
    return *this;
}
ps::value_type const& ps::iterator::operator*() const {
    return current -> data;
}
ps::iterator& ps::iterator::operator++() {
    if (current -> right != nullptr) {
        current = current -> right;
        while (current -> left != nullptr) {
            std::shared_ptr<ps::node> it = current -> left;
            current = it;
        }
    } else {
        current = root -> next_node(current);
    }
    return *this;
}
ps::iterator& ps::iterator::operator--() {
    if (current -> left != nullptr) {
        current = current -> left;
        while (current -> right != nullptr) {
            std::shared_ptr<ps::node> it = current -> right;
            current = it;
        }
    } else {
        current = root -> prev_node(current);
    }
    return *this;
}
ps::iterator ps::iterator::operator++(int) {
    iterator current = *this;
    ++*this;
    return current;
}
ps::iterator ps::iterator::operator--(int) {
    iterator current = *this;
    --(*this);
    return current;
}
bool operator==(ps::iterator f, ps::iterator s) {
    return f.current -> data == s.current -> data;
}
bool operator!=(ps::iterator f, ps::iterator s) {
    return f.current -> data != s.current -> data;
}
ps::iterator ps::begin() const {
    std::shared_ptr<ps::node> it = root;
    while (it -> left != nullptr) {
        it = it -> left;
    }
    return iterator(it, root);
}
ps::iterator ps::end() const {
    std::shared_ptr<node> it = root;
    while (it -> right != nullptr) {
        it = it -> right;
    }
    return iterator(it, root);
}

//persistent set operations
ps& ps::operator=(ps const& param) {
    root = param.root;
    return *this;
}
ps::ps(ps const& param) {
    root = std::shared_ptr<node> (new node(param.root));
}
ps::ps() {
    root = std::shared_ptr<node> (new node((1<<31)-1));
}
ps::~ps(){
    root.reset();
}
ps::iterator ps::find(value_type param) {
    std::shared_ptr<node> it = root -> find_node(param);                //search from root
    if (it == nullptr) return end();                                    //not found
    return iterator(it, root);                                          //found -> return it
}
std::pair<ps::iterator, bool> ps::insert(value_type param) {
    ps::iterator it = find(param);                                      //found insert node, with creation new route
    if (it != end()) return std::make_pair(it, false);                  //not found
    it = iterator(root -> insert_node(param), root);                    //found -> add and return it
    return std::make_pair(it, true);
}
void ps::erase(ps::iterator it) {
    root -> erase_node(it.current, root);
}

//constructors
ps::iterator::iterator(std::shared_ptr<ps::node>param, std::shared_ptr<ps::node>parent) {
    current = param;
    root = parent;
}
ps::iterator::iterator(ps::iterator const &param) {
    current = param.current;
    root = param.root;
}
ps::node::node(value_type param) {
    data = param;
    left = nullptr;
    right = nullptr;
}
ps::node::node(std::shared_ptr<node> const &param) {
    data = param -> data;
    left = param -> left;
    right = param -> right;
}

