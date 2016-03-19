#define BOOST_TEST_MODULE sqltest

#include <iostream>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include "sql.h"

BOOST_AUTO_TEST_SUITE(sqltest)

BOOST_AUTO_TEST_CASE(insert)
{
    InsertModel m;
    m.insert("id", (short)32312)
        .insert("name", "six")
        .insert("address", std::string("beijing"))
        .insert("age", 18)
        .into("info");
    std::cout<<m<<std::endl;
    std::cout<<m.str()<<std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
