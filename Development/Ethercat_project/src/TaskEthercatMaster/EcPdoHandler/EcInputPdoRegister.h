#ifndef EC_INPUT_PDO_REGISTER_H
#define EC_INPUT_PDO_REGISTER_H

#include "EcMaster.h"
#include "EcPdoVariable.h"
#include <iostream>

inline uint32_t lookupInputObjectInfoByIndex(const uint16_t slaveAddress, const uint16_t objectIndex, const uint16_t objectSubIndex, EC_T_PROCESS_VAR_INFO& objectInfo, bool& isSupported)
{
    EC_T_DWORD slaveId = ecatGetSlaveId(slaveAddress);

    isSupported = false;

    std::cout << "-----------\n";
    std::cout << "Slave ID         = " << slaveId << std::endl;
    std::cout << "Slave address    = " << slaveAddress << std::endl;
    std::cout << "Object Index     = " << std::hex << objectIndex << std::endl;
    std::cout << "Object Sub Index = " << std::hex << objectSubIndex << std::endl;

    EC_T_WORD numOfVarsToRead;

    EC_T_WORD wReadEntries;

    EC_T_DWORD result = ecatGetSlaveInpVarInfoNumOf(true, slaveAddress, &numOfVarsToRead);

    if (result != EC_E_NOERROR)
    {
        std::cout << "\tSlave not found " << std::endl;
        return 1<<10;
    }

    std::vector<EC_T_PROCESS_VAR_INFO_EX> variables(numOfVarsToRead);

    result = ecatGetSlaveInpVarInfoEx(true, slaveAddress, numOfVarsToRead, variables.data(), &wReadEntries);

    if (result != EC_E_NOERROR)
    {
        std::cout << "\tecatGetSlaveInpVarInfoEx failed: entries read = " << std::dec << wReadEntries << std::endl;
        return 1<<10;
    }

    if (variables.empty() || wReadEntries == 0)
    {
        std::cout << "\tecatGetSlaveInpVarInfoEx failed: variables are empty " << std::dec << wReadEntries << std::endl;
        return 1<<10;
    }

    for (uint32_t ii = 0; ii < wReadEntries; ++ii)
    {
        if ((variables[ii].wIndex == objectIndex) && (variables[ii].wSubIndex == objectSubIndex))
        {
            std::cout << "\tMatch found " << variables[ii].szName << std::endl;
            ecatFindInpVarByName(variables[ii].szName, &objectInfo);
            isSupported = true;
            break;
        }
    }

    std::cout << "\tInput PDO Object Index = " << std::hex << objectIndex << std::dec << ":" << objectSubIndex << " " << (isSupported ? "FOUND" : "NOT FOUND") << std::endl;

    return isSupported ? 0 : 1<<10;
}

template<typename ObjectType>
inline uint32_t lookupInputPdoObject(const uint16_t slaveAddress, ObjectType& object)
{
    uint32_t retVal = lookupInputObjectInfoByIndex(slaveAddress, object.objectIndex, object.objectSubIndex, object.variableInfo, object.isSupported);

    std::cout << "\t---\n";

    if (retVal != 0)
    {
        std::cout << "\tlookupInputPdoObject - FAILED : slave = " << slaveAddress << "\n";
        std::cout << "\tobject index                          = " << std::hex << std::showbase << object.objectIndex << "\n";
        std::cout << "\tobject subindex                       = " << std::hex << std::showbase << static_cast<uint16_t>(object.objectSubIndex) << std::endl;
        return retVal;
    }

    if (static_cast<EC_T_INT>(sizeof(object.value)) * 8 >= object.variableInfo.nBitSize)
    {
        std::cout << "\tlookupInputPdoObject succeeded for " << object.variableInfo.szName << "\n";
        return retVal;
    }

    object.isSupported = false;
    std::cout << "\tsize mismatch for data type " << "\tobject name = " << object.variableInfo.szName << "\n" << "\t value size: " << sizeof(object.value) * 8 << ", nBitSize: " << object.variableInfo.nBitSize << std::endl;
    return 1<<10;
}

#endif // EC_INPUT_PDO_REGISTER_H
