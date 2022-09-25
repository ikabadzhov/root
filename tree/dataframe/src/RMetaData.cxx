// Author: Ivan Kabadzhov CERN  10/2022

/*************************************************************************
 * Copyright (C) 1995-2022, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "ROOT/RDF/RMetaData.hxx"

namespace ROOT {
namespace RDF {
namespace Experimental {

RMetaData::RMetaData() {}

RMetaData &RMetaData::AddMetaData(const std::string &category, int value) {
   fContainer[category] = value;
   return *this;
}

RMetaData &RMetaData::AddMetaData(const std::string &category, float value) {
   fContainer[category] = value;
   return *this;
}

RMetaData &RMetaData::AddMetaData(const std::string &category, const std::string &value) {
   fContainer[category] = value;
   return *this;
}

mpark::variant<int, float, std::string> &RMetaData::operator[](const std::string &key)
{
   // TODO: maybe sanity checks
   return fContainer[key];
}

template <typename T>
T RMetaData::Get(const std::string &key) {
   return mpark::get<T>(fContainer[key]);
}

} // namespace Experimental
} // namespace RDF
} // namespace ROOT
