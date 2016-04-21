#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <msgpack.hpp>

#include "jubatus/core/fv_converter/util.hpp"
#include "jubatus/core/fv_converter/binary_feature.hpp"

#include "jubatus/core/common/assert.hpp"
#include "jubatus/util/lang/cast.h"

#include "face_expr.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::pair;
using std::make_pair;
using std::ifstream;
using jubatus::util::lang::lexical_cast;

namespace jubatus {
namespace plugin {
namespace fv_converter {

namespace {

inline double sgn(double x) {
  if (x < 0) return -1;
  if (x > 0) return 1;
  return 0;
}


}  // namespace

class face_expr::impl_ {
 public:
  impl_() {}

  virtual void add_feature(
      const string& key,
      const string& value,
      vector<pair<string, float> >& ret_fv) const {
    std::ofstream of("/tmp/" + key);
    of << value;

    // value should be a jpeg file binary
    ret_fv.push_back(make_pair("bin#" + key + "_" +
                               lexical_cast<string>(i),
                               dence_feature_vector[i]));
  }

 private:
};

face_expr::impl_* face_expr::create_model() {
  return new face_expr::impl_();
}

face_expr::face_expr()
  : model_(create_model()) {
  //cv::initModule_nonfree();
}

void face_expr::add_feature(
    const string& key,
    const string& value,
    vector<pair<string, float> >& ret_fv) const {
  model_->add_feature(key, value, ret_fv);
}

}  // namespace fv_converter
}  // namespace plugin
}  // namespace jubatus

extern "C" {

  jubatus::plugin::fv_converter::face_expr* create(
      const std::map<std::string, std::string>& params) {
    return new jubatus::plugin::fv_converter::face_expr();
}

string version() {
  return "0.0.1";
}

}
