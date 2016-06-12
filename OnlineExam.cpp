#include <iomanip>
#include <iostream>
#include <random>
#include <string.h>
#include <sstream>

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

string int2string(int number){
  stringstream ss; 
  ss << number;
  return ss.str();
}

int g_answer[N];
int g_tempAnswer[N];
int g_maxScore;

class OnlineExam {
  public:
    void init() {
      memset(g_answer, 0, sizeof(g_answer));

      string answer0 = answer2string();
      int score0 = sendAnswer(answer0);

      memset(g_answer, 1, sizeof(g_answer));
      string answer1 = answer2string();
      int score1 = sendAnswer(answer1);

      if (score0 < score1) {
        memset(g_answer, 1, sizeof(g_answer));
        g_maxScore = score1;
      } else {
        memset(g_answer, 0, sizeof(g_answer));
        g_maxScore = score0;
      }
    }

    void run() {
      for (int turn = 0; turn < X-2; turn++) {
        updateAnswer(turn);

        string answer = answer2string();
        int score = sendAnswer(answer);

        if (g_maxScore < score) {
          g_maxScore = score;
        } else {
          rollback();
        }

        fprintf(stderr,"%d: %d\n", turn, score);
      }
    }

    void updateAnswer(int turn) {
      memcpy(g_tempAnswer, g_answer, sizeof(g_answer));

      int index = turn*50;

      for (int i = 0; i < 50; i++) {
        if (g_answer[index+i] == 0) {
          g_answer[index+i] = 1;
        } else {
          g_answer[index+i] = 0;
        }
      }
    }

    void rollback() {
      memcpy(g_answer, g_tempAnswer, sizeof(g_tempAnswer));
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
        if (g_answer[i] == 0) {
          answer += "0";
        } else {
          answer += "1";
        }
      }

      return answer;
    }

    string createRandomAnswer() {
      string answer = "";

      for (int i = 0; i < N; i++) {
        if (xor128()%2 == 0) {
          answer += "0";
        } else {
          answer += "1";
        }
      }

      return answer;
    }
};

int main() {
  OnlineExam oe;
  oe.init();
  oe.run();
  return 0;
}
