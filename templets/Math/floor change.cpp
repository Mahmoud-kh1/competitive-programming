
// chang[x] contain all postion it's value will change if divided by x
vector<vector<int>> change(maxX+2);
    for(int i = 1; i <= n; i++){
        int v = temp[i];
        int x = 1;
        while(x <= maxX){
            change[x].push_back(i);
            int q = v / x;
            if(q == 0) break;
            int r = v / q;
            x = r + 1;
        }
    }
