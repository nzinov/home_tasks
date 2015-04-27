#include <vector>
#include <string>
#include <iostream>

using namespace std;

const size_t INF = 1000000000;

int main() {
    string a, b;
    cin >> a;
    cin >> b;
    size_t n = max(a.length(), b.length()) + 1;
    vector<vector<size_t> > ih(n, vector<size_t>(n));
    vector<vector<size_t> > iv(n, vector<size_t>(n));
    vector<size_t> VG(n);
    vector<size_t> DG0(n);
    for (size_t j = 0; j <= b.length(); ++j) {
        ih[0][j] = j;
    }
    for (size_t l = 0; l <= a.length(); ++l) {
        iv[l][0] = 0;
    }
    for (size_t l = 1; l <= a.length(); ++l) {
        for (size_t j = 1; j <= b.length(); ++j) {
            if (a[l] != b[j]) {
                ih[l][j] = max(iv[l][j-1], ih[l-1][j]);
                iv[l][j] = min(iv[l][j-1], ih[l-1][j]);
            } else {
                ih[l][j] = iv[l][j-1];
                iv[l][j] = ih[l-1][j];
            }
        }
    }
    for (size_t j = 1; j <= b.length(); ++j) {
        VG[j] = INF;
    }
    DG0[0] = 0;
    size_t i = 1;
    for (size_t j = 1; j <= b.length(); ++j) {
        if (ih[a.length()][j] == 0) {
            DG0[i] = j;
            ++i;
        } else {
            VG[ih[a.length()][j]] = j;
        }
    }
    for (size_t l = i; l <= a.length(); ++l) {
        DG0[l] = INF;
    }
    for (size_t el : DG0) {
        cout << el << " ";
    }
    cout << endl;
    for (size_t el : VG) {
        cout << el << " ";
    }
    cout << endl;
    vector<vector<size_t> > D(n, vector<size_t>(n));
    std::copy(DG0.begin(), DG0.end(), D[0].begin());
    for (size_t i = 1; i < n; ++i) {
        size_t k = 1;
        for (size_t j = 0; j < n; ++j) {
            size_t cur = (k >=n ? INF : D[i - 1][k]);
            if (VG[i] < cur) {
                D[i][j] = VG[i];
                VG[i] = INF;
            } else {
                D[i][j] = cur;
                ++k;
            }
        }
    }
    for (size_t i = 0; i <= a.length(); ++i) {
        for (size_t j = 0; j <= b.length(); ++j) {
            cout << D[i][j] << " ";
        }
        cout << endl;
    }
}
