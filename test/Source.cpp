#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>
#include <climits>
#include "HtmlTableWriter.h"


using namespace std;

#define DEBUG(v) cout << #v << " = " << v << "\n";

int dp[101][101];

bool isSameStrings(string &s1, string &s2, int p)
{
	int sz1 = s1.length();
	int sz2 = s2.length();
	if (abs(sz1 - sz2) > 1)
		return false;
	for (int i = 0; i <= sz1; i++)
		dp[i][0] = i;
	for (int i = 0; i <= sz2; i++)
		dp[0][i] = i;
	for (int i = 1; i <= sz1; i++)
	{
		for (int j = 1; j <= sz2; j++)
		{
			dp[i][j] = min(dp[i - 1][j] + 1,
				min(dp[i][j - 1] + 1, dp[i - 1][j - 1] + (s1[i - 1] != s2[j - 1])));
		}
	}
	
	return dp[sz1][sz2] <= p && (double)dp[sz1][sz2] / (double)sz1 < 0.3;
}

vector <string> files = { 
	string("ARWU2013.txt"),
	string("QS2013.txt"),
	string("THE2013.txt"),
};

vector <vector <string> > universities;

struct file_data
{
	vector <string> columns;
	vector <vector <string> > univ;
	vector <string> data;
	int u_row;
	int rows;
};

void read_file(string file_path, file_data &d)
{
	ifstream f(file_path);
	f >> d.rows >> d.u_row;
	string cur;
	char c;
	f.read(&c, 1);
	int cnt = 0;
	while (1)
	{
		f.read(&c, 1);
		if (c == '\n')
			break;
		if (c == '[')
			cur.clear();
		else if (c == ']')
		{
			cnt++;
			if (cnt != d.u_row)
				d.columns.push_back(cur);
		}
		else 
			cur.push_back(c);
	}
	cnt = 0;
	vector <string> u;
	//bool u_read = false;
	while (!f.eof())
	{
		f >> cur;
		if (cnt == d.u_row - 1)
		{
			if(cur[0] != '|')
				u.push_back(cur);
			else
			{
				d.univ.push_back(u);
				u.clear();
				cnt++;
			}
		}
		else
		{
			d.data.push_back(cur);
			cnt++;
		}
		if (cnt == d.rows)
			cnt = 0;
	}
}

int find_university(vector <string> v)
{
	for (int i = 0; i < universities.size(); i++)
	{
		vector <bool> coincidents;
		coincidents.resize(universities[i].size());
		int find = 0;
		for (int j = 0; j < v.size(); j++)
		{
			for (int k = 0; k < universities[i].size(); k++)
			{
				if (!coincidents[k])
				{
					if (isSameStrings(v[j], universities[i][k], 2))
					{
						find++;
						coincidents[k] = true;
					}
				}
			}
		}

		if (find == (int)v.size() || find == (int)universities[i].size())
			return i;
	}
	return -1;
}

string universityToString(vector <string> v)
{
	string s = "";
	if (!v.empty())
		s += v[0];
	for (int i = 1; i < v.size(); i++)
	{
		s += " " + v[i];
	}
	return s;
}

vector <vector <string> > tableData;

int main()
{
	vector <file_data> DATA;
	printf("Start reading files...\n");
	int columns = 0;
	for (int i = 0; i < files.size(); i++)
	{
		printf("\nread file %s..\n", files[i].c_str());
		file_data d;
		read_file(files[i], d);
		DATA.push_back(d);
		columns += d.columns.size();
	}
	printf("Reading was finished\n");
	printf("Start analysing files...\n");
	int filled_columns = 0;
	for (int i = 0; i < files.size(); i++)
	{
		for (int j = 0; j < DATA[i].univ.size(); j++)
		{
			int univ_id = find_university(DATA[i].univ[j]);
			if (univ_id == -1)
			{
				universities.push_back(DATA[i].univ[j]);
				cout << "new university " << universityToString(universities.back()) << "\n";
				tableData.push_back(vector <string>());
				tableData.back().resize(columns);
				univ_id = (int)tableData.size() - 1;
			}
			int to = (j + 1) * DATA[i].columns.size();
			for (int k = j * DATA[i].columns.size(), idx = 0; k < to; k++, idx++)
			{
				tableData[univ_id][idx + filled_columns] = DATA[i].data[k];
			}
		}
		filled_columns += DATA[i].columns.size();
	}

	HtmlTableWriter *table = new HtmlTableWriter();
	HtmlColumn *univColumn = new HtmlColumn();
	univColumn->rowsHeight = 2;
	univColumn->title = "University";
	table->addColumn(univColumn);
	HtmlColumnType <string> *univColumnData = new HtmlColumnType <string>();
	univColumn->addColumn(univColumnData);
	for (int i = 0; i < universities.size(); i++)
	{
		univColumnData->addData(universityToString(universities[i]));
	}
	vector <HtmlColumnType <string>*> dataColumns;
	for (int i = 0; i < files.size(); i++)
	{
		HtmlColumn *fileColumn = new HtmlColumn();
		fileColumn->title = files[i];
		fileColumn->rowsHeight = 1;
		table->addColumn(fileColumn);
		for (int j = 0; j < DATA[i].columns.size(); j++)
		{
			HtmlColumn *c = new HtmlColumn();
			c->title = DATA[i].columns[j];
			c->rowsHeight = 1;
			fileColumn->addColumn(c);
			HtmlColumnType <string> *cData = new HtmlColumnType<string>();
			c->addColumn(cData);
			dataColumns.push_back(cData);
		}
	}
	for (int i = 0; i < tableData.size(); i++)
	{
		for (int j = 0; j < tableData[i].size(); j++)
		{
			dataColumns[j]->addData(tableData[i][j]);
		}
	}
	table->saveToFile(ofstream("table2013.html"));
	printf("Finish!!!\n");
	return 0;
}
 