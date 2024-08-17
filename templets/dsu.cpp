struct dsu{
    vector<int>leader, siz;
    void init(int n){
        leader.resize(n + 4), siz.resize(n + 4, 0);
        for (int i = 1; i <= n; i++){
            leader[i] = i; siz[i] = 1;
        }
    }
    // find leader
    
    // complexity o(alpha(n)) alpha(1e9) almost = 4
    int  find_leader(int u){
        if (leader[u] == u)  return u;
        // path compression 
        // if we didn't make leader[u] = find_leader(....); the time will be log
        // because if I come from another leader the componont will be >= same my size 
        // and when I search again I want >= double my original size because the the last one >= 2* my size 
        return leader[u] = find_leader(leader[u]);
    }
    // merge
    void merge(int u, int v){
        int l1 = find_leader(u);
        int l2 = find_leader(v);
        if (l2 == l1) {
            return;
        }
        // union by size()  
        // you can union by anything the time will depend how you merge the time for find root is constant
        if (siz[l1] > siz[l2]) swap(l1 ,l2);
        leader[l1] = l2;
        siz[l2]+= siz[l1];

    }

};
