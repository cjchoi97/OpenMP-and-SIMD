#include <cmath>
#include <cassert>
#include <random>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <iostream>

const auto USAGE_STRING = "./exec.out print_bool num_points max_dimensions num_threads intervals\n"

std::vector<std::vector<int>> generate_histograms(int num_points, int max_dimensions, int num_threads, int intervals, bool parallel) {
  std::vector<std::vector<int>> hists;
  std::vector<int> histogram(intervals);
  std::vector<std::vector<float>> points(num_points, std::vector<float>(max_dimensions + 2));
  std::vector<float> distances(num_points);

  std::default_random_engine eng;
  std::uniform_real_distribution<double> dist(0, 1);

  for (int dims = 2 ; dims <= max_dimensions; dims++) {
    std::fill(distances.begin(), distances.end(), 0);

    for (std::size_t i = 0; i < points.size(); i++) {
      auto &point = points[i];

      float normal = 0;
      for (int dim = 0; dim < dims + 2; dim++) {
        point[dim] = dist(eng);
        normal += point[dim] * point[dim];
      }

      normal = std::sqrt(normal);

      for (int dim = 0; dim < dims + 2; dim++) point[dim] /= normal;

      for (int dim = 0; dim < dims; dim++) distances[i] += point[dim] * point[dim];

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
  int num_points = std::stoi(argv[2]);
  int max_dimensions = std::stoi(argv[3]);
  int num_threads = std::stoi(argv[4]);
  int intervals = std::stoi(argv[5]);

  std::cout.precision(8);
  std::cout << "\n\t\tDuration(s)\tMops/s" << std::endl;
  generate_histograms(num_points, max_dimensions, num_threads, intervals, true);

  auto start = std::chrono::system_clock::now();
  auto histograms = generate_histograms(n_points, max_dimensions, num_threads, hist_buckets, false);
  auto stop = std::chrono::system_clock::now();
  auto duration = std::chrono::duration<double>(stop - start).count();
  std::cout << "Time:\t\t" << duration << "\t" << n_points / duration / 1'000'000 << std::endl;

  if (print) {
    for (int dims = 0; dims < hists.size(); dims++) {
      std::cout << "\n\t" << dims + MIN_DIM << "-D" << std::endl;
      std::cout << "Histogram with " << hists[dims].size() << " buckets" << std::endl;
      std::cout << "Interval\t\tPercentage\tCount" << std::endl;
      for (size_t i = 0; i < hists[dims].size(); i++) {
        std::cout.precision(2);
        std::cout << std::fixed << ((float) i) / hists[dims].size() << "\t\t"
          << (int)(100.0 * hists[dims][i] / num_points) << "%\t\t" << hists[dims][i] << std::endl;
      }
    }
    std::cout << std::endl;
  }
}
