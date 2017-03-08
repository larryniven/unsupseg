#include "unsupseg/embed.h"
#include "unsupseg/dtw.h"
#include <algorithm>

namespace embed {

    la::vector<double> dtw_embed(seg_t const& seg, std::vector<seg_t> const& basis)
    {
        la::vector<double> result;
    
        result.resize(basis.size());
    
        for (int i = 0; i < basis.size(); ++i) {
            result(i) = dtw::dtw(seg, basis.at(i));
        }
    
        return result;
    }

    la::tensor<double> to_tensor(std::vector<std::vector<double>> const& seg)
    {
        la::tensor<double> result;
        result.resize({(unsigned int)(seg.size()), (unsigned int)(seg.front().size()), 1});
    
        for (int i = 0; i < result.size(0); ++i) {
            for (int j = 0; j < result.size(1); ++j) {
                result({i, j, 0}) = seg[i][j];
            }
        }
    
        return result;
    }
    
    la::tensor<double> to_tensor(std::vector<std::vector<std::vector<double>>> const& basis)
    {
        la::tensor<double> result;
        result.resize({(unsigned int)(basis.front().size()),
            (unsigned int)(basis.front().front().size()), (unsigned int)(basis.size())});
    
        for (int i = 0; i < result.size(0); ++i) {
            for (int j = 0; j < result.size(1); ++j) {
                for (int c = 0; c < result.size(2); ++c) {
                    result({i, j, c}) = basis[c][i][j];
                }
            }
        }
    
        return result;
    }

    la::tensor<double> conv_embed(la::tensor<double> const& seg, la::tensor<double> const& basis)
    {
        la::tensor<double> seg_lin;
    
        seg_lin.resize({seg.size(0) - basis.size(0) + 1, basis.size(0) * basis.size(1)});
    
        la::corr_linearize_valid(seg_lin, seg, basis.size(0), basis.size(1));
    
        la::tensor<double> corr = la::mul(seg_lin, basis);
    
        double inf = std::numeric_limits<double>::infinity();
    
        std::vector<std::pair<double, int>> vec;
        for (int i = 0; i < corr.size(1); ++i) {
            vec.push_back(std::make_pair(-inf, i));
        }
    
        for (int i = 0; i < corr.size(0); ++i) {
            for (int c = 0; c < corr.size(1); ++c) {
                vec[c].first = std::max(vec[c].first, corr({i, c}));
            }
        }

        la::tensor<double> result;
        result.resize({corr.size(1)});

        auto less = [](std::pair<double, int> const& p1, std::pair<double, int> const& p2)
            { return p1.first < p2.first; };

        std::make_heap(vec.begin(), vec.end(), less);

        for (int k = 0; k < 100; ++k) {
            std::pop_heap(vec.begin(), vec.end(), less);
            auto& p = vec.back();
            result({p.second}) = p.first;
            vec.pop_back();
        }
    
        return result;
    }

}

