#include"fwd.h"
#include"algo.h"
#include<iostream>

int main() {
	using namespace std;
	auto map=graph({
	{0,1,0,0,0,1,0},
	{0,1,0,1,0,1,0},
	{0,0,0,1,0,0,0},
	{1,1,0,1,1,0,1},
	{0,0,0,0,0,0,0},
	{0,1,1,1,1,1,0},
	{0,0,0,0,0,0,0}
		});
	auto solution = dfs_OSstack(map);
	auto ans = solution.get_ans();
	for (auto&& i : ans) {
		for (char comma[] = { '\0',',','\0' };  auto&& j : i) {
			cout << comma << j;
			comma[0] = ' ';
		}
		cout << endl;
	}
}