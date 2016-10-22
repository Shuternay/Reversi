#include <iostream>
#include <array>
#include <cmath>
#include <vector>



constexpr int FIELD_SIZE = 8;
enum COLOR {
    me = 1, enemy = 2
};

struct Field {
    std::array<std::array<char, FIELD_SIZE>, FIELD_SIZE> field;

    Field() {
        for (int i = 0; i < FIELD_SIZE; ++i)
            for (int j = 0; j < FIELD_SIZE; ++j)
                field[i][j] = 0;
    }

    const char &get(size_t x, size_t y) const
    { return field.at(x).at(y); }

    char &get(size_t x, size_t y)
    { return field.at(x).at(y); }

    const char &getc(char x, int y) const
    { return field.at(x - 'a').at(y - 1); }

    char &getc(char x, int y)
    { return field.at(x - 'a').at(y - 1); }

    void applyMove(char x, int y, char who) {
        int dx[] = {1, 1, 0, -1, -1, -1, 0, 1};
        int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

        for (int i = 0; i < 8; ++i) {
            for (char d = 1;
                 'a' <= x + d * dx[i] &&
                 x + d * dx[i] <= 'h' &&
                 1 <= y + d * dy[i] &&
                 y + d * dy[i] <= 8;
                 ++d) {
                if (getc(x + d * dx[i], y + d * dy[i]) == 0)
                    break;
                if (getc(x + d * dx[i], y + d * dy[i]) == who) {
                    colorLine(x, y, x + d * dx[i], y + d * dy[i], who);
                    break;
                }
            }
        }
    }

    int countMove(char x, int y, char who) const {
        int dx[] = {1, 1, 0, -1, -1, -1, 0, 1};
        int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};
        int ans = 0;

        for (int i = 0; i < 8; ++i) {
            for (char d = 1;
                 'a' <= x + d * dx[i] &&
                 x + d * dx[i] <= 'h' &&
                 1 <= y + d * dy[i] &&
                 y + d * dy[i] <= 8;
                 ++d) {
                if (getc(x + d * dx[i], y + d * dy[i]) == 0)
                    break;
                if (getc(x + d * dx[i], y + d * dy[i]) == who) {
                    ans += d - 1;
                    break;
                }
            }
        }

        return ans;
    }

    void colorLine(char x1, int y1, char x2, int y2, char color) {
        int dx = sign(x2 - x1);
        int dy = sign(y2 - y1);

        for (int i = 0; x1 + i * dx != x2 || y1 + i * dy != y2; ++i) {
            getc(x1 + i * dx, y1 + i * dy) = color;
        }

        getc(x2, y2) = color;
    }

    template<typename T>
    int sign(T val) {
        return (T(0) < val) - (val < T(0));
    }
};

class Reversi {
public:
    Reversi(int color) {
        if (color == me) {
            field.getc('d', 4) = me;
            field.getc('e', 5) = me;
            field.getc('d', 5) = enemy;
            field.getc('e', 4) = enemy;
        }
        else {
            field.getc('d', 4) = enemy;
            field.getc('e', 5) = enemy;
            field.getc('d', 5) = me;
            field.getc('e', 4) = me;
        }
    }

    std::pair<char, int> doMove() {
        std::pair<std::pair<char, int>, int> move = countMinMaxWeight(field, me, 0, -200, 200);

        field.applyMove(move.first.first, move.first.second, me);

        return move.first;

//        int maxChanged = 0;
//        std::pair<char, int> maxChangedMove;

//        for (char x = 'a'; x <= 'h'; ++x)
//            for (int y = 1; y <= 8; ++y) {
//                if (field.getc(x, y) != 0)
//                    continue;
//
//                int changed = field.countMove(x, y, me);
//                if (changed > 0)
//                    moves.push_back({{x, y}, changed});
//
//                if (changed > maxChanged) {
//                    maxChanged = changed;
//                    maxChangedMove = {x, y};
//                }
//            }


//
//        if (maxChanged > 0) {
//            field.applyMove(maxChangedMove.first, maxChangedMove.second, me);
//        }
//
//        return maxChangedMove;
    }


