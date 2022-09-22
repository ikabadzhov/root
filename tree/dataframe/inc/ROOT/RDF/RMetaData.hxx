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
   std::unordered_map<std::string, std::string> fContainer;

public:
   RMetaData(std::unordered_map<std::string, std::string> &metaData);

   // overload the square brackets
   std::string &operator[](std::string key);
};

} // namespace Experimental
} // namespace RDF
} // namespace ROOT

#endif // ROOT_RDF_RMETADATA