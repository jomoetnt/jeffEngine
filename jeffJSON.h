#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include <stack>
#include <optional>
#include <sstream>

class jeffJSON
{
public:
    union JSONObject
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
        };
        JSONValue val;
        std::unordered_map<std::string, std::optional<JSONValue>> dictionary;
        ~JSONObject() = default;
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

    static std::unordered_map<std::string, JSONObject> readJSON(const char* filename)
    {
        std::unordered_map<std::string, JSONObject> dictionary;

        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        JSONObject rootObject = parseObject(buffer.str());
    }

    static JSONObject parseObject(std::string unparsed)
    {
        while (line.front() == '\t' || line.front() == ' ')
            line.erase(0);
        if (line.front() == '{')
        {
            continue;
        }
        else if (line.front() == '}')
        {
            objects.pop();
        }
        else
        {
            line = replace(line, " ");
            std::vector<std::string> halves = split(line, ":");
            std::string strippedName = replace(halves[0], "\"");

            JSONObject newObj;
            if (halves[1].front() == '{')
            {
                newObj.name = strippedName;
                objects.push(newObj);
            }
            else
            {
                if (halves[1].compare("true") == 0)
                {
                    newObj.val->jeffBool = true;
                }
                dictionary[strippedName] = newObj;
            }
        }
    }

};