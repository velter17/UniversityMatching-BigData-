#pragma once

#include "HtmlColumn.h"

class HtmlTableWriter : public HtmlColumn
{

public:
	HtmlTableWriter();
	~HtmlTableWriter();

	void saveToFile(ofstream &stream);
};

