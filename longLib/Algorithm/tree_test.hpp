#pragma once

#include "BinaryTree.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("test tree") {
    BinaryTree<int> bt;
    bt.Push(3);
    bt.Push(4);
    bt.Push(8);
    bt.Push(5);
    bt.Display();
}
