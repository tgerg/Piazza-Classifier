#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"
#include <sstream>
#include <string>
#include <iostream>
 using namespace std;

TEST(test_insert) {
    BinarySearchTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    BinarySearchTree<int>::Iterator it = tree.begin();
    ASSERT_EQUAL(*it, 1);
    ++(++it);
    ASSERT_EQUAL(*it, 3);
}

TEST(test_empty_size_height) {
    BinarySearchTree<int> tree;

    ASSERT_EQUAL(tree.size(), 0);
    ASSERT_TRUE(tree.empty());

    tree.insert(2);
    tree.insert(1);
    tree.insert(3);

    ASSERT_EQUAL(tree.size(), 3);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.height(), 2);
}

TEST(test_find) {
    BinarySearchTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    ASSERT_TRUE(tree.find(3) != tree.end());
    ASSERT_TRUE(tree.find(5) == tree.end());
    ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(test_min_and_max_element) {
    BinarySearchTree<int> tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);

    ASSERT_TRUE(*tree.max_element() == 4);
    ASSERT_TRUE(*tree.min_element() == 1);
    ASSERT_TRUE(*tree.min_greater_than(2) == 3);
    ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(test_inorder_and_preorder) {
    BinarySearchTree<int> tree;

    tree.insert(2);
    tree.insert(1);
    tree.insert(3);

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    cout << "preorder" << endl;
    cout << oss_preorder.str() << endl << endl;
    ASSERT_TRUE(oss_preorder.str() == "2 1 3 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    cout << "inorder" << endl;
    cout << oss_inorder.str() << endl << endl;
    ASSERT_TRUE(oss_inorder.str() == "1 2 3 ");
}

TEST_MAIN()
