#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>

#include <jubatus/util/data/optional.h>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

namespace jubatus {
namespace plugin {
namespace face_expression {

class face_detector {
 public:
  face_detector()
    : detector_(dlib::get_frontal_face_detector()) {
  }

  util::data::optional<dlib::rectangle>
  get_largest_face(const dlib::array2d<dlib::rgb_pixel>& img) {
    std::vector<dlib::rectangle> dets = detector_(img);
    if (dets.size() == 0) {
      return jubatus::util::data::optional<dlib::rectangle>();
    }
    uint64_t largest_area = 0;
    int64_t largest_idx = -1;
    for (size_t i = 0; i < dets.size(); ++i) {
      const uint64_t area = dets[i].width() * dets[i].height();
      if (largest_area < area) {
        // std::cout << "largest area: " << area << std::endl;
        largest_idx = i;
        largest_area = area;
      }
    }
    return jubatus::util::data::optional<dlib::rectangle>(dets[largest_idx]);
  }
 private:
  dlib::frontal_face_detector detector_;
};

class landmark_detector {
 public:
  landmark_detector(const char* modelpath) {
    dlib::deserialize(modelpath) >> sp_;
  }
  std::vector<dlib::point>
  detect(const dlib::array2d<dlib::rgb_pixel>& img,
         const dlib::rectangle& face) {
    const dlib::full_object_detection shape = sp_(img, face);
    std::vector<dlib::point> ret;
    ret.reserve(68);
    for (size_t i = 0; i < shape.num_parts(); ++i) {
      ret.push_back(shape.part(i));
    }
    assert(ret.size() == 68);
    return ret;
  }

 private:
  dlib::shape_predictor sp_;
};


}  // namespace face_expression
}  // namespace plugin
}  // namespace jubatus
