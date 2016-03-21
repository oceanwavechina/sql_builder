/*
 * sql_builder.h
 *
 *  Created on: Mar 19, 2016
 *      Author: liuyanan
 */

#ifndef SQL_BUILDER_H_
#define SQL_BUILDER_H_

#include "sql_builder.h"

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <tuple>

#include <boost/algorithm/string/join.hpp>


class SqlBuilder {

public:
	const static bool STRING;
	const static bool NUMERIC;

	typedef std::vector<std::string> StringList;
	typedef std::vector<std::tuple<bool/*isString*/, std::string/*value*/> > ValueList;
	typedef std::vector< std::tuple<std::string/*column*/, bool/*isString*/, std::string/*value*/> > ColumnValueList;

public:
	SqlBuilder();
	virtual ~SqlBuilder();

	SqlBuilder& select(const StringList&);
	SqlBuilder& from(const std::string&);
	SqlBuilder& groupby(const std::string&);
	SqlBuilder& orderby(const std::string&);
	SqlBuilder& asc();
	SqlBuilder& desc();
	SqlBuilder& limit(uint64_t);
	SqlBuilder& offset(uint64_t);

	SqlBuilder& insert(const StringList&);
	SqlBuilder& into(const std::string&);
	SqlBuilder& values(const ValueList&);

	SqlBuilder& update(const std::string&);
	SqlBuilder& set(const ColumnValueList&);

	SqlBuilder& where(const std::string&);

	std::string toString();
	void reset();

private:
	std::string _selectToString();
	std::string _updateToString();
	std::string _insertToString();

private:
	enum OperationType{
		NONE,
		SELECT,
		INSERT,
		UPDATE,
		DELETE
	};

private:
	OperationType _operationMode;
	StringList _columns;
	std::string _tableName;
	ValueList _values;
	ColumnValueList _columnsWithValue;
	std::string _where;
	std::string _offset;
	std::string _limit;
	std::string _groupBy;
	std::string _orderBy;
	std::string _orderByType;

	std::stringstream _sqlResult;
};

/* ************************** helper functions ************************** */
std::string quote(const std::string& s, char delim='`');

// "and" "or" is keyword in c++
std::string _and(const SqlBuilder::StringList& filters);
std::string _or(const SqlBuilder::StringList& filters);

template <typename T>
std::string cmp(const std::string& column, const T& data, std::string sign) {
	std::stringstream ss;
	if (typeid(std::string("")).hash_code() == typeid(data).hash_code())
		ss << '`' << data << '`';
	else
		ss << data;
	return quote(column) + std::string(sign) + std::string(ss.str());
}

template <typename T>
std::string eq(const std::string& column, const T& data) {
	return cmp(column, data, "=");
}

template <typename T>
std::string lt(const std::string& column, const T& data) {
	return cmp(column, data, "<");
}

template <typename T>
std::string gt(const std::string& column, const T& data) {
	return cmp(column, data, ">");
}

template <typename T>
std::string le(const std::string& column, const T& data) {
	return cmp(column, data, "<=");
}

template <typename T>
std::string ge(const std::string& column, const T& data) {
	return cmp(column, data, ">=");
}

#endif /* SQL_BUILDER_H_ */
