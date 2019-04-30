#include "RigidbodySearcher.h"
#include "FieldManager.h"
#include "nanoflann.hpp"

RigidbodySearcher::RigidbodySearcher()
	: m_index(2, m_cloud, KDTreeSingleIndexAdaptorParams(10000))
{

}

RigidbodySearcher::~RigidbodySearcher()
{

}

std::vector<std::pair<size_t, double>>  RigidbodySearcher::GetNearRigidbodies(Vector2D position, double radius) const
{
	const double query_pt[2] = {  position.m_x, position.m_y };
	const double search_radius = radius * radius;
	std::vector<std::pair<size_t, double>>   ret_matches;
	nanoflann::SearchParams params;

	const size_t nMatches = m_index.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

	return ret_matches;
}

unique_ptr<RigidbodySearcher> g_rigidbodySearcherPtr = nullptr;
