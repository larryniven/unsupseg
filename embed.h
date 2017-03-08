#ifndef EMBED_H
#define EMBED_H

#include "unsupseg/dtw.h"

namespace embed {

    using seg_t = std::vector<std::vector<double>>;

    la::vector<double> dtw_embed(seg_t const& seg, std::vector<seg_t> const& basis);

    la::tensor<double> to_tensor(seg_t const& seg);
    la::tensor<double> to_tensor(std::vector<seg_t> const& basis);
    la::tensor<double> conv_embed(la::tensor<double> const& seg, la::tensor<double> const& basis);

}

#endif
