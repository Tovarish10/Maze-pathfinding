#pragma once
#include<vector>
#include<initializer_list>
#include<stdexcept>
#include<algorithm>
#include<iosfwd>
namespace Color {
	const char* RED = "\033[31m";
	const char* GREEN = "\033[32m";
	const char* YELLOW = "\033[33m";
	const char* BLUE = "\033[34m";
	const char* RESET = "\033[0m";
}
using graph_t = std::vector<std::vector<bool>>;

struct point {
	size_t x, y;
	inline auto operator<=>(const point&)const = default;
	template<typename charT, typename traits>
	friend std::basic_ostream<charT, traits>& \
		operator<<(std::basic_ostream<charT, traits>& os,point p) {
		os << '(' << p.x << ',' << p.y << ')';
		return os;
	}
};
class graph {
	graph_t board;
public:
	point dst;
	graph(size_t row, size_t col): board(row + 2, std::vector<bool>(col + 2, true)),
		dst(row, col)
	{
		for (size_t i = 1; i <= row; ++i) std::fill(board[i].begin() + 1, board[i].begin() + 1 + col, false);
	}
	graph(std::initializer_list<std::initializer_list<bool>> ini)
		: board([&]() -> graph_t {
			const auto row = ini.size();
			const auto col = row ? ini.begin()->size() : 0;
			graph_t tmp(row + 2, std::vector<bool>(col + 2, true));
			size_t i = 1;
			for (const auto& r : ini) {
				if (r.size() != col)[[unlikely]] throw std::invalid_argument("Columns don't align.");
				size_t j = 1;
				for (bool v : r) {
					tmp[i][j++] = v;
				}
				++i;
			}
			return tmp;
		}()),
		dst(ini.size(), ini.size() ? ini.begin()->size() : 0){
		if (board[1][1])throw std::invalid_argument("Cannot enter the graph.");
	}
	inline auto operator()(size_t x, size_t y) const { return board[x][y]; }
	inline bool operator()(point p)const { return board[p.x][p.y]; }
	inline std::vector<bool>::reference operator()(point p) { return board[p.x][p.y]; }
};