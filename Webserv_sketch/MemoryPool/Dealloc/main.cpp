/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:20:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/25 11:14:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SharedMPool_Dealloc.hpp" 

#include <map>
#include <set>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <cstdlib>

int main1(void)
{
    std::cout << "                  listtest" << std::endl;         

    SharedMPool_Dealloc<std::string> pool1 = SharedMPool_Dealloc<std::string>::make_SharedPool();
    
    typedef std::list<std::string, SharedMPool_Dealloc<std::string> >::iterator PoolList_iter;

    std::list<std::string, SharedMPool_Dealloc<std::string> > list(pool1);
    std::list<std::string, SharedMPool_Dealloc<std::string> > list2(pool1);
    std::list<std::string, SharedMPool_Dealloc<std::string> > list3(pool1);

    std::cout << "heyhey" << std::endl; 

    list.push_back("one");
    list.push_back("two");
    list2.push_back("one");
    list2.push_back("two");
    list.push_back("three");
    list.push_back("four");

    list3.push_back("one");
    list3.push_back("two");
    list3.push_back("one");
    list3.push_back("two");
    list.push_back("one");
    list.push_back("two");
    
    for(PoolList_iter iter = list.begin(); iter != list.end(); ++iter)
        std::cout << *iter << std::endl;


    std::cout << "                  map test" << std::endl; 
    SharedMPool_Dealloc<std::pair<int, std::string> > pool2 = SharedMPool_Dealloc<std::pair<int, std::string> >::make_SharedPool();
    std::less<int> comparator;

    typedef std::map<int, std::string, std::less<int>, SharedMPool_Dealloc<std::pair<int, std::string> > >             PoolMap;
    typedef std::map<int, std::string, std::less<int>, SharedMPool_Dealloc<std::pair<int, std::string> > >::iterator   PoolMap_iter;

    

    PoolMap map1(comparator, pool2);
    PoolMap map2(comparator, pool2);
    
    for ( int i = 0; i < 5; i++)
    {
        map1[i] = "map1 one";
        map2[i] = "map2 two";
    }


    for(PoolMap_iter iter = map1.begin(); iter != map1.end(); ++iter)
        std::cout << iter->first << ": " << iter->second << std::endl;

    for(PoolMap_iter iter = map2.begin(); iter != map2.end(); ++iter)
        std::cout << iter->first << ": " << iter->second << std::endl;

    std::cout << "                  set test" << std::endl; 


    SharedMPool_Dealloc<std::string> pool4 = SharedMPool_Dealloc<std::string>::make_SharedPool();
    std::less<std::string> comparator4;

    typedef std::set<std::string, std::less<std::string>, SharedMPool_Dealloc<std::string> >  PoolSet;
    typedef std::set<std::string, std::less<std::string>, SharedMPool_Dealloc<std::string> >::iterator  PoolSet_iter;

    PoolSet set1(comparator4, pool4);

    set1.insert("set1 one");
    set1.insert("set2 two");

    for(PoolSet_iter iter = set1.begin(); iter != set1.end(); ++iter)
        std::cout << *iter << std::endl;

    return (0);
}

int main0()
{
    typedef SharedMPool_Dealloc<std::string> Pool;
    Pool pool1 = Pool::make_SharedPool();

    std::list<std::string, Pool> list(pool1);

    list.push_back("one");
    list.push_back("two");
    list.push_back("one");
    list.push_back("two");


    std::list<std::string, SharedMPool_Dealloc<std::string> > list2;
    list2.push_back("one");
    list2.push_back("two");
    list2.push_back("two");
    list2.push_back("two");
    list2.push_back("two");
    list2.push_back("two");
    list2.push_back("two");

    return (0);
}

