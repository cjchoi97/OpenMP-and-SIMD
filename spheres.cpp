#include <cmath>
#include <cassert>
#include <random>
#include <fstream>
#include <assert.h>
#include <cmath>
#include <iomanip>
#include <iostream>


constexpr int N = 20'000;

const auto USAGE_STRING = "./exec.out print_bool num_points max_dimensions num_threads intervals\n"

std::vector<std::vector<int>> generate_histograms(int num_points, int max_dim, int num_threads, int intervals, bool parallel) {
  std::vector<std::vector<int>> hists;
  std::vector<int> histogram(intervals);

  std::vector<std::vector<float>> points(num_points, std::vector<float>(max_dim + 2));
  std::vector<float> distances(num_points);

  std::default_random_engine eng;
  std::uniform_real_distribution<double> dist(0, 1);

  for (int dims = 2 ; dims <= max_dim; dims++) {
    std::fill(distances.begin(), distances.end(), 0);

    for (std::size_t i = 0; i < points.size(); i++) {
      auto &pt = points[i];

      float normal = 0;
      for (int dim = 0; dim < dims + 2; dim++) {
        pt[dim] = dist(eng);
        normal += pt[dim] * pt[dim];
      }

      normal = std::sqrt(normal);

      for (int dim = 0; dim < dims + 2; dim++) pt[dim] /= normal;

      for (int dim = 0; dim < dims; dim++) distances[i] += pt[dim] * pt[dim];

      distances[i] = std::sqrt(distances[i]);
    }

    std::fill(histogram.begin(), histogram.end(), 0);

    for (size_t i = 0; i < distances.size(); i++) histogram[(int) (distances[i] * histogram.size())] += 1;
    hists.push_back(histogram);
  }
  return hists;
}


int
main() {

  if (argc != 6) {
    std::cerr << "\nExecution format: " << USAGE_STRING
      << std::endl << std::endl;
    exit(1);
  }

  bool print = std::string(argv[1]) == "true";
  int num_points = std::stoi(argv[2]); assert(num_points > 0);
  int max_dim = std::stoi(argv[3]); assert(max_dim >= 2);
  int num_threads = std::stoi(argv[4]);
  int intervals = std::stoi(argv[5]);

  std::cout.precision(8);
  std::cout << "\n\t\tDuration(s)\tMops/s" << std::endl;

  generate_histograms(num_points, max_dim, num_threads, intervals, true);

  // Generate random line passing through origin, then generate point
  // uniformly along that line.
  /*
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
  */
}
