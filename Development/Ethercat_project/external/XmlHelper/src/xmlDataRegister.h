#ifndef XML_DATA_REGISTER_H
#define XML_DATA_REGISTER_H

#include "ITC.h"
#include "xmlHelper.h"
#include "CallbackStatus.h"

class XmlDataRegister
{
public:
    XmlDataRegister()
    {
        m_xmlHelper.loadFile("config.xml");
    }

    virtual ~XmlDataRegister()
    {
    }

    virtual uint32_t registerXml()
    {
        uint32_t dwRes = CallbackStatus::SUCCESS;

        return dwRes;
    }

    void setXPath(const std::string& xPath)
    {
        m_xPath = xPath;
        std::cout << "taskData.xPath (xmlDataRegister) : " << xPath << std::endl;
    }

    const std::string& getXPath()
    {
        return m_xPath;
    }

    template <typename T>
    void registerXmlAttributeValue(const std::string &attributeName, T &data)
    {
        m_xmlHelper.registerAttributeValue<T>(m_xPath, attributeName, data);
    }

    template <typename T>
    void registerXmlElementValue(const std::string &elementName, T &data)
    {
        m_xmlHelper.registerElementValue<T>(m_xPath, elementName, data);
    }

    template <typename T>
    void registerXmlAttributeVector(const std::string &attributeVectorName, std::vector<T> &data)
    {
        m_xmlHelper.registerAttributeVector<T>(m_xPath, attributeVectorName, data);
    }

    template <typename T>
    void registerXmlElementVector(const std::string &elementName, std::vector<T> &data)
    {
        std::cout << "elementName: "  << elementName << std::endl;
        m_xmlHelper.registerElementVector<T>(m_xPath, elementName, data);
    }

    template <typename T>
    void registerXmlElementDataStoreValue(const std::string &dataElementName, Data_store_element<T> &dse)
    {
        std::string nodePath__ = m_xmlHelper.gotoNode(m_xPath, dataElementName, 0);

        std::string key__;
        std::string path__;
        T value__;
        bool overwrite__;

        m_xmlHelper.registerAttributeValue(nodePath__, "key", key__);
        m_xmlHelper.registerAttributeValue(nodePath__, "path", path__);
        m_xmlHelper.registerAttributeValue(nodePath__, "value", value__);
        m_xmlHelper.registerAttributeValue(nodePath__, "overwrite", overwrite__);

        std::cout << "key__       : " << key__ << std::endl;
        std::cout << "path__      : " << path__ << std::endl;
        std::cout << "value__     : " << value__ << std::endl;
        std::cout << "overwrite__ : " << overwrite__ << std::endl;

        dse.set_info(key__, path__, value__, overwrite__);
    }

    template <typename T>
    void registerXmlElementDataStoreVector(const std::string &dataElementName, Data_store_element<std::vector<T>> &dse)
    {
        std::string nodePath__ = m_xmlHelper.gotoNode(m_xPath, dataElementName, 0);

        std::string key__;
        std::string path__;
        std::vector<T> value__;
        bool overwrite__;

        m_xmlHelper.registerAttributeValue(nodePath__, "key", key__);
        m_xmlHelper.registerAttributeValue(nodePath__, "path", path__);
        m_xmlHelper.registerAttributeVector(nodePath__, "value", value__);
        m_xmlHelper.registerAttributeValue(nodePath__, "overwrite", overwrite__);

        std::cout << "key__       : " << key__ << std::endl;
        std::cout << "path__      : " << path__ << std::endl;
        std::cout << "value__     : "; for(size_t i = 0; i < (value__.size()-1); i++) {std::cout << value__[i] << ", ";} std::cout << value__[value__.size()-1] << std::endl;
        std::cout << "overwrite__ : " << overwrite__ << std::endl;

        dse.set_info(key__, path__, value__, overwrite__);
    }

protected:
    XmlHelper &m_xmlHelper = XmlHelper::getInstance();
    std::string m_xPath;
};

#endif // XML_DATA_REGISTER_H
