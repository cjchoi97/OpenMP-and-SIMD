#include <cmath>
#include <cassert>
#include <random>
#include <fstream>


constexpr int N = 50'000;


int
main() {

    std::default_random_engine eng(12345);
    std::uniform_real_distribution<double> dist(0, 1);

    // Generate random line passing through origin, then generate point
    // uniformly along that line.
    {
        std::ofstream f("radially_uniform.data");
        assert(f);
        for (int i = 0; i < N; i++) {
            auto theta = 2*M_PI*dist(eng);
            auto r = 1 - 2*dist(eng);
            auto x = r*cos(theta);
            auto y = r*sin(theta);
            f << x << " " << y << std::endl;
        }
    }

    // Generate random x, then generate y uniformly in the correct range.
    {
        std::ofstream f("uniform_xy.data");
        for (int i = 0; i < N; i++) {
            auto x = 1 - 2*dist(eng);
            auto y = sqrt(1 - x*x) - 2*sqrt(1 - x*x)*dist(eng);
            f << x << " " << y << std::endl;
        }
    }

    // First generate theta, then generate r to match.
    {
        std::ofstream f("inv_cdf_r.data");
        assert(f);
        for (int i = 0; i < N; i++) {
            auto theta = 2*M_PI*dist(eng);
            auto r = std::sqrt(dist(eng));
            auto x = r*cos(theta);
            auto y = r*sin(theta);
            f << x << " " << y << std::endl;
        }
    }

    // Generate random x using inverse of cdf.  Approximate cdf using 10 terms
    // of series.  Then generate y uniformly in the correct range.
    {
        std::ofstream f("inv_cdf_x-10_terms.data");
        auto u_to_x = [](double u) {
            return (M_PI*(-0.5+u))/2.+(std::pow(M_PI,3)*std::pow(-0.5+u,3))/48.+
             (13*std::pow(M_PI,5)*std::pow(-0.5+u,5))/3840.+
             (493*std::pow(M_PI,7)*std::pow(-0.5+u,7))/645120.+
             (37369*std::pow(M_PI,9)*std::pow(-0.5+u,9))/1.8579456e8;
        };
        for (int i = 0; i < N; i++) {
            auto x = u_to_x(dist(eng));
            auto y = sqrt(1 - x*x) - 2*sqrt(1 - x*x)*dist(eng);
            f << x << " " << y << std::endl;
        }
    }

    // Generate random x using inverse of cdf.  Approximate cdf using 20 terms
    // of series.  Then generate y uniformly in the correct range.
    {
        std::ofstream f("inv_cdf_x-20_terms.data");
        auto u_to_x = [](double u) {
            return (M_PI*(-0.5+u))/2.+(std::pow(M_PI,3)*std::pow(-0.5+u,3))/48.+
             (13*std::pow(M_PI,5)*std::pow(-0.5+u,5))/3840.+
             (493*std::pow(M_PI,7)*std::pow(-0.5+u,7))/645120.+
             (37369*std::pow(M_PI,9)*std::pow(-0.5+u,9))/1.8579456e8+
             (4732249LL*std::pow(M_PI,11)*std::pow(-0.5+u,11))/8.17496064e10+
             (901188997LL*std::pow(M_PI,13)*std::pow(-0.5+u,13))/5.10117543936e13+
             (240798388357LL*std::pow(M_PI,15)*std::pow(-0.5+u,15))/4.2849873690624e16+
             (6611433892597LL*std::pow(M_PI,17)*std::pow(-0.5+u,17))/3.586204813492224e18+
             (39504564917358001LL*std::pow(M_PI,19)*std::pow(-0.5+u,19))/6.377706640314571e22;
        };
        for (int i = 0; i < N; i++) {
            auto x = u_to_x(dist(eng));
            auto y = sqrt(1 - x*x) - 2*sqrt(1 - x*x)*dist(eng);
            f << x << " " << y << std::endl;
        }
    }
    // Generate random x using inverse of cdf.  Invert numerically.  Then
    // generate y uniformly in the correct range.  Inverse is easy to find
    // because cdf is guaranteed to be increasing.
    {
        std::ofstream f("inv_cdf_x-numeric.data");

        auto u_to_x = [](const double u) {
            auto cdf = [](const double x) { return (x*std::sqrt(1.0 - x*x) + std::asin(x))/M_PI + .5; };
            double x_high = 1, x_low = -1, x = 0;
            // Do binary search.
            while (true) {
                double error = cdf(x) - u;
                if (std::fabs(error) < 1E-13) {
                    return x;
                } else if (error > 0) {
                    // Our guess was too high.
                    x_high = x;
                } else if (error < 0) {
                    // Our guess was too low.
                    x_low = x;
                }
                x = (x_low + x_high)/2.0;
            }
        };
        for (int i = 0; i < N; i++) {
            auto x = u_to_x(dist(eng));
            auto y = sqrt(1 - x*x) - 2*sqrt(1 - x*x)*dist(eng);
            f << x << " " << y << std::endl;
        }
    }
}
