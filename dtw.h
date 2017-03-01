#ifndef DTW_H
#define DTW_H

#include "la/la.h"

namespace dtw {

    using seg_t = std::vector<std::vector<double>>;

    double l2_dist(std::vector<double> const& v1, std::vector<double> const& v2);
    double dtw(seg_t const& seg1, seg_t const& seg2);

    la::vector<double> embed(seg_t const& seg, std::vector<seg_t> const& basis);

}

#endif
