// Main Implementation
// Contains all algorithms and user interface


#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <ratio>
#include <vector>
#include <iostream>
#include <list>
#include <optional>
#include <string>
#include <set>
#include <ctime>

using Board = std::vector<int>;
int index(Board b) { return std::find(b.begin(), b.end(), 0) - b.begin(); }
int typeOf(Board b) { return b.size(); }
bool is_solution(Board b) {
	if (typeOf(b) == 9) {
		return b == std::vector({0,1,2,3,4,5,6,7,8});
	}
	else {
		return b == std::vector({0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15});
	}
}
using Pos = std::pair<int, int>;
enum Action { up, left, right, down };
Action op(Action m) {
	switch(m) {
		case up: return down;
		case down: return up;
		case right: return left;
		case left: return right;
	}
}
inline std::ostream & operator<<(std::ostream & os, Action m) {
	switch (m) {
		case left: return os << "left";
		case right: return os << "right";
		case up: return os << "up";
		case down: return os << "down";
	}
}
std::string to_string(Action m) {
	switch (m) {
		case left: return "left";
		case right: return "right";
		case up: return "up";
		case down: return "down";
	}

}
int fromPos(int type, Pos p) {
	auto [x,y] = p;
	return std::sqrt(type)*y + x;
}
Pos toPos(int type, int i) {
	int t = std::sqrt(type);
	return {i%t, i/t};

}
std::string vec_to_str(Board b) {
	std::string s = "{ ";
	for (auto v : b) {
		s += std::to_string(v) + ", ";
	}
	s += "}";
	return s;
}
std::string board_to_str(Board b) {
	std::string s = "";
	s += std::to_string(b[0]) + std::to_string(b[1]) + std::to_string(b[2]) + '\n';
	s += std::to_string(b[3]) + std::to_string(b[4]) + std::to_string(b[5]) + '\n';
	s += std::to_string(b[6]) + std::to_string(b[7]) + std::to_string(b[8]);
	return s;
}
std::string pos_to_str(Pos p) {
	return "(" + std::to_string(p.first) + ", " + std::to_string(p.second) + ")";
}
Pos add_move(Pos current, Action m) {
	auto [x,y] = current;
	switch (m) {
		case left: return {x-1, y};
		case right: return {x+1, y};
		case up: return {x, y-1};
		case down: return {x, y+1};
	}
}
int manhattan(Board b) {
	int sum = 0;
	for (int i=0; i < b.size(); i++) {
		auto t = typeOf(b);
		auto n = b[i];
		auto p1 = toPos(t, n);
		auto p2 = toPos(t, i);
		auto [x1, y1] = p1;
		auto [x2, y2] = p2;
		sum += abs(x1-x2) + abs(y1-y2);
	}
	return sum;
}
using Path = std::list<Action>;
std::string list_to_string(Path p) {
	std::string s = "(";
	for (auto a : p) {
		s += to_string(a) + ", ";
	}
	s += ")";
	return s;
}
std::string list_to_string(std::list<int> p) {
	std::string s = "(";
	for (auto a : p) {
		s += std::to_string(a) + ", ";
	}
	s += ")";
	return s;
}
std::optional<Board> move(Board board, Action m) {
	auto t = typeOf(board);
	Board b (t);
	std::copy(board.begin(), board.end(), b.begin());
	auto holePos = toPos(t,index(b));
	auto [x,y] = add_move(holePos, m);
	if (x < 0 || y < 0 || x >= std::sqrt(t) || y >= std::sqrt(t)) { return std::nullopt; }
	auto holeInd = fromPos(t, holePos);
	auto target = fromPos(t, {x,y});
	auto aux = b[target];
	b[target] = 0;
	b[holeInd] = aux;
	return b;
}
std::list<std::pair<Board, Action>> expand(Board b, std::optional<Action> oa) {
	std::list<std::pair<Board, Action>> l = {};
	for (auto a : {up, left, right, down}) {
		if (oa.has_value() and op( oa.value() ) == a) { continue; }
		auto nb = move(b, a);
		if (nb.has_value()) { l.push_back({nb.value(), a}); }
	}
	return l;
}

