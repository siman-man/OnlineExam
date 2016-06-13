#include <iomanip>
#include <iostream>
#include <random>
#include <string.h>
#include <queue>
#include <cassert>

using namespace std;

const int N = 5000;
const int K = 2000;
const int X =  100;

unsigned long long xor128(){
  static unsigned long long rx=123456789, ry=362436069, rz=521288629, rw=88675123;
  unsigned long long rt = (rx ^ (rx<<11));
  rx=ry; ry=rz; rz=rw;
  return (rw=(rw^(rw>>19))^(rt^(rt>>8)));
}

int g_answer[N];
bool g_commit[N];
int g_bestAnswer[N];
int g_definiteValue[N];
int g_maxScore;
const int FIRST_TRY_COUNT = 5;

class OnlineExam {
  public:
    void init() {
      memset(g_answer, 0, sizeof(g_answer));
      memset(g_commit, false, sizeof(g_commit));
      memset(g_definiteValue, -1, sizeof(g_definiteValue));
      memcpy(g_bestAnswer, g_answer, sizeof(g_answer));
      g_maxScore = 0;

      for (int i = 0; i < FIRST_TRY_COUNT; i++) {
        createRandomAnswer();

        string answer = answer2string();
        int score = sendAnswer(answer);

        if (g_maxScore < score) {
          g_maxScore = score;
          memcpy(g_bestAnswer, g_answer, sizeof(g_answer));
        } else {
          rollback();
        }
      }

      fprintf(stderr,"First Score = %d\n", g_maxScore);
    }

    void run() {
      for (int turn = 0; turn < X-FIRST_TRY_COUNT; turn++) {
        commit(g_maxScore-1);
        updateAnswer(turn);
      }
    }

    void updateAnswer(int turn) {
      int index = turn*42;

      flipValue(index, index+42);

      string answer = answer2string();
      int score = sendAnswer(answer);

      if (g_maxScore < score) {
        g_maxScore = score;
        memcpy(g_bestAnswer, g_answer, sizeof(g_answer));
      } else {
        commit(score-1);
        rollback();
      }

      fprintf(stderr,"turn %d: sc = %d, max sc = %d\n", turn, score, g_maxScore);
    }

    void commit(int index) {
      if (!g_commit[index]) {
        g_answer[index] ^= 1;
        g_definiteValue[index] = g_answer[index];
        g_commit[index] = true;
      }
    }

    void flipValue(int from, int to) {
      for (int i = from; i < to; i++) {
        if (g_commit[i]) continue;
        g_answer[i] ^= 1;
      }
    }

    void rollback() {
      for (int i = 0; i < N; i++) {
        if (g_commit[i]) continue;
        g_answer[i] = g_bestAnswer[i];
      }
    }

    int sendAnswer(string answer) {
      cout << answer << endl;
      int res;
      cin >> res;

      return res;
    }

    string answer2string() {
      string answer = "";

      for (int i = 0; i < N; i++) {
        answer += '0' + g_answer[i];
      }

      return answer;
    }

    void createRandomAnswer() {
      for (int i = 0; i < N; i++) {
        if (g_commit[i]) continue;
        g_answer[i] = xor128()%2;
      }
    }
};

int main() {
  OnlineExam oe;
  oe.init();
  oe.run();
  return 0;
}
