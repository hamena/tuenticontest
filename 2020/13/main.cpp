#include <fstream>
#include <iostream>

using namespace std;

size_t perimeter(size_t x, size_t y) { return 2 * (x + y) - 4; }
size_t area(size_t x, size_t y) { return x * y; }

class Fortress {
public:
    Fortress() : th(3),tx(1),ty(1),fx(5),fy(5),packs(43) {
	s1 = (2 + th-1) * 2 * (th-2);
	s2 = (1 + th-2) * 2 * (th-2);
    }

    size_t height() { return th; }
    size_t npacks() { return packs; }

    void grow_tower() {
	++th;
	s1 = ((2 + th-1) * (th-2))/2;
	s2 = ((1 + th-2) * (th-2))/2;
	fx += 4;
	fy += 4;
	packs += area(fx-2, fy-2) + perimeter(fx, fy)*2;
    }
    
    void reduce_tower() {
	--th;
	s1 = ((2 + th-1) * (th-2))/2;
	s2 = ((1 + th-2) * (th-2))/2;
	packs -= area(fx-2,fy-2) + perimeter(fx, fy)*2;
	fx -= 4;
	fy -= 4;
    }

    void grow_base() {
	if (tx > ty) {
	    ++ty;
	    packs += th*tx + (s1 + s2)*2;
        } else {
	    ++tx;
	    packs += th*ty + (s1 + s2)*2;
	}
    }

    void reduce_base() {
	if (tx > ty) {
	    --tx;
	    packs -= th*ty + (s1 + s2)*2;
	} else {
	    --ty;
	    packs -= th*tx + (s1 + s2)*2;
	}
    }

private:
    size_t th, tx, ty;
    size_t fx, fy;
    size_t s1, s2;
    size_t packs;
};

int main(int argc, char **argv) {

  if (argc >= 2) {

	ifstream ifs(argv[1]);
	size_t n_cases, i_case=1;
	ifs >> n_cases;

	while (n_cases --> 0) {
	    size_t max;
	    ifs >> max;

	    if (max < 43) {
		cout << "Case #" << i_case++ << ": IMPOSSIBLE" << endl;
	    } else {
		Fortress fortress;
		while (fortress.npacks() <= max) {
		    fortress.grow_tower();
		}
		fortress.reduce_tower();

		while (fortress.npacks() <= max) {
		    fortress.grow_base();
		}
		fortress.reduce_base();

		cout << "Case #" << i_case++ << ": "
		     << fortress.height() << " " << fortress.npacks() << endl;
	    }
	}
	
	ifs.close();
	
    } else {
	return 1;
    }
    return 0;
}
