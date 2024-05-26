#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>
using namespace std;

#define file "test"

/*
    Cấu hình đầu vào của code SA
    [số lượng bộ test q]:
        [số người n] [số nhóm t] [số người bệnh d]
        [ma trận M]
        [vector kết quả y]
        [vector x mong muốn]
*/

int n, t, d;
vector<vector<int>> M;
vector<int> y, TARGET;
mt19937 generator(random_device{}());

int cnt = 0;

// Hàm trả về tỉ lệ ngẫu nhiên trong khoảng từ 0.0 đến 1.0.
double random_rate()
{
    uniform_real_distribution<double> distReal(0.0, 1.0);
    return distReal(generator);
}

// Hàm xác định tính đúng đẵn của x hiện tại so với bộ test có kết quả là 0.
bool case0(const vector<int> &solution, int test)
{
    for (int i = 0; i < n; i++)
        if (M[test][i] == 1 && solution[i] == 1)
            return false;
    return true;
}

// Hàm xác định tính đúng đẵn của x hiện tại so với bộ test có kết quả là 1.
bool case1(const vector<int> &solution, int test)
{
    for (int i = 0; i < n; i++)
        if (M[test][i] == 1 && solution[i] == 1)
            return true;
    return false;
}

// Hàm tính toán số lượng bộ test đúng tương ứng với x hiện tại.
int objective_function(vector<int> &solution)
{
    int count = 0;
    for (int test = 0; test < t; test++)
        if (y[test] == 1)
            count += case1(solution, test);
        else
            count += case0(solution, test);
    return count;
}

// Hàm thay đổi ngẫu nhiên d phần tử trong x.
vector<int> mutate(vector<int> solution)
{
    uniform_int_distribution<int> distInt(0, n - 1);
    unordered_set<int> pos_list;
    while (pos_list.size() < d)
        pos_list.insert(distInt(generator));

    for (int i : pos_list)
        solution[i] = 1 - solution[i];
    return solution;
}

// Tính toán xác suất chấp nhận giải pháp mới dựa trên nhiệt độ hiện tại.
double acceptance_probability(double new_objective, double best_objective, double T)
{
    return exp(-(new_objective - best_objective) / T);
}

// Thực hiện thuật toán Simulated Annealing (SA).
vector<int> simulated_annealing(vector<int> init_solution, double T, double T_min, double cooling_rate, int iterations)
{
    // Khởi tạo giải pháp tốt nhất là giải pháp ban đầu.
    vector<int> best_solution = init_solution;
    int best_objective = objective_function(best_solution);

    // Thực hiện giải thuật để tính toán và tìm ra giải pháp tối ưu trong khi T > T_min.
    while (T > T_min)
    {
        for (int i = 0; i < iterations; i++)
        {
            // Tạo một giải pháp mới bằng cách thay đổi ngẫu nhiên d phần tử trong x.
            vector<int> new_solution = mutate(init_solution);
            // Tính toán số lượng bộ test đúng của giải pháp mới.
            int new_objective = objective_function(new_solution);

            // Chấp nhận giải pháp mới nếu nó tốt hơn giải pháp hiện tại hoặc với xác suất dựa trên nhiệt độ hiện tại.
            if (new_objective > best_objective || random_rate() > acceptance_probability(new_objective, best_objective, T))
            {
                best_solution = new_solution;
                best_objective = new_objective;
            }
        }
        // Hạ nhiệt độ.
        T *= cooling_rate;
    }

    // Trả về kết quả tối ưu nhất.
    return best_solution;
}

void solve_problem(int test_case)
{
    // Nhập input.
    cin >> n >> t >> d;
    M.resize(t);
    for (int i = 0; i < t; i++)
    {
        M[i].resize(n, 0);
        for (int j = 0; j < n; j++)
            cin >> M[i][j];
    }
    y.resize(t, 0);
    for (int i = 0; i < t; i++)
        cin >> y[i];
    TARGET.resize(n, 0);
    for (int i = 0; i < n; i++)
        cin >> TARGET[i];

    // Khởi tạo x ban đầu.
    vector<int> init_x;
    init_x.resize(M[0].size(), 0);

    // Khởi tạo các tham số cần thiết cho thuật toán SA.
    double T = 100.00;
    double T_min = 0.01;
    double cooling_rate = 0.99;
    int iterations = 150;

    // Thực hiện thuật toán SA.
    vector<int> x = simulated_annealing(init_x, T, T_min, cooling_rate, iterations);

    cout << "TEST #" << test_case << ": ";

    // In ra x.
    // cout << "x = ";
    // for (int bit : x)
    //     cout << bit << ' ';
    // cout << '\n';
    // // cout << "TARGET = ";
    // for (int bit : TARGET)
    //     cout << bit << ' ';
    // cout << '\n';

    // In ra kết quả.
    if (x == TARGET)
    {
        cnt++;
        cout << "[AC]\n";
    }
    else
        cout << "[WA]\n";
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
    int test_case = 1;
    cin >> test_case;
    for (int i = 1; i <= test_case; i++)
        solve_problem(i);
    cout << "Accepted: " << cnt << "/" << test_case << '\n';
}
