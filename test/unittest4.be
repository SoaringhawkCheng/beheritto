class CountDown:
    def SumTo(nValue):
        if nValue <=1:
            return nValue
        else:
            return SumTo(nValue - 1) + nValue

if __name__=="__main__":
    countdown=CountDown()
    println("Result: ", countdown.SumTo(5))
done
