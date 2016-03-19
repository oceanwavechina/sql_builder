#include "sql.h"
#include <iostream>
#include <sstream>

int TableModel::initColumnType() {
    
    return 0;
}

SqlModel::SqlModel() {
    _any_type_info[boost::any((int16_t)0).type().name()] = COLUMN_TYPE_INT16;
    _any_type_info[boost::any((uint16_t)0).type().name()] = COLUMN_TYPE_UINT16;
    _any_type_info[boost::any((int32_t)0).type().name()] = COLUMN_TYPE_INT32;
    _any_type_info[boost::any((uint32_t)0).type().name()] = COLUMN_TYPE_UINT32;
    _any_type_info[boost::any((int64_t)0).type().name()] = COLUMN_TYPE_INT64;
    _any_type_info[boost::any((uint64_t)0).type().name()] = COLUMN_TYPE_UINT64;
    _any_type_info[boost::any(0.0f).type().name()] = COLUMN_TYPE_FLOAT;
    _any_type_info[boost::any(0.0).type().name()] = COLUMN_TYPE_DOUBLE;
    _any_type_info[boost::any("").type().name()] = COLUMN_TYPE_CSTRING;
    _any_type_info[boost::any(std::string("")).type().name()] = COLUMN_TYPE_STRING;
}

InsertModel& InsertModel::insert(const std::string& column, const boost::any& data) {
    const char* type_name = data.type().name();
    ColumnType t = _any_type_info[type_name];
#ifdef _DEBUG
    std::cout<<"|"<<type_name<<"|"<<t<<std::endl;
#endif
    _insert_data[column] = std::make_pair(t, data);
    _in_sql = true;
    return *this;
}

InsertModel& InsertModel::insert(const boost::unordered_map<std::string, boost::any>& columns) {
    boost::unordered_map<std::string, boost::any>::const_iterator iter;
    for(iter = columns.begin(); iter != columns.end(); ++iter) {
        insert(iter->first, iter->second);
    }
    return *this;
}

std::string InsertModel::str() {
    if(!_in_sql) return _sql;
    std::stringstream c_ss, v_ss;
    c_ss<<"insert into "<<_table_name<<"(";
    v_ss<<" values(";
    size_t i = 0, size = _insert_data.size();
    boost::unordered_map<std::string, std::pair<ColumnType, boost::any> >::const_iterator iter;
    for(iter = _insert_data.begin(); iter != _insert_data.end(); ++iter) {
#ifdef SQL_NO_EXCEPTION
        try {
#endif
            if(++i < size) {
                c_ss<<"`"<<iter->first<<"`,";
                switch(iter->second.first) {
                    case COLUMN_TYPE_INT16:
                        v_ss<<boost::any_cast<int16_t>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_UINT16:
                        v_ss<<boost::any_cast<uint16_t>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_INT32:
                        v_ss<<boost::any_cast<int32_t>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_UINT32:
                        v_ss<<boost::any_cast<uint32_t>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_INT64:
                        v_ss<<boost::any_cast<int64_t>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_UINT64:
                        v_ss<<boost::any_cast<uint64_t>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_FLOAT:
                        v_ss<<boost::any_cast<float>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_DOUBLE:
                        v_ss<<boost::any_cast<double>(iter->second.second)<<",";
                        break;
                    case COLUMN_TYPE_CSTRING:
                        v_ss<<"'"<<boost::any_cast<const char*>(iter->second.second)<<"',";
                        break;
                    case COLUMN_TYPE_STRING:
                        v_ss<<"'"<<boost::any_cast<std::string>(iter->second.second)<<"',";
                        break;
                    default:
                        throw boost::bad_any_cast();
                        break;
                }
            } else {
                c_ss<<"`"<<iter->first<<"`)";
                switch(iter->second.first) {
                    case COLUMN_TYPE_INT16:
                        v_ss<<boost::any_cast<int16_t>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_UINT16:
                        v_ss<<boost::any_cast<uint16_t>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_INT32:
                        v_ss<<boost::any_cast<int32_t>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_UINT32:
                        v_ss<<boost::any_cast<uint32_t>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_INT64:
                        v_ss<<boost::any_cast<int64_t>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_UINT64:
                        v_ss<<boost::any_cast<uint64_t>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_FLOAT:
                        v_ss<<boost::any_cast<float>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_DOUBLE:
                        v_ss<<boost::any_cast<double>(iter->second.second)<<")";
                        break;
                    case COLUMN_TYPE_CSTRING:
                        v_ss<<"'"<<boost::any_cast<const char*>(iter->second.second)<<"')";
                        break;
                    case COLUMN_TYPE_STRING:
                        v_ss<<"'"<<boost::any_cast<std::string>(iter->second.second)<<"')";
                        break;
                    default:
                        throw boost::bad_any_cast();
                        break;
                }
            }
#ifdef SQL_NO_EXCEPTION
        } catch (boost::bad_any_cast& e) {
            std::cerr<<"boost::bad_any_cast column: "<<iter->first<<" exception:"<<e.what()<<std::endl;
        }
#endif
    }
    c_ss<<v_ss.str();
    _in_sql = false;
    _sql = c_ss.str();
    return _sql;
}
