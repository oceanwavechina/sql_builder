#include "sql.h"
#include <iostream>
#include <sstream>

InsertModel& InsertModel::insert(const std::string& column, const int16_t& data) {
    _insert_data[column] = std::make_pair(COLUMN_TYPE_INT16, boost::lexical_cast<std::string>(data));
    return *this;
}

InsertModel& InsertModel::insert(const std::string& column, const uint16_t& data) {
    _insert_data[column] = std::make_pair(COLUMN_TYPE_UINT16, boost::lexical_cast<std::string>(data));
    return *this;
}

InsertModel& InsertModel::insert(const std::string& column, const int32_t& data) {
    _insert_data[column] = std::make_pair(COLUMN_TYPE_INT32, boost::lexical_cast<std::string>(data));
    return *this;
}

InsertModel& InsertModel::insert(const std::string& column, const uint32_t& data) {
    _insert_data[column] = std::make_pair(COLUMN_TYPE_UINT32, boost::lexical_cast<std::string>(data));
    return *this;
}

InsertModel& InsertModel::insert(const std::string& column, const int64_t& data) {
    _insert_data[column] = std::make_pair(COLUMN_TYPE_INT64, boost::lexical_cast<std::string>(data));
    return *this;
}

InsertModel& InsertModel::insert(const std::string& column, const uint64_t& data) {
    _insert_data[column] = std::make_pair(COLUMN_TYPE_UINT64, boost::lexical_cast<std::string>(data));
    return *this;
}

InsertModel& InsertModel::insert(const std::string& column, const float& data) {
    _insert_data[column] = std::make_pair(COLUMN_TYPE_FLOAT, boost::lexical_cast<std::string>(data));
    return *this;
}

InsertModel& InsertModel::insert(const std::string& column, const double& data) {
    _insert_data[column] = std::make_pair(COLUMN_TYPE_DOUBLE, boost::lexical_cast<std::string>(data));
    return *this;
}

InsertModel& InsertModel::insert(const std::string& column, const std::string& data) {
    _insert_data[column] = std::make_pair(COLUMN_TYPE_STRING, data);
    return *this;
}

std::string InsertModel::str() {
    if(!_in_sql) return _sql;
    std::stringstream c_ss, v_ss;
    c_ss<<"insert into "<<_table_name<<"(";
    v_ss<<" values(";
    size_t i = 0, size = _insert_data.size();
    boost::unordered_map<std::string, std::pair<ColumnType, std::string> >::const_iterator iter;
    for(iter = _insert_data.begin(); iter != _insert_data.end(); ++iter) {
#ifdef SQL_NO_EXCEPTION
        try {
#endif
            if(++i < size) {
                c_ss<<"`"<<iter->first<<"`,";
                switch(iter->second.first) {
                    case COLUMN_TYPE_INT16:
                        v_ss<<boost::lexical_cast<int16_t>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_UINT16:
                        v_ss<<boost::lexical_cast<uint16_t>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_INT32:
                        v_ss<<boost::lexical_cast<int32_t>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_UINT32:
                        v_ss<<boost::lexical_cast<uint32_t>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_INT64:
                        v_ss<<boost::lexical_cast<int64_t>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_UINT64:
                        v_ss<<boost::lexical_cast<uint64_t>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_FLOAT:
                        v_ss<<boost::lexical_cast<float>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_DOUBLE:
                        v_ss<<boost::lexical_cast<double>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_STRING:
                        v_ss<<"'"<<iter->second.second<<"',";
                        break;
                    default:
                        throw boost::bad_lexical_cast();
                        break;
                }
            } else {
                c_ss<<"`"<<iter->first<<"`)";
                switch(iter->second.first) {
                    case COLUMN_TYPE_INT16:
                        v_ss<<boost::lexical_cast<int16_t>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_UINT16:
                        v_ss<<boost::lexical_cast<uint16_t>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_INT32:
                        v_ss<<boost::lexical_cast<int32_t>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_UINT32:
                        v_ss<<boost::lexical_cast<uint32_t>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_INT64:
                        v_ss<<boost::lexical_cast<int64_t>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_UINT64:
                        v_ss<<boost::lexical_cast<uint64_t>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_FLOAT:
                        v_ss<<boost::lexical_cast<float>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_DOUBLE:
                        v_ss<<boost::lexical_cast<double>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_STRING:
                        v_ss<<"'"<<iter->second.second<<"')";
                        break;
                    default:
                        throw boost::bad_lexical_cast();
                        break;
                }
            }
#ifdef SQL_NO_EXCEPTION
        } catch (boost::bad_lexical_cast& e) {
            std::cerr<<"boost::bad_lexical_cast column: "<<iter->first<<" exception:"<<e.what()<<std::endl;
        }
#endif
    }
    c_ss<<v_ss.str();
    _in_sql = false;
    _sql = c_ss.str();
    return _sql;
}
