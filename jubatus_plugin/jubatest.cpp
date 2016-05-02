#define DLIB_USE_BLAS
#include "face_expr.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <string>
#include <iostream>

int main(int argc, char** argv) {
  jubatus::plugin::fv_converter::face_expr f("shape_predictor_68_face_landmarks.dat");
  std::ifstream ifs(argv[1]);
  std::stringstream buffer;
  buffer << ifs.rdbuf();

  std::vector<std::pair<std::string, float> > ret_fv;
  f.add_feature(argv[1], buffer.str(), ret_fv);
  for (size_t i = 0; i < ret_fv.size(); ++i) {
    std::cout << "(" << ret_fv[i].first << " => "
      << ret_fv[i].second << ")"
      << std::endl;
  }
  return 0;
}
