#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <msgpack.hpp>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>

#include "jubatus/core/fv_converter/util.hpp"
#include "jubatus/core/fv_converter/binary_feature.hpp"

#include "jubatus/core/common/assert.hpp"
#include "jubatus/util/lang/cast.h"

#include "face_expr.hpp"
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

inline double sgn(double x) {
  if (x < 0) return -1;
  if (x > 0) return 1;
  return 0;
}


}  // namespace

class face_expr::impl_ {
 public:
  impl_()
    : detector_(dlib::get_frontal_face_detector()) {
    dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> sp_;
  }

  virtual void add_feature(
      const string& key,
      const string& value,
      vector<pair<string, float> >& ret_fv) {
    // write data as file(FIXME!)
    std::string filepath = "/tmp/" + key;
    std::ofstream ofs;
    ofs.open(filepath.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    ofs << value;

    dlib::image_window win, win_faces;
    dlib::array2d<dlib::rgb_pixel> img;
    dlib::load_image(img, filepath);
    dlib::pyramid_up(img);
    std::vector<dlib::rectangle> dets = detector_(img);
    cout << "Number of faces detected: " << dets.size() << endl;

    std::vector<dlib::full_object_detection> shapes;
    for (size_t j = 0; j < dets.size(); ++j) {
      dlib::full_object_detection shape = sp_(img, dets[j]);
      std::cout << "number of parts: "<< shape.num_parts() << endl;
      std::cout << "pixel position of first part:  " << shape.part(0) << endl;
      std::cout << "pixel position of second part: " << shape.part(1) << endl;
      // You get the idea, you can get all the face part locations if
      // you want them.  Here we just store them in shapes so we can
      // put them on the screen.
      shapes.push_back(shape);
    }

    win.clear_overlay();
    win.set_image(img);
    win.add_overlay(dlib::render_face_detections(shapes));

    // value should be a jpeg file binary
    /*
    ret_fv.push_back(make_pair("bin#" + key + "_" +
                               lexical_cast<string>(i),
                               dence_feature_vector[i]));
    */


    dlib::array<dlib::array2d<dlib::rgb_pixel> > face_chips;
    dlib::extract_image_chips(img, dlib::get_face_chip_details(shapes), face_chips);
    win_faces.set_image(tile_images(face_chips));

    cout << "Hit enter to process the next image..." << endl;
    std::cin.get();
  }

 private:
  dlib::frontal_face_detector detector_;
  dlib::shape_predictor sp_;
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
