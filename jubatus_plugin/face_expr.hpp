#ifndef JUBATUS_FACE_EXPR_PLUGIN_H_
#define JUBATUS_FACE_EXPR_PLUGIN_H_
#include <map>
#include <string>
#include <vector>
#include <utility>
#include "jubatus/util/lang/shared_ptr.h"
#include "jubatus/core/fv_converter/binary_feature.hpp"

namespace jubatus {
namespace plugin {
namespace fv_converter {

class face_expr : public jubatus::core::fv_converter::binary_feature {
 public:
  virtual ~face_expr() {}
  face_expr();

  void add_feature(
      const std::string& key,
      const std::string& value,
      std::vector<std::pair<std::string, float> >& ret_fv) const;

 private:
  class impl_;
  impl_* create_model();
  jubatus::util::lang::shared_ptr<impl_> model_;
};

}  // namespace fv_converter
}  // namespace plugin
}  // namespace jubatus

extern "C" {
jubatus::plugin::fv_converter::face_expr* create(
    const std::map<std::string, std::string>& params);
std::string version();
}

#endif  // JUBATUS_FACE_EXPR_PLUGIN_H_
