#include "HtmlColumn.h"


HtmlColumn::HtmlColumn()
{
}


HtmlColumn::~HtmlColumn()
{
}

void HtmlColumn::saveToFile(ofstream &stream, int row_id)
{
	if (row_id < rowsHeight)
	{
		if (row_id == 0)
			stream << "<td colspan = " << getColumnsNum() << " rowspan = " << rowsHeight << ">" << title << "</td>";
	}
	else
	{
		int sz = columns.size();
		for (int i = 0; i < sz; i++)
		{
			columns[i]->saveToFile(stream, row_id - rowsHeight);
		}
	}
}

void HtmlColumn::addColumn(HtmlColumn *c)
{
	columns.push_back(c);
}

int HtmlColumn::getColumnsNum()
{
	if (columns.empty())
		return 1;
	else
	{
		int sz = columns.size();
		int cnt = 0;
		for (int i = 0; i < sz; i++)
			cnt += columns[i]->getColumnsNum();
		return cnt;
	}
}

int HtmlColumn::getRowsNum()
{
	if (!columns.empty())
		return rowsHeight + columns[0]->getRowsNum();
	return rowsHeight;
}


template <typename T>
void HtmlColumnType<T>::saveToFile(ofstream &stream, int row_id)
{
	stream << "<td>" << data[row_id] << "</td>";
}

template <typename T>
void HtmlColumnType<T>::addData(T d)
{
	data.push_back(d);
}

template <typename T>
int HtmlColumnType<T>::getRowsNum()
{
	return data.size();
}