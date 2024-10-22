#ifndef GSTD_MAP_H
#define GSTD_MAP_H

namespace gstd {

    template<typename KeyT,
             typename ValueT>
    class Map {
    public:


    };

    template<typename FirstT,
             typename SecondT>
    class BiMap {
    public:

        BiMap(Map<FirstT, SecondT> keyToValue) {
            _keyToValue = std::move(keyToValue);

            for (const auto &pair : _keyToValue) {
                _valueToKey[pair.second] = pair.first;
            }
        }

    public:

        GSTD_CONSTEXPR auto operator[](const FirstT &key) -> SecondT & {
            return _keyToValue[key];
        }

        GSTD_CONSTEXPR auto operator[](const SecondT &value) -> FirstT & {
            return _valueToKey[value];
        }

    private:

        Map<FirstT, SecondT> _keyToValue;

        Map<SecondT, FirstT> _valueToKey;
    };

}

#endif //GSTD_MAP_H
