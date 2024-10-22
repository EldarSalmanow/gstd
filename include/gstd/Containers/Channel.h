#ifndef GSTD_CHANNEL_H
#define GSTD_CHANNEL_H

#include <queue>

namespace gstd {

    template<typename ValueT>
    class Channel {
    public:

        using ValueType = ValueT;

    public:

        auto operator<<(const ValueType &value) {
            _queue.push(value);

            return *this;
        }

        auto operator>>(ValueType &value) {
            value = _queue.back();

            _queue.pop();

            return *this;
        }

    private:

        std::queue<ValueType> _queue;
    };

    void q() {
        Channel<int> c;

        c << 1 << 2;
        int i, j;
        c >> i >> j;
        // i = 1, j = 2
    }

}

#endif //GSTD_CHANNEL_H
