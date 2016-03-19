#ifndef _SQL_H_
#define _SQL_H_

#include <string>
#include <boost/any.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>

enum ColumnType {
    COLUMN_TYPE_NONE,
    COLUMN_TYPE_INT16,
    COLUMN_TYPE_UINT16,
    COLUMN_TYPE_INT32,
    COLUMN_TYPE_UINT32,
    COLUMN_TYPE_INT64,
    COLUMN_TYPE_UINT64,
    COLUMN_TYPE_FLOAT,
    COLUMN_TYPE_DOUBLE,
    COLUMN_TYPE_CSTRING,
    COLUMN_TYPE_STRING
};

class TableModel //: public boost::serialization::singleton<TableModel>
{
public:
    TableModel(const std::string& db_name, const std::string& table_name)
        : _db_name(db_name), _table_name(table_name) {}
    virtual ~TableModel() {}
    int initColumnType();

protected:
    std::string _db_name;
    std::string _table_name;
    boost::unordered_map<std::string, ColumnType> column;
};

class SqlModel {
public:
    SqlModel();
    virtual ~SqlModel() {}
protected:
    boost::unordered_map<std::string, ColumnType> _any_type_info;
};

class InsertModel : public SqlModel
{
public:
    InsertModel() : _in_sql(true) {}
    virtual ~InsertModel() {}

    InsertModel& insert(const std::string& column, const boost::any& data);
    InsertModel& insert(const boost::unordered_map<std::string, boost::any>& columns);

    inline void into(const std::string& table_name) {
        _table_name = table_name;
    }

    std::string str();
    inline void reset() {
        _in_sql = true;
        _insert_data.clear();
    }

    friend inline std::ostream& operator<< (std::ostream& out, InsertModel& mod) {
        out<<mod.str();
        return out;
    }

protected:
    boost::unordered_map<std::string, std::pair<ColumnType, boost::any> > _insert_data;
    std::string _table_name;
    std::string _sql;
    bool _in_sql;
};

#endif
