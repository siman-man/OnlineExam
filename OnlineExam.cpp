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
const int FIRST_TRY_COUNT = 5;

struct Block {
  int length;
  int correctCount;
  double correctRate;

  Block(int cc = 0, int len = 0) {
    this->correctCount = cc;
    this->length = len;
    this->correctRate = correctCount / (double) length;
  }

  bool operator >(const Block &b) const{
    return correctRate > b.correctRate;
  }    
};

//priority_queue<Block, vector<Block>, greater<Block> > pque;

class OnlineExam {
  public:
    void init() {
      memset(g_answer, 0, sizeof(g_answer));
      memcpy(g_tempAnswer, g_answer, sizeof(g_answer));
      g_maxScore = 0;

      for (int i = 0; i < FIRST_TRY_COUNT; i++) {
        createRandomAnswer();

        string answer = answer2string();
        int score = sendAnswer(answer);

        if (g_maxScore < score) {
          g_maxScore = score;
          memcpy(g_tempAnswer, g_answer, sizeof(g_answer));
        } else {
          rollback();
        }
      }
    }

    void run() {
      for (int turn = 0; turn < X-FIRST_TRY_COUNT; turn++) {
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

      int index = turn*45;

      for (int i = 0; i < 45; i++) {
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

    void  createRandomAnswer() {
      for (int i = 0; i < N; i++) {
        if (xor128()%2 == 0) {
          g_answer[i] = 0;
        } else {
          g_answer[i] = 1;
        }
      }
    }
};

int main() {
  OnlineExam oe;
  oe.init();
  oe.run();
  return 0;
}
