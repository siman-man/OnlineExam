#include <iostream>
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
  int diff;
  int from;
  int to;
  int length;
  int divideCount;

  Block(int diff = 0, int from = 0, int to = 0) {
    this->diff = diff;
    this->from = from;
    this->to = to;
    this->length = to - from;
    this->divideCount = 0;
  }

  bool operator >(const Block &b) const{
    return length - 4*abs(diff) < b.length - 4*abs(b.diff);
  }    
};

priority_queue<Block, vector<Block>, greater<Block> > g_pque;

int g_answer[N];
bool g_commit[N];
int g_maxScore;
int g_turn;

class OnlineExam {
  public:
    void init() {
      memset(g_answer, 0, sizeof(g_answer));
      memset(g_commit, false, sizeof(g_commit));

      setRandomAnswer();
      g_maxScore = getScore();
      commit(g_maxScore-1);

      int divide = 56;
      for (int i = 0; i < 71; i++) {
        Block b(0, i*divide, (i+1)*divide);
        g_pque.push(b);
      }

      fprintf(stderr,"First Score = %d\n", g_maxScore);
    }

    void run() {
      for (g_turn = 0; g_turn < X-1; g_turn++) {
        Block block = g_pque.top(); g_pque.pop();
        updateAnswerBlock(block);
      }
    }

    void updateAnswerBlock(Block block) {
      //fprintf(stderr,"%d -> %d, block diff = %d\n", block.from, block.to, block.diff);

      flipValue(block.from, block.to);

      int score = getScore();
      int diff = score - g_maxScore;
      commit(score-1);

      if (g_maxScore < score) {
        g_maxScore = score;
      } else {
        flipValue(block.from, block.to);

        if (block.divideCount >= 1 && block.diff > diff) {
          g_maxScore += 3;
          flipValue(block.to, block.to + block.length);
        }
      }

      int mid = (block.from + block.to) / 2;
      Block b1(diff, block.from, mid);
      b1.divideCount = block.divideCount + 1;

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

    int getScore() {
      string answer = answer2string();
      return sendAnswer(answer);
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

    void setRandomAnswer() {
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
