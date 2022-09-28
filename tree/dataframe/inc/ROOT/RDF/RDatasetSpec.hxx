// Author: Vincenzo Eduardo Padulano CERN/UPV, Ivan Kabadzhov CERN  06/2022

/*************************************************************************
 * Copyright (C) 1995-2022, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_RDF_RDATASETSPEC
#define ROOT_RDF_RDATASETSPEC

#include <limits>
#include <string>
#include <utility> // std::pair
#include <vector>

#include "ROOT/InternalTreeUtils.hxx" // ROOT::Internal::TreeUtils::RFriendInfo
#include "ROOT/RDF/RMetaData.hxx"     // RMetaData
#include "RtypesCore.h"               // Long64_t

//#include <ROOT/variant.hxx>

namespace ROOT {

namespace Detail {
namespace RDF {
class RLoopManager;
} // namespace RDF
} // namespace Detail

namespace RDF {

namespace Experimental {

class RDatasetSpec {

   friend class ROOT::Detail::RDF::RLoopManager;

public:
   struct REntryRange {
      Long64_t fBegin{0};
      Long64_t fEnd{std::numeric_limits<Long64_t>::max()};
      REntryRange();
      REntryRange(Long64_t endEntry);
      REntryRange(Long64_t startEntry, Long64_t endEntry);
   };

   // groups need to fulfill:
   // 1. preserve the original order -> arrange them in a vector, store also number of fileglobs in this group
   // 2. there is 1:1 correspondence between group and meta data => group and metadata can go together
   // Current solution: create a simple struct to hold the triplet {groupName, groupSize, MetaData}
   // This Group structure does not need an exposure to the user.
public: // TODO: make private, now is public for test purposes only!
   struct Group {
      std::string fName;
      Long64_t fSize;      // the matching between fileGlobs and group sizes is relative!
      RMetaData fMetaData; // behaves like a heterogenuous dictionary
      Group(const std::string &name, Long64_t size, const RMetaData &metaData);
   };

private:
   /**
    * A list of names of trees.
    * This list should go in lockstep with fFileNameGlobs, only in case this dataset is a TChain where each file
    * contains its own tree with a different name from the global name of the dataset.
    * Otherwise, fTreeNames contains 1 treename, that is common for all file globs.
    */
   std::vector<std::string> fTreeNames;
   /**
    * A list of file names.
    * They can contain the globbing characters supported by TChain. See TChain::Add for more information.
    */
   std::vector<std::string> fFileNameGlobs;
   REntryRange fEntryRange; ///< Start (inclusive) and end (exclusive) entry for the dataset processing
   ROOT::Internal::TreeUtils::RFriendInfo fFriendInfo; ///< List of friends
   std::vector<Group> fGroups;                         ////< List of groups

public:
   RDatasetSpec(const std::vector<std::string> &trees, const std::vector<std::string> &fileGlobs,
                const std::vector<Group> &groups = {}, const ROOT::Internal::TreeUtils::RFriendInfo &friendInfo = {},
                const REntryRange &entryRange = {});
   /*
   public:
      RDatasetSpec(const std::string &treeName, const std::string &fileNameGlob, const REntryRange &entryRange = {});

      RDatasetSpec(const std::string &treeName, const std::vector<std::string> &fileNameGlobs,
                   const REntryRange &entryRange = {});

      RDatasetSpec(const std::vector<std::pair<std::string, std::string>> &treeAndFileNameGlobs,
                   const REntryRange &entryRange = {});

      void AddFriend(const std::string &treeName, const std::string &fileNameGlob, const std::string &alias = "");

      void
      AddFriend(const std::string &treeName, const std::vector<std::string> &fileNameGlobs, const std::string &alias =
   "");

      void AddFriend(const std::vector<std::pair<std::string, std::string>> &treeAndFileNameGlobs,
                     const std::string &alias = "");
   */
};

// TODO: this can be derived from RDatasetSpec, hence remove the first 4 members, and Build() would "simply cast"
class SpecBuilder {
   std::vector<std::string> fTreeNames;                // for all groups, groups come one after another
   std::vector<std::string> fFileNameGlobs;            // for all groups, groups come one after another
   RDatasetSpec::REntryRange fEntryRange;              // global! range
   ROOT::Internal::TreeUtils::RFriendInfo fFriendInfo; // global! friends
   std::vector<RDatasetSpec::Group> fGroups;

public:
   SpecBuilder &AddGroup(const std::string &groupName, const std::string &treeName, const std::string &fileNameGlob,
                         const RMetaData &metaData);

   SpecBuilder &AddGroup(const std::string &groupName, const std::string &treeName,
                         const std::vector<std::string> &fileNameGlobs, const RMetaData &metaData);

   SpecBuilder &AddGroup(const std::string &groupName,
                         const std::vector<std::pair<std::string, std::string>> &treeAndFileNameGlobs,
                         const RMetaData &metaData);

   SpecBuilder &AddGroup(const std::string &groupName, const std::vector<std::string> &trees,
                         const std::vector<std::string> &files, const RMetaData &metaData);

   SpecBuilder &
   WithFriends(const std::string &treeName, const std::string &fileNameGlob, const std::string &alias = "");

   SpecBuilder &WithFriends(const std::string &treeName, const std::vector<std::string> &fileNameGlobs,
                            const std::string &alias = "");

   SpecBuilder &WithFriends(const std::vector<std::pair<std::string, std::string>> &treeAndFileNameGlobs,
                            const std::string &alias = "");

   SpecBuilder &WithFriends(const std::vector<std::string> &trees, const std::vector<std::string> &files,
                            const std::string &alias = "");

   SpecBuilder &WithRange(const RDatasetSpec::REntryRange &entryRange = {});

   RDatasetSpec Build();
};

} // namespace Experimental
} // namespace RDF
} // namespace ROOT

#endif // ROOT_RDF_RDATASETSPEC
