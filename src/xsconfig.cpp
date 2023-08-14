/**
 * @file xsconfig.cpp 
 * @brief: 配置文件类的实现
 * @author: XSSunet
 * @date: 2023-08-10 01:04:15
 * 
 **/
#include <fstream>
#include <cstdlib>
#include "xsconfig.h"

XSConfig::XSConfig(std::string filename)
{
	this->m_info.clear();
	std::ifstream ifs(filename.c_str());
	std::string str_line, str_key, str_value, str_section;
	std::map<std::string, std::string> k_v;
	std::map<std::string, std::map<std::string, std::string> >::iterator it;
	while (getline(ifs, str_line))
	{
		if (this->m_analyse_line(str_line, str_section, str_key, str_value))
		{
			it = this->m_info.find(str_section);
			if (it != this->m_info.end())
			{
				k_v[str_key] = str_value;
				it->second = k_v;
			}
			else
			{
				k_v.clear();
				this->m_info.insert(std::make_pair(str_section, k_v));
			}
		}
		str_key.clear();
		str_value.clear();
	}
	ifs.close();
}

XSConfig::~XSConfig()
{
	
}

void XSConfig::set(const char *section, const char *item, const char *value)
{
	std::string str_section(section);
	std::string str_item(item);
	std::string str_val(value);
	if(this->m_info.count(str_section))
	{
		if(this->m_info[str_section].count(str_item))
		{
			this->m_info[str_section][str_item] = str_val;
		}
		else
		{
			this->m_info[str_section].insert(std::make_pair(str_item, str_val));
		}
	}
	else
	{
		std::map<std::string, std::string> k_v;
		k_v.insert(std::make_pair(str_item, str_val));
		this->m_info.insert(std::make_pair(str_section, k_v));
	}
	
}


void XSConfig::set_int(const char *section, const char *item, int value)
{
	std::string str_section(section);
	std::string str_item(item);
	char tmp[260];
	sprintf(tmp, "%d", value);
	std::string str_value(tmp);
	if(this->m_info.count(str_section))
	{
		if(this->m_info[str_section].count(str_item))
		{
			this->m_info[str_section][str_item] = str_value;
		}
		else
		{
			this->m_info[str_section].insert(std::make_pair(str_item, str_value));
		}
	}
	else
	{
		std::map<std::string, std::string> k_v;
		k_v.insert(std::make_pair(str_item, str_value));
		this->m_info.insert(std::make_pair(str_section, k_v));
	}
}

void XSConfig::set_float(const char *section, const char *item, double value)
{
	std::string str_section(section);
	std::string str_item(item);
	char tmp[260];
	sprintf(tmp, "%f", value);
	std::string str_value(tmp);
	if(this->m_info.count(str_section))
	{
		if(this->m_info[str_section].count(str_item))
		{
			this->m_info[str_section][str_item] = str_value;
		}
		else
		{
			this->m_info[str_section].insert(std::make_pair(str_item, str_value));
		}
	}
	else
	{
		std::map<std::string, std::string> k_v;
		k_v.insert(std::make_pair(str_item, str_value));
		this->m_info.insert(std::make_pair(str_section, k_v));
	}
}

void XSConfig::open(std::string filename)
{
	this->m_info.clear();
	std::ifstream ifs(filename.c_str());
	std::string str_line, str_key, str_value, str_section;
	std::map<std::string, std::string> k_v;
	std::map<std::string, std::map<std::string, std::string> >::iterator it;
	while (getline(ifs, str_line))
	{
		if (this->m_analyse_line(str_line, str_section, str_key, str_value))
		{
			it = this->m_info.find(str_section);
			if (it != this->m_info.end())
			{
				k_v[str_key] = str_value;
				it->second = k_v;
			}
			else
			{
				k_v.clear();
				this->m_info.insert(std::make_pair(str_section, k_v));
			}
		}
		str_key.clear();
		str_value.clear();
	}
	ifs.close();
}

void XSConfig::save(std::string filename)
{
	std::ofstream ofs(filename.c_str());
	for(auto i : this->m_info)
	{
		ofs << '[' << i.first << ']' << std::endl;
		for(auto j : i.second)
		{
			ofs << j.first << '=' << j.second << std::endl;
		}
		ofs << std::endl;
	}
	ofs.close();
}

