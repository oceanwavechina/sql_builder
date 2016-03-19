#include "sql.h"
#include <iostream>
#include <sstream>

std::string InsertModel::str() {
    if(!_in_sql) return _sql;
    std::stringstream c_ss, v_ss;
    c_ss<<"insert into "<<_table_name<<"(";
    v_ss<<" values(";
    size_t size = _columns.size();
    for(int i = 0; i < size; ++i) {
        if(i < size - 1) {
            c_ss<<_columns[i]<<",";
            v_ss<<_values[i]<<",";
        } else {
            c_ss<<_columns[i]<<")";
            v_ss<<_values[i]<<")";
        }
    }
    c_ss<<v_ss.str();
    _in_sql = false;
    _sql = c_ss.str();
    return _sql;
}

std::string SelectModel::str() {
    if(!_in_sql) return _sql;
    std::stringstream ss_w;
    ss_w<<"select ";
    size_t size = _select_columns.size();
    for(size_t i = 0; i < size; ++i) {
        if(i < size - 1) {
            ss_w<<_select_columns[i]<<",";
        } else {
            ss_w<<_select_columns[i];
        }
    }
    ss_w<<" from "<<_table_name;
    size = _where_condition.size();
    if(size > 0) {
        ss_w<<" where ";
        for(size_t i = 0; i < size; ++i) {
            if(i < size - 1) {
                ss_w<<_where_condition[i]<<" and ";
            } else {
                ss_w<<_where_condition[i];
            }
        }
    }
    _in_sql = false;
    _sql = ss_w.str();
    return _sql;
}

SelectModel& SelectModel::select(const std::string& columns) {
    std::vector<std::string> data;
    std::stringstream ss(columns);
    std::string ele;
    while(std::getline(ss, ele, ',')) {
        _select_columns.push_back(ele);
    }
    _in_sql = true;
    return *this;
}

SelectModel& SelectModel::select(const std::vector<std::string> columns) {
    _select_columns.insert(_select_columns.end(), columns.begin(), columns.end());
    _in_sql = true;
    return *this;
}