int main123(int ac, char **av)
{
    /*MemoryPool<std::string> pool1;*/

    if (ac != 2)
        return (0);
    int times = std::atoi(av[1]);

    SharedMPool_Dealloc<std::string> pool1 = SharedMPool_Dealloc<std::string>::make_SharedPool();
    typedef std::list<std::string, SharedMPool_Dealloc<std::string> >  PoolList;
    typedef std::list<std::string, SharedMPool_Dealloc<std::string> >::iterator PoolList_iter;
    
    PoolList list1(pool1);
    PoolList list2(pool1);
    PoolList list3(pool1);

    for (int i = 0; i < times; i++)
    {
        list1.push_back("one");
        list1.push_front("two");
        list2.push_back("three");
        list3.push_back("four");
        list2.push_front("five");
        list1.push_back("three");
        list1.pop_front();  // Removes "two"
        list1.pop_front();  // Removes "one"
        list1.push_back("four");

        list3.push_back("six");
        list1.push_back("seven");
        list2.push_back("eight");
        list2.pop_back();   // Removes "eight"
        list3.pop_front();  // Removes "four"
        list1.push_front("nine");
        list1.pop_back();   // Removes "four"
        list1.push_back("ten");
        list2.push_front("eleven");
        list2.pop_front();  // Removes "eleven"
        list3.push_back("twelve");

        list1.push_front("thirteen");
        list2.push_back("fourteen");
        list1.pop_front();  // Removes "thirteen"
        list3.push_back("fifteen");
        list3.pop_back();   // Removes "fifteen"
        list2.push_front("sixteen");
        list1.push_back("seventeen");
        list1.pop_back();   // Removes "seventeen"
        list1.push_back("eighteen");
        list3.push_front("nineteen");
        list2.pop_front();  // Removes "sixteen"
        list3.push_back("twenty");

        // Additional 70 randomized operations
        list1.push_front("twenty-one");
        list2.push_back("twenty-two");
        list3.push_back("twenty-three");
        list1.pop_back();  // Removes "eighteen"
        list2.pop_back();  // Removes "twenty-two"
        list3.pop_back();  // Removes "twenty-three"

        list1.push_back("twenty-four");
        list3.push_back("twenty-five");
        list2.push_front("twenty-six");
        list1.pop_front();  // Removes "twenty-one"
        list3.pop_front();  // Removes "nineteen"
        list2.pop_back();   // Removes "three"

        list1.push_front("twenty-seven");
        list1.push_back("twenty-eight");
        list2.push_back("twenty-nine");
        list2.pop_front();  // Removes "twenty-six"
        list3.push_back("thirty");
        list1.pop_back();   // Removes "twenty-eight"

        list3.push_back("thirty-one");
        list1.push_back("thirty-two");
        list2.pop_back();  // Removes "twenty-nine"
        list1.push_front("thirty-three");
        list2.push_front("thirty-four");
        list3.pop_back();  // Removes "thirty-one"

        list3.push_back("thirty-five");
        list1.pop_back();   // Removes "thirty-two"
        list1.push_back("thirty-six");
        list2.push_back("thirty-seven");
        list2.pop_front();  // Removes "thirty-four"
        list1.push_front("thirty-eight");

        list2.push_front("thirty-nine");
        list3.push_back("forty");
        list2.push_back("forty-one");
        list3.pop_front();  // Removes "six"
        list1.pop_back();   // Removes "thirty-six"

        list2.push_back("forty-two");
        list3.push_front("forty-three");
        list1.pop_front();  // Removes "thirty-eight"
        list1.push_back("forty-four");
        list2.pop_front();  // Removes "thirty-nine"
        list3.pop_back();   // Removes "forty"

        list2.push_front("forty-five");
        list1.push_back("forty-six");
        list3.push_back("forty-seven");
        list2.pop_back();   // Removes "forty-two"
        list1.push_front("forty-eight");

        list1.pop_front();  // Removes "forty-eight"
        list1.push_back("forty-nine");
        list3.push_back("fifty");
        list2.push_back("fifty-one");
        list1.pop_back();   // Removes "forty-nine"
        list2.pop_back();   // Removes "fifty-one"

        list3.push_front("fifty-two");
        list1.push_front("fifty-three");
        list2.push_front("fifty-four");
        list1.pop_back();   // Removes "forty-six"
        list2.pop_front();  // Removes "fifty-four"

        list3.push_back("fifty-five");
        list1.push_back("fifty-six");
        list2.pop_back();   // Removes "three"
        list3.pop_back();   // Removes "fifty-five"
        list2.push_back("fifty-seven");
        list1.pop_front();  // Removes "fifty-three"

        list1.push_back("fifty-eight");
        list2.push_front("fifty-nine");
        list3.push_front("sixty");
        list1.pop_back();   // Removes "fifty-eight"
        list3.pop_back();   // Removes "fifty"

        list1.push_front("sixty-one");
        list2.push_back("sixty-two");
        list1.pop_back();   // Removes "fifty-six"
        list3.push_back("sixty-three");
        list2.pop_front();  // Removes "fifty-nine"
        list3.push_back("sixty-four");

        list2.push_front("sixty-five");
        list3.push_back("sixty-six");
        list1.push_back("sixty-seven");
        list1.pop_front();  // Removes "sixty-one"
        list3.pop_back();   // Removes "sixty-six"

        list1.push_front("sixty-eight");
        list3.push_back("sixty-nine");
        list2.pop_back();   // Removes "sixty-two"
        list1.push_back("seventy");
    }

    std::cout << "List 1: ";
    for (PoolList_iter iter = list1.begin(); iter != list1.end(); ++iter){
        std::cout << *iter << " ";
    }
    std::cout << std::endl;

    std::cout << "List 2: ";
    for (PoolList_iter iter = list2.begin(); iter != list2.end(); ++iter){
        std::cout << *iter << " ";
    }
    std::cout << std::endl;

    std::cout << "List 3: ";
    for (PoolList_iter iter = list3.begin(); iter != list3.end(); ++iter){
        std::cout << *iter << " ";
    }
    std::cout << std::endl;

    typedef SharedMPool_Dealloc<std::pair<int, std::string> > PairPool;
    typedef std::map<std::string, int, std::less< std::string>, SharedMPool_Dealloc<std::string> >  PoolMap;
    typedef std::map<std::string, int, std::less< std::string>, SharedMPool_Dealloc<std::string> >::iterator PoolMap_iter;
    
    std::less<std::string> comparator;
    PairPool pool2 = PairPool::make_SharedPool();

    PoolMap map1(comparator, pool2);
    PoolMap map2(comparator, pool2);
    PoolMap map3(comparator, pool2);

    // Number of times to repeat the operations

    for (int i = 0; i < times; i++) {
        map1["one"] = 1;
        map1["two"] = 2;
        map2["three"] = 3;
        map3["four"] = 4;
        map2["five"] = 5;
        map1["three"] = 3; // This will overwrite the existing "three"
        
        // Simulating deletion by erasing keys
        map1.erase("two");
        map1.erase("one");
        map1["four"] = 4;

        map3["six"] = 6;
        map1["seven"] = 7;
        map2["eight"] = 8;
        map2.erase("eight");  // Removes "eight"
        map3.erase("four");    // Removes "four"
        map1["nine"] = 9;
        map1.erase("four");     // Removes "four"
        map1["ten"] = 10;
        map2["eleven"] = 11;
        map2.erase("eleven");   // Removes "eleven"
        map3["twelve"] = 12;

        map1["thirteen"] = 13;
        map1.erase("thirteen"); // Removes "thirteen"
        map3["fifteen"] = 15;
        map3.erase("fifteen");   // Removes "fifteen"
        map2["sixteen"] = 16;
        map1["seventeen"] = 17;
        map1.erase("seventeen"); // Removes "seventeen"
        map1["eighteen"] = 18;
        map3["nineteen"] = 19;
        map2.erase("sixteen");   // Removes "sixteen"
        map3["twenty"] = 20;

        // Additional operations
        map1["twenty-one"] = 21;
        map2["twenty-two"] = 22;
        map3["twenty-three"] = 23;
        map1.erase("eighteen");  // Removes "eighteen"
        map2.erase("twenty-two"); // Removes "twenty-two"
        map3.erase("twenty-three"); // Removes "twenty-three"

        map1["twenty-four"] = 24;
        map3["twenty-five"] = 25;
        map2["twenty-six"] = 26;
        map1.erase("twenty-one"); // Removes "twenty-one"
        map3.erase("nineteen");   // Removes "nineteen"
        map2.erase("three");      // Removes "three"

        map1["twenty-seven"] = 27;
        map1["twenty-eight"] = 28;
        map2["twenty-nine"] = 29;
        map2.erase("twenty-six"); // Removes "twenty-six"
        map3["thirty"] = 30;
        map1.erase("twenty-eight"); // Removes "twenty-eight"

        map3["thirty-one"] = 31;
        map1["thirty-two"] = 32;
        map2.erase("twenty-nine"); // Removes "twenty-nine"
        map1["thirty-three"] = 33;
        map2["thirty-four"] = 34;
        map3.erase("thirty-one");  // Removes "thirty-one"

        map3["thirty-five"] = 35;
        map1.erase("thirty-two");   // Removes "thirty-two"
        map1["thirty-six"] = 36;
        map2["thirty-seven"] = 37;
        map2.erase("thirty-four");  // Removes "thirty-four"
        map1["thirty-eight"] = 38;

        map2["thirty-nine"] = 39;
        map3["forty"] = 40;
        map2["forty-one"] = 41;
        map3.erase("six");          // Assuming "six" was a key in map3
        map1.erase("thirty-six");   // Removes "thirty-six"

        map2["forty-two"] = 42;
        map3["forty-three"] = 43;
        map1.erase("thirty-eight"); // Removes "thirty-eight"
        map1["forty-four"] = 44;
        map2.erase("thirty-nine");  // Removes "thirty-nine"
        map3.erase("forty");        // Removes "forty"

        map2["forty-five"] = 45;
        map1["forty-six"] = 46;
        map3["forty-seven"] = 47;
        map2.erase("forty-two");     // Removes "forty-two"
        map1["forty-eight"] = 48;

        map1.erase("forty-eight"); // Removes "forty-eight"
        map1["forty-nine"] = 49;
        map3["fifty"] = 50;
        map2["fifty-one"] = 51;
        map1.erase("forty-nine");   // Removes "forty-nine"
        map2.erase("fifty-one");     // Removes "fifty-one"

        map3["fifty-two"] = 52;
        map1["fifty-three"] = 53;
        map2["fifty-four"] = 54;
        map1.erase("forty-six");     // Removes "forty-six"
        map2.erase("fifty-four");     // Removes "fifty-four"

        map3["fifty-five"] = 55;
        map1["fifty-six"] = 56;
        map2.erase("three");          // Assuming "three" was a key in map2
        map3.erase("fifty-five");     // Removes "fifty-five"
        map2["fifty-seven"] = 57;
        map1.erase("fifty-three");    // Removes "fifty-three"

        map1["fifty-eight"] = 58;
        map2["fifty-nine"] = 59;
        map3["sixty"] = 60;
        map1.erase("fifty-eight");    // Removes "fifty-eight"
        map3.erase("fifty");           // Removes "fifty"

        map1["sixty-one"] = 61;
        map2["sixty-two"] = 62;
        map1.erase("fifty-six");       // Removes "fifty-six"
        map3["sixty-three"] = 63;
        map2.erase("fifty-nine");      // Removes "fifty-nine"
        map3["sixty-four"] = 64;

        map2["sixty-five"] = 65;
        map3["sixty-six"] = 66;
        map1["sixty-seven"] = 67;
        map1.erase("sixty-one");       // Removes "sixty-one"
        map3.erase("sixty-six");        // Removes "sixty-six"

        map1["sixty-eight"] = 68;
        map3["sixty-nine"] = 69;
        map2.erase("sixty-two");        // Removes "sixty-two"
        map1["seventy"] = 70;
    }

    // Display the contents of the maps
    std::cout << "Map 1: ";
    for (PoolMap_iter iter = map1.begin(); iter != map1.end(); ++iter) {
        std::cout << iter->first << " -> " << iter->second << ", ";
    }
    std::cout << std::endl;

    std::cout << "Map 2: ";
    for (PoolMap_iter iter = map2.begin(); iter != map2.end(); ++iter) {
        std::cout << iter->first << " -> " << iter->second << ", ";
    }
    std::cout << std::endl;

    std::cout << "Map 3: ";
    for (PoolMap_iter iter = map3.begin(); iter != map3.end(); ++iter) {
        std::cout << iter->first << " -> " << iter->second << ", ";
    }
    std::cout << std::endl;

    return (0);

}




