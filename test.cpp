#define BOOST_TEST_MODULE sqltest

#include <iostream>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include "sql.h"

BOOST_AUTO_TEST_SUITE(sqltest)

BOOST_AUTO_TEST_CASE(insert)
{
    InsertModel i;
    i.insert("id", (short)32312)
        .insert("name", std::string("six"))
        .insert("address", std::string("beijing"))
        .insert("time", time(NULL))
        .insert("age", (int64_t)15323892489203488)
        .into("info");
    std::cout<<i<<std::endl;
    std::cout<<i.str()<<std::endl;

    SelectModel s;
    s.select("name, age, address")
        .from("info")
        .where("id", 2)
        .where("time", time(NULL))
        .where("name", std::string("six"))
        .where("address", std::string("beijing"));
    std::cout<<s<<std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
