#pragma once
#include<vector>
#include<initializer_list>
#include<stdexcept>
#include<algorithm>
#include<iosfwd>
template<class T=bool>
using graph_t = std::vector<std::vector<T>>;

struct point {
	size_t x, y;
	//point(std::initializer_list<size_t> pos):x(pos.data()[0]),y(pos.data()[1]){}
	inline auto operator<=>(const point&)const = default;
	template<typename charT, typename traits>
	friend std::basic_ostream<charT, traits>& \
		operator<<(std::basic_ostream<charT, traits>& os,point p) {
		os << '(' << p.x << ',' << p.y << ')';
		return os;
	}
};
//struct point_p:point
//{
//	point prev;
//	point_p(const point& pos,const point& prev):point(pos),prev(prev){}
//};
class maze {
	graph_t<> board;
public:
	point dst;
	maze(size_t row, size_t col): board(row + 2, std::vector<bool>(col + 2, true)),
		dst(row, col)
	{
		for (size_t i = 1; i <= row; ++i) std::fill(board[i].begin() + 1, board[i].begin() + 1 + col, false);
	}
	maze(std::initializer_list<std::initializer_list<bool>> ini)
		: board([&]() -> graph_t<> {
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
		dst(ini.size(), ini.size() ? ini.begin()->size() : 0){}
	inline auto operator()(size_t x, size_t y) const { return board[x][y]; }
	inline bool operator()(point p)const { return board[p.x][p.y]; }
	inline std::vector<bool>::reference operator()(point p) { return board[p.x][p.y]; }
};

namespace Signals {
	const char RESET[] = "\x1b[0m";
	const char GREEN[] = "\x1b[32m";
	const char RED[] = "\x1b[31m";

	const char BLANK[] = "  ";
	const char WALL[] = "\x1b[47m  \x1b[0m";
	const char LEFT[] = "\x1b[32m-\u2190\x1b[0m";
	const char UP[] = "\x1b[32m\u2191\u2191\x1b[0m";
	const char RIGHT[] = "\x1b[32m-\u2192\x1b[0m";
	const char DOWN[] = "\x1b[32m\u2193\u2193\x1b[0m";

}
template<class A, class B>
auto render_route(std::basic_ostream<A, B>& os, const maze& graph, const std::vector<point>& route) {
	graph_t<const char*> rendergraph(graph.dst.x + 2, std::vector<const char*>(graph.dst.y + 2));
	for (auto i = 0; i < graph.dst.x + 2; ++i) {
		for (auto j = 0; j < graph.dst.y + 2; ++j)
		{
			rendergraph[i][j] = graph(i, j) ? Signals::WALL : Signals::BLANK;
		}
	}
	if (route.size() > 1) {
		auto j = route.begin();
		for (auto&& i = std::next(route.begin()); i < route.end(); ++i) {
			switch ((i->y - j->y + 1) * 2 + (i->x - j->x)) {
			case 0:rendergraph[i->x][i->y] = Signals::LEFT; break;
			case 1:rendergraph[i->x][i->y] = Signals::UP; break;
			case 3:rendergraph[i->x][i->y] = Signals::DOWN; break;
			case 4:rendergraph[i->x][i->y] = Signals::RIGHT; break;
			}
			j = i;
		}
	}
	rendergraph[1][1] = "\x1b[41m  \x1b[0m";
	rendergraph[graph.dst.x][graph.dst.y] = "\x1b[42m  \x1b[0m";
	for (auto&& i : rendergraph) {
		for (auto&& j : i) os << j;
		os << '\n';
	}
}