class Dummy
{
    public:
        Dummy() : i(42), s("yobro") , p(&s), u (i / 4){}
        Dummy(int i, std::string s) : i(i), s(s) , p(&s), u (i / 4){}
        int             i;
        std::string     s;
        std::string*    p;
        u_int16_t       u;

        bool operator<(const Dummy& other) const
        {
            return (i < other.i);
        }
};


int main345(int ac, char **av)
{
    if (ac != 2)
    {
        std::cout << "gime time" << std::endl;
        return (1);
    }

    typedef SharedMPool_Dealloc<std::pair<int, Dummy> >      MapPool;
    typedef std::map<int, Dummy, std::less<int>, MapPool> Map;

    typedef SharedMPool_Dealloc<Dummy>                       ListPool;
    typedef std::list<Dummy, ListPool>                    List;

    typedef SharedMPool_Dealloc<Dummy>                       SetPool;
    typedef std::set<Dummy, std::less<Dummy>, SetPool>    Set;

    std::less<int>      intComp;
    std::less<Dummy>    dummyComp;

    MapPool mapPool;
    Map     map1(intComp, mapPool);
    Map     map2(intComp, mapPool);
    Map     map3(intComp, mapPool);

    ListPool listPool;
    List list1(listPool);
    List list2(listPool);
    List list3(listPool);

    SetPool setPool;

    Set set1(dummyComp, setPool);
    Set set2(dummyComp, setPool);
    Set set3(dummyComp, setPool);

    int times = std::atoi(av[1]);

    for (int i = 0; i < times; ++i)
    {
        int multiplier = i * times;

        // Insert into list1 and then perform random operations on it
        list1.push_back(Dummy(multiplier + 1, "ListTest"));
        if (i % 2 == 0) {
            list1.push_front(Dummy(multiplier + 2, "ListFront"));
        }
        if (i % 3 == 0 && !list1.empty()) {
            list1.pop_back();  // Random pop back
        }

        // Manipulate map2 with insertions and overwrites
        map2[multiplier + 1] = Dummy(multiplier + 1, "MapValue");
        if (i % 4 == 0) {
            map2[multiplier + 2] = Dummy(multiplier + 2, "MapOverwrite");
        }
        if (i % 5 == 0 && map2.size() > 2) {
            map2.erase(multiplier + 1);  // Erase some keys periodically
        }

        // Insert into set3 and then random erases to test allocation
        set3.insert(Dummy(multiplier + 3, "SetTest"));
        if (i % 6 == 0) {
            set3.insert(Dummy(multiplier + 4, "SetTestInsert"));
        }
        if (i % 7 == 0 && !set3.empty()) {
            set3.erase(Dummy(multiplier + 3, "SetTest"));  // Erase by key
        }

        // Cross container operations: inserting into multiple containers at once
        if (i % 10 == 0) {
            list2.push_back(Dummy(multiplier + 5, "CrossInsert"));
            map3[multiplier + 6] = Dummy(multiplier + 6, "CrossInsertMap");
            set1.insert(Dummy(multiplier + 7, "CrossInsertSet"));
        }

        // Erase or pop elements in other containers
        if (i % 8 == 0 && !list2.empty()) {
            list2.pop_back();  // Erase from list2 periodically
        }
        if (i % 9 == 0 && map3.size() > 3) {
            map3.erase(multiplier + 6);  // Remove from map3 periodically
        }
        if (i % 11 == 0 && !set1.empty()) {
            set1.erase(Dummy(multiplier + 7, "CrossInsertSet"));  // Remove from set1
        }

        // Keep adding to all containers to keep them growing
        list3.push_back(Dummy(multiplier + 8, "ListTest3"));
        map1[multiplier + 9] = Dummy(multiplier + 9, "MapTest1");
        set2.insert(Dummy(multiplier + 10, "SetTest2"));
    }

for (int multiplier = 0; multiplier < 10; ++multiplier)
    {

        srand(static_cast<unsigned>(time(0))); // Seed for randomness

        for (int i = 0; i < multiplier * 100; ++i) {
            int randKey = (rand() % multiplier) * 10;  // Random key, scaled by multiplier
            int randValue = (rand() % multiplier) * 10;  // Random value for Dummy objects
            int randIndex = rand() % (multiplier * 5);  // Random index for List operations

            // --------- Map Find Tests ---------
            map1.find(randKey);  // Attempt to find in map1
            map2.find(randKey + 1);  // Find in map2 with some offset
            map3.find(randKey + 2);  // Find in map3 with another offset

            // Random unsuccessful find (key does not exist)
            map1.find(randKey + multiplier * 10);
            map2.find(randKey + multiplier * 20);
            map3.find(randKey + multiplier * 30);

            // --------- Set Find Tests ---------
            Dummy d1(randValue, "FindSet1");
            Dummy d2(randValue + 1, "FindSet2");
            Dummy d3(randValue + 2, "FindSet3");

            set1.find(d1);  // Find in set1
            set2.find(d2);  // Find in set2
            set3.find(d3);  // Find in set3

            // Random unsuccessful find (Dummy object does not exist)
            Dummy d_fail(randValue + multiplier * 10, "FailFindSet");
            set1.find(d_fail);
            set2.find(d_fail);
            set3.find(d_fail);

            // --------- List Find-like Tests ---------
            // List1: simulate a "find" by iterating and checking condition
            if (!list1.empty()) {
                List::iterator iter1 = list1.begin();
                std::advance(iter1, randIndex % list1.size());
                if (iter1 != list1.end() && iter1->i == randIndex) {
                    // Simulate a found condition
                }
            }

            // List2: simulate a "find"
            if (!list2.empty()) {
                List::iterator iter2 = list2.begin();
                std::advance(iter2, randIndex % list2.size());
                if (iter2 != list2.end() && iter2->i == randIndex + 1) {
                    // Simulate a found condition
                }
            }

            // List3: simulate a "find"
            if (!list3.empty()) {
                List::iterator iter3 = list3.begin();
                std::advance(iter3, randIndex % list3.size());
                if (iter3 != list3.end() && iter3->i == randIndex + 2) {
                    // Simulate a found condition
                }
            }
        }
    }

# ifdef PRINTALL

    typedef Set::iterator                                 Set_iter;
    typedef List::iterator                                List_iter;
    typedef Map::iterator                                 Map_iter; 

    for(List_iter iter = list1.begin(); iter != list1.end(); ++iter)
        std::cout << iter->s << " ";
    std::cout << std::endl;
    for(List_iter iter = list2.begin(); iter != list2.end(); ++iter)
        std::cout << iter->s << " ";
    std::cout << std::endl;
    for(List_iter iter = list3.begin(); iter != list3.end(); ++iter)
        std::cout << iter->s << " ";
    std::cout << std::endl;


    for(Set_iter iter = set1.begin(); iter != set1.end(); ++iter)
        std::cout << iter->s << " ";
    std::cout << std::endl;
    for(Set_iter iter = set2.begin(); iter != set2.end(); ++iter)
        std::cout << iter->s << " ";
    std::cout << std::endl;
    for(Set_iter iter = set3.begin(); iter != set3.end(); ++iter)
        std::cout << iter->s << " ";
    std::cout << std::endl;

    for(Map_iter iter = map1.begin(); iter != map1.end(); ++iter)
        std::cout << iter->first << " -> " << iter->second.s << "|||";
    std::cout << std::endl;

    for(Map_iter iter = map2.begin(); iter != map2.end(); ++iter)
        std::cout << iter->first << " -> " << iter->second.s << "|||";
    std::cout << std::endl;

    for(Map_iter iter = map3.begin(); iter != map3.end(); ++iter)
        std::cout << iter->first << " -> " << iter->second.s << "|||";
    std::cout << std::endl;

#endif


    return (0);
}

