#include "actionjson.h"

ActionJson::ActionJson(QString json, Connection *connection) :
        m_json{json.toUtf8()},
        m_connection{connection} {

}
