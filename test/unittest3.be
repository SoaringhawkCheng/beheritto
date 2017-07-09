class Fibonacci:
    def fib(num):
        if num == 1:
            return 1
        elif num == 0:
            return 0
        else:
            return fib(num-1)+fib(num-2)

if __name__=="__main__":
    choice=0
    num = 0
    fibonacci=Fibonacci()
    while choice!=1:
        num=input("input a number")
        println(fibonacci.fib(num))
        choice=input("Enter 1 to exit, 0 to continue")
done
