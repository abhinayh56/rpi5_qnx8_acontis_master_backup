#ifndef EC_PDO_VARIABLE_H
#define EC_PDO_VARIABLE_H

#include "EcMaster.h"
#include <iostream>

template<EC_T_WORD ObjectIndex, EC_T_BYTE ObjectSubIndex, typename ValueType, EC_T_BYTE DataLength>
struct PdoVariable
{
    static const EC_T_WORD objectIndex = ObjectIndex;
    static const EC_T_BYTE objectSubIndex = ObjectSubIndex;
    ValueType value;
    static const EC_T_BYTE dataLength = DataLength;
    std::string objectName;
    EC_T_PROCESS_VAR_INFO variableInfo;
    bool isSupported;
};

#endif // EC_PDO_VARIABLE_H
