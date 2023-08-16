#include "vector.h"
#include <iostream>

using namespace stl;
class Test {
public:
	Test(int a = 12, char b = 'm', std::string str = "ÄãºÃ")
		:_a(a), _b(b), _str(str) {}
	friend std::ostream& operator<<(std::ostream& cout, const Test& other);
private:
	int _a;
	char _b;
	std::string _str;
};
std::ostream& operator<<(std::ostream& cout, const Test& other) {
	return cout << other._a << other._b << other._str;
}
int main() {
	vector<int> _vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	std::cout << _vector.size() << _vector.capacity() << std::endl;
	_vector.assign({2,3,5});
	for (auto x : _vector) {
		std::cout << x << " ";
	}
	std::cout << std::endl;
	std::cout << _vector.size() << _vector.capacity() << std::endl;
	while (true)
	{
	}

}