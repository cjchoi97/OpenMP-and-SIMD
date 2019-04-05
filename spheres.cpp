#include <cmath>
#include <cassert>
#include <random>
#include <fstream>


constexpr int N = 20'000;


int
main() {

    std::default_random_engine eng;
    std::uniform_real_distribution<double> dist(0, 1);

    // Generate random line passing through origin, then generate point
    // uniformly along that line.
    {
        std::ofstream f("nonuniform_sphere.csv");
        assert(f);
        f << "x,y,z" << std::endl;
        for (int i = 0; i < N; i++) {
            auto theta = 2*M_PI*dist(eng);
            auto phi = M_PI*dist(eng);
            auto r = dist(eng);
            double x = r*std::sin(phi)*std::cos(theta);
            double y = r*std::sin(phi)*std::sin(theta);
            double z = r*std::cos(phi);
            f << x << "," << y << "," << z << std::endl;
        }
    }
}
