#ifndef XML_HELPER_H
#define XML_HELPER_H

#include <stdint.h>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <sstream>
#include <cstdint>
#include <type_traits>
#include <vector>
#include <limits>
#include <map>
#include <stdexcept>
#include "pugixml.hpp"

using namespace pugi;

class XmlHelper
{
public:
    XmlHelper(const XmlHelper &) = delete;

    XmlHelper &operator=(const XmlHelper &) = delete;

    static XmlHelper &getInstance()
    {
        static XmlHelper instance;
        return instance;
    }

    int loadFile(const std::string &fileName)
    {
        // xml_parse_result result = m_doc.load_file(fileName.c_str());
        // if (!result)
        // {
        //     return 1;
        // }

        uint32_t result = 0;

        if(m_configFileLoaded == false)
        {
            try
            {
            	m_doc.reset();
                m_doc = LoadConfigWithIncludes(fileName);
                m_doc.save(std::cout, "    ");
                result = 0;
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << std::endl;
                result = 1;
            }
            m_configFileLoaded = true;
        }

        return result;
    }

    std::string searchRoot(const std::string &parentName)
    {
        xml_node node = m_doc.select_node(parentName.c_str()).node();
        if (!node)
        {
            throw std::runtime_error("XPath node not found: " + parentName);
        }

        return node.path();
    }

    int getNumElement(const std::string &xPath)
    {
        xml_node node = m_doc.select_node(xPath.c_str()).node();
        size_t count = std::distance(node.begin(), node.end());
        return count;
    }

    int getNumElement(const std::string &xPath, const std::string &elementName)
    {
        xml_node node = m_doc.select_node(xPath.c_str()).node();
        auto named_children = node.children(elementName.c_str());
        return std::distance(named_children.begin(), named_children.end());
    }

    int getNumAttribute(const std::string &xPath)
    {
        xml_node node = m_doc.select_node(xPath.c_str()).node();
        return std::distance(node.attributes_begin(), node.attributes_end());
    }

    std::string gotoNode(const std::string &parentName, const std::string &childName, size_t index)
    {
        xml_node parentNode = m_doc.select_node(parentName.c_str()).node();
        xml_node childNode;
        size_t currentIndex = 0;

        for (xml_node child : parentNode.children(childName.c_str()))
        {
            if (currentIndex == index)
            {
                childNode = child;
                break;
            }

            currentIndex++;
        }

        if (!childNode)
        {
            throw std::runtime_error("Child node not found");
        }

        std::string childPath = parentName + "/" + childName + "[" + std::to_string(index + 1) + "]";

        return childPath;
    }

    template <typename T>
    void registerAttributeValue(const std::string xPath, const std::string &attributeName, T &data)
    {
        data = getAttribute<T>(xPath, attributeName);
    }

    template <typename T>
    void registerElementValue(const std::string xPath, const std::string &elementName, T &data)
    {
        data = getElement<T>(xPath, elementName);
    }

    template <typename T>
    void registerAttributeVector(const std::string xPath, const std::string &attributeVectorName, std::vector<T> &data)
    {
        data = getAttributeVector<T>(xPath, attributeVectorName);
    }

    template <typename T>
    void registerElementVector(const std::string xPath, const std::string &elementVectorName, std::vector<T> &data)
    {
        data = getElementVector<T>(xPath, elementVectorName);
    }

private:
    XmlHelper() {}

    ~XmlHelper() {}

    template <typename T>
    T getAttribute(const std::string xPath, const std::string &attributeName)
    {
        xml_node m_node = m_doc.select_node(xPath.c_str()).node();
        xml_attribute attr = m_node.attribute(attributeName.c_str());

        if (!attr)
        {
            throw std::runtime_error("Attribute not found: " + attributeName);
        }

        return fromString<T>(attr.as_string());
    }

    template <typename T>
    T getElement(const std::string xPath, const std::string &elementName)
    {
        xml_node m_node = m_doc.select_node(xPath.c_str()).node();
        xml_node node = m_node.child(elementName.c_str());
        if (!node)
        {
            throw std::runtime_error("Element not found: " + elementName);
        }

        return fromString<T>(node.text().as_string());
    }

    template <typename T>
    std::vector<T> getAttributeVector(const std::string xPath, const std::string &attributeVectorName)
    {
        xml_node m_node = m_doc.select_node(xPath.c_str()).node();
        xml_attribute attr = m_node.attribute(attributeVectorName.c_str());
        if (!attr)
        {
            throw std::runtime_error("Attribute not found: " + attributeVectorName);
        }

        std::vector<T> dataOut;
        std::string dataBuffer;

        std::string attr_data = attr.as_string();
        attr_data.erase(std::remove(attr_data.begin(), attr_data.end(), ' '), attr_data.end());
        std::stringstream ss_attr_data(attr_data);

        while (std::getline(ss_attr_data, dataBuffer, ','))
        {
            dataOut.push_back(fromString<T>(dataBuffer));
        }

        return dataOut;
    }

