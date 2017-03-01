#include "unsupseg/dtw.h"
#include <algorithm>

namespace dtw {

    double l2_dist(std::vector<double> const& v1, std::vector<double> const& v2)
    {
        la::weak_vector<double> u1 { const_cast<double*>(v1.data()), (unsigned int)(v1.size()) };
        la::weak_vector<double> u2 { const_cast<double*>(v2.data()), (unsigned int)(v2.size()) };

        return la::norm(la::sub(u1, u2));
    }
    
    double dtw(std::vector<std::vector<double>> const& seg1, std::vector<std::vector<double>> const& seg2)
    {
        std::vector<std::vector<double>> score;
        score.resize(seg1.size());
        for (int i = 0; i < seg1.size(); ++i) {
            score[i].resize(seg2.size());
        }

        score[0][0] = l2_dist(seg1.front(), seg2.front());
    
        for (int i = 1; i < seg1.size(); ++i) {
            score[i][0] = score[i-1][0] + l2_dist(seg1.at(i), seg2.front());
        }
    
        for (int j = 1; j < seg2.size(); ++j) {
            score[0][j] = score[0][j-1] + l2_dist(seg1.front(), seg2.at(j));
        }
    
        for (int i = 1; i < seg1.size(); ++i) {
            for (int j = 1; j < seg2.size(); ++j) {
                double d = l2_dist(seg1.at(i), seg2.at(j));
    
                std::vector<double> v = {
                    score[i-1][j-1] + d,
                    score[i-1][j] + d,
                    score[i][j-1] + d
                };
    
                score[i][j] = *std::min_element(v.begin(), v.end());
            }
        }
    
        return score[seg1.size() - 1][seg2.size() - 1];
    }

    la::vector<double> embed(seg_t const& seg, std::vector<seg_t> const& basis)
    {
        la::vector<double> result;
    
        result.resize(basis.size());
    
        for (int i = 0; i < basis.size(); ++i) {
            result(i) = dtw(seg, basis.at(i));
        }
    
        return result;
    }

}
