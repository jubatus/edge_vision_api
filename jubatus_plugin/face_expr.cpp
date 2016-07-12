#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <msgpack.hpp>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <iostream>

#include "jubatus/core/fv_converter/util.hpp"
#include "jubatus/core/fv_converter/binary_feature.hpp"

#include "jubatus/core/common/assert.hpp"
#include "jubatus/util/lang/cast.h"

#include "face_expr.hpp"
#include "face_extractor.hpp"
#include <iostream>

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

}  // namespace

class face_expr::impl_ {
 public:
  impl_(const char* modelpath)
    : dt_(), ld_(modelpath) {
  }

  virtual void add_feature(
      const string& key,
      const string& value,
      vector<pair<string, float> >& ret_fv) {
    // writing data as file(FIXME!)
    size_t idx = key.find_last_of("/");
    std::string filename = key.substr(idx+1, key.length() - idx - 1);
    std::string filepath = "/tmp/" + filename;
    std::ofstream ofs;
    ofs.open(filepath.c_str(),
             std::ios::out |
             std::ios::binary |
             std::ios::trunc);
    ofs << value;

    dlib::array2d<dlib::rgb_pixel> img;
    dlib::load_image(img, filepath);
    //dlib::pyramid_up(img);  // up-scan

    util::data::optional<dlib::rectangle> det = dt_.get_largest_face(img);
    if (!det) {
      std::cout << "no face detected" << std::endl;
      return;
    }

    const float left = static_cast<float>(det->left());
    const float top = static_cast<float>(det->top());
    const float width = static_cast<float>(det->width());
    const float height = static_cast<float>(det->height());

    const std::vector<dlib::point> landmarks = ld_.detect(img, *det);
    for (size_t i = 0; i < landmarks.size(); ++i) {
      const float x = (landmarks[i].x() - left) / width;
      const float y = (landmarks[i].y() - top) / height;

      ret_fv.push_back(make_pair("bin#" + filename + "_" +
                                 lexical_cast<string>(i) + "x",
                                 x));
      ret_fv.push_back(make_pair("bin#" + filename + "_" +
                                 lexical_cast<string>(i) + "y",
                                 y));
      // cout << "point: (" << x << ", " << y << ")"<< endl;
    }
  }
 private:
  face_expression::face_detector dt_;
  face_expression::landmark_detector ld_;
};

face_expr::face_expr(const char* modelpath)
  : model_(new face_expr::impl_(modelpath)) {
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
  std::string param =
      jubatus::core::fv_converter::get_with_default(params, "model", "");
  return new jubatus::plugin::fv_converter::face_expr(param.c_str());
}

string version() {
  return "0.0.1";
}

}  // extern "C"
