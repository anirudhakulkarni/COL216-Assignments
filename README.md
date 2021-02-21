Approach:

1. take n as input
2. take n pairs of integers
3. if n=1 print invalid input
4. initialize area variable to 0
5. iterate through inputs for i=0 to n-1
   1. let current coordinates be x,y and next be x1,y1
   2. if y1>y then
      1. area+=(y1-y)X(x1-x)X1/2+yX(x1-x)
   3. else
      1. area+=(y-y1)X(x1-x)X1/2+y1X(x1-x)
6. print area
