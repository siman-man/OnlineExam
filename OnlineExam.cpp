#include <iomanip>
#include <iostream>
#include <random>
#include <string.h>
#include <queue>

using namespace std;

typedef long long ll;

const int N = 5000;
const int K = 2000;
const int X =  100;

unsigned long long xor128(){
  static unsigned long long rx=123456789, ry=362436069, rz=521288629, rw=88675123;
  unsigned long long rt = (rx ^ (rx<<11));
  rx=ry; ry=rz; rz=rw;
  return (rw=(rw^(rw>>19))^(rt^(rt>>8)));
}

struct Block {
  ll id;
  int from;
  int to;
  int score;
  int length;
  int divideCount;
  int from2;
  int to2;

  Block(int score = 0, int length = 0, int from = 0, int to = 0) {
    this->from = from;
    this->to = to;
    this->score = score;
    this->length = length;
    this->divideCount = 0;
  }

  bool operator >(const Block &b) const{
    return length - score < b.length - b.score;
  }    
};

priority_queue<Block, vector<Block>, greater<Block> > g_pque;

int g_answer[N];
bool g_commit[N];
int g_bestAnswer[N];
int g_maxScore;
int g_turn;
const int FIRST_TRY_COUNT = 2;

class OnlineExam {
  public:
    void init() {
      memset(g_answer, 0, sizeof(g_answer));
      memset(g_commit, false, sizeof(g_commit));
      memcpy(g_bestAnswer, g_answer, sizeof(g_answer));
      g_maxScore = 0;

      for (int i = 0; i < FIRST_TRY_COUNT; i++) {
        createRandomAnswer(i);

        string answer = answer2string();
        int score = sendAnswer(answer);

        commit(score-1);

        if (g_maxScore < score) {
          g_maxScore = score;
          memcpy(g_bestAnswer, g_answer, sizeof(g_answer));
        } else {
          rollback();
        }
      }

      int divide = 55;
      for (int i = 0; i < 75; i++) {
        Block b(i-N, divide, i*divide, (i+1)*divide);
        b.id = xor128();
        g_pque.push(b);
      }

      int qsize = g_pque.size();
      fprintf(stderr,"First Score = %d, queue size = %d\n", g_maxScore, qsize);
    }

    void run() {
      for (g_turn = 0; g_turn < X-FIRST_TRY_COUNT; g_turn++) {
        commit(g_maxScore-1);

        Block block = g_pque.top(); g_pque.pop();
        updateAnswerBlock(block);
      }
    }

    void updateAnswerBlock(Block block) {
      fprintf(stderr,"%d -> %d, block score = %d\n", block.from, block.to, block.score);

      flipValue(block.from, block.to);

      string answer = answer2string();
      int score = sendAnswer(answer);
      int diff = abs(score - g_maxScore);

      if (g_maxScore < score) {
        g_maxScore = score;
        memcpy(g_bestAnswer, g_answer, sizeof(g_answer));
      } else {
        commit(score-1);
        rollback();

        if (block.divideCount >= 1) {
          flipValue(block.from2, block.to2);
        }
      }

      int mid = (block.from + block.to) / 2;
      Block b1(diff, block.length/2, block.from, mid);

      b1.id = xor128();
      b1.divideCount++;
      b1.from2 = mid;
      b1.to2 = block.to;

      g_pque.push(b1);

      fprintf(stderr,"turn %d: sc = %d, max sc = %d\n", g_turn, score, g_maxScore);
    }

    void commit(int index) {
      if (!g_commit[index]) {
        g_answer[index] ^= 1;
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

    void createRandomAnswer(int val) {
      for (int i = 0; i < N; i++) {
        if (g_commit[i]) continue;
        g_answer[i] = val;
      }
    }
};

int main() {
  OnlineExam oe;
  oe.init();
  oe.run();
  return 0;
}
