#pragma once
#include"fwd.h"
#include<stack>
#include<array>

inline constexpr auto detect_base(const point p,const maze& visited) {
	std::array<point, 4> ans = { point{p.x + 1,p.y},point{p.x,p.y + 1},\
		point{ p.x - 1,p.y  }, point{p.x,p.y - 1} };
	for (auto&& i : ans)if (visited(i))i = point{ 0,0 };
	return ans;
}
class dfs_OSstack_single {
	maze visited;
	std::vector<point> ans;
	std::deque<point> tmp;
	inline constexpr auto detect(const point p)const {return detect_base(p, visited);}
	constexpr bool solve(point p={1,1}) {
		if (p == visited.dst) {
			tmp.emplace_back(p);
			ans.reserve(tmp.size());
			ans.assign(tmp.begin(),tmp.end());
			return true;
		}
		visited(p) = true;
		tmp.emplace_back(p);
		for (auto&& i : detect(p))if ((i != point{ 0,0 }) && !visited(i) && solve(i))return true;
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
	inline constexpr auto detect(const point p) const{ return detect_base(p, visited); }
	constexpr void solve(point p = { 1,1 }) {
		if (p == visited.dst) {
			tmp.emplace_back(p);
			ans.emplace_back(tmp.begin(),tmp.end());
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
	dfs_OSstack(const maze& map) :visited(map) {
		if (visited(1, 1))throw std::invalid_argument("Cannot enter the maze");
		solve(); }
	auto get_ans() {
		if (ans.empty())return decltype(ans)(1, std::vector(1, point{ 1,1 }));
		return ans;
	}
};

class dfs_single {
	maze visited;
	std::vector<point> ans;
	std::stack<point> tmp;
	inline constexpr auto detect(const point p)const { return detect_base(p, visited); }
	void solve() {
		while (!tmp.empty()) {
			const auto& cur = tmp.top();
			if (cur == visited.dst) {
				ans.reserve(tmp.size());
				auto& tmp_container = tmp._Get_container();
				ans.assign(tmp_container.begin(), tmp_container.end());
				return;
			}
			bool moved = false;
			for (auto&& i : detect(cur)) {
				if ((i != point{ 0,0 }) && !visited(i)) {
					visited(i) = true;
					tmp.emplace(i);
					moved = true;
					break;
				}
			}
			if (!moved) tmp.pop();
		}
		return;
	}
public:
	dfs_single(const maze& map) :visited(map) {
		if (visited(1, 1))throw std::invalid_argument("Cannot enter the maze");
		tmp.emplace(point{ 1,1 });
		solve(); }
	auto get_ans() {
		if (ans.empty())return decltype(ans)(1, point(point{ 1,1 }));
		return ans;
	}
};

class dfs {
	maze visited;
	std::stack<point> tmp;
	std::stack<size_t> index;
	std::vector<std::vector<point>> ans;
	inline constexpr auto detect(const point p)const { return detect_base(p, visited); }
	void solve() {
		auto traceback = [&]() {
			visited(tmp.top()) = false;
			tmp.pop();
			index.pop();
			};
		while (!tmp.empty()) {
			const auto& cur = tmp.top();
			size_t& idx = index.top();
			if (cur == visited.dst) {
				auto& tmp_container = tmp._Get_container();
				ans.emplace_back(tmp_container.begin(), tmp_container.end());
				visited(tmp.top()) = false;

				// 回溯到上一个节点继续探索
				traceback();
			}
			// 获取所有邻接点
			auto neighbors = detect(cur);
			bool moved = false;

			// 从上次探索的位置继续
			for (size_t i = idx; i < 4; i++) {
				auto& next = neighbors[i];
				if ((next != point{ 0, 0 }) && !visited(next)) {
					// 前进
					visited(next) = true;
					tmp.push(next);
					index.push(0);
					idx = i + 1;  // 更新当前节点的探索进度
					moved = true;
					break;
				}
			}

			// 无路可走，回溯
			if (!moved) traceback();
		}
	}
public:
	dfs(const maze& map) :visited(map) {
		if (visited(1, 1))throw std::invalid_argument("Cannot enter the maze");
		tmp.emplace(point{ 1,1 });
		index.push(0);
		solve(); }
	auto get_ans() {
		if (ans.empty())return decltype(ans)(1,std::vector(1,point{1,1}));
		return ans;
	}
};