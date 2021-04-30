#include <iostream>
#include <vector>
#include <array>

using param_pair = std::pair<std::string_view, std::string_view>;
param_pair make_param(std::string_view key, std::string_view value)
{
    return std::make_pair(key, value);
}

template <typename... Ts>
std::array<std::byte, sizeof...(Ts)> make_bytes(Ts &&...args) noexcept
{
    return {std::byte(std::forward<Ts>(args))...};
}

template <typename T>
concept index_access_array = requires(T t)
{
    t[0];
    {
        t.size()
    }
    ->std::convertible_to<std::size_t>;
};

std::vector<param_pair> handle_input(int argc, char **argv);
bool compare_array(const index_access_array auto &d1, const index_access_array auto &d2);

int main(int argc, char **argv)
{
    auto params = handle_input(argc, argv);
    for (const auto &param : params)
    {
        std::cout << "key: " << param.first << " value: " << param.second << "\n";
    }
    auto d1 = make_bytes(0, 1, 2, 3, 4);
    auto d2 = make_bytes(0, 1, 2, 3, 4);
    auto d3 = std::vector<int>{1, 2};
    auto d4 = std::vector<int>{1, 2, 3};

    std::cout << compare_array(d1, d2) << "\n";
    std::cout << compare_array(d3, d4) << "\n";
    return 0;
}

std::vector<param_pair> handle_input(int argc, char **argv)
{
    std::vector<param_pair> params;
    if (argc > 1)
    {
        for (int i = 1; i < argc - 1; i += 2)
        {
            params.push_back(make_param(argv[i], argv[i + 1]));
        }
    }
    return params;
}

bool compare_array(const index_access_array auto &d1, const index_access_array auto &d2)
{
    if (d1.size() != d2.size())
        return false;

    for (std::size_t i = 0; i < d1.size(); ++i)
        if (d1[i] != d2[i])
            return false;

    return true;
}
