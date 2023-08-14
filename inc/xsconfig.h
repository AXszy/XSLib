/**
 * @file xsconfig.h 
 * @brief: 读取和写入配置文件的类
 * @author: XSSunet
 * @date: 2023-08-10 01:03:51
 * 
 **/
#ifndef XSCONFIG_H
#define XSCONFIG_H

#include <map>
#include <string>

class XSConfig {
public:
	XSConfig() = delete;
	XSConfig(std::string filename);			/*构造函数*/
	~XSConfig();							/*析构函数*/
	std::string read(const char *section, const char *item);		/*读取数据*/
	int read_int(const char *section, const char *item);			/*读取数据*/
	double read_float(const char *section, const char *item);		/*读取数据*/
	
	void set(const char *section, const char *item, const char *value);		/*设置数据*/
	void set_int(const char *section, const char *item, int value);			/*设置数据*/
	void set_float(const char *section, const char *item, double value);	/*设置数据*/
	
	void open(std::string filename);		/*打开配置文件*/
	void save(std::string filename);		/*保存配置文件*/
private:
	std::map<std::string, std::map<std::string, std::string>> m_info;		/*文件内容*/
	/*说明        section               item         value */
	bool m_is_space(char c);												/*判断字符是否是空格或缩进*/
	void m_trim(std::string &str);											/*去除字符串首尾空格*/
	bool m_analyse_line(const std::string &line, 
		std::string &section, std::string &key, std::string &value);		/*分析一行数据*/
};

#endif
