#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <future>

std::vector<size_t> read_array() {
    size_t length, a, b, p;
    std::cin >> length >> a >> b >> p;
    std::vector<size_t> result(length);
    result[0] = a % p;
    for (size_t i = 1; i < result.size(); ++i) {
        result[i] = (result[i - 1] * a + b) % p;
    }
    return result;
}


int main() {
    auto array = read_array();
    std::vector<std::future<void>> futures;
    size_t k;
    std::cin >> k;
    
    size_t number_buckets = 0;
    
    for (size_t i = 0; i < array.size(); i += k) { 
        number_buckets++;
        
        futures.emplace_back(std::async(std::launch::async, [i, &array, k]() {
            size_t s = i;
            size_t e = std::min(s + k, array.size());
            auto start = array.begin() + s;
            auto end = array.begin() + e;
            sort(start, end);
        }));
    }

    std::vector<int> answers;
    
    for (auto& future : futures) {
        future.get(); 
    }

    std::vector<int> starts(number_buckets);
    std::vector<int> ends(number_buckets);

    for (int i = 0; i < number_buckets;i++) {
        starts[i] = i * k;
        ends[i] = std::min( (i + 1)*k, array.size() );
    }

    // K-WAY
    
    std::priority_queue<std::pair<int, int>> q;

    for (int i = 0; i < number_buckets;i++) {
        q.push({-array[starts[i]++], i});
    }

    int counter = 0;

    while (!q.empty()) {
        auto [elem, id] = q.top();
        q.pop();
        if (starts[id] < ends[id]) {
            q.push({-array[starts[id]++], id});
        }
        counter++;
        if (counter % k == 0) {
            answers.push_back(-elem);
        }
    }

    for (auto ans : answers) {
        std::cout << ans << " ";
    }
    std::cout << "\n";

    return 0;
}