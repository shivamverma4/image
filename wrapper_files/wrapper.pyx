cdef extern from "PCA.hpp":

	void drw_xs(Mat&, Point, Point, Scalar, const float);

        double get_orient(const vector<Point> &, Mat&);

        int main()


def drw_xspy():
	drw_xs(0,0,0,0,0)

def get_orientpy():
	get_orient(0,0)

def maincpy():

	main()
