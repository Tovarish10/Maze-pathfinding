#pragma once
#include"fwd.h"
#include<deque>
#include<memory>
#include<array>

class dfs_OSstack_single {
	graph visited;
	std::deque<point> ans,tmp;
	inline auto detect(const point p) {
		std::array<point, 4> ans = { point{p.x - 1,p.y},point{p.x + 1,p.y},\
			point{ p.x,p.y - 1 }, point{p.x,p.y + 1} };
		for (auto&& i : ans)if (visited(i))i = point{ 0,0 };
		return ans;
	}
	bool solve(point p={1,1}) {
		if (p == visited.dst) {
			tmp.emplace_back(p);
			ans = tmp;
			return true;
		}
		visited(p) = true;
		tmp.emplace_back(p);
		for (auto&& i : detect(p))if ((i != point{ 0,0 }) && !visited(i) && solve(i))return true;
		tmp.pop_back();
		return false;
	}
public:

	dfs_OSstack_single(const graph& map) :visited(map) { solve(); }
	auto get_ans() {
		if (ans.empty())return decltype(ans)(1, point{ 1,1 });
		return ans;
	}
};

class dfs_OSstack {
	graph visited;
	std::deque<point> tmp;
	std::vector<decltype(tmp)> ans;
	inline auto detect(const point p) {
		std::array<point, 4> ans = { point{p.x - 1,p.y},point{p.x + 1,p.y},\
			point{ p.x,p.y - 1 }, point{p.x,p.y + 1} };
		for (auto&& i : ans)if (visited(i))i = point{ 0,0 };
		return ans;
	}
	void solve(point p = { 1,1 }) {
		if (p == visited.dst) {
			tmp.emplace_back(p);
			ans.emplace_back(tmp);
			tmp.pop_back();
			return;
		}
		visited(p) = true;
		tmp.emplace_back(p);
		for (auto&& i : detect(p))if ((i != point{ 0,0 }) && !visited(i))solve(i);
		tmp.pop_back();
		visited(p) = false;
	}
public:
	dfs_OSstack(const graph& map) :visited(map) { solve(); }
	auto get_ans() {
		if (ans.empty())return decltype(ans)(1, decltype(tmp)(1, point{ 1,1 }));
		return ans;
	}
};