class Node {
	public:
		std::optional<Node*> parent;
		std::optional<Action> action;
		int cost;
		Board state;
		Node(Board b) {
			this->state = b;
			this->parent = std::nullopt;
			this->action = std::nullopt;
			this->cost = 0;
		}
		Node(Board b, Node* parent, Action action) {
			this->state = b;
			this->parent = parent;
			this->action = action;
			this->cost = parent->cost +1;
		}
		Path path() {
			std::list<Action> path = {};
			auto current = this;
			while (current->parent && current->action) {
				path.push_back(current->action.value());
				current = current->parent.value();
			}
			return path;
		}
		friend inline bool operator==(const Node& lhs, const Node& rhs) {
			return lhs.state == rhs.state;
		}

};

struct Output {
	std::optional<Path> solution;
	int nodes_expanded;
	double average_heuristic_value;
	int initial_heuristic_value;
};

Output bfs(Board b) {
	std::set<Board> closed = {b};
	std::list<Node*> open = {new Node(b)};
	auto nodes = 0;
	double acc = 0;
	while (!open.empty()) {
		auto n = open.front();
		open.pop_front();		
		for (auto [s, a] : expand(n->state, n->action)) {
			auto n2 = new Node(s, n, a);
			if (is_solution(s)) {
				nodes++;
				acc += manhattan(s);
				return Output{n2->path(), nodes, acc/(float)(nodes), 0};
			}
			if (!closed.count(s)) {
				closed.insert(s);
				open.push_back(n2);
				acc += manhattan(s);
			}
		}
		nodes++;
	}
}

