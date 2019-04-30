func sumofweight(weights: [Float] )-> Float{
var sum: Float=0
for component in weights {
sum+=component
}
return sum
}

func earned(marks: [Float:Float] )-> Float{
var sum: Float=0

for (w,m) in marks {
var tempw=w
var tempm=m
if(0<=m&&m<=100){
tempm=(m/100)
}
sum+=(tempw*tempm)
}
return sum
}

func average(mark: [Float:Float], input: [Float] )->Float {
var e = earned(marks:mark)
var total = sumofweight(weights:input)
return (e/total)*100
}

func remaining(weight: [Float] )-> Float{
var total = sumofweight(weights:weight)
return (100-total)
}

func wanted_mark(wanted:Float ,input: [Float], mark: [Float:Float])->Float{
var rem = remaining(weight: input)
var e = earned(marks: mark)
var still_needed = wanted-e
return (still_needed/rem)*100
}

func if_I_get_with_remaining(rec:Float ,input: [Float], mark: [Float:Float])->Float{
var rem = remaining(weight: input)
var e = earned(marks: mark)
return (e+(rem*(rec/100)))
}

func calc_gpa(gpa:[Float: Float]) -> Float{
var sumofw:Float = 0
var gbyw:Float=0
for(grade, weight) in gpa{
sumofw+=weight
gbyw+=(weight*grade)
}
return (gbyw/sumofw)
}

var weight: [Float] = [60,20]
var w = sumofweight(weights: weight)
print(w)

var input: [Float:Float] = [60 : 100, 20: 100]
var e = earned(marks: input)
print(e)

var avg=average(mark:input, input:weight)
print(avg)

var rem = remaining(weight: weight)
print(rem)

var opt = wanted_mark(wanted:90, input: weight, mark: input)
print(opt)

var got = if_I_get_with_remaining(rec: 50, input: weight, mark: input)
print(got)

var gpa: [Float:Float] = [9 : 1.5, 7: 1.5, 5: 1.5]
var num = calc_gpa(gpa:gpa)
print(num)
