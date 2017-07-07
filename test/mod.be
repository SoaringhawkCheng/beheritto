class accumulate:
    def __init__(begin,end):
        begin=begin
        end=end
    def calculate(step):
        sum=0
        for i in range(begin,end,step):
            sum=sum+i

if __init__=="__main__":
    acc=accumulate(1,10)
    print(acc.calculate(1))
done
