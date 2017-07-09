#BinarySearch
class Sort:
    def __init__():
        _arreglo=[70, 40,90,60, 80, 100,900, 34, 98, 120, 109, 14]

    def BubbleSort():
        aux=0
        println("Before sort: ",_arreglo)
        for x in range(1,12):
            for y in range(1,12):
                aux = _arreglo[y]
                if _arreglo[y] < _arreglo[y-1]:
                    _arreglo[y] = _arreglo[y-1]
                    _arreglo[y-1] = aux
        println("After sort: ",_arreglo)
    def BinarySearch(v):
        i = 0
        j = 11

        while i!=j+1:
            m = (i+j)/2
            if _arreglo[m]<v:
                i = m+1
            else:
                j = m-1
        if _arreglo[i] == v:
            println("Number exists in sorted array at pos ", i)
        else:
            println("Number doesn't exist!")

if __name__=="__main__":
    sort=Sort()
    println("BinarySearch:")
    sort.BubbleSort()
    number=input("输入")
    sort.BinarySearch(number)
done
