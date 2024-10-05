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
            INT, STRING, BOOL, FLOAT, NULLVALUE, OBJECT, ARRAY
        };
        objType jeffType = NULLVALUE;
        int jeffInt = 0;
        std::string jeffString = "";
        bool jeffBool = false;
        float jeffFloat = 0.0f;
        std::vector<JSONObject> jeffArray;

        std::unordered_map<std::string, JSONObject> dictionary;
        std::vector<std::string> keys;
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

    static void writeJSON(const char* filename, JSONObject in)
    {
        std::string unparsed = unparseObject(in, 1);

        std::ofstream file(filename);
        file.write(unparsed.c_str(), unparsed.length());
        file.close();
    }

    static std::string unparseObject(JSONObject in, int numIndents)
    {
        std::string unparsed = "{\n";

        for (int i = 0; i < in.keys.size(); i++)
        {
            for (int j = 0; j < numIndents; j++)
                unparsed.push_back('\t');

            unparsed.append("\"");
            unparsed.append(in.keys[i]);
            unparsed.append("\": ");

            std::string val = unparseProperty(in.dictionary[in.keys[i]], numIndents);
            if (i < in.keys.size() - 1)
                val.push_back(',');
            val.append("\n");

            unparsed.append(val);
        }

        for (int j = 0; j < numIndents - 1; j++)
            unparsed.push_back('\t');
        unparsed.push_back('}');
        return unparsed;
    }

    static std::string unparseArray(JSONObject in, int numIndents)
    {
        std::string unparsed;
        unparsed.append("[");

        for (int i = 0; i < in.jeffArray.size(); i++)
        {
            bool isObject = in.jeffArray[i].jeffType == JSONObject::OBJECT;

            if (isObject)
            {
                unparsed.push_back('\n');
                for (int j = 0; j < numIndents; j++)
                    unparsed.push_back('\t');
            }

            std::string val = unparseProperty(in.jeffArray[i], numIndents);
            if (i < in.jeffArray.size() - 1)
            {
                val.append(", ");
            }
            else if (isObject)
            {
                val.append("\n");
                for (int j = 0; j < numIndents - 1; j++)
                    val.push_back('\t');
            }

            unparsed.append(val);
        }

        unparsed.push_back(']');
        return unparsed;
    }

    static std::string unparseProperty(JSONObject value, int numIndents)
    {
        std::string stringval;
        switch (value.jeffType)
        {
        case JSONObject::INT:
            stringval = std::to_string(value.jeffInt);
            break;
        case JSONObject::STRING:
            stringval = value.jeffString;
            stringval.insert(0, "\"");
            stringval.push_back('\"');
            break;
        case JSONObject::BOOL:
            stringval = value.jeffBool ? "true" : "false";
            break;
        case JSONObject::FLOAT:
            stringval = std::to_string(value.jeffFloat);
            for (int i = stringval.length() - 1; i >= 2; i--)
            {
                if (stringval[i] != '0')
                {
                    stringval = stringval.substr(0, i + 1);
                    break;
                }
            }
            break;
        case JSONObject::NULLVALUE:
            stringval = "null";
            break;
        case JSONObject::OBJECT:
            stringval = unparseObject(value, numIndents + 1);
            break;
        case JSONObject::ARRAY:
            stringval = unparseArray(value, numIndents + 1);
            break;
        }
        return stringval;
    }

    static void readJSON(const char* filename, JSONObject* out)
    {
        out->keys.clear();
        out->dictionary.clear();

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
        replaceObjects(body, subObjects);

        // replace arrays as well
        std::vector<std::string> arrays;
        replaceArrays(body, arrays);

        // handle properties
        std::vector<std::string> properties = split(body, ",");
        for (int i = 0; i < properties.size(); i++)
        {
            JSONObject parsedProperty;
            std::string propertyName = parseProperty(properties[i], &parsedProperty, subObjects, arrays);
            out->dictionary[propertyName] = parsedProperty;
            out->keys.emplace_back(propertyName);
        }
    }

    static void parseArray(std::string unparsed, JSONObject* out, std::vector<std::string>& subObjects)
    {
        out->jeffType = JSONObject::ARRAY;

        size_t charPointer = 0;

        // remove whitespace
        while (std::isspace(unparsed[charPointer])) charPointer++;

        if (unparsed[charPointer] == '[') charPointer++;
        else throw std::runtime_error("couldn't parse json: array missing opening bracket");

        // remove whitespace
        while (std::isspace(unparsed[charPointer])) charPointer++;

        // find closing brace and get object body
        size_t closeBracePos = unparsed.rfind(']');
        std::string body = unparsed.substr(charPointer, closeBracePos - charPointer);

        // replace sub-arrays
        std::vector<std::string> arrays;
        replaceArrays(body, arrays);

        // handle elements
        std::vector<std::string> elements = split(body, ",");
        for (int i = 0; i < elements.size(); i++)
        {
            size_t pos = 0;
            JSONObject parsedElement;
            while (std::isspace(elements[i][pos])) pos++;
            parsePropertyValue(elements[i], pos, subObjects, arrays, &parsedElement);
            out->jeffArray.emplace_back(parsedElement);
        }
    }

    static void replaceObjects(std::string& body, std::vector<std::string> &subObjects)
    {
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
    }

    static void replaceArrays(std::string& body, std::vector<std::string>& arrays)
    {
        // find first sub-object start
        size_t arrStart = body.find('[');
        size_t arrEnd = -1;
        while (arrStart != -1)
        {
            // find matching closing bracket
            int numOpenBrackets = 0;
            for (int i = arrStart + 1; i < body.length(); i++)
            {
                if (body[i] == '[')
                {
                    numOpenBrackets++;
                }
                else if (body[i] == ']')
                {
                    if (numOpenBrackets == 0)
                    {
                        arrEnd = i;
                        break;
                    }
                    numOpenBrackets--;
                }
            }
            if (arrEnd == -1) throw std::runtime_error("error parsing JSON: mismatched brackets");

            // store entire sub-object
            std::string unparsedArray = body.substr(arrStart, arrEnd - arrStart + 2);
            // create replacement string
            std::string indexString = std::to_string(arrays.size());

            // add unparsed sub-object to list (for later parsing) and replace with index
            arrays.emplace_back(unparsedArray);
            body.replace(arrStart + 1, arrEnd - arrStart - 1, indexString);
            arrStart = body.find('[', arrStart + 1);
        }
    }


    static std::string parseProperty(std::string unparsed, JSONObject* out, std::vector<std::string> subObjects, std::vector<std::string> arrays)
    {
        size_t i = 0;
        while (std::isspace(unparsed[i])) i++;

        std::string propertyName = parsePropertyName(unparsed, i);

        // skip colon
        i += propertyName.length() + 1;
        while (std::isspace(unparsed[i])) i++;
        if (unparsed[i] != ':') throw std::runtime_error("couldn't parse json: missing colon after property name");
        else i++;
        while (std::isspace(unparsed[i])) i++;

        parsePropertyValue(unparsed, i, subObjects, arrays, out);

        return propertyName;
    }

    static std::string parsePropertyName(std::string unparsed, size_t &i)
    {
        if (unparsed[i] != '"') throw std::runtime_error("couldn't parse json: unquoted property name");
        std::string propertyName = unparsed.substr(++i);
        size_t closeQuote = propertyName.find('"'); if (closeQuote == -1) throw std::runtime_error("couldn't parse json: unquoted property name");
        propertyName = propertyName.substr(0, closeQuote);
        return propertyName;
    }

    static void parsePropertyValue(std::string unparsed, size_t i, std::vector<std::string> subObjects, std::vector<std::string> arrays, JSONObject* out)
    {
        std::string propertyValue = unparsed.substr(i);

        size_t openBracePos = propertyValue.find('{');
        size_t openBracketPos = propertyValue.find('[');

        bool isObject = openBracePos != -1 && (openBracketPos == -1 || openBracePos < openBracketPos);

        // figure out the type of value
        if (isObject)
        {
            int last = propertyValue.find('}');
            std::string innerObject = propertyValue.substr(1, last - 1);
            int objIndex = std::stoi(innerObject);

            parseObject(subObjects[objIndex], out);
        }
        else if (openBracketPos != -1)
        {
            int last = propertyValue.find(']');
            std::string innerObject = propertyValue.substr(1, last - 1);
            int arrIndex = std::stoi(innerObject);

            parseArray(arrays[arrIndex], out, subObjects);
        }
        else if (propertyValue.find('.') != -1)
        {
            out->jeffType = JSONObject::FLOAT;
            out->jeffFloat = std::stof(propertyValue);
        }
        else if (propertyValue.find('"') != -1)
        {
            out->jeffType = JSONObject::STRING;
            int closeQuote = propertyValue.find('"', 1);
            if (closeQuote == -1) throw std::runtime_error("couldn't parse JSON: missing closing quote");
            out->jeffString = propertyValue.substr(1, closeQuote - 1);
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
                out->jeffType = JSONObject::BOOL;
                out->jeffBool = true;
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
                out->jeffType = JSONObject::BOOL;
                out->jeffBool = false;
            }
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
                out->jeffType = JSONObject::NULLVALUE;
            }
        }
        else
        {
            out->jeffType = JSONObject::INT;
            out->jeffInt = std::stoi(propertyValue);
        }
    }

};