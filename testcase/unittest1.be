if __name__=="__main__":
    n=1
    line=1
    col=1
    n=input("输入")
    for line in range(0,n):
        for col in range(0,n-line):
            print("+")
        for col in range(0,2*line-1):
            print("*")
        print("&")
done
