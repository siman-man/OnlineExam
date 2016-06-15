rate = 0.5

K = 2000
N = 5000

cnt = 1

sum = 0
max_score = 0
divide = 20

wrong_list = Array.new(5, 0)

cnt.times do
	score = 0
	wrong = 0

	while score < N
	#while wrong <= K && score < N
		score += 1

		if rand > rate
			wrong += 1
		end

		if score % divide == 0
			index = score/divide

			if index > 1
				wrong_list[index] = (wrong - wrong_list[0..index-1].inject(:+))
			else
				wrong_list[index] = wrong
			end
		end
	end

	max_score = [max_score, score].max
	sum += score
end

p wrong_list.map{|i| (i / divide.to_f).round(2)}
p wrong_list.map{|i| i.zero? ? 0 : 0.5 + (0.5 - i / divide.to_f).abs }.inject(:+) / (wrong_list.size-1)
p wrong_list.map{|i| i.zero? ? 0 : 0.5 + (0.5 - i / divide.to_f).abs }.count{|i| i > 0.57}
#puts "max score = #{max_score}"
puts sum/cnt.to_f