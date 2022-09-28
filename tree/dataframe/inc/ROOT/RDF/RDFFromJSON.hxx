#include "ROOT/RDataFrame.hxx"

namespace ROOT {
namespace RDF {
namespace Experimental {

// create an RDF from a JSON file
// the JSON file must satisfy several requirements
// requirements to be discussed on a PPP
ROOT::RDataFrame MakeDataFrameFromJSON(const std::string &jsonFile);

} // namespace Experimental
} // namespace RDF
} // namespace ROOT