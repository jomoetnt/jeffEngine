#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include <stack>
#include <optional>
#include <sstream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class jeffJSON
{
public:
    struct JSONObject
    {
        enum objType
        {
            INT, STRING, BOOL, FLOAT, OBJECT, NULLVALUE, ARRAY
        };
        objType jeffType = NULLVALUE;
        int jeffInt = 0;
        std::string jeffString = "";
        bool jeffBool = false;
        float jeffFloat = 0.0f;
        std::vector<JSONObject> jeffArray;

        std::unordered_map<std::string, JSONObject> dictionary;
    };

    static std::vector<std::string> split(std::string s, std::string delimiter)
    {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> res;

        while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
        {
            token = s.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back(s.substr(pos_start));
        return res;
    }

    static std::string replace(std::string s, std::string delimiter)
    {
        std::string output = s;

        size_t pos = 0;
        while (pos != -1)
        {
            pos = s.find(delimiter);
            output.erase(pos, delimiter.length());
        }
        return output;
    }

    static std::string fromWidestring(std::wstring input)
    {
        int bufSize = WideCharToMultiByte(CP_ACP, 0, input.c_str(), -1, nullptr, 0, NULL, NULL);
        char* buffer = new char[bufSize];
        WideCharToMultiByte(CP_ACP, 0, input.c_str(), -1, buffer, bufSize, NULL, NULL);
        std::string str(buffer);
        delete[] buffer;
        return str;
    }

    static void readJSON(const char* filename, JSONObject* out)
    {
        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        parseObject(buffer.str(), out);
    }

    static void parseObject(std::string unparsed, JSONObject* out)
    {
        out->jeffType = JSONObject::OBJECT;

        size_t charPointer = 0;

        // remove whitespace
        while (std::isspace(unparsed[charPointer])) charPointer++;

        if (unparsed[charPointer] == '{') charPointer++;
        else throw std::runtime_error("couldn't parse json: object missing opening brace");

        // remove whitespace
        while (std::isspace(unparsed[charPointer])) charPointer++;
        
        // find closing brace and get object body
        size_t closeBracePos = unparsed.rfind('}');
        std::string body = unparsed.substr(charPointer, closeBracePos - charPointer);

        // comb through each sub-object and replace with an index for later recursive parsing (to prevent comma splitting from dissecting objects)
        std::vector<std::string> subObjects;
        // find first sub-object start
        size_t objStart = body.find('{');
        size_t objEnd = -1;
        while (objStart != -1)
        {
            // find matching closing brace
            int numOpenBraces = 0;
            for (int i = objStart + 1; i < body.length(); i++)
            {
                if (body[i] == '{')
                {
                    numOpenBraces++;
                }
                else if (body[i] == '}')
                {
                    if (numOpenBraces == 0)
                    {
                        objEnd = i;
                        break;
                    }
                    numOpenBraces--;
                }
            }
            if (objEnd == -1) throw std::runtime_error("error parsing JSON: mismatched braces");

            // store entire sub-object
            std::string unparsedSubObj = body.substr(objStart, objEnd - objStart + 1);
            // create replacement string
            std::string indexString = std::to_string(subObjects.size());

            // add unparsed sub-object to list (for later parsing) and replace with index
            subObjects.emplace_back(unparsedSubObj);
            body.replace(objStart + 1, objEnd - objStart - 1, indexString);
            objStart = body.find('{', objStart + 1);
        }
        // handle properties
        std::vector<std::string> properties = split(body, ",");
        for (auto& property : properties)
        {
            JSONObject parsedProperty;

            size_t i = 0;
            while (std::isspace(property[i])) i++;

            // parse property name
            if (property[i] != '"') throw std::runtime_error("couldn't parse json: unquoted property name");
            std::string propertyName = property.substr(++i);
            size_t closeQuote = propertyName.find('"'); if (closeQuote == -1) throw std::runtime_error("couldn't parse json: unquoted property name");
            propertyName = propertyName.substr(0, closeQuote);
            
            // parse property value
            i += propertyName.length() + 1;
            while (std::isspace(property[i])) i++;
            if (property[i] != ':') throw std::runtime_error("couldn't parse json: missing colon after property name");
            else i++;
            while (std::isspace(property[i])) i++;
            std::string propertyValue = property.substr(i);
            {
                // figure out the type of value
                if (propertyValue.find('{') != -1)
                {
                    char last = propertyValue.back();
                    int objIndex = std::stoi(propertyValue.substr(1, last));

                    parseObject(subObjects[objIndex], &parsedProperty);
                }
                else if (propertyValue.find('.') != -1)
                {
                    parsedProperty.jeffType = JSONObject::FLOAT;
                    parsedProperty.jeffFloat = std::stof(propertyValue);
                }
                else if (propertyValue.find('"') != -1)
                {
                    parsedProperty.jeffType = JSONObject::STRING;
                    int closeQuote = propertyValue.find('"', 1);
                    if (closeQuote == -1) throw std::runtime_error("couldn't parse JSON: missing closing quote");
                    parsedProperty.jeffString = propertyValue.substr(1, closeQuote - 1);
                }
                else if (propertyValue.find("true") != -1)
                {
                    std::string whitespaceGone = "";
                    for (int k = 0; k < propertyValue.length(); k++)
                    {
                        if (!std::isspace(propertyValue[k]))
                            whitespaceGone.push_back(propertyValue[k]);
                    }
                    if (whitespaceGone.compare("true") == 0)
                    {
                        parsedProperty.jeffType = JSONObject::BOOL;
                        parsedProperty.jeffBool = true;
                    }
                }
                else if (propertyValue.find("false") != -1)
                {
                    std::string whitespaceGone = "";
                    for (int k = 0; k < propertyValue.length(); k++)
                    {
                        if (!std::isspace(propertyValue[k]))
                            whitespaceGone.push_back(propertyValue[k]);
                    }
                    if (whitespaceGone.compare("false") == 0)
                    {
                        parsedProperty.jeffType = JSONObject::BOOL;
                        parsedProperty.jeffBool = false;
                    }
                }
                else if (propertyValue.find('[') != -1)
                {
                    // handle array
                    parsedProperty.jeffType = JSONObject::ARRAY;


                }
                else if (propertyValue.find("null") != -1)
                {
                    std::string whitespaceGone = "";
                    for (int k = 0; k < propertyValue.length(); k++)
                    {
                        if (!std::isspace(propertyValue[k]))
                            whitespaceGone.push_back(propertyValue[k]);
                    }
                    if (whitespaceGone.compare("null") == 0)
                    {
                        parsedProperty.jeffType = JSONObject::NULLVALUE;
                    }
                }
                else
                {
                    parsedProperty.jeffType = JSONObject::INT;
                    parsedProperty.jeffInt = std::stoi(propertyValue);
                }
            }

            out->dictionary[propertyName] = parsedProperty;
        }
    }

};