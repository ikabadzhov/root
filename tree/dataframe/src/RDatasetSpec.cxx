// Author: Vincenzo Eduardo Padulano CERN/UPV, Ivan Kabadzhov CERN  06/2022

/*************************************************************************
 * Copyright (C) 1995-2022, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "ROOT/RDF/RDatasetSpec.hxx"

namespace ROOT {

namespace RDF {

RDatasetSpec::RDatasetSpec(const std::string &treeName, const std::string &fileNameGlob, const REntryRange &entryRange)
   : fTreeNames({treeName}), fFileNameGlobs({fileNameGlob}), fStartEntry(entryRange.fStartEntry),
     fEndEntry(entryRange.fEndEntry)
{
}

RDatasetSpec::RDatasetSpec(const std::string &treeName, const std::vector<std::string> &fileNameGlobs,
                           const REntryRange &entryRange)
   : fTreeNames({treeName}), fFileNameGlobs(fileNameGlobs), fStartEntry(entryRange.fStartEntry),
     fEndEntry(entryRange.fEndEntry)
{
}

RDatasetSpec::RDatasetSpec(const std::vector<std::pair<std::string, std::string>> &treeAndFileNameGlobs,
                           const REntryRange &entryRange)
   : fStartEntry(entryRange.fStartEntry), fEndEntry(entryRange.fEndEntry)
{
   fTreeNames.reserve(treeAndFileNameGlobs.size());
   fFileNameGlobs.reserve(treeAndFileNameGlobs.size());
   for (auto &p : treeAndFileNameGlobs) {
      fTreeNames.emplace_back(p.first);
      fFileNameGlobs.emplace_back(p.second);
   }
}

void RDatasetSpec::AddFriend(const std::string &treeName, const std::string &fileNameGlob, const std::string &alias)
{
   fFriendInfo.fFriendNames.emplace_back(std::make_pair(treeName, alias));
   fFriendInfo.fFriendFileNames.emplace_back(std::vector<std::string>{fileNameGlob});
   fFriendInfo.fFriendChainSubNames.emplace_back(); // this is a tree
}

void RDatasetSpec::AddFriend(const std::string &treeName, const std::vector<std::string> &fileNameGlobs,
                             const std::string &alias)
{
   fFriendInfo.fFriendNames.emplace_back(std::make_pair("", alias));
   fFriendInfo.fFriendFileNames.emplace_back(fileNameGlobs);
   fFriendInfo.fFriendChainSubNames.emplace_back(std::vector<std::string>(fileNameGlobs.size(), treeName));
}

void RDatasetSpec::AddFriend(const std::vector<std::pair<std::string, std::string>> &treeAndFileNameGlobs,
                             const std::string &alias)
{
   fFriendInfo.fFriendNames.emplace_back(std::make_pair("", alias));
   fFriendInfo.fFriendFileNames.emplace_back();
   fFriendInfo.fFriendChainSubNames.emplace_back();
   auto &fileNames = fFriendInfo.fFriendFileNames.back();
   auto &chainSubNames = fFriendInfo.fFriendChainSubNames.back();
   fileNames.reserve(treeAndFileNameGlobs.size());
   chainSubNames.reserve(treeAndFileNameGlobs.size());
   for (auto &p : treeAndFileNameGlobs) {
      chainSubNames.emplace_back(p.first);
      fileNames.emplace_back(p.second);
   }
}

} // namespace RDF
} // namespace ROOT