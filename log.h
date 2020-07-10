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
	comparison temp_time_comp;	//�Ա�ʱ���õ�
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
	std::string abc = "��־����\\";
	mkdir(abc.c_str());
	char buf[256];
	memset(buf, 0, sizeof(buf));
	sprintf_s(buf, "%s%s.txt", abc.c_str(), GetTag().c_str());
	m_fp= fopen(buf, "a");
	if (m_fp == NULL)
		return;

}

void logfile::DebugPrintf(const char *p, ...)	//��ӹ�棬ȱ��ǰ����д������̫�鷳̫��������������ʱ���Զ��ر�
{
	if (m_fp != NULL)
	{
		std::string temp_string;
		temp_string = GetTag(true);
		if (m_fp == NULL)//GetTag���п��ܵ��´�ʧ�ܣ��ش�
		{
			//�����ٴ�һ��
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
	if (flag)	//��־��
	{
		//�˴�����Ԥ������ֹ��־����ӡ��ͬһ��ʱ���ļ���
		if (m_fp != NULL)
		{
			if (temp_time_comp.year == tm_year&&
				temp_time_comp.month == tm_mon&&
				temp_time_comp.day == time_data.tm_mday&&
				temp_time_comp.hour == time_data.tm_hour)
			{
				//��д
			}
			else
			{
				//����һ�������Բ��ϣ����ش�һ���ļ�
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
	else //�����ļ�ר�õ�
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
void DebugPrintf_2(const char *p, ...)//����棬ȱ����ڳ������дһ��fclose�������ر�zhuge_fp,�˳��򲻻������ر�zhuge_fp
{
	std::string abc = "��־\\";//�Ƚ���Ŀ¼
	if (zhuge_fp == NULL || zhuge_fp_year==0)
	{
		mkdir(abc.c_str());
	}
	char buf[256];					//Ȼ����ĸ��ļ�
	char timeStrBuf[256];		//ǰ�����ʱ���ʽ
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
		zhuge_fp = fopen(buf, "a");//���пգ���ǵ��Ƿ���fclose��,����zhuge_fp����Ա���Ƶ����
	}

	if (zhuge_fp_hour == time_data.tm_hour&&
		zhuge_fp_day == time_data.tm_mday&&
		zhuge_fp_mon == tm_mon&&
		zhuge_fp_year == tm_year 	)
	{
		//��ͬʱ��Σ��ɲ����ظ���
	}
	else
	{
		//һ���Բ������´�
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
	//	fclose(zhuge_fp);//�����Ȳ���
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

//��ʡ�°棬Ч�ʿ��ǣ�����־���࣬Ƶ����Ҳ�Ǹ�������ʤ�ڰ�ȫ����
//�ⲿֱ�����ü���ͷ�ļ�������DebugPrintf_3���ɣ�
void DebugPrintf_3(const char *p, ...)
{
	std::string abc = "��־\\";//�Ƚ���Ŀ¼
	mkdir(abc.c_str());

	char buf[256];					//Ȼ����ĸ��ļ�
	char timeStrBuf[256];		//ǰ�����ʱ���ʽ
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

	FILE*	 fp = fopen(buf, "a");//���пգ���ǵ��Ƿ���fclose��
	
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
