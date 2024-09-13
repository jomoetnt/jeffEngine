#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>

class jeffJSON
{
public:
    union JSONValue
    {
        int jeffInt;
        float jeffFloat;
        bool jeffBool;
        std::string jeffString;
        std::vector<JSONValue> jeffArray;
        std::unique_ptr<JSONValue> jeffObject;
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

    static std::unordered_map<std::string, std::string> readJSON(const char* filename)
    {
        std::unordered_map<std::string, std::string> dictionary;

        std::ifstream file(filename);

        std::string line;
        if (file.is_open())
        {

            while (std::getline(file, line))
            {
                while (line.front() == '\t' || line.front == ' ')
                    line.erase(0);
                if (line.front() == '{')
                {

                }
            }
            file.close();
        }
        else
        {
            throw std::runtime_error("could not open JSON");
        }
    }

    static 
};