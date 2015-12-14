#include "HtmlTableWriter.h"


HtmlTableWriter::HtmlTableWriter()
{
}


HtmlTableWriter::~HtmlTableWriter()
{
}

void HtmlTableWriter::saveToFile(ofstream &stream)
{
	stream << "<html><head><title>Table</title></head><body>";
	stream << "<table width=100% height=100% border=1>";
	int rows = 0;
	if (!columns.empty())
		rows = columns[0]->getRowsNum();
	int columns_size = columns.size();
	for (int i = 0; i < rows; i++)
	{
		stream << "<tr>";
		for (int j = 0; j < columns_size; j++)
			columns[j]->saveToFile(stream, i);
		stream << "</tr>";
	}
	stream << "</table></body></html>";
}