    template <typename T>
    std::vector<T> getElementVector(const std::string xPath, const std::string &elementVectorName)
    {
        xml_node m_node = m_doc.select_node(xPath.c_str()).node();
        xml_node node = m_node.child(elementVectorName.c_str());
        if (!node)
        {
            throw std::runtime_error("Element not found: " + elementVectorName);
        }

        std::vector<T> dataOut;
        std::string dataBuffer;

        std::string node_data = node.text().as_string();
        node_data.erase(std::remove(node_data.begin(), node_data.end(), ' '), node_data.end());
        std::stringstream ss_node_data(node_data);

        while (std::getline(ss_node_data, dataBuffer, ','))
        {
            dataOut.push_back(fromString<T>(dataBuffer));
        }

        return dataOut;
    }

    template <typename T>
    T fromString(const std::string &s)
    {
        if constexpr (std::is_same_v<T, std::string>)
        {
            return s;
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            return !(s == "false" || s == "0");
        }
        else if constexpr (std::is_integral_v<T>)
        {
            if constexpr (std::is_same_v<T, uint8_t>)
            {
                long value = std::stoul(s);
                return static_cast<T>(value);
            }
            else if constexpr (std::is_same_v<T, uint16_t>)
            {
                long value = std::stoul(s);
                return static_cast<T>(value);
            }
            else if constexpr (std::is_same_v<T, uint32_t>)
            {
                long value = std::stoul(s);
                return static_cast<T>(value);
            }
            else if constexpr (std::is_same_v<T, uint64_t>)
            {
                long value = std::stoull(s);
                return static_cast<T>(value);
            }
            else if constexpr (std::is_same_v<T, int8_t>)
            {
                long value = std::stol(s);
                return static_cast<T>(value);
            }
            else if constexpr (std::is_same_v<T, int16_t>)
            {
                long value = std::stol(s);
                return static_cast<T>(value);
            }
            else if constexpr (std::is_same_v<T, int32_t>)
            {
                long value = std::stol(s);
                return static_cast<T>(value);
            }
            else if constexpr (std::is_same_v<T, int64_t>)
            {
                long value = std::stoll(s);
                return static_cast<T>(value);
            }
            else
            {
                return static_cast<T>(0);
            }
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            if constexpr (std::is_same_v<T, float>)
            {
                return static_cast<T>(std::stof(s));
            }
            else
            {
                return static_cast<T>(std::stod(s));
            }
        }
        else
        {
            return static_cast<T>(0);
        }
    }

    std::string GetDirectory(const std::string &path)
    {
        size_t pos = path.find_last_of("/\\");

        if (pos == std::string::npos)
        {
            return ".";
        }

        return path.substr(0, pos);
    }

    std::string JoinPath(const std::string &dir, const std::string &file)
    {
        if (dir.empty())
        {
            return file;
        }

        char last = dir.back();

        if (last == '/' || last == '\\')
        {
            return dir + file;
        }

        return dir + "/" + file;
    }

    void ResolveIncludes(xml_document &doc, const std::string &currentDirectory, std::map<std::string, bool> &includeStack)
    {
        std::vector<xml_node> includeNodes;

        for (auto xpathNode : doc.select_nodes("//include"))
        {
            includeNodes.push_back(xpathNode.node());
        }

        for (auto includeNode : includeNodes)
        {
            std::string includeFile = includeNode.attribute("file").as_string();

            if (includeFile.empty())
            {
                throw std::runtime_error("<include> node missing file attribute");
            }

            std::string fullPath = JoinPath(currentDirectory, includeFile);

            if (includeStack[fullPath])
            {
                throw std::runtime_error("Circular include detected:\n" + fullPath);
            }

            includeStack[fullPath] = true;

            xml_document includeDoc;

            xml_parse_result result = includeDoc.load_file(fullPath.c_str(), parse_default | parse_fragment);

            if (!result)
            {
                throw std::runtime_error("Failed to load include file:\n" + fullPath + "\nReason: " + std::string(result.description()));
            }

            ResolveIncludes(includeDoc, GetDirectory(fullPath), includeStack);

            xml_node parent = includeNode.parent();

            for (xml_node child = includeDoc.first_child(); child; child = child.next_sibling())
            {
                if (child.type() == node_declaration)
                {
                    continue;
                }

                parent.insert_copy_before(child, includeNode);
            }

            parent.remove_child(includeNode);

            includeStack[fullPath] = false;
        }
    }

    xml_document LoadConfigWithIncludes(const std::string &fileName)
    {
        xml_document doc;

        xml_parse_result result = doc.load_file(fileName.c_str());

        if (!result)
        {
            throw std::runtime_error("Failed to load xml file:\n" + fileName + "\nReason: " + std::string(result.description()));
        }

        std::map<std::string, bool> includeStack;

        includeStack[fileName] = true;

        ResolveIncludes(doc, GetDirectory(fileName), includeStack);

        return doc;
    }

    xml_document m_doc;
    bool m_configFileLoaded = false;
};

#endif // XML_HELPER_H
