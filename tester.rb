class Tester
  FILENAME = 'OnlineExam'
  N = 5000
  K = 2000
  X = 100

  def initialize(seed: 0)
    @seed = seed
    @answer = generate_answer(seed: seed)
    @solver = IO.popen("./#{FILENAME}", 'w+')
    @high_score = 0
  end

  def generate_answer(seed: 0)
    answer = []
    rnd = Random.new(seed)

    N.times do
      answer << rnd.rand(2)
    end

    answer
  end

  def input(params)
    @solver.puts(params)
  end

  def response
    @solver.gets
  end

  def run
    X.times do |i|
      begin
        res = response.chomp.split('').map(&:to_i)
        score = calc_score(res)
        @high_score = [@high_score, score].max
        input(score)
      rescue Exception => ex
        break
      end
    end

    @high_score = 0 if @high_score < 3000

    puts "Score = #{@high_score}"
  end

  def calc_score(ans)
    score = 0
    wrong = 0

    N.times do |i|
      score += 1

      if ans[i] != @answer[i]
        wrong += 1
      end

      if wrong >= K
        break
      end
    end

    score
  end
end

seed = ARGV[0].to_i
tester = Tester.new(seed: seed)
tester.run

