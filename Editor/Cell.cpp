#include "Cell.h"

ptree CellModel::ToJSON() const
{
	ptree pt;

	// name
	pt.put("name", m_name);

	return pt;
}
