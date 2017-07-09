class accumulate:
    def __init__(begin,end):
        _begin=begin
        _end=end
    def calculate(step):
        sum=0
        for i in range(_begin,_end,step):
            sum=sum+i
        return sum

if __init__=="__main__":
    acc=accumulate(1,10)
    print(acc.calculate(1))
done
