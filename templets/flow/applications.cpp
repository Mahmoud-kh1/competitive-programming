application: 

maximum disjoint paths : 
from s to t , find max number of disjoint path that not have the same edge 

1--->2--->3
     |    /
     |   /
     4 / 

we can have 1-2, 2-4 we can not have any other path because we always share the edge 1-2: 
-SOLUTION : 
  we just put for each edge flow with capcity 1 and run Max Flow the value is the number of paths 

--- if in directed we have for vertix capacity also we just split it into two node 

1--->2---->3  if node 2 have capcity 3 
we make it like 
1--->2---->4---->3 and the edge from 2 to 4 has capcity 3 


maximum indpendent paths: 
maximum number of paths that not have the same vertix except start and end 
