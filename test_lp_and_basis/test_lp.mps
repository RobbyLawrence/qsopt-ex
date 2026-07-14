NAME          TESTLP
OBJSENSE
 MAX
ROWS
 N  obj
 L  c1
 L  c2
 L  c3
COLUMNS
    x1        obj            2.0   c1             1.0
    x1        c2             1.0   c3            -1.0
    x2        obj            1.0   c1             1.0
    x2        c2            -1.0   c3             2.0
RHS
    RHS       c1             4.0   c2             2.0
    RHS       c3             6.0
BOUNDS
 FR BND       x1
 FR BND       x2
ENDATA