// Author: Vincenzo Eduardo Padulano CERN/UPV, Ivan Kabadzhov CERN  06/2022

/*************************************************************************
 * Copyright (C) 1995-2022, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "ROOT/RDF/RDatasetSpec.hxx"
#include <stdexcept> // std::logic_error

namespace ROOT {

namespace RDF {

namespace Experimental {

RDatasetSpec::REntryRange::REntryRange() {}

RDatasetSpec::REntryRange::REntryRange(Long64_t end) : fEnd(end) {}

RDatasetSpec::REntryRange::REntryRange(Long64_t begin, Long64_t end) : fBegin(begin), fEnd(end)
{
   if (fBegin > fEnd)
      throw std::logic_error("The starting entry cannot be larger than the ending entry in the "
                             "creation of a dataset specification.");
}

/**
 * \class ROOT::RDF::Experimental::RDatasetSpec
 * \ingroup dataframe
 * \brief A dataset specification for RDataFrame.
 **/
/*
////////////////////////////////////////////////////////////////////////////
/// \brief Construct an RDatasetSpec for one or more samples with the same tree name.
/// \param[in] treeName Name of the tree
/// \param[in] fileNameGlob Single file name or glob expression for the files where the tree(s) are stored
/// \param[in] entryRange The global entry range to be processed, {begin (inclusive), end (exclusive)}
///
/// The filename glob supports the same type of expressions as TChain::Add().
RDatasetSpec::RDatasetSpec(const std::string &treeName, const std::string &fileNameGlob, const REntryRange &entryRange)
   : fTreeNames({treeName}), fFileNameGlobs({fileNameGlob}), fEntryRange(entryRange)
{
}

////////////////////////////////////////////////////////////////////////////
/// \brief Construct an RDatasetSpec for one or more samples with the same tree name.
/// \param[in] treeName Name of the tree
/// \param[in] fileNameGlobs A vector of file names or glob expressions for the files where the trees are stored
/// \param[in] entryRange The global entry range to be processed, {begin (inclusive), end (exclusive)}
///
/// The filename glob supports the same type of expressions as TChain::Add().
RDatasetSpec::RDatasetSpec(const std::string &treeName, const std::vector<std::string> &fileNameGlobs,
                           const REntryRange &entryRange)
   : fTreeNames({treeName}), fFileNameGlobs(fileNameGlobs), fEntryRange(entryRange)
{
}

////////////////////////////////////////////////////////////////////////////
/// \brief Construct an RDatasetSpec for a chain of several trees (possibly having different names).
/// \param[in] treeAndFileNameGlobs A vector of pairs of tree names and their corresponding file names/globs
/// \param[in] entryRange The global entry range to be processed, {begin (inclusive), end (exclusive)}
///
/// The filename glob supports the same type of expressions as TChain::Add().
///
/// ### Example usage:
/// ~~~{.py}
/// spec = ROOT.RDF.Experimental.RDatasetSpec([("tree1", "a.root"), ("tree2", "b.root")], (5, 10))
/// df = ROOT.RDataFrame(spec)
/// ~~~
RDatasetSpec::RDatasetSpec(const std::vector<std::pair<std::string, std::string>> &treeAndFileNameGlobs,
                           const REntryRange &entryRange)
   : fEntryRange(entryRange)
{
   fTreeNames.reserve(treeAndFileNameGlobs.size());
   fFileNameGlobs.reserve(treeAndFileNameGlobs.size());
   for (auto &p : treeAndFileNameGlobs) {
      fTreeNames.emplace_back(p.first);
      fFileNameGlobs.emplace_back(p.second);
   }
}

////////////////////////////////////////////////////////////////////////////
/// \brief Add a friend tree or chain with the same tree name to the dataset specification.
/// \param[in] treeName Name of the tree
/// \param[in] fileNameGlob Single file name or glob expression for the files where the tree(s) are stored
/// \param[in] alias String to refer to the particular friend
///
/// The filename glob supports the same type of expressions as TChain::Add().
void RDatasetSpec::AddFriend(const std::string &treeName, const std::string &fileNameGlob, const std::string &alias)
{
   fFriendInfo.AddFriend(treeName, fileNameGlob, alias);
}

////////////////////////////////////////////////////////////////////////////
/// \brief Add a friend tree or chain with the same tree name to the dataset specification.
/// \param[in] treeName Name of the tree
/// \param[in] fileNameGlobs A vector of file names or glob expressions for the files where the trees are stored
/// \param[in] alias String to refer to the particular friend
///
/// The filename glob supports the same type of expressions as TChain::Add().
void RDatasetSpec::AddFriend(const std::string &treeName, const std::vector<std::string> &fileNameGlobs,
                             const std::string &alias)
{
   fFriendInfo.AddFriend(treeName, fileNameGlobs, alias);
}

////////////////////////////////////////////////////////////////////////////
/// \brief Add a friend tree or chain (possibly with different tree names) to the dataset specification.
/// \param[in] treeAndFileNameGlobs A vector of pairs of tree names and their corresponding file names/globs
/// \param[in] alias String to refer to the particular friend
///
/// The filename glob supports the same type of expressions as TChain::Add().
///
/// ### Example usage:
/// ~~~{.py}
/// spec = ROOT.RDF.Experimental.RDatasetSpec("tree", "file.root")
/// spec.AddFriend([("tree1", "a.root"), ("tree2", "b.root")], "alias")
/// df = ROOT.RDataFrame(spec)
/// ~~~
void RDatasetSpec::AddFriend(const std::vector<std::pair<std::string, std::string>> &treeAndFileNameGlobs,
                             const std::string &alias)
{
   fFriendInfo.AddFriend(treeAndFileNameGlobs, alias);
}
*/

