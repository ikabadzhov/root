#include "ROOT/RDF/RDFFromJSON.hxx"
#include "ROOT/RDF/RDatasetSpec.hxx"
#include "ROOT/RDF/RMetaData.hxx"

#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept> // std::logic_error

namespace ROOT {
namespace RDF {
namespace Experimental {

ROOT::RDataFrame MakeDataFrameFromJSON(const std::string &jsonFile)
{
   const nlohmann::json fullData = nlohmann::json::parse(std::ifstream(jsonFile));
   SpecBuilder specBuilder;
   /*
   for (const auto& group : fullData["groups"].items()) {
      if (el.key() == "trees")
         std::vector<std::string> trees = group["trees"];
      std::vector<std::string> files = group["files"];
      if (trees.size() != files.size() && trees.size() > 1)
         throw std::logic_error("Mismatch between number of trees and files.")
      std::string tag = (group.find("tag") != group.end()) ? group["tag"] : "";

      RMetaData m();

   }
   */

   for (const auto &groups : fullData["groups"]) {
      std::string tag = groups["tag"];
      std::vector<std::string> trees = groups["trees"];
      std::vector<std::string> files = groups["files"];
      if (files.size() != trees.size() && trees.size() > 1)
         throw std::runtime_error("Mismatch between trees and files.");
      RMetaData m;
      // m.AddMetaData(groups["metadata"]);
      for (const auto &metadata : groups["metadata"].items())
         m.SetMeta(metadata.key(), metadata.value());
      specBuilder.AddGroup(tag, trees, files, m);
   }
   if (fullData.contains("friends")) {
      for (const auto &friends : fullData["friends"].items()) {
         std::string alias = friends.key();
         std::vector<std::string> trees = friends.value()["trees"];
         std::vector<std::string> files = friends.value()["files"];
         if (files.size() != trees.size() && trees.size() > 1)
            throw std::runtime_error("Mismatch between trees and files in a friend.");
         specBuilder.WithFriends(trees, files, alias);
      }
   }

   if (fullData.contains("range")) {
      std::vector<int> range = fullData["range"];

      if (range.size() == 1)
         specBuilder.WithRange({range[0]});
      else if (range.size() == 2)
         specBuilder.WithRange({range[0], range[1]});
   }
   // specBuilder.Build();
   return ROOT::RDataFrame(specBuilder.Build());
}

} // namespace Experimental
} // namespace RDF
} // namespace ROOT
