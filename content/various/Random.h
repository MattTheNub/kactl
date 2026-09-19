/**
 * Author: Mateusz Klosin
 * Date: 2026-09-18
 * License: CC0
 * Description: Better + seeded RNG
 */


mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());