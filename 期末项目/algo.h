#pragma once
#include"fwd.h"
#include<stack>
#include<queue>
#include<map>
#include<array>

inline constexpr auto detect(const point& p,const maze& visited) {
	std::array<point, 4> ans = { point{p.x + 1,p.y},point{p.x,p.y + 1},\
		point{ p.x - 1,p.y  }, point{p.x,p.y - 1} };
	for (auto&& i : ans)if (visited(i))i = point{ 0,0 };
	return ans;
}

class dfs_OSstack_single {
	maze visited;
	std::vector<point> ans;
	std::deque<point> tmp;
	constexpr bool solve(point p={1,1}) {
		if (p == visited.dst) {
			tmp.emplace_back(p);
			ans.reserve(tmp.size());
			ans.assign(tmp.begin(),tmp.end());
			return true;
		}
		visited(p) = true;
		tmp.emplace_back(p);
		for (auto&& i : detect(p,visited))if ((i != point{ 0,0 }) && !visited(i) && solve(i))return true;
		tmp.pop_back();
		return false;
	}
public:

	dfs_OSstack_single(const maze& map) :visited(map) { 
		if(visited(1,1))throw std::invalid_argument("Cannot enter the maze");
		solve(); }
	auto get_ans() {
		if (ans.empty())return decltype(ans)(1, point{ 1,1 });
		return ans;
	}
};

class dfs_OSstack {
	maze visited;
	std::deque<point> tmp;
	std::vector<std::vector<point>> ans;
	constexpr void solve(point p = { 1,1 }) {
		if (p == visited.dst) {
			tmp.emplace_back(p);
			ans.emplace_back(tmp.begin(),tmp.end());
			tmp.pop_back();
			return;
		}
		visited(p) = true;
		tmp.emplace_back(p);
		for (auto&& i : detect(p,visited))if ((i != point{ 0,0 }) && !visited(i))solve(i);
		tmp.pop_back();
		visited(p) = false;
	}
public:
	dfs_OSstack(const maze& map) :visited(map) {
		if (visited(1, 1))throw std::invalid_argument("Cannot enter the maze");
		solve(); }
	auto get_ans() {
		if (ans.empty())return decltype(ans)(1, std::vector(1, point{ 1,1 }));
		return ans;
	}
};

auto dfs_single(const maze& graph) {
	if (graph(1, 1))throw std::invalid_argument("Cannot enter the maze");

	maze visited = graph;
	std::stack<point> tmp;
	tmp.emplace(point{ 1,1 });

	while (!tmp.empty()) {
		const auto& cur = tmp.top();
		if (cur == visited.dst) {
			std::vector<point> ans;
			ans.reserve(tmp.size());
			auto&& tmp_container = tmp._Get_container();
			ans.assign(tmp_container.begin(), tmp_container.end());
			return ans;
		}
		bool moved = false;
		for (auto&& i : detect(cur,visited)) {
			if ((i != point{ 0,0 }) && !visited(i)) {
				visited(i) = true;
				tmp.emplace(i);
				moved = true;
				break;
			}
		}
		if (!moved) tmp.pop();
	}
	return std::vector<point>(1,point{1,1});
}

auto dfs(const maze& graph) {
	if (graph(1, 1))throw std::invalid_argument("Cannot enter the maze");

	maze visited=graph;
	std::stack<point> tmp;
	std::stack<size_t> index;
	std::vector<std::vector<point>> ans;
	tmp.emplace(point{ 1,1 });
	index.push(0);

	auto traceback = [&]() {
		visited(tmp.top()) = false;
		tmp.pop();
		index.pop();
		};

	while (!tmp.empty()) {
		const auto& cur = tmp.top();
		size_t& idx = index.top();
		if (cur == visited.dst) {
			auto&& tmp_container = tmp._Get_container();
			ans.emplace_back(tmp_container.begin(), tmp_container.end());
			visited(tmp.top()) = false;
			traceback();
		}

		auto neighbors = detect(cur,visited);
		bool moved = false;

		for (size_t i = idx; i < 4; i++) {
			auto& next = neighbors[i];
			if ((next != point{ 0, 0 }) && !visited(next)) {
				visited(next) = true;
				tmp.push(next);
				index.push(0);
				idx = i + 1;
				moved = true;
				break;
			}
		}

		if (!moved) traceback();
	}
	if (ans.empty())return decltype(ans)(1, std::vector(1, point{ 1,1 }));
	return ans;
}

auto bfs(const maze& graph) {
	if (graph(1, 1))throw std::invalid_argument("Cannot enter the maze");

	maze visited = graph;
	std::queue<point> pipe;
	std::map<point, point> prev;
	pipe.emplace(point{ 1,1 });

	while (!pipe.empty()) {
		auto cur = std::move(pipe.front());
		pipe.pop();

		if (cur == visited.dst) {
			std::vector<point> ans;
			ans.reserve(prev.size() / 2);
			for (auto p = cur; p != point{ 1,1 }; p = prev[p])ans.emplace_back(p);
			ans.emplace_back(1, 1);
			std::reverse(ans.begin(), ans.end());
			return ans;
		}
		for (auto&& i : detect(cur,visited)) {
			if ((i != point{ 0,0 }) && !visited(i)) {
				visited(i) = true;
				prev[i] = cur;
				pipe.emplace(i);
			}
		}
	}
	return std::vector<point>(1,point{1,1});
}