    Field field;

private:
    int maxRecursionLevel = 4;

    int countNaiveWeight (const Field &f) const {
        int ans = 0;

        for (int i = 1; i < 7; ++i)
            for (int j = 1; j < 7; ++j)
                if (field.get(i, j)) {
                    if (field.get(i, j) == me)
                        ans += 1;
                    else
                        ans -= 1;
                }
        for (int i = 1; i < 7; ++i)
            if (field.get(i, 0)) {
                if (field.get(i, 0) == me)
                    ans += 2;
                else
                    ans -= 2;
            }
        for (int i = 1; i < 7; ++i)
            if (field.get(i, 7)) {
                if (field.get(i, 7) == me)
                    ans += 2;
                else
                    ans -= 2;
            }
        for (int i = 1; i < 7; ++i)
            if (field.get(0, i)) {
                if (field.get(0, i) == me)
                    ans += 2;
                else
                    ans -= 2;
            }
        for (int i = 1; i < 7; ++i)
            if (field.get(7, i)) {
                if (field.get(7, i) == me)
                    ans += 2;
                else
                    ans -= 2;
            }

        if (field.get(0, 0)) {
            if (field.get(0, 0) == me)
                ans += 4;
            else
                ans -= 4;
        }
        if (field.get(0, 7)) {
            if (field.get(0, 7) == me)
                ans += 4;
            else
                ans -= 4;
        }
        if (field.get(7, 0)) {
            if (field.get(7, 0) == me)
                ans += 4;
            else
                ans -= 4;
        }
        if (field.get(7, 7)) {
            if (field.get(7, 7) == me)
                ans += 4;
            else
                ans -= 4;
        }

        return ans;
    }


    std::pair<std::pair<char, int>, int>
    countMinMaxWeight(Field f, char player, int recursionLevel, int alpha, int beta) const {
        if (recursionLevel > maxRecursionLevel) {
            return {{0, 0}, countNaiveWeight(f)};
        }

        int minmaxWeight = player == me ? -200 : 200;
        std::pair<int, int> minmaxTurn = {0, 0};

        for (char x = 'a'; x <= 'h'; ++x)
            for (int y = 1; y <= 8; ++y) {
                if (f.countMove(x, y, player) <= 0)
                    continue;

                Field fieldCopy = f;
                f.applyMove(x, y, player);

                int curWeight = countMinMaxWeight(fieldCopy, 3 - player, recursionLevel + 1, alpha, beta).second;

                if (player == me && curWeight > minmaxWeight ||
                    curWeight < minmaxWeight) {
                    minmaxWeight = curWeight;
                    minmaxTurn = {x, y};
                }

                if (recursionLevel == 0 && curWeight == minmaxWeight && rand() % 4) {
                    minmaxTurn = {x, y};
                }

                if (player == me) {
                    alpha = std::max(alpha, minmaxWeight);
                }
                else {
                    beta = std::min(beta, minmaxWeight);
                }
                if (alpha > beta)
                    break;
            }

        if (std::abs(minmaxWeight) == 200)
            return {{0, 0}, countNaiveWeight(f)};

        return {minmaxTurn, minmaxWeight};
    }
};

void interact() {
    std::string s;
    std::cin >> s;
    std::cin >> s;
    Reversi reversi(s == "white" ? 1 : 2);

    while (true) {
        std::cin >> s;

        if (s == "turn") {
            std::pair<char, int> move = reversi.doMove();

            std::cout << "move " << move.first << " " << move.second << "\n";
            std::cerr << "move " << move.first << " " << move.second << "\n";
            std::cout.flush();
        } else if (s == "move") {
            char x;
            int y;
            std::cin >> x >> y;
            reversi.field.applyMove(x, y, enemy);
        } else if (s == "bad" || s == "win" || s == "lose") {
            return;
        }
    }
}

int main() {
    srand(10010110);
    interact();
    return 0;
}