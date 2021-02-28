# Assignment - 1

##### Student Information:

- Anirudha Kulkarni
- 2019CS50421

##### Input:

n : number of points

next 2 x n lines for X and Y coordinates of each point

##### Output:

Area under a curve formed by joining successive points by a straight line

---

### Approach:

##### Design:

- User enters number of points and x coordinate and y coordinate for each point
- User is notified each time with:
  1. index of current point
  2. whether he is entering x coordinate or y coordinate
  3. area calculated so far so that user can deduce if something is wrong like negative area due to overflow without needing to enter large number of points
- For bad n like negative or 0 program is terminated with custom message
- Program termination message to avoid confusion
- Same registers are rewritten to use less registers as possible
- Intermediate products and area is stored in double so as to give maximum precision possible
- intermediate sum and differences are stored in 32 bit integer as, if products doesnt exceed 64 bit double then it will not exceed 32 bit int
- Absolute area is calculated by taking absolute values for negative portion
- conditioners are handled by using branches

##### Workflow:

- Take number of points as input and store it in $t0 register and `points` memory location
- if number of points is less than 1 then input is invalid and `badterminate` branch is executed
- if number of points is 1 then area is outputed as 0
- else all n integers are taken input and area calculated till that point is also shown for better experience
- finally `terminate` branch is executed, total area is shown and program is terminated

##### Area Calculation:

- Area of complete polynomial formed using n points can be broken into n-1 trapeziums with 2 sides parallel to y axis and 1 side parallel to x axis.
- Area is calculated in absolute values. Area below x-axis is negated to get positive area
- Any general case can be divided into 4 cases. Each area can be calculated individually and added to get area final
- ![](image/README/1614067245720.png)

##### Registers used:

- t0 = No of points
- t1 = iterator for "for-loop"
- t3 = Xprev
- t4 = Yprev
- t5 = Xcurr
- t6 = Ycurr
- f4 = area so far
- t7, t8 = temporary addition, substractions
- f6, f8 = temporary products which are large

---

### Testing:

##### 1. Bad number of inputs:

- ![](image/README/1613964394372.png)
- ![](image/README/1613964422255.png)

##### 2. With single point:

Area of single point is 0 irrespective of its x and y coordinates

![](image/README/1613964504809.png)

##### 3. With 2 points such that positive area = negative area:

3 points at (-10000,-10000), (0,0), (10000,10000). Area1 = Area2. Hence, Area = Area1+Area2= 1/2*10000 *10000 * 2 = 50000000 *2 = 100000000

![](image/README/1614069901251.png)

##### 4. With Large X and Y coordinate to check oveflow:

Works till 10^8 x and y range. When y\*\*2 crosses 64-bit limit overflow occurs

![](image/README/1614069760529.png)

When y\*\*2 crosses 64-bit limit overflow occurs

![](image/README/1614069710035.png)

##### 5. Points with same X coordinate:

for same x coordinate width of curve is 0 hence area should be 0 irrespective of height

![](image/README/1613965264797.png)

##### 6. Decimal area:

2 points at (0,0), (69,89). Area = (69-0)_(89+0)/2=69 _ 89/2 = 6141/2 = 3070.5

![](image/README/1613965025262.png)

##### 7. Random numbers:

This test case contain all the 4 cases mentioned above.

![](image/README/1614070151380.png)

which matches with actual calculated area = 453.918
