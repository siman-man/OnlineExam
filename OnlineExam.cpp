#include <iomanip>
#include <iostream>
#include <random>
#include <string.h>
#include <sstream>
#include <queue>

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
  int from;
  int to;
  int length;
  int wc;
  double correctRate;

  Block(int wc = 0, int from = 0, int to = 0) {
    this->wc = wc;
    this->from = from;
    this->to = to;
    this->length = abs(to - from);
  }

  bool operator >(const Block &b) const{
    return wc > b.wc;
  }    
};

priority_queue<Block, vector<Block>, greater<Block> > g_pque;

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

      fprintf(stderr,"First Score = %d\n", g_maxScore);

      int mid = g_maxScore/2;
      g_pque.push(Block(1000, 0, mid));
      g_pque.push(Block(1000, mid+1, g_maxScore-1));
    }

    void run() {
      for (int turn = 0; turn < X-FIRST_TRY_COUNT; turn++) {

        if (g_pque.empty()) {
          updateAnswer(turn);
        } else {
          Block block = g_pque.top(); g_pque.pop();
          updateAnswerBlock(block);
        }

      }
    }

    void updateAnswerBlock(Block block) {
      memcpy(g_tempAnswer, g_answer, sizeof(g_answer));

      flipValue(block.from, block.to);

      string answer = answer2string();
      int score = sendAnswer(answer);

      if (g_maxScore < score) {
        g_maxScore = score;
      } else {
        rollback();
      }

      //fprintf(stderr,"score = %d, max score = %d\n", score, g_maxScore);
    }

    void updateAnswer(int turn) {
      memcpy(g_tempAnswer, g_answer, sizeof(g_answer));

      int index = turn*45;

      flipValue(index, index+45);
      g_answer[g_maxScore-1] ^= 1;

      string answer = answer2string();
      int score = sendAnswer(answer);

      if (g_maxScore < score) {
        g_maxScore = score;
      } else {
        rollback();
      }

      //fprintf(stderr,"score = %d, max score = %d\n", score, g_maxScore);
    }

    void flipValue(int from, int to) {
      fprintf(stderr,"from = %d, to = %d\n", from, to);
      for (int i = from; i < to; i++) {
        g_answer[i] ^= 1;
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
