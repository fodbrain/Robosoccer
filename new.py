u = int(input("ป้อนจำนวนสินค้า:"))
i=1
k=0
summ=0
while(i<=u):

    money = int(input("ป้อนราคาสินค้าชิ้นที่ %d :"%i))
    if(money >100 ):
        k+=1
        summ = summ+money
    i+=1

print("จำนวนสินค้าที่ราคาเกิน 100 = %d" %k)
print("รวมราคา %d" %summ)
