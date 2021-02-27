#include <iostream>
#include <gmock/gmock.h>

/* Задача:
 *
 *  insert(ключ, значение) // добавление элемента с ключом и значением
    remove(ключ) // удаление элемента дерева по ключу
    find(ключ) // поиск элемента по ключу
    clear // очищение ассоциативного массива
    get_keys // возвращает список ключей
    get_values // возвращает список значений
    print // вывод в консоль
 */

#include "rb_tree.h"

int main() {
//    RB_map<int, int> m;
//    m.insert(10, 10);
//    m.insert(10, 10);
//    m.insert(20, 20);
//    m.insert(10, 10);
//    m.remove(20);
//    m.insert(25, 25);
//    m.insert(5, 5);
//    m.insert(30, 30);
//    m.insert(15, 15);
//    m.remove(25);

    //m.print();
    // return 0;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
