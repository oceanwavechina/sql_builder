#ifndef _SQL_H_
#define _SQL_H_

#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <map>

class column;

class SqlHelper
{
public:
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
};

template <>
std::string SqlHelper::to_string<std::string>(const std::string& data, const std::string& op/* = ""*/) {
    if(op == "is") {
        if(data == "null" || data == "NULL") {
            return data;
        }
    }
    return "'" + data + "'";
}

template <>
std::string SqlHelper::to_string<const char*>(const char* const& data, const std::string& op/* = ""*/) {
    if(op == "is") {
        std::string a(data);
        if(a == "null" || a == "NULL") {
            return a;
        }
    }
    return "'" + std::string(data) + "'";
}

template <>
std::string SqlHelper::to_string<time_t>(const time_t& data, const std::string& op/* = ""*/) {
    char buff[128] = {0};
    struct tm* ttime = localtime(&data);
    strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", ttime);
    return "'" + std::string(buff) + "'";
}


class SqlModel
{
public:
    class SqlValue
    {
        friend class SqlModel;
    public:
        SqlValue() {}

        template <typename T>
        SqlValue(const T&& v) {
            _value = SqlHelper::to_string(v);
        }
        template <typename T>
        SqlValue& operator=(const T& v) {
            _value = SqlHelper::to_string(v);
            return *this;
        }

        std::string str() const {
            if(*_value.begin() == '\'' && *_value.rbegin() == '\'') {
                return std::string(_value.c_str() + 1, _value.size() - 2);
            } else {
                return _value;
            }
        }
    private:
        std::string _str() const {
            return _value;
        }
        std::string _value;
    };

    SqlValue& operator [](const std::string& key) {
        return _values[key];
    }

    template <typename T>
    SqlValue& insert(const std::string& key, const T& value) {
        _values.insert(std::make_pair(key, SqlValue(value)));
    }

    SqlValue& erase(const std::string& key) {
        _values.erase(key);
    }

    void dump(std::vector<std::string> &c, std::vector<std::string> &v) const {
        for(const auto& value : _values) {
            c.push_back(value.first);
            v.push_back(value.second._str());
        }
    }

    void dump(std::vector<std::string> &v) const {
        for(const auto& value : _values) {
            v.push_back(value.first + " = " + value.second._str());
        }
    }

private:
    std::string _key;
    std::map<std::string, SqlValue> _values;
};

class InsertModel
{
public:
    InsertModel() {}
    virtual ~InsertModel() {}

    template <typename T>
    InsertModel& insert(const std::string& c, const T& data) {
        _columns.push_back(c);
        _values.push_back(SqlHelper::to_string(data));
        return *this;
    }

    InsertModel& insert(const SqlModel& mod) {
        mod.dump(_columns, _values);
        return *this;
    }

    inline void into(const std::string& table_name) {
        _table_name = table_name;
    }

    std::string str();
    void reset() {
        _table_name.clear();
        _columns.clear();
        _values.clear();
    }

    friend inline std::ostream& operator<< (std::ostream& out, InsertModel& mod) {
        out<<mod.str();
        return out;
    }

protected:
    std::string _table_name;
    std::vector<std::string> _columns;
    std::vector<std::string> _values;
    std::string _sql;
};

class SelectModel
{
public:
    SelectModel() {}
    virtual ~SelectModel() {}

    template <typename... Args>
    SelectModel& select(Args&&... columns) {
        std::string a[] = {columns...};
        int size = sizeof...(columns);
        _select_columns.insert(_select_columns.end(), a, a + size);
        return *this;
    }

    SelectModel& from(const std::string& table_name) {
        _table_name = table_name;
        return *this;
    }

    SelectModel& where(const std::string& condition) {
        _where_condition.push_back(condition);
        return *this;
    }

    SelectModel& where(column& condition); 

    std::string str();
    void reset() {
        _table_name.clear();
        _select_columns.clear();
        _where_condition.clear();
    }
    friend inline std::ostream& operator<< (std::ostream& out, SelectModel& mod) {
        out<<mod.str();
        return out;
    }

protected:
    std::vector<std::string> _select_columns;
    std::string _table_name;
    std::vector<std::string> _where_condition;
    std::string _sql;
};

class UpdateModel
{
public:
    UpdateModel() {}
    virtual ~UpdateModel() {}

    UpdateModel& update(const std::string& table_name) {
        _table_name = table_name;
        return *this;
    }

    template <typename T>
    UpdateModel& set(const std::string& c, const T& data) {
        _set_columns.push_back(c + " = " + SqlHelper::to_string(data));
        return *this;
    }

    UpdateModel& set(const SqlModel& mod) {
        mod.dump(_set_columns);
        return *this;
    }

    UpdateModel& where(const std::string& condition) {
        _where_condition.push_back(condition);
        return *this;
    }

    UpdateModel& where(column& condition); 

    std::string str();
    void reset() {
        _table_name.clear();
        _set_columns.clear();
        _where_condition.clear();
    }
    friend inline std::ostream& operator<< (std::ostream& out, UpdateModel& mod) {
        out<<mod.str();
        return out;
    }

protected:
    std::vector<std::string> _set_columns;
    std::string _table_name;
    std::vector<std::string> _where_condition;
    std::string _sql;
};

class DeleteModel
{
public:
    DeleteModel() {}
    virtual ~DeleteModel() {}

    DeleteModel& _delete() {
        return *this;
    }

    DeleteModel& from(const std::string& table_name) {
        _table_name = table_name;
        return *this;
    }

    DeleteModel& where(const std::string& condition) {
        _where_condition.push_back(condition);
        return *this;
    }

    DeleteModel& where(column& condition); 

    std::string str();
    void reset() {
        _table_name.clear();
        _where_condition.clear();
    }
    friend inline std::ostream& operator<< (std::ostream& out, DeleteModel& mod) {
        out<<mod.str();
        return out;
    }

protected:
    std::string _table_name;
    std::vector<std::string> _where_condition;
    std::string _sql;
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
    return c + " " + op + " " + SqlHelper::to_string(data, op);
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

    template <typename... Args>
    column& in(Args&&... args) {
        std::string a[] = {SqlHelper::to_string(args)...};
        int size = sizeof...(args);
        std::stringstream ss;
        ss<<_cond<<" in (";
        for(int i = 0; i < size; ++i) {
            if(i < size - 1) {
                ss<<a[i]<<", ";
            } else {
                ss<<a[i];
            }
        }
        ss<<")";
        _cond = ss.str();
        return *this;
    }

    column& operator &&(column& condition);
    column& operator ||(column& condition);
    column& operator &&(const std::string& condition);
    column& operator ||(const std::string& condition);
    column& operator &&(const char* condition);
    column& operator ||(const char* condition);

    template <typename T>
    column& operator ==(const T& data) {
        _cond = _cond + " = " + SqlHelper::to_string(data);
        return *this;
    }

    template <typename T>
    column& operator !=(const T& data) {
        _cond = _cond + " != " + SqlHelper::to_string(data);
        return *this;
    }

    template <typename T>
    column& operator >=(const T& data) {
        _cond = _cond + " >= " + SqlHelper::to_string(data);
        return *this;
    }

    template <typename T>
    column& operator <=(const T& data) {
        _cond = _cond + " <= " + SqlHelper::to_string(data);
        return *this;
    }

    template <typename T>
    column& operator >(const T& data) {
        _cond = _cond + " > " + SqlHelper::to_string(data);
        return *this;
    }

    template <typename T>
    column& operator <(const T& data) {
        _cond = _cond + " < " + SqlHelper::to_string(data);
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
