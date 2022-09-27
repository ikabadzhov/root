#include <gtest/gtest.h>
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>
#include <ROOT/RDFHelpers.hxx>
#include <ROOT/TestSupport.hxx>
#include <TSystem.h>
#include <ROOT/RDF/RMetaData.hxx>

#include <thread> // std::thread::hardware_concurrency

using namespace ROOT;
using namespace ROOT::RDF::Experimental;

using namespace std::literals; // remove ambiguity of using std::vector<std::string>-s and std::string-s

void EXPECT_VEC_EQ(const std::vector<ULong64_t> &vec1, const std::vector<ULong64_t> &vec2)
{
   ASSERT_EQ(vec1.size(), vec2.size());
   for (auto i = 0u; i < vec1.size(); ++i)
      EXPECT_EQ(vec1[i], vec2[i]);
}

// The helper class is also responsible for the creation and the deletion of the root specTestFiles
class RMetaDataTest : public ::testing::TestWithParam<bool> {
protected:
   RMetaDataTest() : NSLOTS(GetParam() ? std::min(4u, std::thread::hardware_concurrency()) : 1u)
   {
      if (GetParam())
         ROOT::EnableImplicitMT(NSLOTS);
   }

   ~RMetaDataTest()
   {
      if (GetParam())
         ROOT::DisableImplicitMT();
   }

   const unsigned int NSLOTS;

   static void SetUpTestCase()
   {
      auto dfWriter0 = RDataFrame(5).Define("z", [](ULong64_t e) { return e + 100; }, {"rdfentry_"});
      dfWriter0.Snapshot<ULong64_t>("tree", "specTestFile0.root", {"z"});
      dfWriter0.Range(0, 2).Snapshot<ULong64_t>("subTree", "specTestFile1.root", {"z"});
      dfWriter0.Range(2, 4).Snapshot<ULong64_t>("subTree", "specTestFile2.root", {"z"});
      dfWriter0.Range(4, 5).Snapshot<ULong64_t>("subTree", "specTestFile3.root", {"z"});
      dfWriter0.Range(0, 2).Snapshot<ULong64_t>("subTreeA", "specTestFile4.root", {"z"});
      dfWriter0.Range(2, 4).Snapshot<ULong64_t>("subTreeB", "specTestFile5.root", {"z"});

      auto dfWriter1 = RDataFrame(10)
                          .Define("x", [](ULong64_t e) { return double(e); }, {"rdfentry_"})
                          .Define("w", [](ULong64_t e) { return e + 1.; }, {"rdfentry_"});
      dfWriter1.Range(5).Snapshot<double>("subTree0", "specTestFile6.root", {"x"});
      dfWriter1.Range(5, 10).Snapshot<double>("subTree1", "specTestFile7.root", {"x"});
      dfWriter1.Range(5).Snapshot<double>("subTree2", "specTestFile8.root", {"w"});
      dfWriter1.Range(5, 10).Snapshot<double>("subTree3", "specTestFile9.root", {"w"});
   }

   static void TearDownTestCase()
   {
      for (auto i = 0u; i < 10; ++i)
         gSystem->Unlink(("specTestFile" + std::to_string(i) + ".root").c_str());
   }
};

// ensure that the chains are created as expected, no ranges, neither friends are passed
TEST_P(RMetaDataTest, SimpleChainsCreation)
{
   RMetaData m0;
   m0.AddMetaData("year", 2020);
   m0.AddMetaData("type", "real");
   m0.AddMetaData("weight", 1.23);
   m0.AddMetaData({{"error", "normalized"}, {"energy", 19.}, {"priority", 1}});

   RMetaData m1;
   m1.AddMetaData("run", 3);
   m1.AddMetaData("type", "MC");
   m1.AddMetaData("mass", 3.14);

   RMetaData m2;
   m2.AddMetaData("I", "am");
   m2.AddMetaData("listening", "to");
   m2.AddMetaData("Katy", 3.14);
   m2.AddMetaData({{"Perry", "Last"}, {"friday", 0}, {"night", 1}});

   RMetaData m3;
   m3.AddMetaData("But", "this");
   m3.AddMetaData("friday", "night");
   m3.AddMetaData("Do", 3.14);
   m2.AddMetaData({{"it", "all"}, {"agaaaaain", 0}});

   SpecBuilder sb;
   RDatasetSpec spec =
      sb.AddGroup("reals1", "tree", "specTestFile0.root", m0)
         .AddGroup("TGIF", "subTree", {"specTestFile1.root"s, "specTestFile2.root"s, "specTestFile3.root"s}, m1)
         .AddGroup("party",
                   {{"subTreeA"s, "specTestFile4.root"s},
                    {"subTreeB"s, "specTestFile5.root"s},
                    {"subTree"s, "specTestFile3.root"s}},
                   m3)
         .AddGroup("hard",
                   {{"subTreeA"s, "specTestFile4.root"s},
                    {"subTreeB"s, "specTestFile5.root"s},
                    {"subTree"s, "specTestFile3.root"s}},
                   m0) // again m0
         .WithRange({5, 9})
         // the friend is misaligned
         .WithFriends({{"subTreeA"s, "specTestFile4.root"s},
                               {"subTreeB"s, "specTestFile5.root"s},
                               {"subTree"s, "specTestFile3.root"s}}, "so you wanna play with magiiic?")
         .Build();

   auto result = *(RDataFrame(spec).Take<ULong64_t>("z"));

   for (const auto &r : result)
      std::cout << r << std::endl;
}

// instantiate single-thread tests
INSTANTIATE_TEST_SUITE_P(Seq, RMetaDataTest, ::testing::Values(false));

// instantiate multi-thread tests
#ifdef R__USE_IMT
INSTANTIATE_TEST_SUITE_P(MT, RMetaDataTest, ::testing::Values(true));
#endif
