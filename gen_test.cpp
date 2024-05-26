#include <iostream>
#include <vector>
#include <chrono>
#include <random>
using namespace std;

#define file "test"
#define el '\n'

/*
    Cấu hình đầu vào của code sinh test
    [q test] [số lượng người ít nhất] [số lượng người nhiều nhất]
*/

/*
    Cấu hình đầu ra của code sinh test
    [q test]:
        [số người] [số nhóm] [số người bệnh]
        [ma trận]
        [kết quả của các nhóm test]
        [kết quả thực tế]
*/

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// kiểm tra các tập cột con có tồn tại một hàng mà có duy nhất 1 mẫu hay không bằng đệ quy
bool find_error_in_subsets(const vector<vector<int>> &M, int k, int idx, int sta, vector<vector<int>> &T)
{
    if (idx >= k)
    {
        // kiểm tra có tồn tại hàng có duy nhất một mẫu
        for (int i = 0; i < T.size(); i++)
        {
            if (accumulate(T[i].begin(), T[i].end(), 0) == 1)
            {
                return true;
            }
        }
        return false;
    }

    if (sta >= M[0].size())
        return true;

    for (int i = sta; i < M[0].size(); i++)
    {
        for (int j = 0; j < M.size(); j++)
            T[j][idx] = M[j][i];
        if (!find_error_in_subsets(M, k, idx + 1, i + 1, T))
        {
            return false;
        }
    }

    return true;
}

// kiểm tra tính đúng đắn của ma trận
bool check_k_disjunct_matrix(const vector<vector<int>> &M, int k)
{
    // kiểm tra mỗi hàng có đủ k + 1 mẫu được chọn không
    for (int i = 0; i < M.size(); i++)
    {
        if (accumulate(M[i].begin(), M[i].end(), 0) < k + 1)
        {
            return false;
        }
    }

    // kiểm tra các tập cột con có tồn tại một hàng mà có duy nhất 1 mẫu hay không
    vector<vector<int>> T(M.size(), vector<int>(k));
    return find_error_in_subsets(M, k, 0, 0, T);
}

// tạo ma trận ngẫu nhiên
vector<vector<int>> disjunct_matrix(int column, int row, int k)
{
    vector<vector<int>> M(row, vector<int>(column));
    int pen = 0;

    do
    {
        if (pen++ == 50)
        {
            M[0][0] = -1;
            return M;
        }

        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                M[i][j] = (rng() % (k + 2)) == 1;
            }
        }
    } while (!check_k_disjunct_matrix(M, k));

    return M;
}

// tìm kiếm nhị phân để tìm ra số lượng nhóm tối ưu nhất
vector<vector<int>> optimal_disjunct_matrix(int n, int k)
{
    int l = n * 3 / 4, r = n;
    vector<vector<int>> res = disjunct_matrix(n, n, k), tmp;

    while (l <= r)
    {
        int mid = (l + r) / 2;
        tmp = disjunct_matrix(n, mid, k);

        if (tmp[0][0] != -1)
        {
            res = tmp;
            r = mid - 1;
        }
        else
        {
            l = mid + 1;
        }
    }

    return res;
}

// tạo kết quả của các nhóm test
vector<int> check_sample(const vector<int> &ans, const vector<vector<int>> &M)
{
    vector<int> sample;
    for (int i = 0; i < M.size(); i++)
    {
        bool check = 0;
        for (int j = 0; j < M[0].size(); j++)
        {
            if (M[i][j])
            {
                check |= ans[j];
            }
        }
        sample.push_back(check);
    }
    return sample;
}

// tạo cấu hình ban đầu
vector<int> make_target(int mi, int ma)
{
    int n = mi + rng() % (ma - mi + 1);
    int k = rng() % (int)(log10(n) / log10(3)) + 1;
    vector<int> tar(n);

    do
    {
        for (int i = 0; i < n; i++)
            tar[i] = rng() % 2;
    } while (accumulate(tar.begin(), tar.end(), 0) != k);

    return tar;
}

void solve(int left, int right)
{
    // cấu hình ban đầu
    vector<int> ans = make_target(left, right);

    // số người bệnh
    int object = accumulate(ans.begin(), ans.end(), 0);

    // ma trận nhị phân
    vector<vector<int>> group_test = optimal_disjunct_matrix(ans.size(), object);

    // kết quả của các nhóm test
    vector<int> test_results = check_sample(ans, group_test);

    // in testcase
    cout << ans.size() << ' ' << group_test.size() << ' ' << object << el;

    for (int i = 0; i < group_test.size(); i++)
    {
        for (auto x : group_test[i])
            cout << x << ' ';
        cout << el;
    }

    for (int x : test_results)
        cout << x << ' ';
    cout << el;

    for (int x : ans)
        cout << x << ' ';
    cout << el;
}

signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    if (fopen(file ".inp", "r"))
    {
        freopen(file ".inp", "r", stdin);
        freopen(file ".out", "w", stdout);
    }

    int q, l, r;
    cin >> q >> l >> r;
    if (l > r)
        swap(l, r);
    cout << q << el;

    while (q--)
        solve(l, r);
}
