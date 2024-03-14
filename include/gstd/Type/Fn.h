#ifndef GSTD_FN_H
#define GSTD_FN_H

namespace gstd {

    template<typename ReturnT,
             typename... ArgumentsT>
    class Fn;

    template<typename ReturnT,
             typename... ArgumentsT>
    class Fn<ReturnT(ArgumentsT...)> {
    private:

        class Holder {
        private:


        };

    private:

        Holder<ReturnT(ArgumentsT...)> _holder;
    };

}

#endif //GSTD_FN_H
