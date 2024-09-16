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
    /*struct JSONObject
    {
        union
        {
            union JSONValue
            {
                int jeffInt;
                float jeffFloat;
                bool jeffBool;
                std::string jeffString;
                std::vector<JSONObject> jeffArray;
                std::unique_ptr<JSONObject> jeffObject;
                JSONValue() = default;
                JSONValue(const int& value)
                {

                }
                ~JSONValue()
                {
                    jeffObject.release();
                }
            };
            JSONValue val;
            std::unordered_map<std::string, std::optional<JSONValue>> dictionary;
        };
    };*/

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

    //static JSONObject readJSON(const char* filename)
    //{
    //    std::unordered_map<std::string, JSONObject> dictionary;

    //    std::ifstream file(filename);
    //    std::stringstream buffer;
    //    buffer << file.rdbuf();
    //    file.close();

    //    return parseObject(buffer.str());
    //}

    //static JSONObject parseObject(std::string unparsed)
    //{
    //    JSONObject parsedObject;

    //    size_t charPointer = 0;

    //    // remove whitespace
    //    while (std::isspace(unparsed[charPointer])) charPointer++;

    //    if (unparsed[charPointer] == '{') charPointer++;
    //    else throw std::runtime_error("couldn't parse json: object missing opening brace");

    //    // remove whitespace
    //    while (std::isspace(unparsed[charPointer])) charPointer++;
    //    
    //    // find closing brace
    //    size_t closeBracePos = unparsed.rfind('}');
    //    for (size_t i = closeBracePos; i < unparsed.length(); i++)
    //    {
    //        if (!std::isspace(unparsed[i])) throw std::runtime_error("couldn't parse json: unmatched opening brace");
    //    }

    //    // handle each property
    //    std::string body = unparsed.substr(charPointer, closeBracePos);
    //    std::vector<std::string> properties = split(body, ",");
    //    for (auto& property : properties)
    //    {
    //        JSONObject parsedProperty;

    //        size_t i = 0;
    //        while (std::isspace(property[i])) i++;

    //        // parse property name
    //        if (property[i] != '"') throw std::runtime_error("couldn't parse json: unquoted property name");
    //        std::string propertyName = property.substr(i++);
    //        size_t closeQuote = propertyName.find('"'); if (closeQuote == -1) throw std::runtime_error("couldn't parse json: unquoted property name");
    //        propertyName = propertyName.substr(0, closeQuote);
    //        
    //        // parse property value
    //        i = closeQuote;
    //        while (std::isspace(property[i])) i++;
    //        if (property[i] != ':') throw std::runtime_error("couldn't parse json: missing colon directly after property name");
    //        else i++;
    //        while (std::isspace(property[i])) i++;
    //        std::string propertyValue = property.substr(i);
    //        {
    //            // figure out the type of value
    //            if (propertyValue.find('.') != -1)
    //                parsedProperty.val.jeffFloat = std::stof(propertyValue);
    //            else if (propertyValue.find('"') != -1)
    //            {
    //                char last = propertyValue.back();
    //                if (propertyValue[0] != '"' || last != '"')
    //                    throw std::runtime_error("couldn't parse json: unexpected quote location");
    //                parsedProperty.val.jeffString = propertyValue.substr(1, last);
    //            }
    //            else if (propertyValue.compare("true"))
    //                parsedProperty.val.jeffBool = true;
    //            else if (propertyValue.compare("false"))
    //                parsedProperty.val.jeffBool = false;
    //            else if (propertyValue.find('[') != -1)
    //            {
    //                // handle array
    //            }
    //            else if (propertyValue.find('{'))
    //            {
    //                JSONObject jefObj = parseObject(propertyValue);
    //                parsedProperty.val.jeffObject.reset(&jefObj);
    //            }
    //            else
    //                parsedProperty.val.jeffInt = std::stoi(propertyValue);
    //        }

    //        parsedObject.dictionary[propertyName].emplace(parsedProperty.val);
    //    }

    //    return parsedObject;
    //}

};