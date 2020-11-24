#pragma once
#include <iostream>
#include <deque>
#include <sstream>
#include <stack>
#include <string>

class Logger final
{
	static Logger& getInstance()
	{
		static Logger instance{};
		return instance;
	}
	std::deque<std::string> logs;
	Logger(){}
	~Logger() = default;
public:
	static void log(const std::string msg)
	{
		auto& inst = getInstance();
		
		if (inst.logs.size() > 20)
			inst.logs.pop_front();
		inst.logs.push_back(msg);
	}

	static std::string dumpLogs()
	{
		std::stringstream out;
		auto& inst = getInstance();
		
		if (inst.logs.empty())
			return "";
		 
		for(auto& msg: inst.logs)
		{
			out << msg<<"\n";
		}
		return out.str();
	}
};
