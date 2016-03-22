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

    template <typename T>
    std::string to_string(const T& data) {
        return std::to_string(data);
    }

    template <size_t N>
    std::string to_string(char const(&data)[N]) {
        return "'" + std::string(data) + "'";
    }

    /*
    std::string to_string(const char* data, size_t len = 0) {
        return "'" + (len == 0 ? std::string(data) : std::string(data, len)) + "'";
    }
    */

    friend inline std::ostream& operator<< (std::ostream& out, SqlModel& mod) {
        out<<mod.str();
        return out;
    }
protected:
    std::string _sql;
};

template <>
std::string SqlModel::to_string<std::string>(const std::string& data) {
    return "'" + data + "'";
}

template <>
std::string SqlModel::to_string<const char*>(const char* const& data) {
    return "'" + std::string(data) + "'";
}

template <>
std::string SqlModel::to_string<time_t>(const time_t& data) {
    char buff[128] = {0};
    struct tm* ttime = localtime(&data);
    strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", ttime);
    return "'" + std::string(buff) + "'";
}

class InsertModel : public SqlModel
{
public:
    InsertModel() : _in_sql(true) {}
    virtual ~InsertModel() {}

    template <typename T>
    InsertModel& insert(const std::string& column, const T& data) {
        _in_sql = true;
        _columns.push_back(column);
        _values.push_back(this->to_string(data));
        return *this;
    }

    inline void into(const std::string& table_name) {
        _in_sql = true;
        _table_name = table_name;
    }

    virtual std::string str();
    virtual void reset() {
        _in_sql = true;
        _table_name.clear();
        _columns.clear();
        _values.clear();
    }

protected:
    std::string _table_name;
    std::vector<std::string> _columns;
    std::vector<std::string> _values;
    bool _in_sql;
};

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
        _where_condition.push_back(column + " = " + this->to_string(data));
        return *this;
    }

    virtual std::string str();
    virtual void reset() {
        _in_sql = true;
        _table_name.clear();
        _select_columns.clear();
        _where_condition.clear();
    }

protected:
    std::vector<std::string> _select_columns;
    std::string _table_name;
    std::vector<std::string> _where_condition;
    bool _in_sql;
};

#endif
