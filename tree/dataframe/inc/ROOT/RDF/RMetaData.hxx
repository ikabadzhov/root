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

#include <nlohmann/json.hpp>

#include <string>
#include <unordered_map>

namespace ROOT {
namespace RDF {
namespace Experimental {

class RMetaData {
   nlohmann::json fJson;

public:
   void AddMetaData(const std::string &cat, int val);
   void AddMetaData(const std::string &cat, double val);
   void AddMetaData(const std::string &cat, const std::string &val);
   void AddMetaData(const nlohmann::json &val);

   void SetMeta(const std::string &cat, const nlohmann::json &val);

   void PrintMetaData();

   int GetI(const std::string &cat);

   double GetD(const std::string &cat);

   std::string GetS(const std::string &cat);

   template <typename T>
   T Get(const std::string &cat);
};

} // namespace Experimental
} // namespace RDF
} // namespace ROOT

#endif // ROOT_RDF_RMETADATA