RDatasetSpec::Group::Group(const std::string &name, Long64_t size, const RMetaData &metaData)
   : fName(name), fSize(size), fMetaData(std::move(metaData))
{
}

RDatasetSpec::RDatasetSpec(const std::vector<std::string> &trees, const std::vector<std::string> &fileGlobs,
                           const std::vector<Group> &groups, const ROOT::Internal::TreeUtils::RFriendInfo &friendInfo,
                           const REntryRange &entryRange)
   : fTreeNames(trees), fFileNameGlobs(fileGlobs), fEntryRange(entryRange), fFriendInfo(friendInfo), fGroups(groups)
{
}

SpecBuilder &SpecBuilder::AddGroup(const std::string &groupName, const std::string &treeName,
                                   const std::string &fileNameGlob, const RMetaData &metaData)
{
   // adding a single fileglob/tree, hence extend the vectors with 1 elem
   fTreeNames.reserve(fTreeNames.size() + 1);
   fTreeNames.emplace_back(treeName);
   fFileNameGlobs.reserve(fFileNameGlobs.size() + 1);
   fFileNameGlobs.emplace_back(fileNameGlob);

   fGroups.reserve(fGroups.size() + 1); // we are always adding a single group
   // the group is of size 1, e.g. a single file glob
   fGroups.emplace_back(RDatasetSpec::Group(groupName, 1, metaData));

   return *this;
}

SpecBuilder &SpecBuilder::AddGroup(const std::string &groupName, const std::string &treeName,
                                   const std::vector<std::string> &fileNameGlobs, const RMetaData &metaData)
{
   // this constructor expects 1 tree name and multiple file names
   // however, in order to align many groups in TChain, we here copy the tree names multiple times
   // e.g. for N files, we store N (repeating) tree names to keep the alignment
   const auto nNewGlobs = fileNameGlobs.size();
   fTreeNames.reserve(fTreeNames.size() + nNewGlobs);
   for (auto i = 0u; i < nNewGlobs; ++i) // TODO: there might be a better intruction to do that
      fTreeNames.emplace_back(treeName);
   fFileNameGlobs.reserve(fFileNameGlobs.size() + nNewGlobs);
   fFileNameGlobs.insert(std::end(fFileNameGlobs), std::begin(fileNameGlobs), std::end(fileNameGlobs));

   fGroups.reserve(fGroups.size() + 1);
   fGroups.emplace_back(
      RDatasetSpec::Group(groupName, nNewGlobs, metaData)); // note that the group is of size nNewGlobs

   return *this;
}

