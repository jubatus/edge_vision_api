#define DLIB_USE_BLAS
#include "face_expr.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <string>

int main(int argc, char** argv) {
  jubatus::plugin::fv_converter::face_expr f;
  std::ifstream ifs(argv[1]);
  std::stringstream buffer;
  buffer << ifs.rdbuf();

  std::vector<std::pair<std::string, float> > ret_fv;
  f.add_feature(argv[1], buffer.str(), ret_fv);
  return 0;
}
