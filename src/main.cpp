// Main Implementation
// Contains all algorithms and user interface


#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iterator>
#include <ostream>
#include <vector>
#include <iostream>
#include <list>
#include <optional>
#include <string>
#include <set>

using Board = std::vector<int>;
int index(Board b) { return std::find(b.begin(), b.end(), 0) - b.begin(); }
using Pos = std::pair<int, int>;
enum Move { left, right, up, down };
inline std::ostream & operator<<(std::ostream & os, Move m) {
	switch (m) {
		case left: return os << "left";
		case right: return os << "right";
		case up: return os << "up";
		case down: return os << "down";
	}
}
std::string to_string(Move m) {
	switch (m) {
		case left: return "left";
		case right: return "right";
		case up: return "up";
		case down: return "down";
	}

}
int typeOf(Board b) { return b.size(); }

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

Pos add_move(Pos current, Move m) {
	auto [x,y] = current;
	switch (m) {
		case left: return {x-1, y};
		case right: return {x+1, y};
		case up: return {x, y-1};
		case down: return {x, y+1};
	}
}
using Path = std::list<Move>;

std::string list_to_string(Path p) {
	std::string s = "(";
	for (auto a : p) {
		s += to_string(a) + ", ";
	}
	s += ")";
	return s;
}

std::optional<Board> move(Board board, Move m) {
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

std::list<std::pair<Board, Move>> expand(Board b) {
	std::list<std::pair<Board, Move>> l = {};
	for (auto a : {up, left, right, down}) {
		auto nb = move(b, a);
		if (nb.has_value()) { l.push_back({nb.value(), a}); }
	}
	return l;
}


class Node {
	private:
		std::optional<Node*> parent;
		std::optional<Move> action;
	public:
		Board state;
		int cost;
		Node(Board b) {
			this->state = b;
			this->cost = 0;
			this->parent = std::nullopt;
			this->action = std::nullopt;
		}
		Node(Board b, Node* parent, Move action, int cost) {
			this->state = b;
			this->parent = parent;
			this->cost = parent->cost + cost;
			this->action = action;
		}
		Path path() {
			std::list<Move> path = {};
			auto current = this;
			while (current->parent && current->action) {
				path.push_back(current->action.value());
				current = current->parent.value();
			}
			return path;
		}
		std::list<Node *> expand() {
			std::list<Node *> ns = {};
			for (auto [n,a] : ::expand(this->state)) {
				ns.push_back(new Node(n, this, a, 1));
			}
			return ns;
		}
		friend inline bool operator==(const Node& lhs, const Node& rhs) {
			return lhs.state == rhs.state;
		}
		bool is_solution() {
			if (typeOf(this->state) == 9) {
				return this->state == std::vector({0,1,2,3,4,5,6,7,8});
			}
			else {
				return this->state == std::vector({0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15});
			}
		}

};

int manhattan(Board b) {
	int sum = 0;
	for (int i=0; i < b.size(); i++) { sum += abs(b[i] - i); }
	return sum;
}

std::optional<Path> bfs(Board b) { //TODO?
	std::set<Board> visited = {};
	std::list<Node*> frontier = {new Node(b)};
	while (true) {
		if (frontier.empty()) { return std::nullopt; }
		auto current = frontier.front();
		frontier.pop_front();
		if (current->is_solution()) {
			std::cout << board_to_str(current->state) << std::endl;
			return current->path();
		}
		visited.insert(current->state);
		for (auto n : current->expand()) {
			if (!visited.count(n->state)) { frontier.push_back(n); }
		}
	}
	return std::nullopt;
}

std::optional<Path> idfs(Board b) { //TODO
	return {};
}

std::optional<Path> astar(Board b) { //TODO
	return {};
}

std::optional<Path> idastar(Board b) { //TODO
	return {};
}

std::optional<Path> gbfs(Board b) { //TODO
	return {};
}


int main(int argc, char ** argv) { //TODO
	Board b = {1,2,3,4,5,6,7,8, 0};
	auto l = bfs(b);
	std::cout << list_to_string(l.value()) << std::endl;
	return 0;
}
