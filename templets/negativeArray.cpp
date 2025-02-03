\\ we put this 
#define arrRange(t, a , mn, mx) t a##_[mx-mn+1], *a=(a##_)-mn;   

\\ we we declare an array 

arrRange((data type), name , mnvalue , mxvalue)) 

ex 
arrRange(int, freq, -n , n) 

when we memset we use 
memset(name_ , -1 or 0 , sizeof name_) we should in memset put _ after name 
now we can say 

freq[-5]  
  
  
   
