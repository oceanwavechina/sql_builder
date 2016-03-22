/*
 * builder_test.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: liuyanan
 */

#define BOOST_TEST_MODULE sqltest

#include <iostream>
#include <sstream>

#include <boost/test/unit_test.hpp>
#include "sql_builder.h"

BOOST_AUTO_TEST_SUITE(sqltest)

BOOST_AUTO_TEST_CASE(insert)
{
	std::string sql = SqlBuilder()
			.select({"*"})
			.from("nginx_log_data")
			.leftJoin("order", "order.id=user.id")
			.where("id>1")
			.groupby("id")
			.orderby("id")
			.asc()
			.offset(0)
			.limit(10)
			.toString();

	sql = SqlBuilder()
			.update("test.nginx_log_data")
			.set({{"count", SqlBuilder::NUMERIC, "710"},
				  {"3xx", SqlBuilder::NUMERIC, "0"}})
			.where(_and({ eq("id", "1"), eq("name", 123) }))
			.toString();

//
//	sql = SqlBuilder()
//			.insert({"id", "name", "age"})
//			.into("user")
//			.values({{SqlBuilder::NUMERIC, "1"},
//					 {SqlBuilder::STRING, "liuyanan"},
//					 {SqlBuilder::NUMERIC, "12"}})
//			.toString();

    std::cout << sql << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()



