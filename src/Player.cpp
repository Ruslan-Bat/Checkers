#include "Player.h"
#include <bits/stdc++.h>

using namespace std;

// ----------------------------
// Гиперпараметры (как в Python)
// ----------------------------
constexpr int STATE_CELLS = 64;
constexpr int STATE_CHANNELS = 6;
constexpr int STATE_DIM = STATE_CELLS * STATE_CHANNELS; // 384
constexpr int ACTION_DIM = 4;
constexpr int INPUT_DIM = STATE_DIM + ACTION_DIM;       // 388
constexpr int H1 = 128;
constexpr int H2 = 64;

// ----------------------------
// Утилиты: индексы/координаты
// ----------------------------
inline pair<int,int> idx_to_rc(int idx) {
    return { idx / 8, idx % 8 };
}
inline int rc_to_idx(int r, int c) {
    return r * 8 + c;
}
inline bool on_board(int r, int c) { return r >= 0 && r < 8 && c >= 0 && c < 8; }
inline bool is_dark(int r, int c) { return (r + c) % 2 == 1; }

// ----------------------------
// one-hot кодирование доски
// board: vector<int> size 64, значения 0..5 (как в твоём Python)
// возвращает vector<float> длины 384
// ----------------------------
vector<float> board_to_onehot(const vector<int>& board) {
    vector<float> out(STATE_DIM, 0.0f);
    for (int i = 0; i < 64; ++i) {
        int v = board[i];
        if (v >= 0 && v < STATE_CHANNELS) {
            out[i * STATE_CHANNELS + v] = 1.0f;
        }
    }
    return out;
}

// ----------------------------
// Нормализация действия: from_r,from_c,to_r,to_c -> /7
// ----------------------------
array<float, ACTION_DIM> action_to_vec_norm(int from_idx, int to_idx) {
    auto [fr, fc] = idx_to_rc(from_idx);
    auto [tr, tc] = idx_to_rc(to_idx);
    return { fr / 7.0f, fc / 7.0f, tr / 7.0f, tc / 7.0f };
}

