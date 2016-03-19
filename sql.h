#ifndef _SQL_H_
#define _SQL_H_

#include <vector>
#include <string>
#include <ctime>

class SqlModel
{
public:
    virtual void reset() = 0;
    virtual std::string str() = 0;

    friend inline std::ostream& operator<< (std::ostream& out, SqlModel& mod) {
        out<<mod.str();
        return out;
    }
protected:
    std::string _sql;
};

class InsertModel : public SqlModel
{
public:
    InsertModel() : _in_sql(true) {}
    virtual ~InsertModel() {}

    template <typename T>
    InsertModel& insert(const std::string& column, const T& data) {
        _in_sql = true;
        _columns.push_back(column);
        _values.push_back(std::to_string(data));
        return *this;
    }

    inline void into(const std::string& table_name) {
        _in_sql = true;
        _table_name = table_name;
    }

    virtual std::string str();
    virtual void reset() {
        _in_sql = true;
        _columns.clear();
        _values.clear();
    }

protected:
    std::string _table_name;
    std::vector<std::string> _columns;
    std::vector<std::string> _values;
    bool _in_sql;
};

template <>
InsertModel& InsertModel::insert<std::string>(const std::string& column, const std::string& data) {
    _in_sql = true;
    _columns.push_back(column);
    _values.push_back("'" + data + "'");
    return *this;
}

template <>
InsertModel& InsertModel::insert<time_t>(const std::string& column, const time_t& data) {
    char buf[128] = {0};
    struct tm* ttime = localtime(&data);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ttime);
    _in_sql = true;
    _columns.push_back(column);
    _values.push_back("'" + std::string(buf) + "'");
    return *this;
}

class SelectModel : public SqlModel
{
public:
    SelectModel() : _in_sql(true) {}
    virtual ~SelectModel() {}

    SelectModel& select(const std::string& columns);
    SelectModel& select(const std::vector<std::string> columns);

    SelectModel& from(const std::string& table_name) {
        _in_sql = true;
        _table_name = table_name;
        return *this;
    }

    template <typename T>
    SelectModel& where(const std::string& column, const T& data) {
        _in_sql = true;
        _where_condition.push_back(column + " = " + std::to_string(data));
        return *this;
    }

    virtual std::string str();
    virtual void reset() {
        _in_sql = true;
        _select_columns.clear();
    }

protected:
    std::vector<std::string> _select_columns;
    std::string _table_name;
    std::vector<std::string> _where_condition;
    bool _in_sql;
};

template <>
SelectModel& SelectModel::where<std::string>(const std::string& column, const std::string& data) {
    _in_sql = true;
    _where_condition.push_back(column + " = '" + data + "'");
    return *this;
}

template <>
SelectModel& SelectModel::where<time_t>(const std::string& column, const time_t& data) {
    char buf[128] = {0};
    struct tm* ttime = localtime(&data);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ttime);
    _in_sql = true;
    _where_condition.push_back(column + " = '" + std::string(buf) + "'");
    return *this;
}

#endif
