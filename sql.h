#ifndef _SQL_H_
#define _SQL_H_

#include <string>
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
    COLUMN_TYPE_STRING
};

class InsertModel
{
public:
    InsertModel() : _in_sql(true) {}
    virtual ~InsertModel() {}

    InsertModel& insert(const std::string& column, const int16_t& data);
    InsertModel& insert(const std::string& column, const uint16_t& data);
    InsertModel& insert(const std::string& column, const int32_t& data);
    InsertModel& insert(const std::string& column, const uint32_t& data);
    InsertModel& insert(const std::string& column, const int64_t& data);
    InsertModel& insert(const std::string& column, const uint64_t& data);
    InsertModel& insert(const std::string& column, const float& data);
    InsertModel& insert(const std::string& column, const double& data);
    InsertModel& insert(const std::string& column, const std::string& data);

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
    boost::unordered_map<std::string, std::pair<ColumnType, std::string> > _insert_data;
    std::string _table_name;
    std::string _sql;
    bool _in_sql;
};

#endif
