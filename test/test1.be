def method(begin,end,step):
    sum=0
    for i in range(begin,end,step):
        sum=sum+i
    return sum

if __name__=="__main__":
    print(method(1,100,1))
done