Output idfs(Board b) {}
Output astar(Board b) {}
Output idastar(Board b) {}
Output gbfs(Board b) {}
/* Output bfs(Board b) {
	std::set<Board> closed = {b};
	std::list<Node*> open = {new Node(b)};
	while (!open.empty()) {
		auto current = open.front();
		open.pop_front();
		for (auto n : current->expand()) {
			if (n->is_solution()) {
				return Output{n->path(), static_cast<int>(closed.size()), 0, manhattan(b)};
			}
			if (!closed.count(n->state)) {
				closed.insert(n->state);
				open.push_back(n);
			}
		}
	}
	return {}; //no solution, should never happen
}

Output dfs(Node* n, std::set<Board>* visited, int max_depth) {
	if (n->is_solution()) { return Output{n->path(), static_cast<int>(visited->size()), 0, 0}; }
	if (max_depth == 0) { return Output{std::nullopt, 0, 0, 0}; }
	visited->insert(n->state);
	for (auto nn : n->expand()) {
		if (visited->count(nn->state)) { continue; }
		auto s = dfs(nn, visited, max_depth-1);
		if (s.solution.has_value()) {
			return s;
		}
	}
	return Output{std::nullopt, 0, 0, 0};
}

Output idfs(Board b) {
	int iteration = 1;
	int nodes = 0;
	while(true) {
		auto [path, nnodes, acc, init] = dfs(new Node(b), new std::set<Board>(), iteration);
		nodes += nnodes;
		if (path) {
			return Output{path, nodes, acc/(double)(nodes), manhattan(b)};
		}
		iteration++;
	}

	return Output{std::nullopt, 0 , 0, 0};
}

Output astar(Board b) {
	std::set<Board> closed = {b};
	std::vector<Node*> open = {new Node(b)};
	std::make_heap(open.begin(), open.end(), compare_Node);
	int acc = 0;
	while(!open.empty()) {
		auto current = open.back();
		open.pop_back();
		if (!closed.count(current->state)) {
			closed.insert(current->state);
			acc += manhattan(current->state);
			if (current->is_solution()) {
				int n = (int)(closed.size());
				double avg = (double)(acc)/(double)(closed.size());
				return Output{current->path(), n, avg, 0};
			}
			for (auto n : current->expand()) {
				open.push_back(n);
				std::push_heap(open.begin(), open.end(), compare_Node);
			}
		}
	}

	return Output{std::nullopt, 0 , 0, 0};
}

Output idastar_rec(Node* n, int limit, std::set<Board>* closed) {
	if (node_f(n) > limit) {
		return Output{std::nullopt, manhattan(n->state), 0, node_f(n)};
	}
	if (n->is_solution()) {
		return Output{n->path(), manhattan(n->state), 0, 0};
	}
	std::optional<int> next_limit = std::nullopt;
	closed->insert(n->state);
	for (auto nn : n->expand()) {
		if(!closed->count(nn->state)) {
			auto [path, avg, zzz, rec_limit] = idastar_rec(nn, limit, closed);
			if (path) {
				return Output{path, avg+manhattan(n->state), 0, 0};
			}
			if (next_limit) {
				next_limit = std::min(next_limit.value(), rec_limit);
			}
			else { next_limit = rec_limit;}
		}
	}
	return Output{std::nullopt, manhattan(n->state), 0, next_limit.value()};

}

Output idastar(Board b) {
	auto init = new Node(b);
	auto limit = manhattan(b);
	while (true) {
		auto closed = new std::set<Board>();
		auto [path, avg, zzz, rec_limit] = idastar_rec(init, limit, closed);
		limit = rec_limit;
		if (path) {
			return Output{path.value(), static_cast<int>(closed->size()), (float)(avg)/(float)(closed->size(), 0)};
		}
	}
}

Output gbfs(Board b) {
	std::set<Board> closed = {b};
	std::vector<Node*> open = {new Node(b)};
	std::make_heap(open.begin(), open.end(),
				   [](Node* a, Node* b){ return manhattan(a->state) < manhattan(b->state);});
	int acc = 0;
	while(!open.empty()) {
		auto current = open.back();
		open.pop_back();
		if (!closed.count(current->state)) {
			closed.insert(current->state);
			acc += manhattan(current->state);
			if (current->is_solution()) {
				int n = (int)(closed.size());
				double avg = (double)(acc)/(double)(closed.size());
				return Output{current->path(), n, avg, 0};
			}
			for (auto n : current->expand()) {
				open.push_back(n);
				std::push_heap(open.begin(), open.end(), compare_Node);
			}
		}
	}

	return Output{std::nullopt, 0 , 0, 0};
} */

void process_instance(char* algo, std::list<int> inst) {
	std::string algorithm = algo;
	Board instance{std::make_move_iterator(std::begin(inst)), std::make_move_iterator(std::end(inst))};
	auto t1 = std::chrono::high_resolution_clock::now();
	Output r;
	if (algorithm == "-bfs") { r = bfs(instance); }
	if (algorithm == "-idfs") { r = idfs(instance); }
	if (algorithm == "-astar") { r = idfs(instance); }
	if (algorithm == "-idastar") { r = idfs(instance); }
	if (algorithm == "-gbfs") { r = idfs(instance); }
	auto t2 = std::chrono::high_resolution_clock::now();
	auto [path, nodes, acc, init] = r;
	std::cout << nodes << "," << path.value().size() << "," <<
		std::chrono::duration<double, std::ratio<1,1>>(t2-t1).count()
			  << "," << acc << "," << init << std::endl;
}

int main(int argc, char ** argv) {
	auto algorithm = argv[1];
	auto has_comma = [](char* s) { return std::strchr(s, ',') != nullptr; };
	auto instance_start = argv+2;
	std::list<int> instance = {};
	while (instance_start != argv+argc) {
		auto n = *instance_start;
		if (has_comma(n)) {
			*(std::strchr(n,',')) = '\0';
			instance.push_back(std::atoi(n));
			process_instance(algorithm, instance);
			instance = {};
		}
		else {
			instance.push_back(std::atoi(n));
		}
		instance_start++;
	}
	if (!instance.empty()) {
		process_instance(algorithm, instance);
	}
	return 0;
}
