#include "../rb_tree.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace std;

TEST(RB_TESTS, insertion_tests) {
    RB_map<int, int> _map;
    _map.insert(10, 10);
    _map.insert(10, 10);
    _map.insert(20, 20);
    _map.insert(10, 10);

    //    10b
    // 10b    20b
    //      10r
    //

    //checks for numbers (BFS)
    vector<int> vec_values = _map.get_values();
    vector<int> vec_right = {10, 10, 20, 10};
    ASSERT_EQ(vec_values, vec_right);

    //checks for colors
    ASSERT_EQ(_map.find(20)->color, BLACK);

    RB_map<int, int> _map2;
    _map2.insert(10, 10);
    _map2.insert(20, 20);
    _map2.insert(8, 8);
    _map2.insert(6, 6);

    // case 3 in action:
    ///     before
    //     10b (g)
    //   8r (p) 20r (u)
    // 6r (n)
    //
    ///     after
    //    10b
    //  8b   20b
    // 6r
    //

    //checks for numbers
    vector<int> vec_values2 = _map2.get_values();
    vector<int> vec_right2 = {10, 8, 20, 6};
    ASSERT_EQ(vec_values2, vec_right2);

    //checks for colors
    ASSERT_EQ(_map2.find(6)->color, RED);
    ASSERT_EQ(_map2.find(8)->color, BLACK);

    //checks for different types
    RB_map<int, char> _map3;
    _map3.insert(100, 'b');
    _map3.insert(200, 'c');
    _map3.insert(300, 'd');
    _map3.insert(400, 'e');
    _map3.insert(50, 'a');

    //        200b
    //    100b    300b
    // 50r            400r

    //checks for values
    vector<char> vec_values3 = _map3.get_values();
    vector<char> vec_right3 = {'c', 'b', 'd', 'a', 'e'};

    ASSERT_EQ(vec_values3, vec_right3);
    //checks for colors
    ASSERT_EQ(_map3.find(50)->color, RED);
    ASSERT_EQ(_map3.find(400)->color, RED);
    ASSERT_EQ(_map3.find(200)->color, BLACK);
    ASSERT_EQ(_map3.find(100)->color, BLACK);
    ASSERT_EQ(_map3.find(300)->color, BLACK);
}

TEST(RB_TESTS, remove_tests) {
    RB_map<int, int> _map;
    _map.insert(10, 10);
    _map.insert(10, 10);
    _map.insert(20, 20);
    _map.insert(10, 10);
    _map.remove(10);

    //    10b
    // 10b    20b
    //

    //checks for numbers
    vector<int> vec_values = _map.get_values();
    vector<int> vec_right = {10, 10, 20};
    ASSERT_EQ(vec_values, vec_right);

    //checks for colors
    ASSERT_EQ(_map.find(20)->color, BLACK);


    RB_map<int, int> _map2;
    _map2.insert(1000, 2);
    _map2.insert(3000, 3);
    _map2.insert(1000, 1);
    _map2.insert(2000, 4);
    _map2.insert(4000, 1000);
    _map2.insert(5000, 5);
    _map2.remove(4000);

    /// before
    //      1000b
    // 1000b      3000r
    //         2000b   4000b
    //                   5000r

    /// after
    //      1000b
    // 1000b      3000r
    //         2000b   5000b

    //checks for keys
    vector<int> vec_keys2 = _map2.get_keys();
    vector<int> vec_right2 = {1000, 1000, 3000, 2000, 5000};
    ASSERT_EQ(vec_keys2, vec_right2);

    //checks for values
    vector<int> vec_values2 = _map2.get_values();
    vector<int> vec_right2_2 = {1, 2, 3, 4, 5};
    ASSERT_EQ(vec_values2, vec_right2_2);


    //checks for colors
    ASSERT_EQ(_map2.find(1000)->color, BLACK);
    ASSERT_EQ(_map2.find(5000)->color, BLACK);
    ASSERT_EQ(_map2.find(3000)->color, RED);

    // hard case of remove
    RB_map<int, int> _map3;
    _map3.insert(500, 1);
    _map3.insert(300, 3);
    _map3.insert(200, 2);
    _map3.insert(200, 4);
    _map3.insert(200, 5);
    _map3.insert(200, 6);
    _map3.remove(200);

    /// before
    //           300b
    //       200r         500b
    //   200b   200b
    //            200r

    // algorithm will find left 200b and remove it
    // after rotation and recolor the tree will look like:

    /// after
    //           300b
    //       200r     500b
    //   200b   200b

    //checks for keys
    vector<int> vec_keys3 = _map3.get_keys();
    vector<int> vec_right3 = {300, 200, 500, 200, 200};
    ASSERT_EQ(vec_keys3, vec_right3);

    //checks for colors
    ASSERT_EQ(_map3.find(500)->color, BLACK);
    ASSERT_EQ(_map3.find(300)->color, BLACK);
}

TEST(RB_TESTS, miscellaneous_tests) {
    RB_map<double, int> empty_map;
    empty_map.insert(2.0, 1);
    empty_map.insert(2.0, 2);
    empty_map.insert(2.0, 3);
    empty_map.clear();

    ASSERT_ANY_THROW(empty_map.get_values());   // no values
    ASSERT_ANY_THROW(empty_map.get_keys());   // no keys
    ASSERT_ANY_THROW(empty_map.find(2.0));
    ASSERT_ANY_THROW(empty_map.remove(2.0));
}