bool XSConfig::m_analyse_line(const std::string & line, 
	std::string& section, std::string & key, std::string & value)
{
	//空行不用分析
	if (line.empty())
		return false;
	int ind_left = 0, ind_right = line.size() - 1, pos, s_startpos, s_endpos;
	//注释不用分析
	if ((pos = line.find(";")) != -1)
	{
		if (pos == 0)
		{
			return false;
		}
		ind_right = pos - 1;
	}
	//读取到section
	if (((s_startpos = line.find("[")) != -1) && ((s_endpos = line.find("]"))) != -1)
	{
		section = line.substr(s_startpos + 1, s_endpos - 1);
		return true;
	}
	//分析有效的内容
	std::string useful_line = line.substr(ind_left, ind_left + 1 - ind_right);
	//没找到等号
	if ((pos = useful_line.find('=')) == -1)
		return false;
	//将键和值分开
	key = useful_line.substr(0, pos);
	value = useful_line.substr(pos + 1, ind_right + 1 - (pos + 1));
	this->m_trim(key);
	this->m_trim(value);
	//键名为空
	if (key.empty()) 
	{
		return false;
	}
	//去掉回车和换行
	if ((pos = value.find("\r")) > 0)
	{
		value.replace(pos, 1, "");
	}
	if ((pos = value.find("\n")) > 0)
	{
		value.replace(pos, 1, "");
	}
	return true;
}

void XSConfig::m_trim(std::string &str)
{
	if (str.empty())
	{
		return;
	}
	unsigned i, ind_left, ind_right;
	for (i = 0; i < str.size(); i++) 
	{
		if (!this->m_is_space(str[i])) 
		{
			break;
		}
	}
	if (i == str.size())
	{
		str = "";
		return;
	}
	ind_left = i;
	for (i = str.size() - 1; i >= 0; i--) 
	{
		if (!this->m_is_space(str[i])) 
		{
			break;
		}
	}
	ind_right = i;
	str = str.substr(ind_left, ind_right - ind_left + 1);
}

bool XSConfig::m_is_space(char c)
{
	if (c == ' ' || c == '\t')
		return true;
	return false;
}

std::string XSConfig::read(const char *section, const char *item)
{
	std::string str_section(section);
	std::string str_item(item);
	std::map<std::string, std::string> k_v;
	std::map<std::string, std::string>::iterator it_item;
	std::map<std::string, std::map<std::string, std::string> >::iterator it;
	it = this->m_info.find(str_section);
	if(it == this->m_info.end())
	{
		this->set(section, item, "(None)");
		return "(None)";
	}
	k_v = it->second;
	it_item = k_v.find(str_item);
	if(it_item == k_v.end())
	{
		this->set(section, item, "(None)");
		return "(None)";
	}
	return it_item->second;
}

int XSConfig::read_int(const char *section, const char *item)
{
	std::string str_section(section);
	std::string str_item(item);
	std::map<std::string, std::string> k_v;
	std::map<std::string, std::string>::iterator it_item;
	std::map<std::string, std::map<std::string, std::string> >::iterator it;
	it = this->m_info.find(str_section);
	if(it == this->m_info.end())
	{
		this->set(section, item, "(None)");
		return 0;
	}
	k_v = it->second;
	it_item = k_v.find(str_item);
	if(it_item == k_v.end())
	{
		this->set(section, item, "(None)");
		return 0;
	}
	return atoi(it_item->second.c_str());
}

double XSConfig::read_float(const char *section, const char *item)
{
	std::string str_section(section);
	std::string str_item(item);
	std::map<std::string, std::string> k_v;
	std::map<std::string, std::string>::iterator it_item;
	std::map<std::string, std::map<std::string, std::string> >::iterator it;
	it = this->m_info.find(str_section);
	if(it == this->m_info.end())
	{
		this->set(section, item, "(None)");
		return 0;
	}
	k_v = it->second;
	it_item = k_v.find(str_item);
	if(it_item == k_v.end())
	{
		this->set(section, item, "(None)");
		return 0;
	}
	return atof(it_item->second.c_str());
}
