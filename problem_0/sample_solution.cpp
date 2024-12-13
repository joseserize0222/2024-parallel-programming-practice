#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <thread>
#include <mutex>

std::istream& operator>>(std::istream& in, __int128& value) {
    std::string s;
    in >> s;
    value = 0;
    bool negative = false;
    size_t i = 0;
    if (s[0] == '-') {
        negative = true;
        i = 1;
    }
    for (; i < s.size(); ++i) {
        value = value * 10 + (s[i] - '0');
    }
    if (negative) value = -value;
    return in;
}

std::ostream& operator<<(std::ostream& out, __int128 value) {
    if (value == 0) {
        out << '0';
        return out;
    }
    std::string s;
    bool negative = false;
    if (value < 0) {
        negative = true;
        value = -value;
    }
    while (value > 0) {
        s += '0' + static_cast<int>(value % 10);
        value /= 10;
    }
    if (negative) s += '-';
    std::reverse(s.begin(), s.end());
    out << s;
    return out;
}


__int128 int128_sqrt(__int128 x) {
    if (x < 0) return -1;
    if (x == 0 || x == 1) return x;

    __int128 low = 0, high = x, result = 0;

    while (low <= high) {
        __int128 mid = low + (high - low) / 2;
        if (mid * mid == x) {
            return mid;
        } else if (mid * mid < x) {
            low = mid + 1;
            result = mid;
        } else {
            high = mid - 1;
        }
    }
    return result;
}

int main() {
    __int128 n;
    std::cin >> n;

    if (n <= 1) {
        return 0;
    }

    __int128 num_workers = std::thread::hardware_concurrency();
    __int128 sqrt_n = int128_sqrt(n);

    if (num_workers > sqrt_n) {
        num_workers = sqrt_n;
    }

    __int128 range = sqrt_n / num_workers;

    std::vector<std::thread> workers;
    std::vector<std::vector<__int128> > ranged_factors(num_workers, std::vector<__int128>());

    std::vector<__int128> factors;

    for (__int128 i = 0; i < num_workers;i++) {
        __int128 start = 2 + i * range;
        __int128 end = start + range - 1;

        if (i == num_workers - 1) {
            end = sqrt_n;
        }

        workers.emplace_back([&ranged_factors, i, n, start, end]() {
            __int128 x = n;
            for (__int128 p = start; p <= end && p <= x / p; ++p) {
                while (x % p == 0) {
                    ranged_factors[i].push_back(p);
                    x /= p;
                }
            }
        });
    }

    for (size_t i = 0; i < num_workers;i++) {
        workers[i].join();
    }

    for (auto& factor_range : ranged_factors) {
        for (auto factor : factor_range) {
            factors.push_back(factor);
        }
    }

    sort(factors.begin(), factors.end());

    std::vector<__int128> ans;

    for (size_t i = 0; i < factors.size();i++) {
        bool flag = true;
        for (size_t j = 0; j < i;j++) {
            if (factors[i] == factors[j])
                continue;
            if (factors[i] % factors[j] == 0) {
                flag = false;
                break;
            }
        }
        if (flag) {
            ans.push_back(factors[i]);
        }
    }

    for (__int128 an : ans) {
        n /= an;
    }

    if (n > 1) {
        ans.push_back(n);
    }

    for (const auto& factor : ans) {
        std::cout << factor << ' ';
    }
    std::cout << '\n';

    return 0;
}