// ----------------------------
// Загрузка весовых матриц и векторов из текстовых файлов
// ----------------------------
vector<vector<float>> load_matrix_txt(const string& path, int rows, int cols) {
    ifstream f(path);
    if (!f) {
        cerr << "ERROR: cannot open matrix file: " << path << '\n';
        exit(1);
    }
    vector<vector<float>> M(rows, vector<float>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            f >> M[i][j];
    return M;
}

vector<float> load_vector_txt(const string& path, int size) {
    ifstream f(path);
    if (!f) {
        cerr << "ERROR: cannot open vector file: " << path << '\n';
        exit(1);
    }
    vector<float> v(size);
    for (int i = 0; i < size; ++i) f >> v[i];
    return v;
}

// ----------------------------
// Матричное умножение + добавление bias: y = W * x + b
// ----------------------------
vector<float> matmul_add(const vector<vector<float>>& W, const vector<float>& x, const vector<float>& b) {
    int rows = (int)W.size();
    int cols = (int)W[0].size();
    vector<float> y(rows, 0.0f);
    for (int i = 0; i < rows; ++i) {
        float sum = 0.0f;
        const auto &Wi = W[i];
        for (int j = 0; j < cols; ++j) sum += Wi[j] * x[j];
        sum += b[i];
        y[i] = sum;
    }
    return y;
}

// ----------------------------
// Активации
// ----------------------------
inline void relu_inplace(vector<float>& v) {
    for (auto &x : v) if (x < 0.0f) x = 0.0f;
}
inline void leaky_relu_inplace(vector<float>& v, float alpha = 0.01f) {
    for (auto &x : v) if (x < 0.0f) x = alpha * x;
}

// ----------------------------
// Q network 
// ----------------------------
float forward_Q(
    const vector<float>& state_action,
    const vector<vector<float>>& w1, const vector<float>& b1,
    const vector<vector<float>>& w2, const vector<float>& b2,
    const vector<vector<float>>& w3, const vector<float>& b3
) {
    vector<float> h1 = matmul_add(w1, state_action, b1);
    relu_inplace(h1);
    vector<float> h2 = matmul_add(w2, h1, b2);
    leaky_relu_inplace(h2, 0.01f);
    vector<float> out = matmul_add(w3, h2, b3);
    return out[0];
}

// ----------------------------
// Преобразование state + действие -> входной вектор
// ----------------------------
vector<float> build_state_action_input(const vector<int>& board, int from_idx, int to_idx) {
    vector<float> s_onehot = board_to_onehot(board); // 384
    array<float, ACTION_DIM> a = action_to_vec_norm(from_idx, to_idx);
    vector<float> in(INPUT_DIM);
    copy(s_onehot.begin(), s_onehot.end(), in.begin());
    copy(a.begin(), a.end(), in.begin() + STATE_DIM);
    return in;
}

// ----------------------------
// Поиск всех простых ходов для одной позиции
// ----------------------------
vector<vector<int>> find_simple_moves_for_piece(const vector<int>& board, int idx, const string& color) {
    vector<vector<int>> moves;
    auto [r, c] = idx_to_rc(idx);
    int piece = board[idx];
    bool is_king = (piece == 3 || piece == 5);
    vector<pair<int,int>> dirs;
    if (is_king) dirs = { {-1,-1},{-1,1},{1,-1},{1,1} };
    else if (color == "black") dirs = { {1,-1}, {1,1} };
    else dirs = { {-1,-1}, {-1,1} };

    for (auto [dr, dc] : dirs) {
        int nr = r + dr, nc = c + dc;
        if (!on_board(nr, nc) || !is_dark(nr, nc)) continue;
        int nidx = rc_to_idx(nr, nc);
        if (board[nidx] == 1) {
            moves.push_back({ idx, nidx });
        }
        if (is_king) {
            int tr = nr + dr, tc = nc + dc;
            while (on_board(tr, tc) && is_dark(tr, tc)) {
                int tidx = rc_to_idx(tr, tc);
                if (board[tidx] != 1) break;
                moves.push_back({ idx, tidx });
                tr += dr; tc += dc;
            }
        }
    }
    return moves;
}

// ----------------------------
// Рекурсивный поиск capture для одной фигуры
// ----------------------------
void find_captures_for_piece_dfs(
    const vector<int>& board, int start_idx, int pos_idx, vector<int>& captured_so_far,
    vector<tuple<int,int,vector<int>>>& results, const string& color
) {
    bool found_any = false;
    int my_piece = (color == "black") ? 2 : 4;
    int my_king = (color == "black") ? 3 : 5;
    int enemy1 = (color == "black") ? 4 : 2;
    int enemy2 = (color == "black") ? 5 : 3;
    vector<pair<int,int>> dirs = { {-1,-1},{-1,1},{1,-1},{1,1} };
    int curr_piece = board[pos_idx];

    // Дамка: долгий поиск вдоль диагоналей
    if (curr_piece == my_king) {
        for (auto [dr, dc] : dirs) {
            int nr = idx_to_rc(pos_idx).first + dr, nc = idx_to_rc(pos_idx).second + dc;
            int enemy_idx = -1;
            while (on_board(nr, nc) && is_dark(nr, nc)) {
                int nidx = rc_to_idx(nr, nc);
                if (board[nidx] == 1) { nr += dr; nc += dc; continue; }
                if (board[nidx] == enemy1 || board[nidx] == enemy2) {
                    enemy_idx = nidx;
                    int tr = nr + dr, tc = nc + dc;
                    if (!(on_board(tr, tc) && is_dark(tr, tc) && board[rc_to_idx(tr,tc)] == 1)) break;
                    while (on_board(tr, tc) && is_dark(tr, tc) && board[rc_to_idx(tr,tc)] == 1) {
                        int land_idx = rc_to_idx(tr, tc);
                        vector<int> nb = board;
                        nb[land_idx] = nb[pos_idx];
                        nb[pos_idx] = 1;
                        nb[enemy_idx] = 1;
                        vector<int> new_captured = captured_so_far;
                        new_captured.push_back(enemy_idx);
                        find_captures_for_piece_dfs(nb, start_idx, land_idx, new_captured, results, color);
                        found_any = true;
                        tr += dr; tc += dc;
                    }
                    break;
                }
                break;
            }
        }
    }

    // Обычная шашка: проверяем соседнюю клетку enemy и пустую за ним
    if (curr_piece == my_piece) {
        for (auto [dr, dc] : dirs) {
            int midr = idx_to_rc(pos_idx).first + dr, midc = idx_to_rc(pos_idx).second + dc;
            int landr = idx_to_rc(pos_idx).first + 2*dr, landc = idx_to_rc(pos_idx).second + 2*dc;
            if (!on_board(midr, midc) || !on_board(landr, landc) || !is_dark(midr, midc) || !is_dark(landr, landc)) continue;
            int mididx = rc_to_idx(midr, midc), landidx = rc_to_idx(landr, landc);
            if ((board[mididx] == enemy1 || board[mididx] == enemy2) && board[landidx] == 1) {
                vector<int> nb = board;
                nb[landidx] = nb[pos_idx];
                nb[pos_idx] = 1;
                nb[mididx] = 1;
                // promotion
                if (color == "black" && nb[landidx] == my_piece && landr == 7) nb[landidx] = my_king;
                if (color == "white" && nb[landidx] == my_piece && landr == 0) nb[landidx] = my_king;
                vector<int> new_captured = captured_so_far;
                new_captured.push_back(mididx);
                find_captures_for_piece_dfs(nb, start_idx, landidx, new_captured, results, color);
                found_any = true;
            }
        }
    }

    // Если ничего не найдено на этом узле рекурсии, и у нас есть хотя бы одно захваченное — то мы достигли листа: сохраняем результат
    if (!found_any && !captured_so_far.empty()) {
        results.emplace_back(start_idx, pos_idx, captured_so_far);
    }
}

// ----------------------------
// Внешняя обёртка для поиска взятий
// ----------------------------
vector<tuple<int,int,vector<int>>> find_captures_for_piece(const vector<int>& board, int idx, const string& color) {
    vector<tuple<int,int,vector<int>>> results;
    vector<int> captured;
    find_captures_for_piece_dfs(board, idx, idx, captured, results, color);
    // Уникализируем по (start,end,sorted(caps))
    vector<tuple<int,int,vector<int>>> unique;
    set<string> seen;
    for (auto &t : results) {
        sort(get<2>(t).begin(), get<2>(t).end());
        string key = to_string(get<0>(t)) + "_" + to_string(get<1>(t));
        for (int x : get<2>(t)) key += "_" + to_string(x);
        if (seen.insert(key).second) unique.push_back(t);
    }
    return unique;
}

// ----------------------------
// Получение всех валидных действий для цвета
// ----------------------------
vector<vector<int>> get_all_valid_actions_for_color(const vector<int>& board, const string& color) {
    vector<vector<int>> captures_all;
    // собираем все захваты
    for (int i = 0; i < 64; ++i) {
        int cell = board[i];
        bool is_my_piece = (color == "black" && (cell == 2 || cell == 3)) || (color == "white" && (cell == 4 || cell == 5));
        if (is_my_piece) {
            auto caps = find_captures_for_piece(board, i, color);
            for (auto &t : caps) {
                vector<int> out = { get<0>(t) };
                out.insert(out.end(), get<2>(t).begin(), get<2>(t).end());
                out.push_back(get<1>(t));
                captures_all.push_back(out);
            }
        }
    }
    if (!captures_all.empty()) return captures_all;

    // иначе — простые ходы
    vector<vector<int>> moves_all;
    for (int i = 0; i < 64; ++i) {
        int cell = board[i];
        bool is_my_piece = (color == "black" && (cell == 2 || cell == 3)) || (color == "white" && (cell == 4 || cell == 5));
        if (is_my_piece) {
            auto mv = find_simple_moves_for_piece(board, i, color);
            moves_all.insert(moves_all.end(), mv.begin(), mv.end());
        }
    }
    return moves_all;
}

// ----------------------------
// Выбор лучшего действия по Q-сети
// ----------------------------
vector<int> select_best_action_by_Q(
    const vector<int>& board, const vector<vector<int>>& actions,
    const vector<vector<float>>& w1, const vector<float>& b1,
    const vector<vector<float>>& w2, const vector<float>& b2,
    const vector<vector<float>>& w3, const vector<float>& b3
) {
    if (actions.empty()) return {};

    float best_q = -numeric_limits<float>::infinity();
    vector<int> best_action;

    for (const auto &act : actions) {
        if (act.size() < 2) continue;
        vector<float> phi = build_state_action_input(board, act.front(), act.back());
        float q = forward_Q(phi, w1, b1, w2, b2, w3, b3);
        if (q > best_q) {
            best_q = q;
            best_action = act;
        }
    }
    return best_action;
}

// ----------------------------
// Главная AI функция
// ----------------------------
vector<int> best_action(const vector<int>& board){
    // Загрузка весов (предполагается текстовый формат)
    auto w1 = load_matrix_txt("w1.txt", H1, INPUT_DIM);
    auto b1 = load_vector_txt("b1.txt", H1);
    auto w2 = load_matrix_txt("w2.txt", H2, H1);
    auto b2 = load_vector_txt("b2.txt", H2);
    auto w3 = load_matrix_txt("w3.txt", 1, H2);
    auto b3 = load_vector_txt("b3.txt", 1);

    auto actions = get_all_valid_actions_for_color(board, "black");
    auto best = select_best_action_by_Q(board, actions, w1,b1,w2,b2,w3,b3);
    return best;
}

// =================================================================================
// ИНТЕГРАЦИЯ С ИГРОЙ
// =================================================================================

std::vector<int> Player::getAIMove(const Board& board) {
    // 1. Получаем доску в формате, понятном для AI
    const std::vector<int>& board_vec = board.getCells();
    
    // 2. Вызываем основную логику AI
    std::vector<int> ai_move = best_action(board_vec);

    if (ai_move.empty()) {
        return {};
    }

    // 3. Конвертируем ход из формата AI (индексы 0-63) в формат игры (координаты X,Y)
    // Формат AI: [start_idx, cap1_idx, cap2_idx, ..., end_idx]
    // Формат игры: [fromX, fromY, cap1X, cap1Y, ..., toX, toY]
    std::vector<int> game_move;
    
    if (ai_move.size() < 2) return {}; // Некорректный ход

    // Начальная позиция
    int start_idx = ai_move.front();
    game_move.push_back(start_idx % Board::SIZE); // fromX
    game_move.push_back(start_idx / Board::SIZE); // fromY
    
    // Позиции съеденных шашек (если есть)
    for (size_t i = 1; i < ai_move.size() - 1; ++i) {
        int captured_idx = ai_move[i];
        game_move.push_back(captured_idx % Board::SIZE); // capturedX
        game_move.push_back(captured_idx / Board::SIZE); // capturedY
    }
    
    // Конечная позиция
    int end_idx = ai_move.back();
    game_move.push_back(end_idx % Board::SIZE); // toX
    game_move.push_back(end_idx / Board::SIZE); // toY

    return game_move;
}