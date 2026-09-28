#ifndef EC_INPUT_PDO_TRANSFER_H
#define EC_INPUT_PDO_TRANSFER_H

#include "EcMaster.h"
#include "EcPdoVariable.h"
#include <iostream>

template<typename ObjectType>
inline void transferInputPdoObject(ObjectType& object, EC_T_BYTE* pBuffer)
{
    if (object.isSupported)
    {
        EC_GETBITS(pBuffer, reinterpret_cast<EC_T_BYTE*>(&object.value), object.variableInfo.nBitOffs, object.variableInfo.nBitSize);
    }
}

#endif // EC_INPUT_PDO_TRANSFER_H
