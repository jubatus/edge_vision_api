#include "face_extractor.hpp"

#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_io.h>

using namespace jubatus::plugin::face_expression;

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "set target filename" << std::endl;
    return 1;
  }

  // std::cout << "target: " << argv[1] << std::endl;
  dlib::array2d<dlib::rgb_pixel> img;
  dlib::load_image(img, argv[1]);

  face_detector dt;
  jubatus::util::data::optional<dlib::rectangle> rec = dt.get_largest_face(img);
  if (!rec) {
    std::cout << "failed to detect face..." << std::endl;
  } else {
    std::cout << "face detected!" << std::endl;
  }
}
