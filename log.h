#ifndef LOG_H
#define LOG_H
#include <cstdarg>
#include <iostream>
#include <time.h>
#include <direct.h>
#pragma warning(disable:4996)

struct comparison
{
	int year;
	int month;
	int day;
	int hour;
	comparison()
	{
		year = 0;
		month = 0;
		day = 0;
		hour = 0;
	}
};

//#define LOGFILE(
class logfile
{
public:
	logfile();
	~logfile();
	std::string GetTag(bool flag = false);
	void Open();
	void DebugPrintf(const char *p, ...);
	void Close();
private:
	FILE* m_fp;
	comparison temp_time_comp;	//对比时辰用的
};

logfile::logfile()
{
	m_fp = NULL;
	Open();
}

logfile::~logfile()
{
	if (m_fp != NULL)
	{
		fclose(m_fp);
	}
}

void logfile::Open()
{
	Close();
	std::string abc = "日志测试\\";
	mkdir(abc.c_str());
	char buf[256];
	memset(buf, 0, sizeof(buf));
	sprintf_s(buf, "%s%s.txt", abc.c_str(), GetTag().c_str());
	m_fp= fopen(buf, "a");
	if (m_fp == NULL)
		return;

}

void logfile::DebugPrintf(const char *p, ...)	//中庸版，缺点前面需写类名，太麻烦太长，但程序析构时会自动关闭
{
	if (m_fp != NULL)
	{
		std::string temp_string;
		temp_string = GetTag(true);
		if (m_fp == NULL)//GetTag内有可能导致打开失败，特此
		{
			//尝试再打开一次
			Open();
			if (m_fp == NULL)
			{
				return;
			}
		}

		va_list arg;
		va_start(arg, p);
		fprintf(m_fp, temp_string.c_str());
		vfprintf(m_fp, p, arg);
		fprintf(m_fp, "\n");
		va_end(arg);
		fflush(m_fp);
	
	}
}
void logfile::Close()
{
	m_fp = NULL;
}

std::string logfile::GetTag(bool flag)
{
	time_t nowTs = time(NULL);
	tm time_data;
	localtime_s(&time_data, &nowTs);
	char timeStrBuf[256];
	memset(timeStrBuf, 0, sizeof(timeStrBuf));
	int tm_year = 1900 + time_data.tm_year;
	int tm_mon = time_data.tm_mon + 1;
	if (flag)	//日志内
	{
		//此处做个预防，防止日志都打印在同一个时辰文件中
		if (m_fp != NULL)
		{
			if (temp_time_comp.year == tm_year&&
				temp_time_comp.month == tm_mon&&
				temp_time_comp.day == time_data.tm_mday&&
				temp_time_comp.hour == time_data.tm_hour)
			{
				//不写
			}
			else
			{
				//当有一个条件对不上，则重打开一个文件
				fclose(m_fp);
				Open();

			}
		}


		sprintf_s(timeStrBuf, "%d-%d-%d [%d:%d:%d] ", tm_year,
			tm_mon,
			time_data.tm_mday,
			time_data.tm_hour,
			time_data.tm_min,
			time_data.tm_sec);
	}
	else //创建文件专用的
	{
		sprintf_s(timeStrBuf, "%d-%d-%d-%d", tm_year,
			tm_mon,
			time_data.tm_mday,
			time_data.tm_hour);

		temp_time_comp.year = tm_year;
		temp_time_comp.month = tm_mon;
		temp_time_comp.day = time_data.tm_mday;
		temp_time_comp.hour = time_data.tm_hour;
	}
	
	std::string ret;
	ret.insert(ret.end(), timeStrBuf, timeStrBuf + strlen(timeStrBuf));
	return ret;
}


