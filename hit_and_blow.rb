$size = 100
$max_score = 0

class HitAndBlow
	def initialize
		@size = $size
		@answre = @size.times.map{rand(2)}
    @weight = @size.times.map{rand(3)+1}
    $max_score = @weight.inject(:+)
		@expect = Array.new(@size, 0)
    @base_score = calc_score(random_answer)
	end

	def run
		check
		last = @expect.map{|i| i < 0 ? 0 : 1}
		score = calc_score(last)
		#p @answre
		#p @expect.map{|i| i < 0 ? 0 : 1}
		puts "score = #{score}"
		#p @expect
		score
	end

	def random_answer
		@size.times.map do |i|
			rand(2)
		end
	end

	def check
		@cnt = 25

		@cnt.times do
			solve = random_answer

			score = calc_score(solve)
			diff = score - @base_score

			if score > @base_score
				@size.times do |i|
					if solve[i] == 0
						@expect[i] -= diff
					else
						@expect[i] += diff
					end
				end
			else
				@size.times do |i|
					if solve[i] == 0
						@expect[i] += diff
					else
						@expect[i] -= diff
					end
				end
			end
		end
	end

	def calc_score(solve)
		score = 0

		@size.times do |i|
			if @answre[i] == solve[i]
        score += @weight[i]
      else
        score -= @weight[i]
      end
		end	

		score
	end
end

sum = 0
20.times do
	sum += HitAndBlow.new.run
end

average = sum/20.0
puts "average = #{average}, rate = #{average/$size.to_f}"
