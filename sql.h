#ifndef _SQL_H_
#define _SQL_H_

#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <vector>

class column;

class SqlModel
{
public:
    virtual void reset() = 0;
    virtual std::string str() = 0;

    template <typename T>
    static std::string to_string(const T& data, const std::string& op = "") {
        return std::to_string(data);
    }

    template <size_t N>
    static std::string to_string(char const(&data)[N], const std::string& op = "") {
        if(op == "is") {
            std::string a(data);
            if(a == "null" || a == "NULL") {
                return a;
            }
        }
        return "'" + std::string(data) + "'";
    }

    friend inline std::ostream& operator<< (std::ostream& out, SqlModel& mod) {
        out<<mod.str();
        return out;
    }
protected:
    std::string _sql;
};

template <>
std::string SqlModel::to_string<std::string>(const std::string& data, const std::string& op/* = ""*/) {
    if(op == "is") {
        if(data == "null" || data == "NULL") {
            return data;
        }
    }
    return "'" + data + "'";
}

template <>
std::string SqlModel::to_string<const char*>(const char* const& data, const std::string& op/* = ""*/) {
    if(op == "is") {
        std::string a(data);
        if(a == "null" || a == "NULL") {
            return a;
        }
    }
    return "'" + std::string(data) + "'";
}

template <>
std::string SqlModel::to_string<time_t>(const time_t& data, const std::string& op/* = ""*/) {
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
    InsertModel& insert(const std::string& c, const T& data) {
        _in_sql = true;
        _columns.push_back(c);
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

    /*
    template <typename T>
    SelectModel& where(const std::string& c, const T& data) {
        _in_sql = true;
        _where_condition.push_back(c + " = " + this->to_string(data));
        return *this;
    }
    */

    SelectModel& where(const std::string& condition) {
        _in_sql = true;
        _where_condition.push_back(condition);
        return *this;
    }

    SelectModel& where(column& condition); 

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

/*
template <typename... Args>
std::string AND(Args&&... args) {
    std::string a[] = {args...};
    int size = sizeof...(args);
    std::stringstream ss;
    ss<<"(";
    for(int i = 0; i < size; ++i) {
        if(i < size - 1) {
            ss<<a[i]<<" and ";
        } else {
            ss<<a[i];
        }
    }
    ss<<")";
    return ss.str();
}

template <typename... Args>
std::string OR(Args&&... args) {
    std::string a[] = {args...};
    int size = sizeof...(args);
    std::stringstream ss;
    ss<<"(";
    for(int i = 0; i < size; ++i) {
        if(i < size - 1) {
            ss<<a[i]<<" or ";
        } else {
            ss<<a[i];
        }
    }
    ss<<")";
    return ss.str();
}

template <typename T>
static std::string MAKE_WHERE(const std::string& c, const std::string& op, const T& data) {
    return c + " " + op + " " + SqlModel::to_string(data, op);
}
*/

class column
{
public:
    column(const std::string& column) {
        _cond = column;
    }

    column& is_null() {
        _cond += " is null";
        return *this;
    }

    template <typename T>
    column& in(const std::vector<T>& t) {
        std::stringstream ss;
        ss<<_cond<<" in (";
        size_t size = t.size();
        for(int i = 0; i < size; ++i) {
            if(i < size - 1) {
                ss<<SqlModel::to_string(t[i])<<", ";
            } else {
                ss<<SqlModel::to_string(t[i]);
            }
        }
        ss<<")";
        _cond = ss.str();
        return *this;
    }

    column& operator &&(column& condition) {
        condition._cond = _cond + " and (" + condition._cond + ")";
        return condition;
    }

    column& operator ||(column& condition) {
        condition._cond = _cond + " or (" + condition._cond + ")";
        return condition;
    }

    column& operator &&(const std::string& condition) {
        _cond = _cond + " and " + condition;
        return *this;
    }

    column& operator ||(const std::string& condition) {
        _cond = _cond + " or " + condition;
        return *this;
    }

    column& operator &&(const char* condition) {
        _cond = _cond + " and " + std::string(condition);
        return *this;
    }

    column& operator ||(const char* condition) {
        _cond = _cond + " or " + std::string(condition);
        return *this;
    }

    template <typename T>
    column& operator ==(const T& data) {
        _cond = _cond + " = " + SqlModel::to_string(data);
        return *this;
    }

    template <typename T>
    column& operator !=(const T& data) {
        _cond = _cond + " != " + SqlModel::to_string(data);
        return *this;
    }

    template <typename T>
    column& operator >=(const T& data) {
        _cond = _cond + " >= " + SqlModel::to_string(data);
        return *this;
    }

    template <typename T>
    column& operator <=(const T& data) {
        _cond = _cond + " <= " + SqlModel::to_string(data);
        return *this;
    }

    template <typename T>
    column& operator >(const T& data) {
        _cond = _cond + " > " + SqlModel::to_string(data);
        return *this;
    }

    template <typename T>
    column& operator <(const T& data) {
        _cond = _cond + " < " + SqlModel::to_string(data);
        return *this;
    }

    std::string str() const {
        return _cond;
    }

    operator bool() {
        return true;
    }
private:
    std::string _cond;
};

#endif
