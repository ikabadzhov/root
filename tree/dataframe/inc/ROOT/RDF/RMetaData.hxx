// Author: Ivan Kabadzhov CERN  10/2022

/*************************************************************************
 * Copyright (C) 1995-2022, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_RDF_RMETADATA
#define ROOT_RDF_RMETADATA

#include <ROOT/variant.hxx> // std::variant backport

#include <string>
#include <unordered_map>

namespace ROOT {
namespace RDF {
namespace Experimental {

// TODO: use the backported std::variant, might also need std::reference_wrapper and std::visit
class RMetaData {
   std::unordered_map<std::string, mpark::variant<int, float, std::string>> fContainer;

public:
   RMetaData();

   // Python does not understand the variant now, hence hide it from it!
   RMetaData &AddMetaData(const std::string &category, int value);
   RMetaData &AddMetaData(const std::string &category, float value);
   RMetaData &AddMetaData(const std::string &category, const std::string &value);

   // overload the square brackets
   mpark::variant<int, float, std::string> &operator[](const std::string &key);

   template <typename T>
   T Get(const std::string &key);
};

} // namespace Experimental
} // namespace RDF
} // namespace ROOT

#endif // ROOT_RDF_RMETADATA