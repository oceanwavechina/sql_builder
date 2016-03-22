/*
 * sql_builder.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: liuyanan
 */

#include "sql_builder.h"

#include <stdexcept>

const bool SqlBuilder::STRING = true;
const bool SqlBuilder::NUMERIC = false;


SqlBuilder::SqlBuilder(): _operationMode(NONE) {
}

SqlBuilder::~SqlBuilder() {
}


SqlBuilder& SqlBuilder::select(const StringList& columns) {
	if (_operationMode != NONE && _operationMode != SELECT)
		throw std::runtime_error("Error SQL operation mode: not select");

	_operationMode = SELECT;

	for(const auto& column : columns){
		_columns.push_back(column);
	}

	return *this;
}

SqlBuilder& SqlBuilder::from(const std::string& table) {
	_tableName = table;
	return *this;
}

SqlBuilder& SqlBuilder::leftJoin(const std::string& table, const std::string& on) {
	_leftJoin.push_back(std::make_tuple(table, on));
	return *this;
}


SqlBuilder& SqlBuilder::groupby(const std::string& groupBy) {
	_groupBy = groupBy;
	return *this;
}

SqlBuilder& SqlBuilder::insert(const StringList& columns) {
	if (_operationMode != NONE && _operationMode != INSERT)
		throw std::runtime_error("Error SQL operation mode: not insert");

	_operationMode = INSERT;

	for(const auto& column : columns) {
		_columns.push_back(column);
	}

	return *this;
}

SqlBuilder& SqlBuilder::into(const std::string& table) {
	_tableName = table;
	return *this;
}

SqlBuilder& SqlBuilder::values(const ValueList& values) {
	for(const auto& value : values) {
		_values.push_back(value);
	}

	return *this;
}

SqlBuilder& SqlBuilder::update(const std::string& table) {
	if (_operationMode != NONE && _operationMode != UPDATE)
		throw std::runtime_error("Error SQL operation mode: not update");

	_operationMode = UPDATE;

	_tableName = table;
	return *this;
}

SqlBuilder& SqlBuilder::set(const ColumnValueList& values) {
	for(const auto& value : values) {
		_columnsWithValue.push_back(value);
	}

	return *this;
}

SqlBuilder& SqlBuilder::where(const std::string& where) {
	_where = where;
	return *this;
}

SqlBuilder& SqlBuilder::offset(uint64_t offset) {
	_offset = std::to_string(offset);
	return *this;
}

SqlBuilder& SqlBuilder::limit(uint64_t limit) {
	_limit = std::to_string(limit);
	return *this;
}

SqlBuilder& SqlBuilder::orderby(const std::string& orderBy) {
	_orderBy = orderBy;
	return *this;
}

SqlBuilder& SqlBuilder::asc() {
	_orderByType = "ASC";
	return *this;
}

SqlBuilder& SqlBuilder::desc() {
	_orderByType = "DESC";
	return *this;
}

std::string SqlBuilder::toString() {

	if(_operationMode == SELECT) {
		return _selectToString();
	} else if (_operationMode == INSERT) {
		return _insertToString();
	} else if (_operationMode == UPDATE) {
		return _updateToString();
	}

	return "";
}

void SqlBuilder::reset() {
	_operationMode = NONE;
	_columns.clear();
	_tableName.clear();
	_values.clear();
	_columnsWithValue.clear();
	_where.clear();
	_offset.clear();
	_limit.clear();
	_orderBy.clear();
	_orderByType.clear();
	_sqlResult.str("");
	_groupBy.clear();
	_leftJoin.clear();
}


std::string SqlBuilder::_selectToString() {
	_sqlResult << "SELECT " << boost::join(_columns, ",");

	if (!_tableName.empty()) {
		_sqlResult << " FROM " << _tableName;

		if(!_leftJoin.empty()) {
			for(const auto& item : _leftJoin){
				_sqlResult << " LEFT JOIN " << std::get<0>(item) << " ON " << std::get<1>(item);
			}
		}

		if (!_where.empty())
			_sqlResult << " WHERE " << _where;

		if (!_groupBy.empty())
			_sqlResult << " GROUP BY " << _groupBy;

		if (!_orderBy.empty()) {
			_sqlResult << " ORDER BY " << _orderBy;
			if (!_orderByType.empty())
				_sqlResult << " " << _orderByType;
		}

		if (!_limit.empty())
			_sqlResult << " LIMIT " << _limit;

		if (!_offset.empty())
			_sqlResult << " OFFSET " << _offset;
	}

	return _sqlResult.str();
}

std::string SqlBuilder::_updateToString() {
	std::string column, value;
	bool isString;

	_sqlResult << "UPDATE " << _tableName << " SET ";
	for(auto it = _columnsWithValue.begin(); it != _columnsWithValue.end(); ++it){
		std::tie(column, isString, value) = *it;
		if (isString)
			value = quote(value, '\'');
		_sqlResult << column << "=" << value;

		if (std::next(it) != _columnsWithValue.end()) {
			_sqlResult << ",";
		}
	}

	if (!_where.empty()) {
		_sqlResult << " WHERE " << _where;
	}

	return _sqlResult.str();
}

std::string SqlBuilder::_insertToString() {
	std::string value;
	bool isString;

	_sqlResult << "INSERT INTO " << _tableName << " (" << boost::join(_columns, ",") << ") VALUES (";
	for(auto it = _values.begin(); it != _values.end(); ++it){
		std::tie(isString, value) = *it;
		if (isString) {
			_sqlResult << quote(value, '\'');
		} else {
			_sqlResult << value;
		}

		if (std::next(it) != _values.end()) {
			_sqlResult << ",";
		}
	}
	_sqlResult << ")";

	return _sqlResult.str();
}

std::string quote(const std::string& s, char delim) {
	if (s.empty() || s[0] == delim)
		return s;
	std::stringstream ss;
	ss << delim << s << delim;
	return ss.str();
}

/* ************************** helper functions ************************** */

std::string _and(const SqlBuilder::StringList& filters) {
	return std::string(" (") + boost::join(filters, " AND ") + std::string(") ");
}
std::string _or(const SqlBuilder::StringList& filters) {
	return std::string(" (") + boost::join(filters, " OR ") + std::string(") ");
}