SpecBuilder &SpecBuilder::AddGroup(const std::string &groupName,
                                   const std::vector<std::pair<std::string, std::string>> &treeAndFileNameGlobs,
                                   const RMetaData &metaData)
{
   const auto nNewGlobs = treeAndFileNameGlobs.size();
   fTreeNames.reserve(nNewGlobs);
   fFileNameGlobs.reserve(nNewGlobs);
   for (auto &p : treeAndFileNameGlobs) {
      fTreeNames.emplace_back(p.first);
      fFileNameGlobs.emplace_back(p.second);
   }

   fGroups.reserve(fGroups.size() + 1);
   fGroups.emplace_back(RDatasetSpec::Group(groupName, nNewGlobs, metaData));

   return *this;
}

SpecBuilder &SpecBuilder::AddGroup(const std::string &groupName, const std::vector<std::string> &trees,
                                   const std::vector<std::string> &files, const RMetaData &metaData)
{
   // TODO: sanity  check for vector sizes
   const auto nNewGlobs = files.size();
   fTreeNames.reserve(fTreeNames.size() + nNewGlobs);
   fFileNameGlobs.reserve(fFileNameGlobs.size() + nNewGlobs);
   fTreeNames.insert(std::end(fTreeNames), std::begin(trees), std::end(trees));
   fFileNameGlobs.insert(std::end(fFileNameGlobs), std::begin(files), std::end(files));
   fGroups.reserve(fGroups.size() + 1);
   fGroups.emplace_back(RDatasetSpec::Group(groupName, nNewGlobs, metaData));
   return *this;
}

SpecBuilder &
SpecBuilder::WithFriends(const std::string &treeName, const std::string &fileNameGlob, const std::string &alias)
{
   fFriendInfo.AddFriend(treeName, fileNameGlob, alias);
   return *this;
}

SpecBuilder &SpecBuilder::WithFriends(const std::string &treeName, const std::vector<std::string> &fileNameGlobs,
                                      const std::string &alias)
{
   fFriendInfo.AddFriend(treeName, fileNameGlobs, alias);
   return *this;
}

SpecBuilder &SpecBuilder::WithFriends(const std::vector<std::pair<std::string, std::string>> &treeAndFileNameGlobs,
                                      const std::string &alias)
{
   fFriendInfo.AddFriend(treeAndFileNameGlobs, alias);
   return *this;
}

SpecBuilder &SpecBuilder::WithRange(const RDatasetSpec::REntryRange &entryRange)
{
   fEntryRange = entryRange;
   return *this;
}

SpecBuilder &SpecBuilder::WithFriends(const std::vector<std::string> &trees, const std::vector<std::string> &files,
                                      const std::string &alias)
{
   std::vector<std::pair<std::string, std::string>> target;
   target.reserve(files.size());
   std::transform(trees.begin(), trees.end(), files.begin(), std::back_inserter(target),
                  [](std::string a, std::string b) { return std::make_pair(a, b); });
   fFriendInfo.AddFriend(target, alias);
   return *this;
}

RDatasetSpec SpecBuilder::Build()
{
   return std::move(RDatasetSpec(fTreeNames, fFileNameGlobs, fGroups, fFriendInfo, fEntryRange));
}

} // namespace Experimental
} // namespace RDF

// RDataFrame RDataFrameFromJSON(const std::string &jsonFile)

} // namespace ROOT
