#include <iomanip>
#include <iostream>
#include <random>
#include <string.h>
#include <queue>
#include <cassert>

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
  int diff;
  int from;
  int to;
  int length;

  Block(int diff = 0, int length = 0, int from = 0, int to = 0) {
    this->diff = diff;
    this->from = from;
    this->to = to;
    this->length = length;
  }

  bool operator >(const Block &b) const{
    return abs(diff) < abs(b.diff);
  }    
};

priority_queue<Block, vector<Block>, greater<Block> > g_pque;

int g_answer[N];
bool g_commit[N];
int g_commitValue[N];
int g_bestAnswer[N];
int g_maxScore;
int g_baseScore;
int g_turn;
vector<Block> g_flipBlockList;
const int FIRST_TRY_COUNT = 1;

class OnlineExam {
  public:
    void init() {
      memset(g_answer, 0, sizeof(g_answer));
      memset(g_commit, false, sizeof(g_commit));
      memset(g_commitValue, -1, sizeof(g_commitValue));
      memcpy(g_bestAnswer, g_answer, sizeof(g_answer));
      g_maxScore = 0;

      createRandomAnswer();
      int score = getScore();
      commit(score-1);

      g_maxScore = score;
      memcpy(g_bestAnswer, g_answer, sizeof(g_answer));

      flipValue(0, 4096);
      score = getScore();
      commit(score-1);
      g_baseScore = score;

      Block b(g_baseScore - g_maxScore, 4096, 0, 4096);
      g_pque.push(b);

      int qsize = g_pque.size();
      fprintf(stderr,"Base Score = %d, queue size = %d\n", g_baseScore, qsize);
      rollback();
    }

    void run() {
      for (g_turn = 0; g_turn < X-2; g_turn++) {
        commit(g_maxScore-1);

        assert(!g_pque.empty());

        if (g_turn == 97) {
          lastSubmit();
        } else {
          Block block = g_pque.top(); g_pque.pop();
          updateAnswerBlock(block);
        }
      }

      /*
      while (!g_pque.empty()) {
        Block block = g_pque.top(); g_pque.pop();

        fprintf(stderr,"%d -> %d, wc = %4.2f\n", block.from, block.to, block.wc);
      }
      */
    }

    void lastSubmit() {
      int commitSize = 0;

      for (int i = 0; i < N; i++) {
        if (g_commitValue[i] >= 0) {
          g_answer[i] = g_commitValue[i];
          commitSize++;
        }
      }

      int bsize = g_flipBlockList.size();
      for (int i = 0; i < bsize; i++) {
        Block block = g_flipBlockList[i];
        flipValue(block.from, block.to);
      }

      while (!g_pque.empty()) {
        Block block = g_pque.top(); g_pque.pop();

        if (block.diff > 0) {
          flipValue(block.from, block.to);
        }
      }

      int score = getScore();
      fprintf(stderr,"Last score = %d, commit size = %d\n", score, commitSize);
    }

    void updateAnswerBlock(Block block) {
      fprintf(stderr,"turn %d: Block %d -> %d\n", g_turn, block.from, block.to);
      int mid = (block.from + block.to) / 2.0;
      flipValue(block.from, mid);

      string answer = answer2string();
      int score = sendAnswer(answer);
      int diffR = score - g_baseScore;
      int diffL = block.diff - diffR;
      double expect = abs(diffR)/4.0;
      commit(score-1);

      fprintf(stderr," %d -> %d, diffR = %d, diffL = %d, score = %d, expect = %4.2f\n",
          block.from, mid, diffR, diffL, score, expect);

      rollback();

      if (block.length/10.0 < expect) {
        fprintf(stderr,"Skip!\n");
        return;
      }
      if (block.length <= 16) {
        fprintf(stderr,"Stop!\n");
        return;
      }

      Block b1(diffR, block.length/2.0, block.from, mid);
      Block b2(diffL, block.length/2.0, mid, block.to);

      if (block.length/20.0 < expect) {
        fprintf(stderr,"b1 Skip!\n");

        if (diffR > 0) {
          g_flipBlockList.push_back(b1);
        }
      } else {
        g_pque.push(b1);
        fprintf(stderr,"Add %d -> %d, size = %d, diffR = %d\n", block.from, mid, block.length/2, diffR);
      }

      if (block.length/20.0 < abs(diffL)/4.0) {
        fprintf(stderr,"b2 Skip!\n");

        if (diffL > 0) {
          g_flipBlockList.push_back(b2);
        }
      } else {
        g_pque.push(b2);
        fprintf(stderr,"Add %d -> %d, size = %d, diffL = %d\n", mid, block.to, block.length/2, diffL);
      }

      //fprintf(stderr,"turn %d: sc = %d, max sc = %d\n", g_turn, score, g_maxScore);
    }

    void commit(int index) {
      if (!g_commit[index]) {
        g_commit[index] = true;
        g_commitValue[index] = g_answer[index] ^ 1;
      }
    }

    void flipValue(int from, int to) {
      for (int i = from; i < to; i++) {
        g_answer[i] ^= 1;
      }
    }

    void rollback() {
      for (int i = 0; i < N; i++) {
        g_answer[i] = g_bestAnswer[i];
      }
    }

    int getScore() {
      string answer = answer2string();
      int score = sendAnswer(answer);
      return score;
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
