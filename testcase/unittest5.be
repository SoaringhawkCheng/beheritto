class Factorial:
    def factorial(num):
        if num <= 0:
            return 1
        else:
            return num * factorial(num -1)

if __name__=="__main__":
    fac=Factorial()
    for x in range(0,11):
        println("Factorial of ",x," is ",fac.factorial(x))
done
