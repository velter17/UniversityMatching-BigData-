#pragma once

#include <vector>
#include <string>
#include <fstream>

using namespace std;

class HtmlColumn
{
public:
	HtmlColumn();
	~HtmlColumn();

	virtual void saveToFile(ofstream &stream, int row_id);
	virtual int getRowsNum();

	string title;
	int rowsHeight;

	void addColumn(HtmlColumn *c);
	int getColumnsNum();
protected:
	vector <HtmlColumn*> columns;
};

template <typename T> class HtmlColumnType : public HtmlColumn
{
public:

	void saveToFile(ofstream &stream, int row_id) override;
	int getRowsNum() override;

	void addData(T d);
private:
	vector <T> data;
};

template class HtmlColumnType<string>;
template class HtmlColumnType<double>;




