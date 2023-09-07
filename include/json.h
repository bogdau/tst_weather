#ifndef __JSON_H__
#define __JSON_H__

#include "boost/property_tree/ptree.hpp"
#include <boost/property_tree/json_parser.hpp>

class Json{
private:
    int time = 0;
public:
    Json();
    int getTime();
};

#endif /* __JSON_H__ */
