#include"fwd.h"
#include"algo.h"
#include<iostream>
int main() {
	using namespace std;
	auto map = maze({
		{0,0,1,0,0,0,1,0},
		{0,0,1,0,0,0,1,0},
		{0,0,0,0,1,1,0,1},
		{0,1,1,1,0,0,1,0},
		{0,0,0,1,0,0,0,0},
		{0,1,0,0,0,1,0,1},
		{0,1,1,1,1,0,0,1},
		{1,1,0,0,0,1,0,1},
		{1,1,0,0,0,0,0,0},
		});
	auto ans = dfs(map);
	for (auto&& i : ans) {
		for (char comma[] = { '\0',',','\0' };  auto&& j : i) {
			cout << comma << j;
			comma[0] = ' ';
		}
		cout << '\n' << route(map, i) << endl;
	}
	cout << "The shortest route is:\n";
	auto shortest = bfs(map);
	for (char comma[] = { '\0',' ','\0' }; auto&& i : shortest) {
		cout << comma << i;
		comma[0] = ' ';
	}
	cout << '\n' << route(map, shortest) << endl;
}