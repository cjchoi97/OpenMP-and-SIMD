#include <immintrin.h>
#include <cmath>
#include <functional>
#include <chrono>
#include <random>
#include <iostream>
#include <cassert>

const int N = 16*1'000'000;

double
time(const std::function<void ()> &f) {
    f(); // Run once to warmup.
    // Now time it for real.
    auto start = std::chrono::system_clock::now();
    f();
    auto stop = std::chrono::system_clock::now();
    return std::chrono::duration<double>(stop - start).count();
}

int
main() {

    alignas(32) static float a[N], b[N], c[N], d[N], w[N], x[N], y[N], z[N];

    /*
     * Generate data.
     */

    std::default_random_engine eng;
    std::uniform_real_distribution<float> dist(-1, 1);
    for (int i = 0; i < N; i++) {
        a[i] = dist(eng);
        b[i] = dist(eng);
        c[i] = dist(eng);
        d[i] = dist(eng);
        w[i] = dist(eng);
        x[i] = dist(eng);
        y[i] = dist(eng);
        z[i] = dist(eng);
    }

    /*
     * Sequential.
     */

    static float l_s[N];
    auto seq = [&]() {
        for (int i = 0; i < N; i++) {
            l_s[i] = std::sqrt(
              (a[i] - w[i]) * (a[i] - w[i]) +
              (b[i] - x[i]) * (b[i] - x[i]) +
              (c[i] - y[i]) * (c[i] - y[i]) +
              (d[i] - z[i]) * (d[i] - z[i]));
        }
    };

    std::cout << "Sequential: " << (N/time(seq))/1000000 << " Mops/s" << std::endl;

    alignas(32) static float l_v[N];
    auto vec = [&]() {
      for (int i = 0; i < N/8; i++) {
          __m256 mm_a = _mm256_load_ps(a + 8*i);
          __m256 mm_b = _mm256_load_ps(b + 8*i);
          __m256 mm_c = _mm256_load_ps(c + 8*i);
          __m256 mm_d = _mm256_load_ps(d + 8*i);

          __m256 mm_w = _mm256_load_ps(w + 8*i);
          __m256 mm_x = _mm256_load_ps(x + 8*i);
          __m256 mm_y = _mm256_load_ps(y + 8*i);
          __m256 mm_z = _mm256_load_ps(z + 8*i);

          __m256 mm_l = _mm256_sqrt_ps(
              _mm256_mul_ps(mm_a - mm_w, mm_a - mm_w) +
              _mm256_mul_ps(mm_b - mm_x, mm_b - mm_x) +
              _mm256_mul_ps(mm_c - mm_y, mm_c - mm_y) +
              _mm256_mul_ps(mm_d - mm_z, mm_d - mm_z));

          _mm256_store_ps(l_v + 8*i, mm_l);
      }
    };

    std::cout << "Parallel: " << (N/time(vec))/1000000 << " Mops/s" << std::endl;

    for (int i = 0; i < N; i++) {
        if (l_s[i] - l_v[i] != 0) {
            assert(false);
        }
    }
}