static FILE* zhuge_fp = NULL;
static int zhuge_fp_year = 0;
static int zhuge_fp_mon = 0;
static int zhuge_fp_day = 0;
static int zhuge_fp_hour = 0;
void DebugPrintf_2(const char *p, ...)//极快版，缺点后期程序需加写一行fclose来主动关闭zhuge_fp,此程序不会主动关闭zhuge_fp
{
	std::string abc = "日志\\";//先建个目录
	if (zhuge_fp == NULL || zhuge_fp_year==0)
	{
		mkdir(abc.c_str());
	}
	char buf[256];					//然后打开哪个文件
	char timeStrBuf[256];		//前面加上时间格式
	memset(buf, 0, sizeof(buf));
	memset(timeStrBuf, 0, sizeof(timeStrBuf));

	time_t nowTs = time(NULL);
	tm time_data;
	localtime_s(&time_data, &nowTs);

	int tm_year = 1900 + time_data.tm_year;
	int tm_mon = time_data.tm_mon + 1;

	sprintf_s(buf, "%s%d-%d-%d-%d_log.txt", 
		abc.c_str(),
		tm_year,
		tm_mon,
		time_data.tm_mday,
		time_data.tm_hour);

	sprintf_s(timeStrBuf, "%d-%d-%d [%d:%d:%d] ",
		tm_year,
		tm_mon,
		time_data.tm_mday,
		time_data.tm_hour,
		time_data.tm_min,
		time_data.tm_sec);

	if (zhuge_fp == NULL || zhuge_fp_year == 0)
	{
		zhuge_fp_year = tm_year;
		zhuge_fp_mon = tm_mon;
		zhuge_fp_day = time_data.tm_mday;
		zhuge_fp_hour = time_data.tm_hour;
		zhuge_fp = fopen(buf, "a");//若有空，则记得是否已fclose过,保存zhuge_fp后可以避免频繁打开
	}

	if (zhuge_fp_hour == time_data.tm_hour&&
		zhuge_fp_day == time_data.tm_mday&&
		zhuge_fp_mon == tm_mon&&
		zhuge_fp_year == tm_year 	)
	{
		//相同时间段，可不用重复打开
	}
	else
	{
		//一旦对不上重新打开
		if (zhuge_fp != NULL)
		{
			fclose(zhuge_fp);
		}
		zhuge_fp_year = tm_year;
		zhuge_fp_mon = tm_mon;
		zhuge_fp_day = time_data.tm_mday;
		zhuge_fp_hour = time_data.tm_hour;
		zhuge_fp = fopen(buf, "a");
	}

	if (zhuge_fp == NULL)
		return;
		
		//
		va_list arg;
		va_start(arg, p);
		fprintf(zhuge_fp, timeStrBuf);
		vfprintf(zhuge_fp, p, arg);
		fprintf(zhuge_fp, "\n");
	//	fclose(zhuge_fp);//这里先不关
		va_end(arg);
		fflush(zhuge_fp);
}

void Close_fp()
{
	if (zhuge_fp != NULL)
	{
		fclose(zhuge_fp);
	}
}

//最省事版，效率堪忧，若日志量多，频繁打开也是负担，但胜在安全开闭
//外部直接引用即可头文件，调用DebugPrintf_3即可，
void DebugPrintf_3(const char *p, ...)
{
	std::string abc = "日志\\";//先建个目录
	mkdir(abc.c_str());

	char buf[256];					//然后打开哪个文件
	char timeStrBuf[256];		//前面加上时间格式
	memset(buf, 0, sizeof(buf));
	memset(timeStrBuf, 0, sizeof(timeStrBuf));

	time_t nowTs = time(NULL);
	tm time_data;
	localtime_s(&time_data, &nowTs);

	int tm_year = 1900 + time_data.tm_year;
	int tm_mon = time_data.tm_mon + 1;

	sprintf_s(buf, "%s%d-%d-%d-%d_log.txt",
		abc.c_str(),
		tm_year,
		tm_mon,
		time_data.tm_mday,
		time_data.tm_hour);

	sprintf_s(timeStrBuf, "%d-%d-%d [%d:%d:%d] ",
		tm_year,
		tm_mon,
		time_data.tm_mday,
		time_data.tm_hour,
		time_data.tm_min,
		time_data.tm_sec);

	FILE*	 fp = fopen(buf, "a");//若有空，则记得是否已fclose过
	
	if (fp == NULL)
		return;

	va_list arg;
	va_start(arg, p);
	fprintf(fp, timeStrBuf);
	vfprintf(fp, p, arg);
	fprintf(fp, "\n");
	fclose(fp);
	va_end(arg);
}



#endif
