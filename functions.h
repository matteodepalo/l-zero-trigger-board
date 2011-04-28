// Bisection method to find the root of the equation f(x) = mean , where f(x) is the linear interpolation of the graph in input
// epsilon is the tollerance and left and right are the starting points for the method

Float_t Bisection(Float_t left,Float_t right,Float_t epsilon, TGraphErrors *g, Float_t mean) {
	Float_t midpoint;
	do {
		midpoint = (right + left) / 2;
		if(((g->Eval(left) - mean) * (g->Eval(midpoint) - mean)) < 0)
			right = midpoint;
		else if(((g->Eval(right) - mean) * (g->Eval(midpoint) - mean)) < 0)
			left = midpoint;
		else return midpoint;
	} while (fabs(right - left) > 2*epsilon);
	return midpoint;
}