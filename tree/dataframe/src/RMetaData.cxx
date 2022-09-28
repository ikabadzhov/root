// Author: Ivan Kabadzhov CERN  10/2022

/*************************************************************************
 * Copyright (C) 1995-2022, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "ROOT/RDF/RMetaData.hxx"

#include <iostream>

namespace ROOT {
namespace RDF {
namespace Experimental {

void RMetaData::AddMetaData(const std::string &cat, int val)
{
   fJson[cat] = val;
}

void RMetaData::AddMetaData(const std::string &cat, double val)
{
   fJson[cat] = val;
}

void RMetaData::AddMetaData(const std::string &cat, const std::string &val)
{
   fJson[cat] = val;
}

void RMetaData::AddMetaData(const nlohmann::json &val)
{
   fJson.insert(val.begin(), val.end());
}

void RMetaData::SetMeta(const std::string &cat, const nlohmann::json &val)
{
   fJson[cat] = val;
}

void RMetaData::PrintMetaData()
{
   std::cout << fJson.dump() << std::endl;
}

int RMetaData::GetI(const std::string &cat)
{
   return fJson[cat].get<int>();
}

double RMetaData::GetD(const std::string &cat)
{
   return fJson[cat].get<double>();
}

std::string RMetaData::GetS(const std::string &cat)
{
   return fJson[cat].get<std::string>();
}

template <typename T>
T RMetaData::Get(const std::string &cat)
{
   return fJson[cat].get<T>();
}

} // namespace Experimental
} // namespace RDF
} // namespace ROOT
