struct chash {
    // any random-ish large odd number will do
    const uint64_t C = uint64_t(2e18 * acos(-1)) + 71;
    // random 32-bit number
    const uint32_t RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();
 
    size_t operator()(uint64_t x) const {
        // see https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
        return __builtin_bswap64((x ^ RANDOM) * C);
    }
};
